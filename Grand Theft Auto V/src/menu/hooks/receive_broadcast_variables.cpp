#include "hooks.hpp"
#include "rage/types/global_types.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/log.hpp"
#include "utils/caller.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"

TranslationString T_DropKick("Drop Kick", true, true);
TranslationString T_IsBeingDropKickedBy("is being drop kicked by", true, true);
TranslationString T_ArrayCrash1("Array Crash 1", true, true);
TranslationString T_ArrayCrash2("Array Crash 2", true, true);
TranslationString T_ArrayCrash3("Array Crash 3", true, true);
TranslationString T_ArrayCrash4("Array Crash 4", true, true);
TranslationString T_ArrayCrash5("Array Crash 5", true, true);

bool Menu::Hooks::ReceiveBroadcastVariablesHook(uint64_t This, Rage::Network::NetGamePlayer *Sender, Rage::BitBuffer *BitBuffer, uint32_t BitCount, uint16_t NetworkID)
{
	if (Sender->m_ID != -1)
	{
		if (Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests > GetTickCount())
		{
			return false;
		}
	}

	auto FlagModder = [=](TranslationString Translation) {
		if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash)
		{
			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(Translation), false, true);
		}

		if (Sender->m_ID != -1)
		{
			Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
			Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
			Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
			NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, Sender->m_ID);
		}

		return false;
	};

	auto ProtectVar = NetworkProtectionGameEventsMenuVars::m_Vars.m_ScriptHostDropKick;

	bool DropKick = false;
	uint64_t Backup[33];

	void *HostKickVariable = &::Global::Vars::g_GlobalCache[Rage::Global::HostDropKick >> 18 & 0x3F][Rage::Global::HostDropKick & 0x3FFFF];
	if (*(uint64_t *)(This + 0x108) == (uint64_t)HostKickVariable)
	{
		DropKick = true;
		memcpy(Backup, HostKickVariable, sizeof(Backup));
	}

	uint32_t CachedNextBit = BitBuffer->m_NextBit;
	uint32_t Next = 0;
	uint32_t v20 = 0;
	bool v10 = true;

	uint64_t LoggingInterface = *(uint64_t *)(Global::Vars::g_NetStubLogginInterface + 0x13C70);

	uint64_t VTable = *(uint64_t *)This;
	bool Unk = Caller::Call<bool>(*(uint64_t *)(VTable + 0x158), This);

	for (uint32_t i = BitBuffer->m_NextBit - CachedNextBit; i < BitCount; i = Next - CachedNextBit)
	{
		uint32_t v14 = BitBuffer->m_NextBit;

		Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread = true;
		Global::Vars::g_WriteArrayThreadInfo.m_Block = false;
		Global::Vars::g_WriteArrayThreadInfo.m_ThreadID = GetCurrentThreadId();

		Caller::Call<int>(*(uint64_t *)(VTable + 0x178), This, BitBuffer, &v20); // reads

		if (Global::Vars::g_WriteArrayThreadInfo.m_Block)
		{
			return FlagModder(T_ArrayCrash5);
		}

		Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread = false;

		if (!Caller::Call<bool>(*(uint64_t *)(VTable + 0x180), This, v20))
		{
			return FlagModder(T_ArrayCrash1);
		}

		bool v23 = false;
		bool v16 = false;

		if (Unk)
		{
			if (!BitBuffer->ReadBool(&v23))
			{
				return FlagModder(T_ArrayCrash2);
			}

			v16 = v23;
		}
		else
		{
			v16 = false;
			v23 = false;
		}

		if (!v16)
		{
			Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread = true;
			Global::Vars::g_WriteArrayThreadInfo.m_Block = false;
			Global::Vars::g_WriteArrayThreadInfo.m_ThreadID = GetCurrentThreadId();

			Caller::Call<int>(*(uint64_t *)(VTable + 0x1C0), This, BitBuffer, v20, LoggingInterface); // reads

			if (Global::Vars::g_WriteArrayThreadInfo.m_Block)
			{
				return FlagModder(T_ArrayCrash5);
			}

			Global::Vars::g_WriteArrayThreadInfo.m_CameFromThread = false;
			v16 = v23;
		}

		if (Caller::Call<bool>(*(uint64_t *)(VTable + 0x1A8), This, v20, Sender, v16))
		{
			if (v23)
			{
				Caller::Call<int>(*(uint64_t *)(VTable + 0x168), This, v20);
				Caller::Call<int>(*(uint64_t *)(VTable + 0x190), This, v20);
			}
			else
			{
				if (Caller::Call<bool>(*(uint64_t *)(VTable + 0x130), This))
				{
					uint64_t v17 = Caller::Call<uint64_t>(*(uint64_t *)(VTable + 0x130), This);
					if (v17)
					{
						uint64_t VTable2 = *(uint64_t *)v17;
						Caller::Call<int>(*(uint64_t *)(VTable2 + 0xA0), v17, v20);
					}
					else
					{
						return FlagModder(T_ArrayCrash3);
					}
				}

				Caller::Call<int>(*(uint64_t *)(VTable + 0x1D8), This, v20, Sender);
				Caller::Call<int>(*(uint64_t *)(VTable + 0x1A0), This, v20);
			}
		}
		else
		{
			v10 = false;
		}

		if (v14 == BitBuffer->m_NextBit)
		{
			return FlagModder(T_ArrayCrash4);
		}

		Next = BitBuffer->m_NextBit;
	}

	Caller::Call<int>(*(uint64_t *)(VTable + 0x198), This);
	if (v10 && Sender->m_ID != -1)
	{
		*(uint16_t *)(This + 0x6C + (Sender->m_ID * 2)) = NetworkID;
	}

	if (DropKick)
	{
		for (int i = 1; i < 33; i++)
		{
			uint64_t Index = (uint64_t)HostKickVariable + (i * 8);
			if (*(int *)(Index) != *(int *)&Backup[i])
			{
				if (*(int *)(Index) == 1)
				{
					if (i - 1 == Menu::GetLocalPlayer().m_ID)
					{
						if (ProtectVar.m_Block)
						{
							*(int *)(Index) = *(int *)&Backup[i];
						}

						if (ProtectVar.m_Notify)
						{
							Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_DropKick), false, true);
						}
					}
					else
					{
						if (ProtectVar.m_Notify)
						{
							if (Native::NetworkIsSessionActive() && Native::NetworkIsInSession())
							{
								if (Menu::GetPlayer(i - 1).m_Name && strcmp(Menu::GetPlayer(i - 1).m_Name, "**Invalid**"))
								{
									Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s %s", Menu::GetPlayer(i - 1).m_Name, TRANSLATE(T_IsBeingDropKickedBy), Sender->m_PlayerInfo->m_Name), Global::UIVars::g_NotificationError);
								}
							}
						}
					}
				}
			}
		}
	}

	return v10;
}