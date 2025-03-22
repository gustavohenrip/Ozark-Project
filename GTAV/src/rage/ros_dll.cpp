#include "ros_dll.h"
#include "global/vars.h"
#include "util/caller.h"
#include "util/util.h"
#include "auth/http/curl.h"
#include "rage/ros.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>

namespace base64 {
	static const char g_base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	uint32_t get_encoded_size(uint32_t raw_size) {
		uint32_t base_size = raw_size;

		if ((raw_size & 3) != 0) {
			base_size += 3 - (raw_size % 3);
		}

		base_size /= 3;
		base_size *= 4;
		return 4 * ((raw_size + 2) / 3);
	}

	uint32_t get_decoded_size(uint32_t raw_size) {
		return (raw_size / 4) * 3;
	}

	uint8_t* decode(uint8_t* raw_buffer, uint32_t raw_size, uint32_t* out_size) {
		int offsets[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
			59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
			21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
			29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
			43, 44, 45, 46, 47, 48, 49, 50, 51 };

		uint32_t size = get_decoded_size(raw_size);
		if (raw_buffer[raw_size - 1] == '=') size--;
		if (raw_buffer[raw_size - 2] == '=') size--;

		if (out_size) {
			*out_size = size;
		}

		uint8_t* decoded = new uint8_t[size];

		for (uint32_t i = 0, j = 0; i < raw_size; i += 4, j += 3) {
			int v = offsets[raw_buffer[i] - 43];
			v = (v << 6) | offsets[raw_buffer[i + 1] - 43];
			v = raw_buffer[i + 2] == '=' ? v << 6 : (v << 6) | offsets[raw_buffer[i + 2] - 43];
			v = raw_buffer[i + 3] == '=' ? v << 6 : (v << 6) | offsets[raw_buffer[i + 3] - 43];

			decoded[j] = (uint8_t)((v >> 16) & 0xFF);

			if (raw_buffer[i + 2] != '=') {
				decoded[j + 1] = (uint8_t)((v >> 8) & 0xFF);
			}

			if (raw_buffer[i + 3] != '=') {
				decoded[j + 2] = (uint8_t)(v & 0xFF);
			}
		}

		return decoded;
	}

	uint8_t* encode(uint8_t* raw_buffer, uint32_t raw_size, uint32_t* out_size) {
		if (out_size) *out_size = get_encoded_size(raw_size);
		uint8_t* encoded = new uint8_t[get_encoded_size(raw_size)];

		for (uint32_t i = 0, j = 0; i < raw_size; i += 3, j += 4) {
			int v = raw_buffer[i];
			v = i + 1 < raw_size ? v << 8 | raw_buffer[i + 1] : v << 8;
			v = i + 2 < raw_size ? v << 8 | raw_buffer[i + 2] : v << 8;

			encoded[j] = g_base64_chars[(v >> 18) & 0x3F];
			encoded[j + 1] = g_base64_chars[(v >> 12) & 0x3F];

			if (i + 1 < raw_size) {
				encoded[j + 2] = g_base64_chars[(v >> 6) & 0x3F];
			} else {
				encoded[j + 2] = '='; // padding
			}

			if (i + 2 < raw_size) {
				encoded[j + 3] = g_base64_chars[v & 0x3F];
			} else {
				encoded[j + 3] = '='; // padding
			}
		}

		// make sure no mem leakz cuz!!
		return encoded;
	}

	std::string encode_to_string(uint8_t* raw_buffer, uint32_t raw_size) {
		uint32_t total_size = 0;
		uint8_t* encoded = encode(raw_buffer, raw_size, &total_size);

		std::string _return;
		_return.assign(encoded, encoded + total_size);

		delete[] encoded;
		return _return;
	}
}

namespace encoding {
	std::string encode(const std::string& value) {
		std::ostringstream escaped;
		escaped.fill('0');
		escaped << std::hex;

		for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
			std::string::value_type c = (*i);

			// Keep alphanumeric and other accepted characters intact
			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
				continue;
			}

