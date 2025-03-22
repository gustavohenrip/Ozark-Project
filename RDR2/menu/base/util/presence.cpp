#include "presence.hpp"
#include "rage/engine.hpp"
#include "global/auth_vars.hpp"
#include "utils/va.hpp"
#include "utils/log.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/memory/pattern.hpp"
#include "rage/session/session.hpp"
#include "rage/invoker/natives.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"

namespace menu {
	presenceInfo presence::decodeHandle(const char* in) {
		int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
			59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
			21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
			29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
			43, 44, 45, 46, 47, 48, 49, 50, 51 };

		BYTE out[32];

		for (std::size_t i = 0, j = 0; i < strlen(in); i += 4, j += 3) {
			int v = b64invs[in[i] - 43];
			v = (v << 6) | b64invs[in[i + 1] - 43];
			v = in[i + 2] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 2] - 43];
			v = in[i + 3] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 3] - 43];

			out[j] = (v >> 16) & 0xFF;
			if (in[i + 2] != '=')
				out[j + 1] = (v >> 8) & 0xFF;

			if (in[i + 3] != '=')
				out[j + 2] = v & 0xFF;
		}

		presenceInfo info;
		memcpy(&info, out, sizeof(info));
		return info;
	}

	std::string presence::buildHandleString() {
		BYTE payload[16];
		memset(payload, 0, 16);
		payload[10] = 0x3;
		*(uint64_t*)(payload) = global::auth::g_rockstarId;

		const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		auto getBaseSize = [] (std::size_t inlen) -> std::size_t {
			std::size_t ret = inlen;
			if (inlen % 3 != 0)
				ret += 3 - (inlen % 3);

			ret /= 3;
			ret *= 4;
			return ret;
		};

		std::string out;
		out.resize(getBaseSize(sizeof(payload)));

		for (std::size_t i = 0, j = 0; i < sizeof(payload); i += 3, j += 4) {
			std::size_t v = payload[i];
			v = i + 1 < sizeof(payload) ? v << 8 | payload[i + 1] : v << 8;
			v = i + 2 < sizeof(payload) ? v << 8 | payload[i + 2] : v << 8;

			out[j] = b64chars[(v >> 18) & 0x3F];
			out[j + 1] = b64chars[(v >> 12) & 0x3F];
			if (i + 1 < sizeof(payload)) {
				out[j + 2] = b64chars[(v >> 6) & 0x3F];
			} else {
				out[j + 2] = '=';
			}

			if (i + 2 < sizeof(payload)) {
				out[j + 3] = b64chars[v & 0x3F];
			} else {
				out[j + 3] = '=';
			}
		}

		return out;
	}

	std::string presence::buildHandleString(uint64_t rid) {
		BYTE payload[16];
		memset(payload, 0, 16);
		payload[10] = 0x3;
		*(uint64_t*)(payload) = rid;

		const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		auto getBaseSize = [](std::size_t inlen) -> std::size_t {
			std::size_t ret = inlen;
			if (inlen % 3 != 0)
				ret += 3 - (inlen % 3);

			ret /= 3;
			ret *= 4;
			return ret;
		};

		std::string out;
		out.resize(getBaseSize(sizeof(payload)));

		for (std::size_t i = 0, j = 0; i < sizeof(payload); i += 3, j += 4) {
			std::size_t v = payload[i];
			v = i + 1 < sizeof(payload) ? v << 8 | payload[i + 1] : v << 8;
			v = i + 2 < sizeof(payload) ? v << 8 | payload[i + 2] : v << 8;

			out[j] = b64chars[(v >> 18) & 0x3F];
			out[j + 1] = b64chars[(v >> 12) & 0x3F];
			if (i + 1 < sizeof(payload)) {
				out[j + 2] = b64chars[(v >> 6) & 0x3F];
			} else {
				out[j + 2] = '=';
			}

			if (i + 2 < sizeof(payload)) {
				out[j + 3] = b64chars[v & 0x3F];
			} else {
				out[j + 3] = '=';
			}
		}

		return out;
	}

	bool presence::sendRockstarMessage(uint64_t id, const char* msg) {
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info->rockstarID = id;
		info->type = 3;
		bool ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"TextMessage\",\"d\":{\"ps\":\"%s\",\"gh\":\"%s\"}}}}"_Protect, msg, buildHandleString().c_str())).c_str());
		//bool ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"rockstar_message\",\"d\":{\"msg\":\"%s\"}}}}"_Protect, msg)).c_str());
		delete info;
		return ret;
	}

	bool presence::sendCrewMessage(uint64_t id, const char* msg, const char* crewTag, int crewId) {
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info->rockstarID = id;
		info->type = 3;

		bool ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"crew_message\",\"d\":{\"msg\":\"%s\",\"crewId\":%i,\"crewTag\":\"%s\"}}}}"_Protect, msg, crewId, crewTag)).c_str());
		delete info;
		return ret;
	}

	bool presence::joinMe(uint64_t id) {
		bool ret = 0;
#if defined(DEV) || defined(TESTER)
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		auto netGamePlayer = rage::engine::getHostNetGamePlayer();
		//auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
		if (netGamePlayer) {
			auto playerInfo = netGamePlayer->playerInfo;
			if (info) {
				info->rockstarID = playerInfo->netInfo.rockstarId2;
				info->type = 3;
				ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"RsInviteResponse\",\"d\":{\"i\":\"%s\",\"n\":\"%s\",\"u\":1,\"if\":8,\"rf\":0,\"p\":0,\"cr\":0,\"ct\":\"(_nO\",\"dm\":true,\"d\":true}}}}"_Protect, buildHandleString(id).c_str(), "1337")).c_str());
			}
		}
		//bool ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"RsAdminJoinEvent\",\"d\":{\"tgt\":\"%s\",\"d\":0,\"s\":0,\"t\":0}}}}"_Protect, buildHandleString().c_str())).c_str());
		delete info;
