#include "sockets.hpp"
#include "utils/log.hpp"
#include "utils/utils.hpp"
#include "utils/config.hpp"
#include "utils/va.hpp"
#include "crypto/crypto.hpp"
#include "global/auth_vars.hpp"

namespace Auth::Sockets {
#define SOCKET_TIMEOUT 20000

	void DecryptKeys(Response::EncryptionKeys* Keys) {
		for (int i = 0; i < 0x10; i++) {
			Keys->m_FirstKey[i] ^= 37;
			Keys->m_FirstKey[i] ^= Keys->m_XorKey;
			Keys->m_FirstKey[i] ^= 13;
			Keys->m_FirstKey[i] ^= Keys->m_FinalKey[i];
		}

		for (int i = 0; i < 0x10; i++) {
			Keys->m_SecondKey[i] ^= 69;
			Keys->m_SecondKey[i] ^= Keys->m_FinalKey[i];
			Keys->m_SecondKey[i] ^= 69;
			Keys->m_SecondKey[i] ^= Keys->m_XorKey;
		}
	}

	void DecryptHash(Response::EncryptionKeys* Keys) {
		((uint8_t*)&Keys->m_Hash)[3] ^= Keys->m_XorKey;

		Keys->m_Hash ^= 420;
		Keys->m_Hash ^= 1337;

		for (int i = 0; i < 0x10; i++) {
			Keys->m_Hash ^= Keys->m_XorKey | Keys->m_FinalKey[i];
			Keys->m_Hash ^= Keys->m_XorKey ^ Keys->m_FinalKey[i];
			Keys->m_Hash ^= Keys->m_SecondKey[i];
			Keys->m_Hash ^= Keys->m_FirstKey[i] << 16;
		}
	}

	void DecryptPayload(Request::Header* Header, uint8_t* Bytes, uint32_t Size, Response::EncryptionKeys* Keys) {
		DecryptKeys(Keys);
		DecryptHash(Keys);

		// RC4 (Modified) buffer with m_RequestKey
		Crypto::RC4Modified(Header->m_EncryptionKeys.m_RequestKey, sizeof(Header->m_EncryptionKeys.m_RequestKey), Bytes, Size);

		// RC4 buffer with m_FinalKey
		Crypto::RC4(Keys->m_FinalKey, sizeof(Keys->m_FinalKey), Bytes, Size);

		// XOR each byte 4x times with each byte of unencrypted m_Hash
		std::vector<uint8_t> HashBytes = Utils::ConvertNumberToBytes(Keys->m_Hash);
		for (uint32_t i = 0; i < Size; i++) {
			Bytes[i] ^= Keys->m_XorKey;

			for (int j = 0; j < 4; j++) {
				Bytes[i] ^= HashBytes[3 - j];
			}
		}

		// RC4 buffer with second 0x10 of m_AuthToken
		if (Header->m_Packet == PACKET_PING) {
			// PACKET_PING wont have an auth token just yet
			uint8_t PingKey[16] = { 0x70, 0x65, 0x6E, 0x69, 0x73, 0x62, 0x69, 0x74, 0x63, 0x68, 0x67, 0x61, 0x67, 0x67, 0x6F, 0x74 };
			Crypto::RC4(PingKey, 0x10, Bytes, Size);
		} else if (Header->m_Packet == PACKET_WELCOME) {
			// PACKET_WELCOME wont have an auth token just yet
			uint8_t WelcomeKey[16] = { 0x68, 0x61, 0x68, 0x61, 0x20, 0x6C, 0x75, 0x6E, 0x61, 0x20, 0x67, 0x6F, 0x20, 0x62, 0x72, 0x72 };
			Crypto::RC4(WelcomeKey, 0x10, Bytes, Size);
		} else {
			Crypto::RC4(Header->m_EncryptionKeys.m_TokenKey, 0x10, Header->m_AuthToken, sizeof(Header->m_AuthToken));
			Crypto::RC4(Header->m_AuthToken + 0x10, 0x10, Bytes, Size);
			Crypto::RC4(Header->m_EncryptionKeys.m_TokenKey, 0x10, Header->m_AuthToken, sizeof(Header->m_AuthToken));
		}

		// XOR buffer with random key
		for (uint32_t i = 0; i < Size; i++) {
			Bytes[i] ^= Keys->m_XorKey;
		}
	}

	bool Network::Initialize() {
		static bool Initialized = false;
		if (Initialized) return Initialized;

		WSADATA WsaData;
		if (WSAStartup(MAKEWORD(2, 2), &WsaData)) return false;

		Initialized = true;
		return true;
	}

	Network& Network::Create(bool* Successful, const char** Error) {
		bool Success = true;

		for (int i = 0; i < 10; i++) {
			m_SocketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_SocketHandle == SOCKET_ERROR) {
				Success = false;
				LOG_CUSTOM_WARN("Network"_Protect, "Failed to create connection (#1), attempt %i to reconect. Status: %X"_Protect, i + 1, GetLastError());
			} else {
				Success = true;
				break;
			}

			Sleep(100);
		}

		if (!Success) {
			if (Successful) *Successful = false;
			if (Error) *Error = "Failed to maintain a connection after 10 tries (#1)"_Protect;
			return *this;
		}