			if (c == ' ') {
				escaped << '+';
				continue;
			}

			// Any other characters are percent-encoded
			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << int((unsigned char)c);
			escaped << std::nouppercase;
		}

		return escaped.str();
	}

	std::string url_encode(std::unordered_map<std::string, std::string> map) {
		std::string encoded = "";

		for (auto& m : map) {
			if (m.first == XOR("data")) {
				// Compression doesn't use encoding!
				encoded += m.first + XOR("=") + m.second + XOR("&");
			} else {
				encoded += m.first + XOR("=") + encode(m.second) + XOR("&");
			}
		}

		return encoded.substr(0, encoded.length() - 1);
	}
}

namespace rc4_ros {
	struct rc4_state {
		uint8_t S[0x100];
		uint8_t I;
		uint8_t J;
	};

	void set_key(rc4_state* state, uint8_t* raw_buffer, uint32_t raw_size) {
		for (int i = 0; i < 256; i++) {
			state->S[i] = (uint8_t)i;
		}

		state->I = 0;
		state->J = 0;

		int j = 0;

		for (int i = 0; i < 256; i++) {
			j = (j + state->S[i] + raw_buffer[i % raw_size]) % 256;

			uint8_t temp = state->S[i];
			state->S[i] = state->S[j];
			state->S[j] = temp;
		}
	}

	uint8_t* get_final(rc4_state* state, uint8_t* raw_buffer, uint32_t raw_size, uint32_t offset) {
		uint8_t* output = new uint8_t[raw_size];
		memset(output, 0, raw_size);

		int i = (int)state->I;
		int j = (int)state->J;

		for (uint32_t n = offset; n < raw_size; n++) {
			i = (i + 1) % 256;
			j = (j + state->S[i]) % 256;

			uint8_t temp = state->S[i];
			state->S[i] = state->S[j];
			state->S[j] = temp;

			uint8_t K = state->S[(state->S[i] + state->S[j]) % 256];
			output[n] = (uint8_t)(raw_buffer[n] ^ K);
		}

		return output;
	}

	uint8_t* get_final_with_drop(rc4_state* state, uint8_t* raw_buffer, uint32_t raw_size, uint32_t drop) {
		uint8_t* output = new uint8_t[raw_size];
		memset(output, 0, raw_size);

		int i = (int)state->I;
		int j = (int)state->J;

		for (uint32_t n = 0; n < drop + raw_size; n++) {
			i = (i + 1) % 256;
			j = (j + state->S[i]) % 256;

			uint8_t temp = state->S[i];
			state->S[i] = state->S[j];
			state->S[j] = temp;

			if (n >= drop) {
				uint8_t K = state->S[(state->S[i] + state->S[j]) % 256];
				output[n - drop] = (uint8_t)(raw_buffer[n - drop] ^ K);
			}
		}

		return output;
	}
}

namespace sha1_ros {
	struct sha1_state {
		char _0x0000[0x500];
	};

	void init(sha1_state* state) {
		caller::call<void>(global::vars::g_crypto_state.m_sha1_init, state);
	}

	void transform(sha1_state* state, uint8_t* key, uint32_t key_len) {
		caller::call<void>(global::vars::g_crypto_state.m_sha1_transform, state, key, key_len);
	}

	void update(sha1_state* state, uint8_t* data, uint32_t data_len) {
		caller::call<void>(global::vars::g_crypto_state.m_sha1_update, state, data, data_len);
	}

	void finalize(sha1_state* state, uint8_t* output) {
		caller::call<void>(global::vars::g_crypto_state.m_sha1_final, state, output);
	}
}

namespace ros_ {
	void create_challenge(ros_state* state) {
		uint8_t bytes[0x8];
		util::create_random_bytes(bytes, sizeof(bytes));
		state->m_challenge = base64::encode_to_string(bytes, sizeof(bytes));
	}

