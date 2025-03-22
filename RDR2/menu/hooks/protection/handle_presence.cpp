#include "../hooks.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/submenus/main/network/protection/protection.hpp"
#include "menu/base/submenus/main/network/overseer.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/log.hpp"
#include "utils/va.hpp"
#include "menu/base/util/presence.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "utils/memory/pattern.hpp"
#include "utils/caller.hpp"
#include "menu/base/util/presence.hpp"

bool menu::hooks::handlePresenceEventHook(uint64_t rcx, uint64_t rdx, presenceData* r8, uint64_t r9) {
	dev("Presence: %s", r8->presence);//presenceData

	//if (strstr(r8->presence, "RsSessionServerInfoResponse")) {
	//	printf("[RsSessionServerInfoResponse] %s\n", getStringFromJson(r8->presence, "uri").c_str());
	//	menu::getPresence()->idkwhatimdoing(171111310, getStringFromJson(r8->presence, "uri"));
	//	/*
	//		{"gm.evt":{"e":"RsInviteServerInfo","d":{"gh":"jvMyCgAAAAAAAAMAAAAAAA==","n":"RecklessOutlaws","i":"YSSqAgAAAAAAAAMAAAAAAA==","u":1,"f":8,"v":true,"uri":"wss://ewrsession3-rdr2-prod.ros.rockstargames.com:30213","ss":3596011127}}}
	//		{"gm.evt":{"e":"RsInviteRequestResultEvent","d":{"gh":"jvMyCgAAAAAAAAMAAAAAAA==","i":"YSSqAgAAAAAAAAMAAAAAAA==","r":1,"u":1,"f":8}}}
	//	
	//	*/
	//	//"wss://ewrsession3-rdr2-prod.ros.rockstargames.com:30213","ss":3596011127}}}
	//}


	return originalHandlePresenceEvent(rcx, rdx, r8, r9);
}

bool menu::hooks::inviteResponseHook(joinResponseData* rcx, uint64_t rdx) {
	auto ret = originalInviteResponse(rcx, rdx);
	if (rcx) {

#ifdef DEV
		if (!rcx->kickFlag) {
			return false;
		}
#endif // DEV

		if (!rcx->kickFlag && std::string("0x52613323"_Protect).compare(rcx->name)) { // filter potential network bail
			return false;
		}
	}
	return ret;
}

bool menu::hooks::presJoinRequestHook(joinRequestData* rcx, uint64_t rdx) {
	auto ret = originalPresJoinRequest(rcx, rdx);

	if (rcx) {
		bool isFriend = false;
		uint64_t incomingRid = rcx->rid;
		for (int i = 0; i < 250; i++) {
			uint64_t ptr = *(uint64_t*)(global::vars::g_friendData + (i * 8));
			if (ptr) {
				uint64_t rid = *(uint64_t*)ptr;
				if (rid == incomingRid) {
					isFriend = true;
					break;
				}
			}
		}
		if (incomingRid == 83765581 || incomingRid == 19541822) {
			return false;
		}
		if (networkProtectionMenuVars::vars.presenceJoin) {
			switch (networkProtectionMenuVars::vars.presenceJoinSelection) {
			case 0: // friends
				if (isFriend) {
					menu::getNotify()->right(std::string(utils::va::va("~e~Blocked~q~ friend ~pa~%s~q~ joining session"_Protect, rcx->name)));
					return false;
				}
				break;
			case 1: // rid joiners
				if (!isFriend) {
					menu::getNotify()->right(std::string(utils::va::va("~e~Blocked~q~ ~pa~%s~q~ joining session with rid joiner"_Protect, rcx->name)));
					return false;
				}
				break;
			case 2: // both
				menu::getNotify()->right(std::string(utils::va::va("~e~Blocked~q~ ~pa~%s~q~ joining session"_Protect, rcx->name)));
				return false;
			}
		}
		bool blockJoiner = false;
		if (networkOverseerMenuVars::vars.blockJoiners.size()) {
			std::for_each(networkOverseerMenuVars::vars.blockJoiners.begin(), networkOverseerMenuVars::vars.blockJoiners.end(), [&](std::string &data) {
				if (std::to_string(incomingRid) == data) {
					menu::getNotify()->right(std::string(utils::va::va("~e~Blocked~q~ ~pa~%s~q~ joining session"_Protect, rcx->name)));
					blockJoiner = true;
				}
			});
		}
		if (blockJoiner) {
			return false;
		}

		if (networkProtectionMenuVars::vars.presenceNotifyJoins) {
			menu::getNotify()->right(std::string(utils::va::va("~pa~%s~q~ is joining you (%s)"_Protect, rcx->name, isFriend ? "friend" : "using rid joiner")));
		}
		rcx->flag = 1;
	}

	return ret;
}