#endif
		return ret;
	}

	bool presence::forceRidIntoSession(uint64_t rid, uint64_t toRid) {
		bool ret = 0;
#if defined(DEV) || defined(TESTER)
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info->rockstarID = rid;
		info->type = 3;
		ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"RsInviteResponse\",\"d\":{\"i\":\"%s\",\"n\":\"%s\",\"u\":1,\"if\":8,\"rf\":0,\"p\":0,\"cr\":0,\"ct\":\"(_nO\",\"dm\":true,\"d\":true}}}}"_Protect, buildHandleString(152804745).c_str(), "-sa7an")).c_str());
		//bool ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"RsAdminJoinEvent\",\"d\":{\"tgt\":\"%s\",\"d\":0,\"s\":0,\"t\":0}}}}"_Protect, buildHandleString(toRid).c_str())).c_str());
		delete info;
#endif // DEV
		return ret;
	}

	bool presence::kickPlayer(uint64_t rid) {
#if defined(DEV) || defined(TESTER)
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info->rockstarID = rid;
		info->type = 3;
		bool ret = rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"RsInviteResponse\",\"d\":{\"i\":\"%s\",\"n\":\"%s\",\"u\":0,\"if\":8,\"rf\":0,\"p\":0,\"cr\":0,\"ct\":\"(_nO\",\"dm\":true,\"d\":true}}}}"_Protect, buildHandleString(rid).c_str(), "0x52613323"_Protect)).c_str());
		delete info; 
		return ret;