	uint8_t* encrypt_request(ros_state* state, uint8_t* encoded_body, uint32_t* raw_size, bool use_security) {
		uint8_t rc4_key[0x10];
		uint8_t rc4_key_clone[0x10];
		uint8_t session_key[0x10];

		std::vector<uint8_t> output;

		// generate seeded rc4 key
		util::create_random_bytes(rc4_key, sizeof(rc4_key));

		if (use_security) {
			uint32_t out_size = 0;
			uint8_t* decoded = base64::decode((uint8_t*)state->m_session_key, (uint32_t)strlen(state->m_session_key), &out_size);

			memcpy(session_key, decoded, 0x10);
			delete[] decoded;
		}

		// backup rc4 key before encrypting it
		memcpy(rc4_key_clone, rc4_key, sizeof(rc4_key));

		for (int i = 0; i < sizeof(rc4_key); i++) {
			output.push_back(rc4_key[i] ^ state->m_platform_xor[i]);

			if (use_security) {
				rc4_key_clone[i] ^= session_key[i];
			}
		}

		// setup the rc4 key
		rc4_ros::rc4_state rc4;
		rc4_ros::set_key(&rc4, rc4_key_clone, sizeof(rc4_key_clone));

		// encrypt encoded data
		encoded_body = rc4_ros::get_final(&rc4, encoded_body, *raw_size, 0);

		// write the encoded data
		for (uint32_t i = 0; i < *raw_size; i++) {
			output.push_back(encoded_body[i]);
		}

		sha1_ros::sha1_state sha1_state;
		uint8_t sha1_hash[0x14];

		if (!use_security) {
			sha1_ros::init(&sha1_state);
			sha1_ros::update(&sha1_state, output.data(), (uint32_t)output.size());
			sha1_ros::update(&sha1_state, state->m_platform_hashing, sizeof(state->m_platform_hashing));
			sha1_ros::finalize(&sha1_state, sha1_hash);
		} else {
			// sha1 with transform (hmac)
			sha1_ros::init(&sha1_state);
			sha1_ros::transform(&sha1_state, rc4_key_clone, sizeof(rc4_key_clone));
			sha1_ros::update(&sha1_state, output.data(), (uint32_t)output.size());
			sha1_ros::update(&sha1_state, state->m_platform_hashing, sizeof(state->m_platform_hashing));
			sha1_ros::finalize(&sha1_state, sha1_hash);
		}

		for (int i = 0; i < sizeof(sha1_hash); i++) {
			output.push_back(sha1_hash[i]);
		}

		*raw_size = (uint32_t)output.size();

		uint8_t* data = new uint8_t[output.size()];
		memcpy(data, output.data(), output.size());
		return data;
	}

	std::string get_version_string() {
		std::string byte_string = XOR("e=1,t=gta5,p=pcros,v=11");

		// Create the buffer
		uint8_t* buffer = new uint8_t[byte_string.length() + 4];

		// Copy the string
		memcpy(&buffer[4], byte_string.data(), byte_string.length());

		// Generate a xor key
		uint8_t xor_key[4];
		util::create_random_bytes(xor_key, sizeof(xor_key));

		// Copy the xor key into the buffer
		memcpy(buffer, xor_key, sizeof(xor_key));

		// Xor the string
		for (int i = 4; i < byte_string.length() + 4; i++) {
			buffer[i] ^= xor_key[i & 3];
		}

		return XOR("ros ") + base64::encode_to_string(buffer, (uint32_t)byte_string.length() + 4);
	}

