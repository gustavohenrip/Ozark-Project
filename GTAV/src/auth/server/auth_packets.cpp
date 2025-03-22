#include "auth_packets.h"
#include "util/log.h"
#include "util/util.h"
#include "util/va.h"
#include "global/auth_vars.h"
#include "global/vars.h"
#include "auth/crypto/crypto.h"
#include "auth/sockets.h"
#include "util/fiber_pool.h"
#include "menu/base/util/notify.h"
#include <random>

static localization t_new_update("New Update", true, true);
static localization t_update_available("A new update is available to download!", true, true);

namespace auth::server {
	void populate_header(request::auth_header* header, uint8_t packet, uint32_t size) {
		LOG_CUSTOM_DEV(XOR("Auth"), XOR("Populating packet %i"), packet);

		header->m_magic = 0x434F434B;
		header->m_size = size;
		header->m_packet = packet;
		header->m_cheat.m_cheat = CHEAT_GRAND_THEFT_AUTO_V;
		header->m_cheat.m_cheat_version = VERSION;
		header->m_cheat.m_tester = false;

#ifdef TESTER_MODE
		header->m_cheat.m_tester = true;
#endif

		memcpy(header->m_hardware_id, global::auth::g_hardware_id, sizeof(global::auth::g_hardware_id));
		memcpy(header->m_auth_token, global::auth::g_auth_token, sizeof(global::auth::g_auth_token));

		std::random_device r;
		std::seed_seq seed2 { r(), r(), r(), r(), r(), r(), r(), r() };
		std::mt19937 rng(seed2);
		std::uniform_int_distribution<int> val(0, 255);

		header->m_encryption_keys.xor_key = (uint8_t)val(rng);
		util::create_random_bytes(header->m_encryption_keys.m_hardware_key, sizeof(header->m_encryption_keys.m_hardware_key));
		util::create_random_bytes(header->m_encryption_keys.m_token_key, sizeof(header->m_encryption_keys.m_token_key));
		util::create_random_bytes(header->m_encryption_keys.m_request_key, sizeof(header->m_encryption_keys.m_request_key));

		crypto::rc4(header->m_encryption_keys.m_hardware_key, sizeof(header->m_encryption_keys.m_hardware_key), header->m_hardware_id, sizeof(header->m_hardware_id));
		crypto::rc4(header->m_encryption_keys.m_token_key, sizeof(header->m_encryption_keys.m_token_key), header->m_auth_token, sizeof(header->m_auth_token));
	}

	void encrypt_payload(request::auth_header* header) {
		if (header->m_size > sizeof(request::auth_header)) {
			crypto::rc4_modified(header->m_encryption_keys.m_request_key, sizeof(header->m_encryption_keys.m_request_key), (uint8_t*)header, header->m_size, sizeof(request::auth_header));
		}
	}

	bool packet_ping() {
		if (!sockets::Network::load_wsa()) {
			LOG_CUSTOM_ERROR(XOR("Auth"), XOR("Failed to initialize"));
			return false;
		}

		request::auth_packet_ping* packet_ping = util::alloc<request::auth_packet_ping>();
		populate_header(packet_ping, PACKET_PING, sizeof(request::auth_packet_ping));
		response::auth_packet_ping* packet_ping_response = util::alloc<response::auth_packet_ping>();

		int received_bytes = 0;
		const char* error_message;

		sockets::Network network;

		if (network.create()) {
			if (network.connect()) {
				request::auth_packet_ping packet_ping_backup;
				memcpy(&packet_ping_backup, packet_ping, sizeof(packet_ping_backup));

				encrypt_payload(packet_ping);
				network.send(packet_ping, packet_ping->m_size);

				if (network.receive(&packet_ping_backup, packet_ping_response, sizeof(response::auth_packet_ping), &received_bytes)) {
					if (received_bytes == sizeof(response::auth_packet_ping)) {
						if (packet_ping_response->m_ping_test == 69) {
							free(packet_ping);
							free(packet_ping_response);
							return true;
						} else {
							error_message = util::va::va(XOR("PING failed security checks (%i)"), packet_ping_response->m_ping_test);
						}
					} else {
						error_message = util::va::va(XOR("Server didn't return enough data to pass security checks on %i"), packet_ping->m_packet);
					}
				}
			}
		}

		if (!error_message) error_message = network.get_error();

		if (error_message) {
			LOG_CUSTOM_ERROR(XOR("Auth"), error_message);
		}

		free(packet_ping);
		free(packet_ping_response);
		return false;
	}

