#include "ros.h"
#include "util/caller.h"
#include "global/vars.h"
#include "util/xml/pugixml.h"
#include "util/log.h"
#include "menu/base/util/textures.h"
#include "util/fiber_pool.h"
#include "ros_dll.h"

namespace rage::ros {
	bool decode_stats(const char* in, rage::network::read_stats_by_gamer* out_stats) {
		int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
			59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
			21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
			29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
			43, 44, 45, 46, 47, 48, 49, 50, 51 };

		uint8_t out[0x100];
		int size = 0;

		for (std::size_t i = 0, j = 0; i < strlen(in); i += 4, j += 3) {
			int v = b64invs[in[i] - 43];
			v = (v << 6) | b64invs[in[i + 1] - 43];
			v = in[i + 2] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 2] - 43];
			v = in[i + 3] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 3] - 43];

			size++;

			out[j] = (v >> 16) & 0xFF;
			if (in[i + 2] != '=') {
				out[j + 1] = (v >> 8) & 0xFF;
				size++;
			}

			if (in[i + 3] != '=') {
				out[j + 2] = v & 0xFF;
				size++;
			}
		}

		int real_size = 0;
		for (int i = 0; i < size; i++) {
			if (out[i] == 0) {
				real_size++;
				i += 8;
			}

			if (out[i] == 4) {
				real_size++;
				i += 4;
			}
		}

		if (real_size == (sizeof(rage::network::read_stats_by_gamer) / 4)) {
			int process_id = 0;
			for (int i = 0; i < size; i++) {
				if (out[i] == 0) {
					rage::network::stat_response_item* item = (rage::network::stat_response_item*) & out[i];
					if (process_id == 0) {
						out_stats->m_bank_balance = item->value.m_uint;
					} else if (process_id == 1) {
						out_stats->m_wallet_balance = item->value.m_uint;
					} else if (process_id == 2) {
						out_stats->m_rank = item->value.m_uint;
					} else if (process_id == 3) {
						out_stats->m_kills = item->value.m_uint;
					} else if (process_id == 4) {
						out_stats->m_deaths = item->value.m_uint;
					}

					real_size++;
					i += 8;
				} else if (out[i] == 4) {
					if (process_id == 0) {
						out_stats->m_bank_balance = *(uint32_t*)(&out[i + 1]);
					} else if (process_id == 1) {
						out_stats->m_wallet_balance = *(uint32_t*)(&out[i + 1]);
					} else if (process_id == 2) {
						out_stats->m_rank = *(uint32_t*)(&out[i + 1]);
					} else if (process_id == 3) {
						out_stats->m_kills = *(int*)(&out[i + 1]);
					} else if (process_id == 4) {
						out_stats->m_deaths = *(int*)(&out[i + 1]);
					}

					real_size++;
					i += 4;
				}

				process_id++;
			}

			out_stats->m_bank_balance = _byteswap_ulong(out_stats->m_bank_balance);
			out_stats->m_wallet_balance = _byteswap_ulong(out_stats->m_wallet_balance);
			out_stats->m_rank = _byteswap_ulong(out_stats->m_rank);
			out_stats->m_kills = _byteswap_ulong(out_stats->m_kills);
			out_stats->m_deaths = _byteswap_ulong(out_stats->m_deaths);
			return true;
		}

		return false;
	}

	void send_stat_requests(uint32_t* targets, int count) {
		if (global::vars::g_unloading) return;

		rage::ros::read_stats_state* state = new read_stats_state();

		std::vector<uint32_t> non_dupes;
		for (int i = 0; i < count; i++) {
			auto vit = std::find_if(begin(global::vars::g_read_stats_by_gamer_results), end(global::vars::g_read_stats_by_gamer_results), [=](std::unordered_map<uint64_t, rage::network::read_stats_by_gamer>::value_type& elem) { return elem.first == targets[i]; });
			if (vit == end(global::vars::g_read_stats_by_gamer_results)) {
				non_dupes.push_back(targets[i]);
			} else {
				if (vit->second.m_rank == 0) {
					non_dupes.push_back(targets[i]);
				}
			}
		}

		if (non_dupes.empty()) {
			delete state;
			return;
		}

		state->m_gamers = (uint64_t)malloc(4 * non_dupes.size());
		state->m_gamers_count = (int)non_dupes.size();

		for (int i = 0; i < state->m_gamers_count; i++) {
			*(uint32_t*)(state->m_gamers + (i * sizeof(uint32_t))) = non_dupes[i];
		}

		state->m_stats = (uint64_t)malloc(sizeof(uint32_t) * 5);
		state->m_stats_count = 5;

		*(uint32_t*)(state->m_stats) = joaat("BANK_BALANCE");
		*(uint32_t*)(state->m_stats + 4) = joaat("MP0_WALLET_BALANCE");
		*(uint32_t*)(state->m_stats + 8) = joaat("MP0_CHAR_RANK_FM");
		*(uint32_t*)(state->m_stats + 12) = joaat("MPPLY_KILLS_PLAYERS");
		*(uint32_t*)(state->m_stats + 16) = joaat("MPPLY_DEATHS_PLAYER");

		rage::ros::ros_state* ros_state = new rage::ros::ros_state();
		memcpy(ros_state, &global::vars::g_primary_ros_state, sizeof(rage::ros::ros_state)); // copy the keys

		if (paragoncheatsdotcom(ros_state, state)) {
			free((void*)state->m_gamers);
			free((void*)state->m_stats);
			delete state;

			if (ros_state->m_decrypted_response) {
				pugi::xml_document document;
				if (document.load_buffer(ros_state->m_decrypted_response, ros_state->m_decrypted_response_size).status == pugi::status_ok) {
					pugi::xml_node root = document.child("Response");
					if (root) {
						int status = root.child("Status").text().as_int();
						if (status == 1) {
							pugi::xml_node results = root.child("Results");
							if (results) {
								int result_count = results.attribute("count").as_int();
								if (result_count > 0) {
									for (pugi::xml_node& child : results) {
										std::string sc_id = child.attribute("gh").as_string();
										if (!sc_id.empty()) {
											uint64_t rockstar_id = std::stoull(sc_id.substr(3));
											if (rockstar_id) {
												std::string encoded_stats = child.child_value();
												if (!encoded_stats.empty()) {
													rage::network::read_stats_by_gamer stats;
													if (decode_stats(encoded_stats.c_str(), &stats)) {
														global::vars::g_read_stats_by_gamer_results[rockstar_id] = stats;
														// LOG_DEV("%i has %i in the bank", rockstar_id, stats.m_bank_balance);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}

				if (ros_state->m_decrypted_response) delete[] ros_state->m_decrypted_response;
			}

			delete ros_state;
			return;
		}

		free((void*)state->m_gamers);
		free((void*)state->m_stats);
		delete state;
		delete ros_state;
	}

	void download_player_image(uint32_t rid) {
		if (global::vars::g_unloading) return;

		if (std::find_if(begin(menu::textures::get_list()), end(menu::textures::get_list()), [=](menu::textures::texture_context& elem) {return elem.m_name == std::string("playerimg_" + std::to_string(rid)); }) != end(menu::textures::get_list())) return;

		rage::ros::ros_state* ros_state = new rage::ros::ros_state();
		memcpy(ros_state, &global::vars::g_primary_ros_state, sizeof(rage::ros::ros_state)); // copy the keys

		if (gtaaresdotcom(ros_state, rid)) {
			global::vars::g_player_image_queue.push_back({ rid, ros_state->m_decrypted_response });
		}

		delete ros_state;
	}
}