	std::string create_headers_hmac(ros_state* state, std::string url, bool post = true) {
		uint8_t key[0x10];
		uint32_t total_size = 0;

		uint8_t* xor_key = base64::decode((uint8_t*)state->m_session_key, (uint32_t)strlen(state->m_session_key), &total_size);

		for (int i = 0; i < sizeof(key); i++) {
			key[i] = xor_key[i] ^ state->m_platform_xor[i];
		}

		sha1_ros::sha1_state sha1_state;
		sha1_ros::transform(&sha1_state, key, sizeof(key));

		uint8_t zero[1] = { 0x0 };

		// method
		if (post) {
			sha1_ros::update(&sha1_state, (uint8_t*)std::string(XOR("POST")).data(), 4);
		} else sha1_ros::update(&sha1_state, (uint8_t*)std::string(XOR("GET")).data(), 3);
		sha1_ros::update(&sha1_state, zero, 1);

		// endpoint
		sha1_ros::update(&sha1_state, (uint8_t*)url.data(), (uint32_t)url.length());
		sha1_ros::update(&sha1_state, zero, 1);

		// ros-SecurityFlags
		sha1_ros::update(&sha1_state, (uint8_t*)std::string(XOR("239")).data(), 3);
		sha1_ros::update(&sha1_state, zero, 1);

		// ros-SessionTicket
		sha1_ros::update(&sha1_state, (uint8_t*)state->m_session_ticket, (uint32_t)strlen(state->m_session_ticket));
		sha1_ros::update(&sha1_state, zero, 1);

		// ros-Challenge
		sha1_ros::update(&sha1_state, (uint8_t*)state->m_challenge.data(), (uint32_t)state->m_challenge.length());
		sha1_ros::update(&sha1_state, zero, 1);

		// update with the hashing key
		sha1_ros::update(&sha1_state, state->m_platform_hashing, sizeof(state->m_platform_hashing));

		uint8_t hash[0x14];
		sha1_ros::finalize(&sha1_state, hash);

		delete[] xor_key;

		return base64::encode_to_string(hash, sizeof(hash));
	}

	std::unordered_map<std::string, std::string> create_headers(ros_state* state, std::string url, bool post = true) {
		std::unordered_map<std::string, std::string> headers;

		headers[XOR("user-Agent")] = get_version_string();
		headers[XOR("ros-SecurityFlags")] = XOR("239");
		headers[XOR("ros-SessionTicket")] = state->m_session_ticket;
		headers[XOR("ros-Challenge")] = state->m_challenge;
		headers[XOR("ros-HeadersHmac")] = create_headers_hmac(state, url, post);
		headers[XOR("Content-Type")] = XOR("application/x-www-form-urlencoded; charset=utf8");
		return headers;
	}

	uint8_t* decrypt_response(ros_state* state, uint8_t* response, uint32_t* raw_size, bool use_security) {
		uint8_t rc4_key[0x10];
		uint8_t session_key[0x10];

		if (use_security) {
			uint8_t* decoded = base64::decode((uint8_t*)state->m_session_key, (uint32_t)strlen(state->m_session_key), nullptr);
			memcpy(session_key, decoded, 0x10);
			delete[] decoded;
		}

		for (int i = 0; i < sizeof(rc4_key); i++) {
			rc4_key[i] = (uint8_t)(response[i] ^ state->m_platform_xor[i]);

			if (use_security) {
				rc4_key[i] ^= session_key[i];
			}
		}

		uint8_t block_size_bytes[4];
		memcpy(block_size_bytes, response + 0x10, sizeof(block_size_bytes));

		rc4_ros::rc4_state rc4;
		rc4_ros::set_key(&rc4, rc4_key, sizeof(rc4_key));

		uint8_t* block_size_decypted = rc4_ros::get_final(&rc4, block_size_bytes, sizeof(block_size_bytes), 0);
		int block_size = (block_size_decypted[3] << 0) | (block_size_decypted[2] << 8) | (block_size_decypted[1] << 16) | (block_size_decypted[0] << 24);
		delete[] block_size_decypted;

		std::vector<uint8_t> blocks;

		if (block_size > 1024 || block_size < 0) {
			// no blocks, straight rc4
			uint8_t* full_block = new uint8_t[*raw_size - 0x10 - 0x14]; // Minus header + hash, cba checking hash
			memcpy(full_block, response + 0x10, *raw_size - 0x10 - 0x14);

			rc4_ros::set_key(&rc4, rc4_key, sizeof(rc4_key));
			int S = *raw_size;
			*raw_size = S - 0x10 - 0x14;
			return rc4_ros::get_final(&rc4, full_block, S - 0x10 - 0x14, 0);
		} else {
			block_size += 20;

			uint32_t start = 20;
			while (true) {
				uint32_t length = std::min(*raw_size, start + block_size) - start - 20;

				uint8_t* block = new uint8_t[length];
				memcpy(block, response + start, length);

				for (uint32_t i = 0; i < length; i++) {
					blocks.push_back(block[i]);
				}

				start += block_size;
				if (start >= *raw_size) break;
			}
		}

		*raw_size = (int)blocks.size();
		rc4_ros::set_key(&rc4, rc4_key, sizeof(rc4_key));
		return rc4_ros::get_final_with_drop(&rc4, blocks.data(), (uint32_t)blocks.size(), 4);
	}
}

