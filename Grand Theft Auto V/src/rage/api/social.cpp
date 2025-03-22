#include "social.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/json.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/va.hpp"
#include "utils/utils.hpp"
#include "global/variables.hpp"
#include "auth/http/win_http_client.hpp"

namespace Rage::API {
	void SocialClub::RequestThread() {
		if (m_Queue.size()) {
			auto& Item = m_Queue.front();
			HttpClient Client(std::wstring(Utils::VA::VAW(Item.first.first.c_str())));

			Client.SetAdditionalDataToSend(reinterpret_cast<BYTE*>(const_cast<char*>(Item.first.second.c_str())), (unsigned int)Item.first.second.size());
			Client.SetRequireValidSslCertificates(true);

			std::vector<std::pair<std::string, std::string>> AdditionalHeaders;
			AdditionalHeaders.emplace_back("User-Agent"_Protect, Utils::CreateRandomString(10));
			AdditionalHeaders.emplace_back("X-Requested-With"_Protect, "XMLHttpRequest"_Protect);
			AdditionalHeaders.emplace_back("Authorization"_Protect, Utils::VA::VA("SCAUTH val=\"%s\""_Protect, Global::Vars::g_RsInfo->m_ScsTicket));
			AdditionalHeaders.emplace_back("Content-Type"_Protect, "application/json; charset=utf-8"_Protect);
			AdditionalHeaders.emplace_back("Content-Length"_Protect, std::to_string(Item.first.second.size()));

			std::wstring Headers = L"";
			for (const auto& Header : AdditionalHeaders) {
				Headers += std::wstring(Utils::VA::VAW("%s: %s\r\n"_Protect, Header.first.c_str(), Header.second.c_str()));
			}

			Client.SetAdditionalRequestHeaders(Headers);
			Client.SendHttpRequest(std::wstring(Utils::VA::VAW("POST"_Protect)));
			if (Item.second) Item.second(Client.GetResponseContent());

			m_Queue.pop();

			Sleep(1000);
		}
	}

	void SocialClub::SendSocialClub(std::string URL, std::string Data, std::function<void(std::wstring)> Callback) {
		m_Queue.emplace(std::make_pair(URL, Data), Callback);
	}

	void SocialClub::GetProfileFromRockstarID(uint64_t RockstarID, std::function<void(bool, RockstarAccount)> Callback, bool Notify) {
		if (RockstarID < 0x1000) {
			Utils::GetFiberPool()->Push([=] {
				Callback(false, {});
			});

			return;
		}

		SendSocialClub("https://scui.rockstargames.com/api/friend/getprofile",
			nlohmann::json({
				{ "RockstarId", RockstarID }
				}).dump(), [=] (std::wstring JResponse) {
			if (!nlohmann::json::accept(JResponse)) {
				Utils::GetFiberPool()->Push([=] {
					Callback(false, {});
				});

				return;
			}

			try {
				nlohmann::json Json = nlohmann::json::parse(JResponse);

				if ((Json["Status"].is_null() || !Json["Status"].get<bool>()) || (Json["Accounts"].is_null() || Json["Accounts"].get<nlohmann::json::array_t>().size() == 0)) {
					Utils::GetFiberPool()->Push([=] {
						Callback(false, {});
					});

					return;
				}

				int Total = Json["Total"].is_null() ? 1 : Json["Total"].get<int>();

				if (Total > 1 && Notify) {
					Menu::GetNotify()->NotifyStacked("Multiple accounts found in query, using first", Global::UIVars::g_NotificationError);
				}

				Json = Json["Accounts"][0]["RockstarAccount"];

				Utils::GetFiberPool()->Push([=] {
					Callback(true, RockstarAccount(Json["Name"].is_null() ? "null" : Json["Name"].get<std::string>(), RockstarID, Total));
				});

				return;
			} catch (...) {
				Utils::GetFiberPool()->Push([=] {
					Callback(false, {});
				});
			}
		});
	}

	void SocialClub::GetProfileFromName(std::string Name, std::function<void(bool, RockstarAccount)> Callback) {
		if (Name.empty()) return;

		SendSocialClub("https://scui.rockstargames.com/api/friend/accountsearch",
			nlohmann::json({
				{ "env", "prod" },
				{ "title", "gta5" },
				{ "version", 11 },
				{ "searchNickname", Name },
				{ "pageIndex", 0 }
				}).dump(), [=] (std::wstring JResponse) {
			if (!nlohmann::json::accept(JResponse)) {
				Utils::GetFiberPool()->Push([=] {
					Callback(false, {});
				});

				return;
			}

			try {
				nlohmann::json Json = nlohmann::json::parse(JResponse);

				if ((Json["Status"].is_null() || !Json["Status"].get<bool>()) || (Json["Accounts"].is_null() || Json["Accounts"].get<nlohmann::json::array_t>().size() == 0)) {
					Utils::GetFiberPool()->Push([=] {
						Callback(false, {});
					});

					return;
				}

				int Total = Json["Total"].is_null() ? 1 : Json["Total"].get<int>();

				if (Total > 1) {
					Menu::GetNotify()->NotifyStacked("Multiple accounts found in query, using first", Global::UIVars::g_NotificationError);
				}

				Json = Json["Accounts"][0];

				Utils::GetFiberPool()->Push([=] {
					Callback(true, RockstarAccount(Json["Nickname"].is_null() ? "null" : Json["Nickname"].get<std::string>(), Json["RockstarId"].get<int>(), Total));
				});
			} catch (...) {
				Utils::GetFiberPool()->Push([=] {
					Callback(false, {});
				});
			}
		});
	}

	SocialClub* GetSocialClub() {
		static SocialClub Instance;
		return &Instance;
	}
}