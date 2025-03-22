#include "sockets.h"
#include "crypto/crypto.h"
#include "global/auth_vars.h"
#include "util/util.h"
#include "util/log.h"
#include "security/PPCVM/virtual_machine.h"
#include "security/PPCVM/payloads.h"
#include <vector>

namespace auth::sockets {
#define SOCKET_TIMEOUT 20000

#pragma pack(push)
#pragma pack(1)
	struct rc4 {
		uint64_t m_key;
		uint32_t m_key_size;
		uint64_t m_data;
		uint32_t m_data_size;
		uint32_t m_offset;
	};
#pragma pack(pop)

	void rc4_vm(uint8_t* pbKey, uint32_t cbKey, uint8_t* pbInpOut, uint32_t cbInpOut, uint32_t startOffset = 0) {
#ifdef DEV_MODE
		return crypto::rc4(pbKey, cbKey, pbInpOut, cbInpOut, startOffset);
#endif

		security::vm::virtual_machine* machine = security::vm::get_core()->create_vm();
		if (machine) {
			rc4* input = new rc4();
			input->m_key = _byteswap_uint64((uint64_t)pbKey);
			input->m_key_size = _byteswap_ulong(cbKey);
			input->m_data = _byteswap_uint64((uint64_t)pbInpOut);
			input->m_data_size = _byteswap_ulong(cbInpOut);
			input->m_offset = _byteswap_ulong(startOffset);

			machine->execute(security::vm::payloads::payload_rc4, input);

			while (!machine->get_current_context()->complete) Sleep(0);

			delete machine;
			delete input;
		}
	}

	void rc4_modified_vm(uint8_t* pbKey, uint32_t cbKey, uint8_t* pbInpOut, uint32_t cbInpOut, uint32_t startOffset = 0) {
#ifdef DEV_MODE
		return crypto::rc4_modified(pbKey, cbKey, pbInpOut, cbInpOut, startOffset);
#endif

		security::vm::virtual_machine* machine = security::vm::get_core()->create_vm();
		if (machine) {
			rc4* input = new rc4();
			input->m_key = _byteswap_uint64((uint64_t)pbKey);
			input->m_key_size = _byteswap_ulong(cbKey);
			input->m_data = _byteswap_uint64((uint64_t)pbInpOut);
			input->m_data_size = _byteswap_ulong(cbInpOut);
			input->m_offset = _byteswap_ulong(startOffset);

			machine->execute(security::vm::payloads::payload_rc4_modified, input);

			while (!machine->get_current_context()->complete) Sleep(0);

			delete machine;
			delete input;
		}
	}

	void decrypt_keys(response::auth_encryption_keys* keys) {
		for (int i = 0; i < 0x10; i++) {
			keys->m_first_key[i] ^= 37;
			keys->m_first_key[i] ^= keys->xor_key;
			keys->m_first_key[i] ^= 13;
			keys->m_first_key[i] ^= keys->m_final_key[i];
		}

		for (int i = 0; i < 0x10; i++) {
			keys->m_second_key[i] ^= 69;
			keys->m_second_key[i] ^= keys->m_final_key[i];
			keys->m_second_key[i] ^= 69;
			keys->m_second_key[i] ^= keys->xor_key;
		}
	}

	void decrypt_hash(response::auth_encryption_keys* keys) {
		((uint8_t*)&keys->m_hash)[3] ^= keys->xor_key;

		keys->m_hash ^= 420;
		keys->m_hash ^= 1337;

		for (int i = 0; i < 0x10; i++) {
			keys->m_hash ^= keys->xor_key | keys->m_final_key[i];
			keys->m_hash ^= keys->xor_key ^ keys->m_final_key[i];
			keys->m_hash ^= keys->m_second_key[i];
			keys->m_hash ^= keys->m_first_key[i] << 16;
		}
	}