namespace request_ros {
	namespace memory {
		struct write_memory_chunk {
			uint8_t* m_memory;
			std::size_t m_size = 0;
		};

		struct read_memory_chunk {
			uint8_t* m_memory;
			std::size_t m_size_left;
			std::size_t m_size_offsetted;
		};

		std::size_t write_memory_callback(void* contents, std::size_t size, std::size_t bytes, void* user) {
			if (!is_valid_ptr(user)) return 0;
			if (!is_valid_ptr(contents)) return 0;

			std::size_t read_size = size * bytes;
			write_memory_chunk* memory = (write_memory_chunk*)user;

			uint8_t* ptr = (uint8_t*)realloc(memory->m_memory, memory->m_size + read_size);
			if (!ptr) {
				return 0;
			}

			memory->m_memory = ptr;
			memcpy(&(memory->m_memory[memory->m_size]), contents, read_size);
			memory->m_size += read_size;
			return read_size;
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

		std::size_t seek_memory_callback(void* user, curl_off_t offset, int origin) {
			read_memory_chunk* memory = (read_memory_chunk*)user;
			memory->m_memory -= memory->m_size_offsetted;
			memory->m_memory += offset;
			return CURL_SEEKFUNC_OK;
		}
	}

	struct request_state {
		CURL* m_curl;
		curl_slist* m_header_list;
		memory::read_memory_chunk m_read_chunk;
		memory::write_memory_chunk m_write_chunk;

		request_state(uint8_t* send_data, std::size_t send_size) {
			m_write_chunk.m_memory = (uint8_t*)malloc(1);
			m_read_chunk.m_memory = send_data;
			m_read_chunk.m_size_left = send_size;
		}
	};

	void setup_context(request_state* out) {
		out->m_curl = curl_easy_init();

		curl_easy_setopt(out->m_curl, CURLOPT_READFUNCTION, memory::read_memory_callback);
		curl_easy_setopt(out->m_curl, CURLOPT_READDATA, &out->m_read_chunk);

		curl_easy_setopt(out->m_curl, CURLOPT_WRITEFUNCTION, memory::write_memory_callback);
		curl_easy_setopt(out->m_curl, CURLOPT_WRITEDATA, &out->m_write_chunk);
	}

