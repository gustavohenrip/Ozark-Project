#include "curl.h"

static std::unordered_map<uint32_t, CURL*> g_curl;

namespace curl {
	void load() {
		static bool loaded = false;
		if (!loaded) {
			loaded = true;
			curl_global_init(CURL_GLOBAL_ALL);
		}
	}

	namespace memory {
		std::size_t write_memory_callback(void* contents, std::size_t size, std::size_t bytes, void* user) {
			if (!is_valid_ptr(user)) return 0;
			if (!is_valid_ptr(contents)) return 0;

			((std::string*)user)->append((char*)contents, size * bytes);
			return size * bytes;

			/*std::size_t read_size = size * bytes;
			write_memory_chunk* memory = (write_memory_chunk*)user;

			uint8_t* ptr = (uint8_t*)realloc(memory->m_memory, memory->m_size + read_size);
			if (!ptr) {
				return 0;
			}

			memory->m_memory = ptr;
			memcpy(&(memory->m_memory[memory->m_size]), contents, read_size);
			memory->m_size += read_size;
			return read_size;*/
		}

		std::size_t read_memory_callback(void* contents, std::size_t size, std::size_t bytes, void* user) {
			std::size_t read_size = size * bytes;
			read_memory_chunk* memory = (read_memory_chunk*)user;

			if (memory->m_size_left) {
				std::size_t copy_count = memory->m_size_left;
				if (copy_count > read_size) copy_count = read_size;
				memcpy(contents, memory->m_memory, copy_count);

				memory->m_memory += copy_count;
				memory->m_size_left -= copy_count;
				memory->m_size_offsetted += copy_count;
				return copy_count;
			}

			return 0;
		}
	}

	request_state::request_state(uint8_t* send_data, std::size_t send_size) {
		m_read_chunk.m_memory = send_data;
		m_read_chunk.m_size_left = send_size;
	}

	void setup_context(request_state* out) {
		load();

		if (g_curl[GetCurrentThreadId()] == nullptr) {
			g_curl[GetCurrentThreadId()] = curl_easy_init();
		}

		out->m_curl = g_curl[GetCurrentThreadId()];
		out->m_header_list = nullptr;

		if (!out->m_curl) return;

		curl_easy_setopt(out->m_curl, CURLOPT_READFUNCTION, memory::read_memory_callback);
		curl_easy_setopt(out->m_curl, CURLOPT_READDATA, &out->m_read_chunk);

		curl_easy_setopt(out->m_curl, CURLOPT_WRITEFUNCTION, memory::write_memory_callback);
		curl_easy_setopt(out->m_curl, CURLOPT_WRITEDATA, &out->m_write_chunk);
	}

	void set_url(request_state* request_state, std::string endpoint, bool post) {
		if (!request_state->m_curl) return;

		curl_easy_setopt(request_state->m_curl, CURLOPT_URL, endpoint.c_str());
		curl_easy_setopt(request_state->m_curl, CURLOPT_POST, post);
		curl_easy_setopt(request_state->m_curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(request_state->m_curl, CURLOPT_SSL_VERIFYHOST, false);
	}

	void set_headers(request_state* request_state, std::unordered_map<std::string, std::string> headers) {
		if (!request_state->m_curl) return;

		for (std::pair<std::string, std::string> h : headers) {
			request_state->m_header_list = curl_slist_append(request_state->m_header_list, (h.first + XOR(": ") + h.second).c_str());
		}

		curl_easy_setopt(request_state->m_curl, CURLOPT_HTTPHEADER, request_state->m_header_list);
	}

	CURLcode send(request_state* request_state) {
		if (!request_state->m_curl) return CURLE_SEND_ERROR;

		CURLcode result = curl_easy_perform(request_state->m_curl);
		if (request_state->m_header_list) curl_slist_free_all(request_state->m_header_list);
		return result;
	}
}