		if (Successful) *Successful = true;
		return *this;
	}

	Network& Network::Connect(bool* Successful, const char** Error) {
		int SendRecvSize = 0x100000;
		setsockopt(m_SocketHandle, SOL_SOCKET, SO_SNDBUF | SO_RCVBUF, (const char*)&SendRecvSize, sizeof(SendRecvSize));

		int Timeout = SOCKET_TIMEOUT;
		setsockopt(m_SocketHandle, SOL_SOCKET, SO_SNDTIMEO | SO_RCVTIMEO, (const char*)&Timeout, sizeof(Timeout));

		sockaddr_in Address;
		Address.sin_family = AF_INET;
		Address.sin_port = htons(6969);

		Address.sin_addr.S_un.S_un_b.s_b1 = 209;
		Address.sin_addr.S_un.S_un_b.s_b2 = 141;
		Address.sin_addr.S_un.S_un_b.s_b3 = 39;
		Address.sin_addr.S_un.S_un_b.s_b4 = 192;

		/*Address.sin_addr.S_un.S_un_b.s_b1 = 209;
		Address.sin_addr.S_un.S_un_b.s_b2 = 141;
		Address.sin_addr.S_un.S_un_b.s_b3 = 50;
		Address.sin_addr.S_un.S_un_b.s_b4 = 21;*/

		bool Success = true;
		for (int i = 0; i < 10; i++) {
			if (::connect(m_SocketHandle, (sockaddr*)&Address, sizeof(Address)) == SOCKET_ERROR) {
				Success = false;
				LOG_CUSTOM_WARN("Network"_Protect, "Failed to create connection (#2), attempt %i to reconect. Status: %X"_Protect, i + 1, GetLastError());
			} else {
				Success = true;
				break;
			}

			Sleep(100);
		}

		if (!Success) {
			closesocket(m_SocketHandle);
			if (Successful) *Successful = false;
			if (Error) *Error = "Failed to maintain a connection after 10 tries (#2)"_Protect;
			return *this;
		}

		LOG_CUSTOM_DEV("Network", "Connected to main server (%i.%i.%i.%i)", Address.sin_addr.S_un.S_un_b.s_b1, Address.sin_addr.S_un.S_un_b.s_b2, Address.sin_addr.S_un.S_un_b.s_b3, Address.sin_addr.S_un.S_un_b.s_b4);

		if (Successful) *Successful = true;
		return *this;
	}

	Network& Network::Send(void* Buffer, int Size) {
		char* CurrentData = (char*)Buffer;
		int DataLeft = Size;

		while (DataLeft > 0) {
			int sendStatus = ::send(m_SocketHandle, CurrentData, 2048 < DataLeft ? 2048 : DataLeft, 0);
			if (sendStatus == SOCKET_ERROR) break;

			CurrentData += sendStatus;
			DataLeft -= sendStatus;
		}

		return *this;
	}

	Network& Network::Receive(Request::Header* Header, void* RecvBuffer, int Size, int* ReadBytes, bool* Successful, const char** Error) {
		char* ReceivedBuffer = new char[Size];
		int Remaining = Size;
		int Received = 0;

		while (Remaining > 0) {
			int Receive = ::recv(m_SocketHandle, (char*)ReceivedBuffer + Received, 2048 < Remaining ? 2048 : Remaining, 0);

			if (Receive == SOCKET_ERROR) {
				if (Successful) *Successful = false;
				if (Error) *Error = "Failed receiving payload from server"_Protect;
				closesocket(m_SocketHandle);
				delete[] ReceivedBuffer;
				return *this;
			}

			Remaining -= Receive;
			Received += Receive;

			if (Receive == 0) break;
		}

		closesocket(m_SocketHandle);

		if (ReadBytes) *ReadBytes = Received;
		if (Successful) *Successful = true;

		if (Received == 5) {
			if (*(int*)&ReceivedBuffer[0] == 0x13371337) {
				eSocketErrorCodes ErrorCode = (eSocketErrorCodes)ReceivedBuffer[4];
				switch (ErrorCode) {
					case ERROR_CANT_READ_MAGIC:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to read magic"_Protect;
						break;

					case ERROR_INVALID_MAGIC:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to verify magic"_Protect;
						break;

					case ERROR_CANT_READ_REQUIRED_HEADER:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to read header"_Protect;
						break;

					case ERROR_INVALID_SIZE:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to verify size"_Protect;
						break;

					case ERROR_INVALID_PACKET:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to verify packet"_Protect;
						break;

					case ERROR_INVALID_CHEAT:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to verify cheat"_Protect;
						break;

					case ERROR_CANT_READ_HEADER_DATA:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to read data"_Protect;
						break;

					case ERROR_INVALID_PACKET_SIZE:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to verify packet size"_Protect;
						break;

					case ERROR_AUTH_TOKEN_NOT_FOUND:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to verify auth token"_Protect;
						break;

					case ERROR_ALLOCATING_MEMORY:
						if (Successful) *Successful = false;
						if (Error) *Error = "Failed to allocate server memory for request (ouch)"_Protect;
						break;
				}

				delete[] ReceivedBuffer;
				return *this;
			}
		}

		if (Received > sizeof(Response::EncryptionKeys)) {
			Response::EncryptionKeys* Keys = (Response::EncryptionKeys*)ReceivedBuffer;
			char* NewBytes = ReceivedBuffer + sizeof(Response::EncryptionKeys);
			int NewSize = Size - sizeof(Response::EncryptionKeys);

			DecryptPayload(Header, (uint8_t*)NewBytes, NewSize, Keys);
			memcpy((void*)((uint64_t)RecvBuffer + sizeof(Response::EncryptionKeys)), NewBytes, NewSize);
		} else {
			if (Successful) *Successful = false;
			if (Error) *Error = "Failed to receive full payload from server"_Protect;
		}

		delete[] ReceivedBuffer;
		return *this;
	}
}