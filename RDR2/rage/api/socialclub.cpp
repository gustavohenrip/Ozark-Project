#include "socialclub.hpp"
#include "auth/http/win_http_client.hpp"
#include "utils/va.hpp"
#include "utils/utils.hpp"
#include "utils/log.hpp"
#include "global/variables.hpp"
#include "utils/thread_pool.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "src/menu/base/util/fiber_manager.hpp"

namespace rage::api {
	int cooldown = 0;

	void socialclub::requestThread() {
		Sleep(1000);

		if (queue.size()) {
			for (std::size_t i = 0; i < queue.size(); i++) {
				auto item = queue[i];

				httpClient client(std::wstring(utils::va::vaw(item.first.first.c_str())));

				client.SetAdditionalDataToSend(reinterpret_cast<BYTE*>(const_cast<char*>(item.first.second.c_str())), (unsigned int)item.first.second.size());
				client.SetRequireValidSslCertificates(true);

				std::vector<std::pair<std::string, std::string>> additionalHeaders;
				additionalHeaders.emplace_back("User-Agent"_Protect, utils::createRandomString(10));
				additionalHeaders.emplace_back("X-Requested-With"_Protect, "XMLHttpRequest"_Protect);
				additionalHeaders.emplace_back("Authorization"_Protect, utils::va::va("SCAUTH val=\"%s\""_Protect, (const char*)(global::vars::g_rockstarTicketInfo)));
				additionalHeaders.emplace_back("Content-Type"_Protect, "application/json; charset=utf-8"_Protect);
				additionalHeaders.emplace_back("Content-Length"_Protect, std::to_string(item.first.second.size()));

				std::wstring headers = L"";
				for (const auto& header : additionalHeaders) {
					headers += std::wstring(utils::va::vaw("%s: %s\r\n"_Protect, header.first.c_str(), header.second.c_str()));
				}

				client.SetAdditionalRequestHeaders(headers);
				client.SendHttpRequest(std::wstring(utils::va::vaw("POST"_Protect)));
				if (item.second) item.second(client.GetResponseContent());

				queue.erase(queue.begin() + i);
				Sleep(4000);
			}
		}
	}

	void socialclub::send(std::string url, std::string data, std::function<void(std::wstring)> d) {
		queue.emplace_back(std::make_pair(url, data), d);
	}

	void socialclub::getProfileResponseFromName(std::string name, int pageIndex, std::function<void(bool, std::wstring)> callback) {
		if (name.empty()) return;

		menu::getNotify()->right("Getting RID from name, please wait...");
		send("https://scui.rockstargames.com/api/friend/accountsearch"_Protect,
			nlohmann::json({
				{ "env"_Protect, "prod"_Protect },
				{ "title"_Protect, "rdr2"_Protect },
				{ "version"_Protect, 11 },
				{ "searchNickname"_Protect, name },
				{ "pageIndex"_Protect, pageIndex }
				}).dump(), [=] (std::wstring response) {
			if (!nlohmann::json::accept(response)) {
				menu::util::getFiberPool()->run([=] {
					callback(false, L"");
				});

				return;
			}

			menu::util::getFiberPool()->run([=] {
				callback(true, response);
			});
		});
	}

	void socialclub::getProfileFromName(std::string name, std::function<void(bool, rage::network::rockstarAccount)> callback) {
		if (name.empty()) return;

		menu::getNotify()->right("Getting RID from name, please wait...");
		send("https://scui.rockstargames.com/api/friend/accountsearch"_Protect,
			nlohmann::json({
				{ "env"_Protect, "prod"_Protect },
				{ "title"_Protect, "rdr2"_Protect },
				{ "version"_Protect, 11 },
				{ "searchNickname"_Protect, name },
				{ "pageIndex"_Protect, 0 }
				}).dump(), [=] (std::wstring response) {
			if (!nlohmann::json::accept(response)) {
				menu::util::getFiberPool()->run([=] {
					callback(false, {});
				});

				return;
			}

			try {
				auto json = nlohmann::json::parse(response);

				if (json["Status"_Protect].is_null() || !json["Status"_Protect].get<bool>()) {
					menu::util::getFiberPool()->run([=] {
						callback(false, {});
					});

					return;
				}

				if (json["Accounts"_Protect].is_null() || json["Accounts"_Protect].get<nlohmann::json::array_t>().size() == 0) {
					menu::util::getFiberPool()->run([=] {
						callback(false, {});
					});

					return;
				}

				int total = json["Total"].is_null() ? 1 : json["Total"].get<int>();
				json = json["Accounts"_Protect][0];

				menu::util::getFiberPool()->run([=] {
					callback(true, rage::network::rockstarAccount(safeRead<std::string>(json, "Nickname"_Protect), json["RockstarId"_Protect].get<int>(), total));
				});
			} catch (...) {
				menu::util::getFiberPool()->run([=] {
					callback(false, {});
				});
			}
		});
	}