#endif // DEV
		return 0;
	}

	void presence::gameTriggerEvent(uint64_t rid) {
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info->rockstarID = rid;
		info->type = 3;
		auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
		if (netGamePlayer) {
			auto playerInfo = netGamePlayer->playerInfo;
			if (info) {
				//info->rockstarID = playerInfo->netInfo.rockstarId2;
				//"ros.publish":{"channel":"self", "msg" : {"gm.evt":{"e":"RsJoinRequest", "d" : {"j":"YSSqAgAAAAAAAAMAAAAAAA==", "n" : "ICUCyberSec", "u" : 1884394201, "f" : 8257, "d" : 427032674, "a" : 0}}}}
				//rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"AssetDataRequest\",\"d\":{\"gh\":\"%s\"}}}}"_Protect, buildHandleString(playerInfo->netInfo.rockstarId2).c_str())).c_str());
				//rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"RsSessionServerInfoRequest\",\"d\":{\"gh\":\"%s\",\"f\":4}}}}"_Protect, buildHandleString(playerInfo->netInfo.rockstarId2).c_str())).c_str());
				//rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.posse.event\":{\"h\":\"SC %lli\",\"n\":\"%s\",\"s\":\"%s\"}"_Protect, from, "Ozark"_Protect, rage::engine::getGameInfoString("gsinfo"_Protect))).c_str());
			}
		}
		//rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"gtri\",\"d\":{\"event\":%i,\"p1\":1,\"p2\":1,\"ps\":\"idkwhatimdoing\"}}}}"_Protect, 2)).c_str());
		delete info;
	}

	void presence::joinThem(uint64_t id) {
		uint64_t get_u_table = memory::getMemory()->getAddressFromInstruction(global::vars::g_joinPlayerU + 0x1A);
		uint64_t get_u = memory::getMemory()->getAddressFromInstruction(global::vars::g_joinPlayerU + 0x24, 1, 5);

		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info("Attempting to join %i", id);
		info->rockstarID = id;
		info->type = 3;
#if defined(DEV) || defined(TESTER)
		auto netGamePlayer = rage::engine::getNetGamePlayerFromIndex(menu::getLocalPlayer().id);
		if (netGamePlayer) {
			auto playerInfo = netGamePlayer->playerInfo;
			if (playerInfo) {
				info->rockstarID = id;
				info->type = 3;
				rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.publish\":{\"channel\":\"self\",\"msg\":{\"gm.evt\":{\"e\":\"RsInviteResponse\",\"d\":{\"i\":\"%s\",\"n\":\"%s\",\"u\":1,\"if\":8,\"rf\":0,\"p\":0,\"cr\":0,\"ct\":\"(_nO\",\"dm\":true,\"d\":true}}}}"_Protect, buildHandleString(playerInfo->netInfo.rockstarId2).c_str(), "0x52613323"_Protect)).c_str());
			}
		}
#else
		caller::call<int>(global::vars::g_joinPlayer, memory::getMemory()->getAddressFromInstruction(global::vars::g_joinPlayerTable), info, caller::call<int>(get_u, get_u_table), 1);
#endif
		delete info;
	}

	bool presence::invitePlayer(uint64_t to, uint64_t from) {
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info->rockstarID = to;
		info->type = 3;
		//ros.gamer.richpresence":{"h":"SC 128380389","p":"Playing Red Dead Online: Roaming in Valentine"}}
		rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.mp.invite\":{\"h\":\"SC %lli\",\"n\":\"%s\",\"s\":\"%s\"}"_Protect, from, "Ozark"_Protect, rage::engine::getGameInfoString("gsinfo"_Protect))).c_str());
		delete info;
		return 0;
	}

	bool presence::socialclubMessageNotify(uint64_t to, std::string name) {
		presenceInfo* info = new presenceInfo();
		memset(info, 0, sizeof(presenceInfo));
		info->rockstarID = to;
		info->type = 3;
		rage::engine::triggerPresence(info, std::string(utils::va::va("\"ros.gamer.textmessage\":{\"h\":\"SC %lli\",\"n\":\"%s\"}"_Protect, 666, name.c_str())).c_str());
		delete info;
		return 0;
	}

	//{"gm.evt":{"e":"RsInviteUpdateEvent","d":{"i":"luctBwAAAAAAAAMAAAAAAA==","u":1,"if":8,"rf":8}}}

	bool presence::refreshSession(std::function<void()> callback) {
		static bool hasFinished = true;
		if (hasFinished) {
			auto host = menu::getPlayer(rage::network::getNetworkTask()->getSessionHost());
			uint64_t joinbackRid = 0;
			if (host.id != menu::getLocalPlayer().id) {
				joinbackRid = rage::network::getNetworkTask()->getNetPlayerInfo(rage::network::getNetworkTask()->getSessionHost())->rockstarId2;
			} else {
				joinbackRid = rage::network::getNetworkTask()->getNetPlayerInfo(1)->rockstarId2;
			}

			menu::util::getFiberPool()->run([=] {
				if (native::networkIsSessionPrivate()) {
					callback();
				}
				else {
					hasFinished = false;
					static char sessionID[0x30];
					memset(sessionID, 0, sizeof(sessionID));
					native::createPrivateSession(17, 1, 1, sessionID);
					menu::util::waitFiber(3000);
					callback(); // call out function
					//menu::util::waitFiber(2000);
					menu::getPresence()->joinThem(joinbackRid);
					hasFinished = true;
				}
			});
			return true;
		}
		else {
			return false;
		}
	}

	presence* getPresence() {
		static presence instance;
		return &instance;
	}
}