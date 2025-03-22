#include "sockets.hpp"
#include "utils/utils.hpp"

namespace Network {
#define SOCKET_TIMEOUT_SEND 10000
#define SOCKET_TIMEOUT_RECV 10000
#define min(a, b) ((a) < (b) ? (a) : (b))

	Socket::Socket(uint16_t Port) {
		m_Port = Port;
	}

	Socket::Socket(int Socket) {
		m_Socket = Socket;
	}

	Socket::Socket(const char* Host, uint16_t Port) {
		SetIP(Host);
		m_Port = Port;
	}

	bool Socket::Send(uint8_t* Packet, ssize_t Length) {
		ssize_t SendStatus = 0;
		ssize_t RemainingLength = Length;
		uint32_t StartTick = Utils::GetTickCount();

		char* CurrentData = (char*)Packet;

		while (RemainingLength > 0) {
			if ((Utils::GetTickCount() - StartTick) > SOCKET_TIMEOUT_SEND || !IsSocketConnected()) {
				return false;
			}

			size_t ChunkSize = min(2048, RemainingLength);
			SendStatus = send(m_Socket, CurrentData, ChunkSize, MSG_NOSIGNAL);
			
			if (SendStatus == -1 && errno != EWOULDBLOCK) {
				break;
			}

			CurrentData += SendStatus;
			RemainingLength -= SendStatus;
		}

		return SendStatus != -1;
	}

	bool Socket::Receive(uint8_t* Packet, ssize_t Length) {
		ssize_t RecvStatus = 0;
		ssize_t RemainingLength = Length;
		uint32_t StartTick = Utils::GetTickCount();

		char* CurrentData = (char*)Packet;

		while (RemainingLength > 0) {
			if ((Utils::GetTickCount() - StartTick) > SOCKET_TIMEOUT_RECV) {
				return false;
			}

			size_t ChunkSize = min(2048, RemainingLength);
			RecvStatus = recv(m_Socket, CurrentData, ChunkSize, MSG_NOSIGNAL);

			if (RecvStatus == -1 && errno != EWOULDBLOCK) {
				break;
			}

			CurrentData += RecvStatus;
			RemainingLength -= RecvStatus;

			if (RecvStatus == 0) {
				break;
			}
		}

		return RecvStatus != -1;
	}

	bool Socket::SendError(uint8_t ErrorCode) {
		uint8_t Packet[] = { 0x13, 0x37, 0x13, 0x37, ErrorCode };
		return Send(Packet, sizeof(Packet));
	}

	void Socket::Close(std::vector<void*> Allocations) {
		for (auto& Allocation : Allocations) {
			if (Allocation) {
				free(Allocation);
			}
		}

		Allocations.clear();
		close(m_Socket);
	}

	bool Socket::IsSocketConnected() {
		char Test = 0;

		ssize_t Status = recv(m_Socket, &Test, 1, MSG_PEEK | MSG_DONTWAIT);
		if (Status == -1 && errno != EWOULDBLOCK) {
			return false;
		}

		return true;
	}

	bool Socket::Initialize() {
		struct sockaddr_in Address = { 0 };
		
		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == -1) return false;

		Address.sin_family = AF_INET;
		Address.sin_port = htons(m_Port);

		struct hostent* Host = gethostbyname(m_IP);
		if (Host && Host->h_addrtype == AF_INET) {
			Address.sin_addr = *(in_addr*)Host->h_addr;
		}

		if (connect(m_Socket, (sockaddr*)&Address, sizeof(Address)) < 0) return false;
		return true;
	}

	bool Socket::Listen() {
		struct sockaddr_in Address = { 0 };

		m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == -1) return false;

		int Enable = 1;
		setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, &Enable, sizeof(Enable));

		Address.sin_family = AF_INET;
		Address.sin_port = htons(m_Port);
		Address.sin_addr.s_addr = INADDR_ANY;

		if (bind(m_Socket, (sockaddr*)&Address, sizeof(Address))) return false;
		if (listen(m_Socket, 4096)) return false;
		return true;
	}

	Socket& GetServerSocket() {
		static Socket Instance;
		return Instance;
	}
}