	void socialclub::searchProfiles(std::string name, std::function<void(bool, std::vector<std::pair<std::string, uint64_t>>)> callback) {
		if (name.empty()) return;

		menu::util::getFiberPool()->run([=] {

			int nextOffset = 0;
			std::vector<std::pair<std::string, uint64_t>> accounts;
			bool doUpdate = false;
			bool isDone = false;
		update:
			send("https://scui.rockstargames.com/api/friend/accountsearch"_Protect,
				nlohmann::json({
					{ "env"_Protect, "prod"_Protect },
					{ "title"_Protect, "rdr2"_Protect },
					{ "version"_Protect, 11 },
					{ "searchNickname"_Protect, name },
					{ "pageIndex"_Protect, nextOffset }
					}).dump(), [&](std::wstring response) {
						if (!nlohmann::json::accept(response)) {
							menu::util::getFiberPool()->run([=] {
								callback(false, {});
								});

							return;
						}
						auto json = nlohmann::json::parse(response);

						int total = json["Total"].is_null() ? 1 : json["Total"].get<int>();

						int jsonNextOffset = json["NextOffset"_Protect].get<int>();

						printf("next offset: %i\n", jsonNextOffset);

						auto responseAccountSize = json["Accounts"_Protect].get<nlohmann::json::array_t>().size();

						if (responseAccountSize) {
							accounts.resize((size_t)responseAccountSize + accounts.size());
						}

						//if (responseAccountSize == 10) {
						//	printf("resizing[pog] 1\n");
						//	accounts.resize((size_t)jsonNextOffset);
						//	printf("resizing[pog] 2\n");
						//}
						//else {
						//	printf("resizing[pog2] 1\n");
						//	accounts.resize((size_t)total);
						//	printf("resizing[pog2] 2\n");
						//}
						//std::vector<std::pair<std::string, uint64_t>> accounts(json["Accounts"_Protect].get<nlohmann::json::array_t>().size());
						//accounts.resize
						int count = 0;
						std::for_each(accounts.begin(), accounts.end(), [&](std::pair<std::string, uint64_t>& data) {
							//printf("%s %lli\n", safeRead<std::string>(json["Accounts"_Protect][count], "Nickname"_Protect).c_str(), json["Accounts"_Protect][count]["RockstarId"_Protect].get<int>());
							if (data.second == 0) {
								data = std::make_pair(safeRead<std::string>(json["Accounts"_Protect][count], "Nickname"_Protect),
									json["Accounts"_Protect][count]["RockstarId"_Protect].get<int>());
								count++;
							}
							});
						if (accounts.size() == total || accounts.size() >= 100) {
							menu::util::getFiberPool()->run([&] {
								callback(true, accounts);
								isDone = true;
							});
						}
						else {
							printf("next offset: %i | account.size(): %zi | total: %i\n", nextOffset, accounts.size(), total);
							nextOffset = jsonNextOffset;
							doUpdate = true;
							return;
						}
				});
			while (!isDone) {
				menu::util::waitFiber(20);
				if (doUpdate) {
					doUpdate = false;
					goto update;
				}
			}
		});
	}

	void socialclub::getProfileFromRID(uint64_t rockstarId, std::function<void(bool, rage::network::rockstarAccount)> callback) {
		if (rockstarId == 0) return;

		send("https://scui.rockstargames.com/api/friend/getprofile"_Protect, 
			nlohmann::json({
				{ "RockstarId"_Protect, rockstarId }
			}).dump(), [=] (std::wstring response) {
			if (!nlohmann::json::accept(response)) {
				menu::util::getFiberPool()->run([=] {
					callback(false, {});
				});

				return;
			}

			try {
				auto json = nlohmann::json::parse(response);

				if (json["Status"_Protect].is_null() || !json["Status"_Protect].get<bool>()) {
					menu::util::getFiberPool()->run([=] {
						callback(false, {});
					});

					return;
				}

				if (json["Accounts"_Protect].is_null() || json["Accounts"_Protect].get<nlohmann::json::array_t>().size() == 0) {
					menu::util::getFiberPool()->run([=] {
						callback(false, {});
					});

					return;
				}

				int total = json["Total"].is_null() ? 1 : json["Total"].get<int>();
				json = json["Accounts"_Protect][0]["RockstarAccount"_Protect];

				menu::util::getFiberPool()->run([=] {
					callback(true, rage::network::rockstarAccount(safeRead<std::string>(json, "Name"_Protect), rockstarId, total));
				});
			} catch (...) {
				menu::util::getFiberPool()->run([=] {
					callback(false, {});
				});
			}
		});
	}

	socialclub* getSocialclub() {
		static socialclub instance;
		return &instance;
	}
}