	void decrypt_payload(request::auth_header* header, uint8_t* bytes, uint32_t size, response::auth_encryption_keys* keys) {
		decrypt_keys(keys);
		decrypt_hash(keys);

		// rc4 (Modified) buffer with m_request_key
		rc4_modified_vm(header->m_encryption_keys.m_request_key, sizeof(header->m_encryption_keys.m_request_key), bytes, size);

		// rc4 buffer with m_final_key
		rc4_vm(keys->m_final_key, sizeof(keys->m_final_key), bytes, size);

		// XOR each byte 4x times with each byte of unencrypted m_hash
		std::vector<uint8_t> hash_bytes = util::convert_number_to_bytes(keys->m_hash);
		for (uint32_t i = 0; i < size; i++) {
			bytes[i] ^= keys->xor_key;

			for (int j = 0; j < 4; j++) {
				bytes[i] ^= hash_bytes[3 - j];
			}
		}

		// rc4 buffer with second 0x10 of m_AuthToken
		if (header->m_packet == PACKET_PING) {
			// PACKET_PING wont have an auth token just yet
			static uint8_t ping_key[16] = { 0x6C, 0x6F, 0x6C, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20, 0x66, 0x75, 0x6E, 0x20, 0x78, 0x64, 0x64 };
			rc4_vm(ping_key, 0x10, bytes, size);
		} else if (header->m_packet == PACKET_WELCOME) {
			// PACKET_WELCOME wont have an auth token just yet
			static uint8_t welcome_key[16] = { 0x62, 0x72, 0x6F, 0x20, 0x67, 0x69, 0x76, 0x65, 0x20, 0x75, 0x70, 0x20, 0x6C, 0x6D, 0x61, 0x6F };
			rc4_vm(welcome_key, 0x10, bytes, size);
		} else {
			rc4_vm(header->m_encryption_keys.m_token_key, 0x10, header->m_auth_token, sizeof(header->m_auth_token));
			rc4_vm(header->m_auth_token + 0x10, 0x10, bytes, size);
			rc4_vm(header->m_encryption_keys.m_token_key, 0x10, header->m_auth_token, sizeof(header->m_auth_token));
		}

		// XOR buffer with random key
		for (uint32_t i = 0; i < size; i++) {
			bytes[i] ^= keys->xor_key;
		}
	}