//inline bool(*originalPresenceJoin)(uint64_t, uint64_t);
//bool presenceJoin(uint64_t rcx, uint64_t rdx) {
//
//	auto ret = originalPresenceJoin(rcx, rdx);
//	if (rcx) {
//		printf("[presenceJoinRequest] changing flag %i\n", *(uint32_t*)(rcx + 0x34));
//
//
//
//		/*
//DEV | Presence: {"gm.evt":{"e":"RsJoinRequest","d":{"j":"Ri3CAAAAAAAAAAMAAAAAAA==","n":"notRancher","u":1183175428,"f":8397322,"d":2521614434,"a":0}}}
//[presenceJoinRequest] changing flag 1183175428
//[presenceJoinRequest] changed flag 1
//			*/
//		*(uint32_t*)(rcx + 0x34) = 1;
//		printf("[presenceJoinRequest] changed flag %i\n", *(uint32_t*)(rcx + 0x34));
//	}
//
//	return ret;
//}

//bool menu::hooks::handlePresenceEventHook(uint64_t rcx, uint64_t rdx, presenceData* r8, uint64_t r9) {
//	dev("Presence: %s", r8->presence);
//
//	try {
//		auto &data = nlohmann::json::parse(r8->presence);
//		auto key = "gm.evt";
//
//		if (data["gm.evt"].is_null()) {
//			if (data["gta5.game.event"].is_null()) {
//				return true;
//			}
//
//			key = "gta5.game.event";
//		}
//
//		auto& eventPayload = data[key];
//
//		if (eventPayload["e"].is_null()) {
//			return true;
//		}
//
//		auto presenceName = eventPayload["e"].get<std::string>();
//		auto presenceHash = native::getHashKey(eventPayload["e"].get<std::string>().c_str());
//
//		switch (presenceHash) {
//			case PRESENCE_JOIN_REQUEST: {
//				bool isFriend = false;
//				uint64_t incomingRid = menu::getPresence()->decodeHandle(eventPayload["d"]["j"].get<std::string>().c_str()).rockstarID;
//
//				for (int i = 0; i < 250; i++) {
//					uint64_t ptr = *(uint64_t*)(global::vars::g_friendData + (i * 8));
//					if (ptr) {
//						uint64_t rid = *(uint64_t*)ptr;
//						if (rid == incomingRid) {
//							isFriend = true;
//							break;
//						}
//					}
//				}
//
//				if (networkProtectionMenuVars::vars.presenceJoin) {
//					switch (networkProtectionMenuVars::vars.presenceJoinSelection) {
//						case 0: // friends
//							if (isFriend) {
//								menu::getNotify()->right(std::string(utils::va::va("~e~Blocked~q~ friend ~pa~%s~q~ joining session"_Protect, eventPayload["d"]["n"].get<std::string>().c_str())));
//								return true;
//							}
//							break;
//						case 1: // rid joiners
//							if (!isFriend) {
//								menu::getNotify()->right(std::string(utils::va::va("~e~Blocked~q~ ~pa~%s~q~ joining session with rid joiner"_Protect, eventPayload["d"]["n"].get<std::string>().c_str())));
//								return true;
//							}
//							break;
//						case 2: // both
//							menu::getNotify()->right(std::string(utils::va::va("~e~Blocked~q~ ~pa~%s~q~ joining session"_Protect, eventPayload["d"]["n"].get<std::string>().c_str())));
//							return true;
//					}
//				}
//
//				if (networkProtectionMenuVars::vars.presenceNotifyJoins) {
//					menu::getNotify()->right(std::string(utils::va::va("~pa~%s~q~ is joining you (%s)"_Protect, eventPayload["d"]["n"].get<std::string>().c_str(), isFriend ? "friend" : "using rid joiner")));
//				}
//
//				auto& d = eventPayload["d"];
//				if (!d.is_null()) {
//					d["f"] = 1;
//					strcpy(r8->presence, data.dump().c_str());
//				}
//
//				break;
//			}
//
//			case PRESENCE_JOIN_RESPONSE:
//			case PRESENCE_INVITE_REQUEST_RESULT_EVENT: {
//				auto& d = eventPayload["d"];
//				if (!d.is_null()) {
//					d["f"] = 1;
//					strcpy(r8->presence, data.dump().c_str());
//				}
//
//				break;
//			}
//
//			case PRESENCE_ROCKSTAR_MESSAGE:
//				if (networkProtectionMenuVars::vars.presenceRockstarMessage) {
//					menu::getNotify()->bottom("[Presence] Blocked rockstar message"_Protect);
//					return true;
//				}
//
//				break;
//
//			case PRESENCE_CREW_MESSAGE:
//				if (networkProtectionMenuVars::vars.presenceCrewMessage) {
//					menu::getNotify()->bottom("[Presence] Blocked crew message"_Protect);
//					return true;
//				}
//
//				break;
//
//			case PRESENCE_ADMIN_JOIN_EVENT:
//				if (networkProtectionMenuVars::vars.presenceForceJoin) {
//					menu::getNotify()->bottom("[Presence] Blocked force join"_Protect);
//					return true;
//				}
//
//				break;
//		}
//
//		return originalHandlePresenceEvent(rcx, rdx, r8, r9);
//	} catch (...) {
//		return false;
//	}
//}