#pragma once
#include "stdafx.h"
#include "server/auth_types.h"
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")

namespace auth::sockets {
	class Network {
	public:
		bool create();
		bool connect();
		void send(void* buffer, int length);
		bool receive(request::auth_header* header, void* buffer, int length, int* read_length, bool close_socket = true);

		static bool load_wsa();
		const char* get_error();
	private:
		sockaddr_in m_server_handle;
		SOCKET m_socket;
		const char* m_last_error;
	};
}