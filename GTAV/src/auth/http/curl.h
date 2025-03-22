#pragma once
#include "stdafx.h"
#include <unordered_map>

#define CURL_STATICLIB
#include "../Dependencies/curl_ssl/curl.h"
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "cryptui.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcurl_b.lib")

namespace curl {
	namespace memory {
		struct write_memory_chunk {
			uint8_t* m_memory = nullptr;
			std::size_t m_size = 0;
		};

		struct read_memory_chunk {
			uint8_t* m_memory = nullptr;
			std::size_t m_size_left = 0;
			std::size_t m_size_offsetted = 0;
		};

		std::size_t write_memory_callback(void* contents, std::size_t size, std::size_t bytes, void* user);
		std::size_t read_memory_callback(void* contents, std::size_t size, std::size_t bytes, void* user);
	}

	struct request_state {
		CURL* m_curl = nullptr;
		curl_slist* m_header_list = nullptr;
		memory::read_memory_chunk m_read_chunk;
		// memory::write_memory_chunk m_write_chunk;
		std::string m_write_chunk = "";

		request_state() {}

		request_state(uint8_t* send_data, std::size_t send_size);
	};

	void setup_context(request_state* out);
	void set_url(request_state* request_state, std::string endpoint, bool post = true);
	void set_headers(request_state* request_state, std::unordered_map<std::string, std::string> headers);
	CURLcode send(request_state* request_state);
}