	bool Network::load_wsa() {
		static bool initialized = false;
		if (initialized) return initialized;

		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data)) return false;

		initialized = true;
		return true;
	}

	bool Network::create() {
		bool created = false;

		for (int i = 0; i < 10; i++) {
			m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_socket == SOCKET_ERROR) {
				LOG_CUSTOM_WARN(XOR("Network"), XOR("Failed to create connection (#1), attempt %i to reconect. Status: %X"), i + 1, GetLastError());
			} else {
				created = true;
				break;
			}

			Sleep(100);
		}

		if (!created) {
			m_last_error = XOR("Failed to create a connection");
		}

		return created;
	}

	bool Network::connect() {
		int size = 0x100000;
		setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF | SO_RCVBUF, (const char*)&size, sizeof(size));

		int timeout = SOCKET_TIMEOUT;
		setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO | SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(6968);

		/*address.sin_addr.S_un.S_un_b.s_b1 = 209;
		address.sin_addr.S_un.S_un_b.s_b2 = 141;
		address.sin_addr.S_un.S_un_b.s_b3 = 50;
		address.sin_addr.S_un.S_un_b.s_b4 = 21;*/

		address.sin_addr.S_un.S_un_b.s_b1 = 209;
		address.sin_addr.S_un.S_un_b.s_b2 = 141;
		address.sin_addr.S_un.S_un_b.s_b3 = 39;
		address.sin_addr.S_un.S_un_b.s_b4 = 192;

		bool connected = false;

		for (int i = 0; i < 10; i++) {
			if (::connect(m_socket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
				LOG_CUSTOM_WARN(XOR("Network"), XOR("Failed to create connection (#2), attempt %i to reconect. Status: %X"), i + 1, GetLastError());
			} else {
				connected = true;
				break;
			}

			Sleep(100);
		}

		if (!connected) {
			m_last_error = XOR("Failed to connect");
			closesocket(m_socket);
		} else {
			LOG_CUSTOM_DEV(XOR("Network"), XOR("Connected to server (%i.%i.%i.%i)"), address.sin_addr.S_un.S_un_b.s_b1, address.sin_addr.S_un.S_un_b.s_b2, address.sin_addr.S_un.S_un_b.s_b3, address.sin_addr.S_un.S_un_b.s_b4);
		}

		return connected;
	}

	void Network::send(void* buffer, int length) {
		char* current_data = (char*)buffer;
		int data_left = length;

		while (data_left > 0) {
			int send_status = ::send(m_socket, current_data, 2048 < data_left ? 2048 : data_left, 0);
			if (send_status == SOCKET_ERROR) break;

			current_data += send_status;
			data_left -= send_status;
		}
	}

	bool Network::receive(request::auth_header* header, void* buffer, int length, int* read_length, bool close_socket) {
		char* received_buffer = new char[length];
		int remaining = length;
		int received = 0;

		while (remaining > 0) {
			int Receive = ::recv(m_socket, (char*)received_buffer + received, 2048 < remaining ? 2048 : remaining, 0);

			if (Receive == SOCKET_ERROR) {
				m_last_error = XOR("Failed receiving from server");
				closesocket(m_socket);
				delete[] received_buffer;
				return false;
			}

			remaining -= Receive;
			received += Receive;

			if (Receive == 0) break;
		}

		if (read_length) *read_length = received;
		if (close_socket) closesocket(m_socket);

		if (received == 5) {
			if (*(int*)&received_buffer[0] == 0x13371337) {
				eSocketErrorCodes error_code = (eSocketErrorCodes)received_buffer[4];
				switch (error_code) {
					case ERROR_CANT_READ_MAGIC:
						m_last_error = XOR("Failed to read magic");
						break;

					case ERROR_INVALID_MAGIC:
						m_last_error = XOR("Failed to verify magic");
						break;

					case ERROR_CANT_READ_REQUIRED_HEADER:
						m_last_error = XOR("Failed to read header");
						break;

					case ERROR_INVALID_SIZE:
						m_last_error = XOR("Failed to verify size");
						break;

					case ERROR_INVALID_PACKET:
						m_last_error = XOR("Failed to verify packet");
						break;

					case ERROR_INVALID_CHEAT:
						m_last_error = XOR("Failed to verify cheat");
						break;

					case ERROR_CANT_READ_HEADER_DATA:
						m_last_error = XOR("Failed to read data");
						break;

					case ERROR_INVALID_PACKET_SIZE:
						m_last_error = XOR("Failed to verify packet size");
						break;

					case ERROR_AUTH_TOKEN_NOT_FOUND:
						m_last_error = XOR("Failed to verify auth token");
						break;

					case ERROR_ALLOCATING_MEMORY:
						m_last_error = XOR("Failed to allocate server memory for request (ouch)");
						break;
				}

				delete[] received_buffer;
				return false;
			}
		}

		if (received > sizeof(response::auth_encryption_keys)) {
			response::auth_encryption_keys* keys = (response::auth_encryption_keys*)received_buffer;
			char* new_bytes = received_buffer + sizeof(response::auth_encryption_keys);
			int new_size = length - sizeof(response::auth_encryption_keys);

			decrypt_payload(header, (uint8_t*)new_bytes, new_size, keys);
			memcpy((void*)((uint64_t)buffer + sizeof(response::auth_encryption_keys)), new_bytes, new_size);

			delete[] received_buffer;
			return true;
		} else {
			m_last_error = XOR("Failed receiving full from server");
		}

		delete[] received_buffer;
		return false;
	}

	const char* Network::get_error() {
		return m_last_error;
	}
}