	bool packet_welcome() {
		request::auth_packet_welcome* packet_welcome = util::alloc<request::auth_packet_welcome>();
		populate_header(packet_welcome, PACKET_WELCOME, sizeof(request::auth_packet_welcome));
		response::auth_packet_welcome* packet_welcome_response = util::alloc<response::auth_packet_welcome>();

		memcpy(packet_welcome->m_password_hash, util::convert_string_to_bytes(crypto::rotate(crypto::rotate_number_reverse(global::auth::g_password))).data(), 0x40);
		memcpy(packet_welcome->m_username, global::auth::g_username.c_str(), global::auth::g_username.length());

		/*DWORD ComputerNameSize = sizeof(packet_welcome->m_pc_name);
		if (GetUserNameA(packet_welcome->m_pc_name, &ComputerNameSize) == 0) {
			strcpy(packet_welcome->m_pc_name, "n/a");
		}*/
		strcpy(packet_welcome->m_pc_name, "n/a");

		int received_bytes = 0;
		const char* error_message;

		sockets::Network network;

		if (network.create()) {
			if (network.connect()) {
				request::auth_packet_welcome packet_welcome_backup;
				memcpy(&packet_welcome_backup, packet_welcome, sizeof(packet_welcome_backup));

				encrypt_payload(packet_welcome);
				network.send(packet_welcome, packet_welcome->m_size);

				if (network.receive(&packet_welcome_backup, packet_welcome_response, sizeof(response::auth_packet_welcome), &received_bytes)) {
					if (received_bytes == sizeof(response::auth_packet_welcome)) {
						memcpy(global::auth::g_auth_token, packet_welcome_response->m_auth_token, sizeof(packet_welcome_response->m_auth_token));

						switch (packet_welcome_response->m_response_status) {
							case RESPONSE_WELCOME_DISABLED_ACCOUNT:
								error_message = XOR("This cheat is currently disabled. Check our Discord for more information");
								break;

							case RESPONSE_WELCOME_NO_ACCOUNT:
								error_message = XOR("Please check your credentials");
								break;

							case RESPONSE_WELCOME_OUTDATED:
								error_message = XOR("This cheat is outdated");
								break;

							case RESPONSE_WELCOME_BANNED_ACCOUNT:
								error_message = XOR("Your account is banned. Check the website for more information");
								break;

							case RESPONSE_WELCOME_DISABLED:
								error_message = XOR("Your account is disabled. Check the website for more information");
								break;

							case RESPONSE_WELCOME_HWID_MISMATCH:
								error_message = XOR("Your HWID doesn't match. Please go to ozark.gg and reset it");
								break;

							case RESPONSE_WELCOME_PTB_DISABLED:
								error_message = XOR("PTB is currently disabled");
								break;

							case RESPONSE_SUCCESS:
								free(packet_welcome);
								free(packet_welcome_response);
								return true;
						}
					} else {
						error_message = util::va::va(XOR("Server didn't return enough data to pass security checks on %i"), packet_welcome->m_packet);
					}
				}
			}
		}

		if (!error_message) error_message = network.get_error();

		if (error_message) {
			LOG_CUSTOM_ERROR(XOR("Auth"), error_message);
		}

		free(packet_welcome);
		free(packet_welcome_response);
		return false;
	}

	bool packet_get_time() {
		request::auth_packet_get_time* packet_get_time = util::alloc<request::auth_packet_get_time>();
		populate_header(packet_get_time, PACKET_GET_TIME, sizeof(request::auth_packet_get_time));
		response::auth_packet_get_time* packet_get_time_response = util::alloc<response::auth_packet_get_time>();

		int received_bytes = 0;
		const char* error_message;

		sockets::Network network;

		if (network.create()) {
			if (network.connect()) {
				request::auth_packet_get_time packet_get_time_backup;
				memcpy(&packet_get_time_backup, packet_get_time, sizeof(packet_get_time_backup));

				encrypt_payload(packet_get_time);
				network.send(packet_get_time, packet_get_time->m_size);

				if (network.receive(&packet_get_time_backup, packet_get_time_response, sizeof(response::auth_packet_get_time), &received_bytes)) {
					if (received_bytes == sizeof(response::auth_packet_get_time)) {
#undef max
						global::auth::g_has_lifetime = packet_get_time_response->m_seconds_left == std::numeric_limits<int>::max();
						global::auth::g_seconds_left = packet_get_time_response->m_seconds_left;

						switch (packet_get_time_response->m_response_status) {
							case RESPONSE_ERROR:
								error_message = XOR("An error occured getting your subscriptions");
								break;

							case RESPONSE_SUCCESS:
								free(packet_get_time);
								free(packet_get_time_response);
								return true;
						}
					} else {
						error_message = util::va::va(XOR("Server didn't return enough data to pass security checks on %i"), packet_get_time->m_packet);
					}
				}
			}
		}

		if (!error_message) error_message = network.get_error();

		if (error_message) {
			LOG_CUSTOM_ERROR(XOR("Auth"), error_message);
		}

		free(packet_get_time);
		free(packet_get_time_response);
		return false;
	}

