#pragma once
#include "stdafx.hpp"
#include "server/server_types.hpp"
#include <winsock.h>

namespace Auth::Sockets {
#define SOCKET_TIMEOUT 20000

	class Network {
	public:
		static bool Initialize();

		Network& Create(bool* Successful, const char** Error);
		Network& Connect(bool* Successful, const char** Error);
		Network& Send(void* Buffer, int Size);
		Network& Receive(Request::Header* Header, void* RecvBuffer, int Size, int* ReadBytes, bool* Successful, const char** Error);
	private:
		sockaddr_in m_ServerHandle;
		SOCKET m_SocketHandle;
	};
}