	void set_url(request_state* request_state, std::string endpoint, bool post = true) {
		curl_easy_setopt(request_state->m_curl, CURLOPT_URL, endpoint.c_str());
		curl_easy_setopt(request_state->m_curl, CURLOPT_POST, post);
		curl_easy_setopt(request_state->m_curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(request_state->m_curl, CURLOPT_SSL_VERIFYHOST, false);
	}

	void set_headers(request_state* request_state, std::unordered_map<std::string, std::string> headers) {
		request_state->m_header_list = nullptr;
		request_state->m_header_list = curl_slist_append(request_state->m_header_list, XOR("Expect:")); // remove this header
		request_state->m_header_list = curl_slist_append(request_state->m_header_list, XOR("Accept:")); // remove this header

		for (std::pair<std::string, std::string> h : headers) {
			request_state->m_header_list = curl_slist_append(request_state->m_header_list, (h.first + XOR(": ") + h.second).c_str());
		}

		curl_easy_setopt(request_state->m_curl, CURLOPT_HTTPHEADER, request_state->m_header_list);
	}

	CURLcode send(request_state* request_state) {
		CURLcode result = curl_easy_perform(request_state->m_curl);
		curl_easy_cleanup(request_state->m_curl);
		curl_slist_free_all(request_state->m_header_list);
		return result;
	}
}

bool paragoncheatsdotcom(ros_state* state, read_stats_state* stats) {
	std::string url = XOR("http://ps-gta5-prod.ros.rockstargames.com/gta5/11/gameservices/ProfileStats.asmx/ReadStatsByGamer2");

	uint32_t* stat_buffer = new uint32_t[stats->m_stats_count];
	for (int i = 0; i < stats->m_stats_count; i++) {
		stat_buffer[i] = _byteswap_ulong(*(uint32_t*)(stats->m_stats + (i * 4)));
	}

	std::string gamer_handles = "";
	for (int i = 0; i < stats->m_gamers_count; i++) {
		gamer_handles += XOR("SC ") + std::to_string(*(uint32_t*)(stats->m_gamers + (i * 4)));
		if (i != stats->m_gamers_count - 1) {
			gamer_handles += XOR(",");
		}
	}

	std::unordered_map<std::string, std::string> fields;
	fields[XOR("ticket")] = state->m_ticket;
	fields[XOR("gamerHandles")] = gamer_handles;
	fields[XOR("statIds")] = base64::encode_to_string((uint8_t*)stat_buffer, stats->m_stats_count * 4);

	delete[] stat_buffer;

	std::string encoded_body = encoding::url_encode(fields);

	uint32_t total_size = (uint32_t)encoded_body.length();
	uint8_t* encrypted_body = ros_::encrypt_request(state, (uint8_t*)encoded_body.data(), &total_size, true);

	ros_::create_challenge(state);

	std::unordered_map<std::string, std::string> headers;
	headers = ros_::create_headers(state, url.substr(url.find(XOR("gta5/")) - 1));
	headers[XOR("Content-Length")] = std::to_string(total_size);

	request_ros::request_state context(encrypted_body, (std::size_t)total_size);
	request_ros::setup_context(&context);
	request_ros::set_url(&context, url);
	request_ros::set_headers(&context, headers);

	CURLcode code = request_ros::send(&context);
	if (code != CURLE_OK) {
		delete[] encrypted_body;
		free(context.m_write_chunk.m_memory);
		return false;
	}

	delete[] encrypted_body;

	total_size = (uint32_t)context.m_write_chunk.m_size;
	state->m_decrypted_response = ros_::decrypt_response(state, context.m_write_chunk.m_memory, &total_size, true);
	state->m_decrypted_response_size = total_size;

	free(context.m_write_chunk.m_memory);
	return true;
}

bool gtaaresdotcom(ros_state* state, uint32_t rockstar_id) {
	std::string url = XOR("http://prod.ros.rockstargames.com/cloud/11/cloudservices/members/sc/") + std::to_string(rockstar_id) + XOR("/share/gta5/mpchars/0.dds");

	ros_::create_challenge(state);

	std::unordered_map<std::string, std::string> headers;
	headers = ros_::create_headers(state, url.substr(url.find(XOR("cloud/")) - 1), false);
	headers[XOR("Scs-Ticket")] = state->m_ticket;

	request_ros::request_state context(nullptr, 0);
	request_ros::setup_context(&context);
	request_ros::set_url(&context, url, false);
	request_ros::set_headers(&context, headers);

	CURLcode code = request_ros::send(&context);
	if (code != CURLE_OK) {
		free(context.m_write_chunk.m_memory);
		return false;
	}

	uint32_t total_size = (uint32_t)context.m_write_chunk.m_size;
	state->m_decrypted_response = ros_::decrypt_response(state, context.m_write_chunk.m_memory, &total_size, true);

	if (total_size == 16512) {
		state->m_decrypted_response_size = total_size;
		return true;
	}

	delete[] state->m_decrypted_response;
	return false;
}