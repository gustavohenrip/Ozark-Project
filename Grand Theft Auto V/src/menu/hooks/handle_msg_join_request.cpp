#include "hooks.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main/network/toolkit/network_host_toolkit_join_block.hpp"
#include "menu/base/submenus/main/network/toolkit/network_host_toolkit_join_block_selected.hpp"
#include "utils/va.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/memory/memory.hpp"
#include "menu/base/submenus/main/network/network_overseer.hpp"
#include "utils/translation_string.hpp"
#include "utils/log.hpp"

TranslationString T_Prevented("Prevented", true, true);
TranslationString T_Joining("joining", true, true);
TranslationString T_JoiningNotFriend("joining, they're not your friend", true, true);
TranslationString T_JoiningNotOzark("joining, they're not an Ozark user", true, true);
TranslationString T_JoiningWithForceHost("joining with force host", true, true);
TranslationString T_JoiningWithASpoofedRID("joining with a spoofed RID", true, true);
TranslationString T_JoiningWithASpoofedName("joining with a spoofed name", true, true);
TranslationString T_JoiningWithASpoofedIP("joining with a spoofed IP. Their real IP is", true, true);

struct BlacklistedIP {
	uint8_t m_1;
	uint8_t m_2;
	uint8_t m_3;
	uint8_t m_4;
};

std::vector<BlacklistedIP> IPs = {
	{ 104, 206, 12, 199 },
	{ 1, 159, 58, 170 },
	{ 5, 180, 62, 142 },
	{ 139, 99, 234, 42 },
	{ 139, 99, 234, 43 },
	{ 139, 99, 235, 167 },
	{ 124, 170, 101, 44 },
	{ 139, 99, 145, 127 },
	{ 139, 99, 234, 46 }
};

std::vector<uint32_t> RockstarIDs = {
	99760757,
	98042282,
	27232953,
	112323207,
	104516056,
	76705345,
	98932544,
	81665528,
	125642770,
	108803216,
	102267130,
	142314594,
	104231567,
	104032434,
	125518606,
	104337890,
	69160994,
	108608304,
	126427379,
	136720574,
	83765581,
	19541822,
	182093032,
	148254157,
	176014334,
	111632819,
	95888690
};

