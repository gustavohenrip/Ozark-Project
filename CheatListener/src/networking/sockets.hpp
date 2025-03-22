#pragma once
#include "stdafx.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

namespace Network {
	class Socket {
	public:
		Socket() {}
		Socket(uint16_t Port);
		Socket(int Socket);
		Socket(const char* Host, uint16_t Port);

		bool Send(uint8_t* Packet, ssize_t Length);
		bool Receive(uint8_t* Packet, ssize_t Length);

		bool SendError(uint8_t ErrorCode);
		void Close(std::vector<void*> Allocations = {});
		bool IsSocketConnected();

		bool Initialize();
		bool Listen();

		// Setters n Getters
		void SetIP(const char* IP) { strcpy(m_IP, IP); };
		const char* GetIP() { return m_IP; }
		uint16_t GetPort() { return m_Port; }
		int GetSocket() { return m_Socket; }
	private:
		int m_Socket;
		uint16_t m_Port;
		char m_IP[16];
	};

	Socket& GetServerSocket();
}