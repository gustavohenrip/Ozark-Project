#include "socket_encryption.hpp"
#include "utils/utils.hpp"
#include "networking/crypto/crypto.hpp"

namespace Network {
	EncryptionProcess* StartEncryptionProcess(IO::BinaryWriter* Writer, Structs::Request::Header* Header) {
		EncryptionProcess* Allocation = (EncryptionProcess*)malloc(sizeof(EncryptionProcess));

		if (Allocation) {
			Allocation->m_Header = Header;
			GenerateEncryptionKeys(Allocation);
			FinalizeEncryptionHash(Allocation);
			FinalizeEncryptionKeys(Allocation);

			Writer->WriteByte(Allocation->m_Keys.m_XorKey);
			Writer->WriteBytes(Allocation->m_Keys.m_FinalKey, sizeof(Allocation->m_Keys.m_FinalKey));
			Writer->WriteBytes(Allocation->m_Keys.m_FirstKey, sizeof(Allocation->m_Keys.m_FirstKey));
			Writer->WriteBytes(Allocation->m_Keys.m_SecondKey, sizeof(Allocation->m_Keys.m_SecondKey));
			Writer->WriteUInt32(Allocation->m_Keys.m_Hash);
		}

		return Allocation;
	}

	void GenerateEncryptionKeys(EncryptionProcess* Process) {
		Utils::GenerateRandomBytes(Process->m_Keys.m_FinalKey, sizeof(Process->m_Keys.m_FinalKey));
		Utils::GenerateRandomBytes(Process->m_Keys.m_FirstKey, sizeof(Process->m_Keys.m_FirstKey));
		Utils::GenerateRandomBytes(Process->m_Keys.m_SecondKey, sizeof(Process->m_Keys.m_SecondKey));

		Process->m_Keys.m_XorKey = (uint8_t)(rand() % 256);
		Process->m_Time = Utils::GetTickCount() + (rand() % 99999);
		Process->m_Keys.m_Hash = Process->m_Time;
	}

	void FinalizeEncryptionKeys(EncryptionProcess* Process) {
		for (int i = 0; i < 0x10; i++) {
			Process->m_Keys.m_FirstKey[i] ^= Process->m_Keys.m_FinalKey[i];
			Process->m_Keys.m_FirstKey[i] ^= 13;
			Process->m_Keys.m_FirstKey[i] ^= Process->m_Keys.m_XorKey;
			Process->m_Keys.m_FirstKey[i] ^= 37;
		}

		for (int i = 0; i < 0x10; i++) {
			Process->m_Keys.m_SecondKey[i] ^= Process->m_Keys.m_XorKey;
			Process->m_Keys.m_SecondKey[i] ^= 69;
			Process->m_Keys.m_SecondKey[i] ^= Process->m_Keys.m_FinalKey[i];
			Process->m_Keys.m_SecondKey[i] ^= 69;
		}
	}

	void FinalizeEncryptionHash(EncryptionProcess* Process) {
		for (int i = 0; i < 0x10; i++) {
			Process->m_Keys.m_Hash ^= Process->m_Keys.m_FirstKey[i] << 16;
			Process->m_Keys.m_Hash ^= Process->m_Keys.m_SecondKey[i];
			Process->m_Keys.m_Hash ^= Process->m_Keys.m_XorKey ^ Process->m_Keys.m_FinalKey[i];
			Process->m_Keys.m_Hash ^= Process->m_Keys.m_XorKey | Process->m_Keys.m_FinalKey[i];
		}

		Process->m_Keys.m_Hash ^= 1337;
		Process->m_Keys.m_Hash ^= 420;

		// First byte is always 15 of 16 (decimal), this looks like it's a timestamp.
		((uint8_t*)&Process->m_Keys.m_Hash)[3] ^= Process->m_Keys.m_XorKey;
	}

	void SendEncryptedPacket(Socket Writer, uint8_t* Packet, uint32_t Length, EncryptionProcess* Process) {
		uint32_t EncryptionHeaderSize = sizeof(Structs::Response::EncryptionKeys);
		if (Length > EncryptionHeaderSize) {
			// XOR buffer with random key
			for (uint32_t i = EncryptionHeaderSize; i < Length; i++) {
				Packet[i] ^= Process->m_Keys.m_XorKey;
			}

			// RC4 buffer with second 0x10 of m_AuthToken
			if (Process->m_Header->m_Packet == Structs::PACKET_PING) {
				// PACKET_PING wont have an auth token just yet
#ifdef NEW_SERVER
				uint8_t PingKey[16] = { 0x6C, 0x6F, 0x6C, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20, 0x66, 0x75, 0x6E, 0x20, 0x78, 0x64, 0x64 };
#else
				uint8_t PingKey[16] = { 0x70, 0x65, 0x6E, 0x69, 0x73, 0x62, 0x69, 0x74, 0x63, 0x68, 0x67, 0x61, 0x67, 0x67, 0x6F, 0x74 };
#endif

				Crypto::RC4(PingKey, 0x10, Packet, Length, EncryptionHeaderSize);
			} else if (Process->m_Header->m_Packet == Structs::PACKET_WELCOME) {
#ifdef NEW_SERVER
				uint8_t WelcomeKey[16] = { 0x62, 0x72, 0x6F, 0x20, 0x67, 0x69, 0x76, 0x65, 0x20, 0x75, 0x70, 0x20, 0x6C, 0x6D, 0x61, 0x6F };
#else
				uint8_t WelcomeKey[16] = { 0x68, 0x61, 0x68, 0x61, 0x20, 0x6C, 0x75, 0x6E, 0x61, 0x20, 0x67, 0x6F, 0x20, 0x62, 0x72, 0x72 };
#endif

				Crypto::RC4(WelcomeKey, 0x10, Packet, Length, EncryptionHeaderSize);
			} else {
				Crypto::RC4(Process->m_Header->m_AuthToken + 0x10, 0x10, Packet, Length, EncryptionHeaderSize);
			}

			// XOR each byte 4x times with each byte of unencrypted m_Hash
			std::vector<uint8_t> HashBytes = Utils::ConvertIntegerToBytes(Process->m_Time);
			for (uint32_t i = EncryptionHeaderSize; i < Length; i++) {
				for (int j = 0; j < 4; j++) {
					Packet[i] ^= HashBytes[j];
				}

				Packet[i] ^= Process->m_Keys.m_XorKey;
			}

			// RC4 buffer with m_FinalKey
			Crypto::RC4(Process->m_Keys.m_FinalKey, sizeof(Process->m_Keys.m_FinalKey), Packet, Length, EncryptionHeaderSize);

			// RC4 (Modified) buffer with m_RequestKey
			Crypto::RC4Modified(Process->m_Header->m_EncryptionKeys.m_RequestKey, sizeof(Process->m_Header->m_EncryptionKeys.m_RequestKey), Packet, Length, EncryptionHeaderSize);
		}

		free(Process);
		Writer.Send(Packet, Length);
	}
}