bool Menu::Hooks::HostHandleJoinRequestHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	if (*(uint8_t*)(Global::Vars::g_FunctionPatches[4].m_Address) == Global::Vars::g_FunctionPatches[4].m_Patch[0]) {
		Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Original);
	}

	using namespace NetworkHostToolkitJoinBlockingMenuVars;

	uint32_t IP = *(uint32_t*)(R9 + 0x8);
	Rage::Network::NetworkGamerCmd* GamerCmd = (Rage::Network::NetworkGamerCmd*)(R8 - 0x8);

	if (GamerCmd) {
		// Paragon faggots
		if (std::find(begin(RockstarIDs), end(RockstarIDs), GamerCmd->m_Identifier.m_RockstarID) != end(RockstarIDs)
			|| std::find_if(begin(IPs), end(IPs), [=](BlacklistedIP& E) { return E.m_1 == ((IP & 0xFF000000) >> 24) && E.m_2 == ((IP & 0xFF0000) >> 16) && E.m_3 == ((IP & 0xFF00) >> 8) && E.m_4 == (IP & 0xFF); }) != end(IPs)) {
			LOG_DEV("Prevented paragon staff joining session (%s)", GamerCmd->m_Name);
			Global::Vars::g_FunctionPatches[4].m_Patch[2] = UNABLE_TO_CONNECT_TO_SESSION;
			Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
			return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
		}

		if (NetworkOverseerMenuVars::ProcessJoinRequest(GamerCmd->m_Identifier.m_RockstarID, GamerCmd->m_Name, *(uint32_t*)(R9 + 0x8))) {
			Global::Vars::g_FunctionPatches[4].m_Patch[2] = UNABLE_TO_CONNECT_TO_SESSION;
			Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
			return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
		}

		if (m_Vars.m_Responses[TOOLKIT_NEXT_SESSION_HOST].m_Enabled) {
			if (((GamerCmd->m_HostToken >> 32) & 0xFFFFFFFF) < 0x1000 || GamerCmd->m_HostToken < 0x1000) {
				if (m_Vars.m_Responses[TOOLKIT_NEXT_SESSION_HOST].m_Notify) {
					if ((GetTickCount() - Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken]) > 10000) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Prevented), Menu::Helpers::CleanPlayerName(GamerCmd->m_Name).c_str(), TRANSLATE(T_JoiningWithForceHost)), Global::UIVars::g_NotificationError);
					}
				}

				Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken] = GetTickCount();

				Global::Vars::g_FunctionPatches[4].m_Patch[2] = NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Responses[m_Vars.m_Responses[TOOLKIT_NEXT_SESSION_HOST].m_Context.m_Selected].m_Response;
				Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
				return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
			}
		}

		if (m_Vars.m_Responses[TOOLKIT_RID].m_Enabled) {
			auto CheckRockstarID = [=] {
				if (GamerCmd->m_Identifier.m_RockstarID != GamerCmd->m_Identifier.m_RockstarID) return false;
				if (GamerCmd->m_Identifier.m_RockstarID < 10000) return false;

				if (GamerCmd->m_PlayerIdentifiers[0].m_RockstarID != 0) {
					if (GamerCmd->m_Identifier.m_RockstarID != GamerCmd->m_PlayerIdentifiers[0].m_RockstarID) {
						return false;
					}
				}

				return true;
			};

			if (!CheckRockstarID()) {
				if (m_Vars.m_Responses[TOOLKIT_RID].m_Notify) {
					if ((GetTickCount() - Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken]) > 10000) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Prevented), Menu::Helpers::CleanPlayerName(GamerCmd->m_Name).c_str(), TRANSLATE(T_JoiningWithASpoofedRID)), Global::UIVars::g_NotificationError);
					}
				}

				Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken] = GetTickCount();

				Global::Vars::g_FunctionPatches[4].m_Patch[2] = NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Responses[m_Vars.m_Responses[TOOLKIT_RID].m_Context.m_Selected].m_Response;
				Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
				return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
			}
		}

		if (m_Vars.m_Responses[TOOLKIT_NAME].m_Enabled) {
			if (std::string(GamerCmd->m_Name).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.") != std::string::npos || strlen(GamerCmd->m_Name) < 6) {
				if (m_Vars.m_Responses[TOOLKIT_NAME].m_Notify) {
					if ((GetTickCount() - Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken]) > 10000) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Prevented), Menu::Helpers::CleanPlayerName(GamerCmd->m_Name).c_str(), TRANSLATE(T_JoiningWithASpoofedName)), Global::UIVars::g_NotificationError);
					}
				}

				Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken] = GetTickCount();

				Global::Vars::g_FunctionPatches[4].m_Patch[2] = NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Responses[m_Vars.m_Responses[TOOLKIT_NAME].m_Context.m_Selected].m_Response;
				Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
				return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
			}
		}

		if (m_Vars.m_Responses[TOOLKIT_IP].m_Enabled) {
			if (GamerCmd->m_MsgIP != IP) {
				if (m_Vars.m_Responses[TOOLKIT_IP].m_Notify) {
					if ((GetTickCount() - Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken]) > 10000) {
						char Buffer[16];
						sprintf(Buffer, "%i.%i.%i.%i", ((IP & 0xFF000000) >> 24), ((IP & 0xFF0000) >> 16), ((IP & 0xFF00) >> 8), (IP & 0xFF));
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s %s", TRANSLATE(T_Prevented), Menu::Helpers::CleanPlayerName(GamerCmd->m_Name).c_str(), TRANSLATE(T_JoiningWithASpoofedIP), Buffer), Global::UIVars::g_NotificationError);
					}
				}

				Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken] = GetTickCount();

				Global::Vars::g_FunctionPatches[4].m_Patch[2] = NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Responses[m_Vars.m_Responses[TOOLKIT_IP].m_Context.m_Selected].m_Response;
				Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
				return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
			}
		}

		if (m_Vars.m_WhoMayJoin != 0) {
			if (m_Vars.m_WhoMayJoin == 1) {
				if ((GetTickCount() - Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken]) > 10000) {
					Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Prevented), Menu::Helpers::CleanPlayerName(GamerCmd->m_Name).c_str(), TRANSLATE(T_Joining)), Global::UIVars::g_NotificationError);
				}

				Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken] = GetTickCount();

				Global::Vars::g_FunctionPatches[4].m_Patch[2] = UNABLE_TO_CONNECT_TO_SESSION;
				Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
				return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
			}

			if (m_Vars.m_WhoMayJoin == 2) {
				bool CanJoin = false;
				for (int i = 0; i < Native::NetworkGetFriendCount(); i++) {
					Rage::Network::Friends* Friend = (Rage::Network::Friends*) & Global::Vars::g_Friends[i];
					if (Friend->m_RockstarID == GamerCmd->m_Identifier.m_RockstarID) {
						if (!strcmp(GamerCmd->m_Name, Friend->m_Name)) {
							if (abs(Friend->m_State) == 3) {
								CanJoin = true;
								break;
							}
						}
					}
				}

				if (!CanJoin) {
					if ((GetTickCount() - Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken]) > 10000) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Prevented), Menu::Helpers::CleanPlayerName(GamerCmd->m_Name).c_str(), TRANSLATE(T_JoiningNotFriend)), Global::UIVars::g_NotificationError);
					}

					Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken] = GetTickCount();

					Global::Vars::g_FunctionPatches[4].m_Patch[2] = UNABLE_TO_CONNECT_TO_SESSION;
					Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
					return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
				}
			}

			if (m_Vars.m_WhoMayJoin == 3) {
				if (*(uint8_t*)((uint64_t)GamerCmd + 0x5F) != 0x69) {
					if ((GetTickCount() - Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken]) > 10000) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", TRANSLATE(T_Prevented), Menu::Helpers::CleanPlayerName(GamerCmd->m_Name).c_str(), TRANSLATE(T_JoiningNotOzark)), Global::UIVars::g_NotificationError);
					}

					Global::Vars::g_SpoofedDataJoinAttempts[GamerCmd->m_HostToken] = GetTickCount();

					Global::Vars::g_FunctionPatches[4].m_Patch[2] = UNABLE_TO_CONNECT_TO_SESSION;
					Memory::WriteVector(Global::Vars::g_FunctionPatches[4].m_Address, Global::Vars::g_FunctionPatches[4].m_Patch);
					return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
				}
			}
		}
	}

	return OriginalHostHandleJoinRequest(Rcx, Rdx, R8, R9);
}