	bool packet_get_signatures() {
		request::auth_packet_get_signatures* packet_get_signatures = util::alloc<request::auth_packet_get_signatures>();
		populate_header(packet_get_signatures, PACKET_GET_SIGNATURES, sizeof(request::auth_packet_get_signatures));
		response::auth_packet_get_signatures_length* packet_get_signatures_response = util::alloc<response::auth_packet_get_signatures_length>();

		int received_bytes = 0;
		const char* error_message;

		sockets::Network network;

		if (network.create()) {
			if (network.connect()) {
				request::auth_packet_get_signatures packet_get_signatures_backup;
				memcpy(&packet_get_signatures_backup, packet_get_signatures, sizeof(packet_get_signatures_backup));

				encrypt_payload(packet_get_signatures);
				network.send(packet_get_signatures, packet_get_signatures->m_size);

				if (network.receive(&packet_get_signatures_backup, packet_get_signatures_response, sizeof(response::auth_packet_get_signatures_length), &received_bytes, false)) {
					if (received_bytes == sizeof(response::auth_packet_get_signatures_length)) {
						switch (packet_get_signatures_response->m_response_status) {
							case RESPONSE_ERROR:
								error_message = XOR("An error occured getting menu data (#1)");
								break;

							case RESPONSE_SUCCESS:
								uint32_t size = packet_get_signatures_response->m_length;
								uint8_t* data = new uint8_t[size + sizeof(auth::response::auth_encryption_keys)];

								if (network.receive(&packet_get_signatures_backup, data, size + sizeof(auth::response::auth_encryption_keys), &received_bytes)) {
									if (received_bytes == size + sizeof(auth::response::auth_encryption_keys)) {
										for (uint32_t i = 0; i < size; i++) {
											request::auth_base_signature* Signature = (request::auth_base_signature*)((uint64_t)data + sizeof(auth::response::auth_encryption_keys) + i);

											std::string SignatureStr = util::convert_bytes_to_string(&Signature->m_signature, Signature->m_signature_length, true);
											util::replace_string(SignatureStr, XOR("3F"), XOR("?"));
											global::auth::g_signatures.insert(std::make_pair(Signature->m_name, pattern_info(SignatureStr, Signature->m_offset, Signature->m_name, Signature->m_important)));

											i += sizeof(request::auth_base_signature) + Signature->m_signature_length - 2;
										}
									}
								}

								memset(data, 0, size);
								delete[] data;

								free(packet_get_signatures);
								free(packet_get_signatures_response);

								return true;
						}
					} else {
						error_message = util::va::va(XOR("Server didn't return enough data to pass security checks on %i"), packet_get_signatures->m_packet);
					}
				}
			}
		}

		if (!error_message) error_message = network.get_error();

		if (error_message) {
			LOG_CUSTOM_ERROR(XOR("Auth"), error_message);
		}

		free(packet_get_signatures);
		free(packet_get_signatures_response);
		return false;
	}

	bool packet_heartbeat() {
		request::auth_packet_heartbeat* packet_heartbeat = util::alloc<request::auth_packet_heartbeat>();
		populate_header(packet_heartbeat, PACKET_HEARTBEAT, sizeof(request::auth_packet_heartbeat));
		response::auth_packet_heartbeat* packet_heartbeat_response = util::alloc<response::auth_packet_heartbeat>();

		strcpy_s(packet_heartbeat->m_game_username, is_valid_ptr(global::vars::g_rs_info) ? global::vars::g_rs_info->m_name : "");
		strcpy_s(packet_heartbeat->m_game_build, crypto::rotate(global::auth::g_build).c_str());
		packet_heartbeat->m_rockstar_id = is_valid_ptr(global::vars::g_rs_info) ? global::vars::g_rs_info->m_rockstar_id : 0;

		int received_bytes = 0;
		const char* error_message;

		sockets::Network network;

		if (network.create()) {
			if (network.connect()) {
				request::auth_packet_heartbeat packet_heartbeat_backup;
				memcpy(&packet_heartbeat_backup, packet_heartbeat, sizeof(packet_heartbeat_backup));

				encrypt_payload(packet_heartbeat);
				network.send(packet_heartbeat, packet_heartbeat->m_size);

				if (network.receive(&packet_heartbeat_backup, packet_heartbeat_response, sizeof(response::auth_packet_heartbeat), &received_bytes)) {
					if (received_bytes == sizeof(response::auth_packet_heartbeat)) {
						switch (packet_heartbeat_response->m_response_status) {
							case RESPONSE_ERROR:
								error_message = XOR("An error occurred sending a heartbeat");
								break;

							case RESPONSE_SUCCESS:
#undef max
								global::auth::g_has_lifetime = packet_heartbeat_response->m_seconds_left == std::numeric_limits<int>::max();
								global::auth::g_seconds_left = packet_heartbeat_response->m_seconds_left;

								if (packet_heartbeat_response->m_update_available) {
									util::fiber::pool::add([] {
										menu::notify::stacked(TRANSLATE(t_new_update), TRANSLATE(t_update_available), global::ui::g_success, 10000);
									});
								}

								free(packet_heartbeat);
								free(packet_heartbeat_response);
								return true;
						}
					} else {
						error_message = util::va::va(XOR("Server didn't return enough data to pass security checks on %i"), packet_heartbeat->m_packet);
					}
				}
			}
		}

		if (!error_message) error_message = network.get_error();

		if (error_message) {
			LOG_CUSTOM_ERROR(XOR("Auth"), error_message);
		}

		free(packet_heartbeat);
		free(packet_heartbeat_response);
		return false;
	}
}