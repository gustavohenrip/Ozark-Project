#include "hooks.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "rage/engine.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/log.hpp"
#include "utils/caller.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/memory/memory.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_net_events.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/submenus/main/network/network_protection.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/helpers.hpp"
#include "rage/types/global_types.hpp"
#include "menu/base/submenus/main.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"

TranslationString T_InvalidStat("Invalid Stat", true, true);
TranslationString T_Griefing("Griefing", true, true);
TranslationString T_GameExploits("Game Exploits", true, true);
TranslationString T_Exploits("Exploits", true, true);
TranslationString T_Cheating("Cheating", true, true);
TranslationString T_AnnoyingInVC("Annoying in VC", true, true);
TranslationString T_HatefulInVC("Hateful in VC", true, true);
TranslationString T_AnnoyingInChat("Annoying in Chat", true, true);
TranslationString T_HatefulInChat("Hateful in Chat", true, true);
TranslationString T_BeingFriendly("Being Friendly", true, true);
TranslationString T_BeingHelpful("Being Helpful", true, true);
TranslationString T_SE("SE", true, true);
TranslationString T_ClearTasks("Clear Tasks", true, true);
TranslationString T_RemoveWeapon("Remove Weapon", true, true);
TranslationString T_RemoveAllWeapons("Remove All Weapons", true, true);
TranslationString T_GiveWeapon("Give Weapon", true, true);
TranslationString T_GCCrash("GC Crash", true, true);
TranslationString T_PTFX("PTFX", true, true);
TranslationString T_SESCECrash("SESCE Crash", true, true);
TranslationString T_SWSECrash("SWSE Crash", true, true);
TranslationString T_IncrementStat("Increment Stat", true, true);
TranslationString T_ExplosionBlaming("Explosion - Blaming", true, true);
TranslationString T_Explosion("Explosion", true, true);
TranslationString T_WeatherCrash("Weather - Crash", true, true);
TranslationString T_WeatherUpdate("Weather Update", true, true);
TranslationString T_ClockCrash("Clock - Crash", true, true);
TranslationString T_Sound("Sound", true, true);
TranslationString T_NetworkEventMisalign("Network Event Misalign", true, true);

// Events that are handled inside the reader (TODO: Handle manually)
std::vector<eNetworkEvent> Whitelist = {
	RESPAWN_PLAYER_PED_EVENT,
	SCRIPT_WORLD_STATE_EVENT};

uint64_t GetEventSetup(int16_t EventID, uint64_t *OutCallback = nullptr)
{
	uint64_t Table = *(uint64_t *)Global::Vars::g_NetworkEventRegistration;
	if (Table)
	{
		uint64_t Callback = *(uint64_t *)(Table + (EventID * 8) + 0x3ABD0);
		if (Callback)
		{
			uint64_t Setup = 0;

			for (int i = 0; i < 0x40; i++)
			{
				if (*(uint8_t *)(Callback + i) == 0xE8)
				{
					Setup = Callback + i;
					break;
				}
			}

			if (Setup)
			{
				if (OutCallback)
					*OutCallback = Setup;
				return Memory::GetAddressFromInstruction(Setup, 1, 5);
				;
			}
		}
	}

	return 0;
}

void FixScriptedGameEvent(uint32_t Count, int *Raw, int *Fixed)
{
	bool Save = true;
	for (uint32_t i = 0; i < Count; i++)
	{
		if (Save)
		{
			Fixed[i == 0 ? 0 : i / 2] = Raw[i];
		}

		Save ^= true;
	}
}

std::string GetReportNameFromStatHash(uint32_t Hash, bool *Report)
{
	const char *Return = TRANSLATE(T_InvalidStat);

	*Report = true;

	switch (Hash)
	{
	case 0x9C6A0C42:
		Return = TRANSLATE(T_Griefing);
		break;
	case 0xCBFD04A4:
		Return = TRANSLATE(T_GameExploits);
		break;
	case 0x9F79BA0B:
		Return = TRANSLATE(T_Exploits);
		break;
	case 0xEC3CE96A:
		Return = TRANSLATE(T_Cheating);
		break;
	case 0x62EB8C5A:
		Return = TRANSLATE(T_AnnoyingInVC);
		break;
	case 0x0E7072CD:
		Return = TRANSLATE(T_HatefulInVC);
		break;
	case 0x762F9994:
		Return = TRANSLATE(T_AnnoyingInChat);
		break;
	case 0xB722D6C0:
		Return = TRANSLATE(T_HatefulInChat);
		break;
	case 0xDAFB10F9:
		Return = TRANSLATE(T_BeingFriendly);
		*Report = false;
		break;
	case 0x893E1390:
		Return = TRANSLATE(T_BeingHelpful);
		*Report = false;
		break;
	}

	return Return;
}

void HandleEventAck(int16_t EventID, Rage::Network::NetGamePlayer *Sender, Rage::Network::NetGamePlayer *Receiver, int EventIndex, int EventBitset)
{
	uint64_t Table = *(uint64_t *)Global::Vars::g_NetworkEventRegistration;

	uint64_t Setup = GetEventSetup(EventID);
	if (Setup)
	{
		uint8_t SetupBuffer[0x500];
		Caller::Call<int>(Setup, SetupBuffer, 0);

		if (*(uint8_t *)(SetupBuffer + 0xA) & 1)
		{
			Rage::Engine::SendNetworkEventReply(Table, SetupBuffer, Sender, Receiver, EventIndex, EventBitset);
		}
		else
		{
			Rage::Engine::SendNetworkEventAck(Table, SetupBuffer, Sender, Receiver, EventIndex);
		}
	}
}

bool HandleScriptEvent(Rage::Network::NetGamePlayer *Sender, std::string Name, int Count, int *Arguments)
{
	if (Name == "Crash")
	{
		Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
		Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
		Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
	}

	auto ProtectVar = NetworkProtectionNetEventsMenuVars::m_Vars;

	if (ProtectVar.m_ScriptEvents.m_Notify)
	{
		Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, std::string(TRANSLATE(T_SE)) + ": " + Name, ProtectVar.m_ScriptEvents.m_Redirect, ProtectVar.m_ScriptEvents.m_Block);
	}

	if (ProtectVar.m_ScriptEvents.m_Redirect)
	{
		Utils::GetFiberPool()->Push([=] {
			Menu::Helpers::TriggerScriptEvent(Arguments, Count >> 3, Sender->m_ID);
		});
	}

	return ProtectVar.m_ScriptEvents.m_Block;
}

void Menu::Hooks::HandleNetworkEventHook(uint64_t This, Rage::Network::NetGamePlayer *Sender, Rage::Network::NetGamePlayer *Receiver, int16_t EventID, int EventIndex, int EventBitset, uint64_t BitbufferSize, Rage::BitBuffer *BitBuffer)
{
	Rage::BitBuffer Cache;
	memcpy(&Cache, BitBuffer, sizeof(Cache));

	if (Sender->m_ID == -1)
	{
		return OriginalHandleNetworkEvent(This, Sender, Receiver, EventID, EventIndex, EventBitset, BitbufferSize, BitBuffer);
	}

	if (Sender && Sender->m_ID >= 0 && Sender->m_ID < 34)
	{
		if (Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests > GetTickCount())
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}
	}
	else
	{
		LOG_DEV("Not allowing network event, player id is %i", Sender->m_ID);
		return OriginalHandleNetworkEvent(This, Sender, Receiver, EventID, EventIndex, EventBitset, BitbufferSize, BitBuffer);
	}

	if (Receiver == *(Rage::Network::NetGamePlayer **)(*(uint64_t *)(Global::Vars::g_NetworkPlayerManager) + 0xE8) && (EventBitset << 0x18) - (*(DWORD *)(This + 4 * (Sender->m_ID + 0x45 * EventIndex) + 0x454) << 0x18) <= 0)
	{
		return OriginalHandleNetworkEvent(This, Sender, Receiver, EventID, EventIndex, EventBitset, BitbufferSize, BitBuffer);
	}

	// Misalignment testing
	/*if (EventID < 91 && EventIndex < 0x200 && std::find(begin(Whitelist), end(Whitelist), EventID) == end(Whitelist)) {
		uint64_t Callback = 0;
		uint64_t EventSetup = GetEventSetup(EventID, &Callback);
		if (EventSetup) {
			uint8_t SetupBuffer[0x500];
			Caller::Call<int>(EventSetup, SetupBuffer, 0, 0, 0, 0);

			uint64_t VTable = *(uint64_t*)&SetupBuffer[0];
			if (VTable) {
				bool Found = true;
				if (EventID == 74 || EventID == 78 || EventID == 87) {
					for (int i = 0; i < 0x100; i++) {
						if (!memcmp((void*)(Callback + i), "\x48\x8D\x05", 3)) {
							VTable = Memory::GetAddressFromInstruction(Callback + i);
							*(uint64_t*)&SetupBuffer[0] = VTable;
							goto iamwhatiam;
						}
					}

					Found = false;
				}

			iamwhatiam:
				LOG_DEV("Handling event %i (%llx)", EventID, VTable - Global::Vars::g_GameAddress);
				if (Found) {
					Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread = true;
					Global::Vars::g_ReadNetworkEventThreadInfo.m_Block = false;
					Global::Vars::g_ReadNetworkEventThreadInfo.m_ThreadID = GetCurrentThreadId();

					Caller::Call<int>(*(uint64_t*)(VTable + 0x30), SetupBuffer, BitBuffer, Sender, Receiver);

					Global::Vars::g_ReadNetworkEventThreadInfo.m_CameFromThread = false;

					if (Global::Vars::g_ReadNetworkEventThreadInfo.m_Block) {
						Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
						Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
						Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
						NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, Sender->m_ID);

						if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash) {
							Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_NetworkEventMisalign), false, true);
						}

						return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
					}
				}
			}
		}
	}*/

	memcpy(BitBuffer, &Cache, sizeof(Cache));
	auto ProtectVar = NetworkProtectionNetEventsMenuVars::m_Vars;

	switch (EventID)
	{
	case REQUEST_CONTROL_EVENT:
	{
		int16_t NetworkID = 0;
		BitBuffer->ReadShort(&NetworkID, 0xD);

		if (Menu::GetLocalPlayer().m_InVehicle)
		{
			uint64_t Vehicle = Rage::Engine::GetEntityAddress(Menu::GetLocalPlayer().m_Vehicle);
			if (Vehicle)
			{
				Rage::Network::NetObj *NetObject = *(Rage::Network::NetObj **)(Vehicle + 0xD0);
				if (NetObject)
				{
					Rage::Network::NetObj *Object = Rage::Engine::GetNetworkObjectFromNetworkID(*(uint64_t *)Global::Vars::g_NetworkObjectManager, NetworkID, true);
					if (Object == NetObject)
					{
						if (NetObject->m_Owner == Menu::GetLocalPlayer().m_ID)
						{
							if (ProtectVar.m_Control.m_Block)
							{
								LOG_DEV("Prevented %s taking control of vehicle", Sender->m_PlayerInfo->m_Name);
								return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
							}
						}
					}
				}
			}
		}

		break;
	}

	case NETWORK_CLEAR_PED_TASKS_EVENT:
	{
		if (ProtectVar.m_Freeze.m_Notify)
		{
			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_ClearTasks), ProtectVar.m_Freeze.m_Redirect, ProtectVar.m_Freeze.m_Block);
		}

		if (ProtectVar.m_Freeze.m_Redirect)
		{
			Utils::GetFiberPool()->Push([=] {
				Native::ClearPedTasksImmediately(Menu::GetPlayer(Sender->m_ID).m_Ped);
			});
		}

		if (ProtectVar.m_Freeze.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case REMOVE_WEAPON_EVENT:
	{
		int16_t NetworkID = 0;
		BitBuffer->ReadShort(&NetworkID, 0xD);

		uint32_t WeaponHash = 0;
		BitBuffer->ReadUint(&WeaponHash, 0x20);

		if (ProtectVar.m_RemoveWeapon.m_Notify)
		{
			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_RemoveWeapon), ProtectVar.m_RemoveWeapon.m_Redirect, ProtectVar.m_RemoveWeapon.m_Block);
		}

		if (ProtectVar.m_RemoveWeapon.m_Redirect)
		{
			Utils::GetFiberPool()->Push([=] {
				Native::RemoveWeaponFromPed(Menu::GetPlayer(Sender->m_ID).m_Ped, WeaponHash);
			});
		}

		if (ProtectVar.m_RemoveWeapon.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case REMOVE_ALL_WEAPONS_EVENT:
	{
		if (ProtectVar.m_RemoveAllWeapons.m_Notify)
		{
			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_RemoveAllWeapons), ProtectVar.m_RemoveAllWeapons.m_Redirect, ProtectVar.m_RemoveAllWeapons.m_Block);
		}

		if (ProtectVar.m_RemoveAllWeapons.m_Redirect)
		{
			Native::RemoveAllPedWeapons(Menu::GetPlayer(Sender->m_ID).m_Ped, true);
		}

		if (ProtectVar.m_RemoveAllWeapons.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case GIVE_WEAPON_EVENT:
	{
		int16_t NetworkID = 0;
		BitBuffer->ReadShort(&NetworkID, 0xD);

		uint32_t WeaponHash = 0;
		BitBuffer->ReadUint(&WeaponHash, 0x20);

		int AmmoCount = -1;
		BitBuffer->ReadInt(&AmmoCount, 0x10);

		if (ProtectVar.m_GiveWeapon.m_Notify)
		{
			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_GiveWeapon), ProtectVar.m_GiveWeapon.m_Redirect, ProtectVar.m_GiveWeapon.m_Block);
		}

		if (ProtectVar.m_GiveWeapon.m_Redirect)
		{
			Native::GiveDelayedWeaponToPed(Menu::GetPlayer(Sender->m_ID).m_Ped, WeaponHash, 0, true);
			Native::SetPedAmmo(Menu::GetPlayer(Sender->m_ID).m_Ped, WeaponHash, AmmoCount, 0);
		}

		if (ProtectVar.m_GiveWeapon.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case GIVE_CONTROL_EVENT:
	{
		if (*(uint8_t *)(Global::Vars::g_FunctionPatches[3].m_Address) == Global::Vars::g_FunctionPatches[3].m_Patch[0])
		{
			Memory::WriteVector(Global::Vars::g_FunctionPatches[3].m_Address, Global::Vars::g_FunctionPatches[3].m_Original);
		}

		uint8_t Data[0x200];
		if (Rage::Engine::SetupGiveControlEvent(Data, BitBuffer))
		{
			if (!*(bool *)(Data + 0x42))
			{
				int32_t Current = 0;

				if (*(int32_t *)(Data + 0x44) > 3)
				{
					return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
				}

				do
				{
					int16_t NetworkID = *(int16_t *)(Data + 0x50 + (Current * 20));
					int16_t Type = *(int16_t *)(Data + 0x52 + (Current * 20));

					if (Type < NetObjectAutomobile || Type > NetObjectTrain)
					{
						return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
					}

					Rage::Network::SyncTree *SyncTree = Rage::Engine::GetSyncTreeFromCloneType(Type);
					if (!SyncTree)
					{
						return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
					}

					if (SyncTree->m_DataNodeCount == 0 || SyncTree->m_DataNodeCount > 100)
					{
						return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
					}

					Rage::Network::NetObj *Object = Rage::Engine::GetNetworkObjectFromNetworkID(*(uint64_t *)Global::Vars::g_NetworkObjectManager, NetworkID, true);
					if (!Object)
					{
						return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
					}

					if (Object->m_Type != Type)
					{
						return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
					}

					Current++;
				} while (Current < *(int32_t *)(Data + 0x44));
			}
		}

		Global::Vars::g_BrokenNodeSenders[2] = Sender;
		break;
	}

	case NETWORK_PTFX_EVENT:
	{
		uint32_t Name = 0;
		uint32_t Dict = 0;
		uint32_t Scale = 0;
		Math::Vector3_<float> XYZ;
		Math::Vector3_<float> ROT;

		auto Instance = BitBuffer->CreateSyncReaderInstance();
		BitBuffer->ReadUint(&Name, 0x20);
		BitBuffer->ReadUint(&Dict, 0x20);
		BitBuffer->ReadVector3(&Instance, &XYZ, 0x0, 0x13);
		BitBuffer->ReadVector3(&Instance, &ROT, 0x0, 0x13);
		BitBuffer->ReadUint(&Scale, 0xA);

		float FixedScale = ((((float)Scale) * 0.00097751711f) * 10.0f);

		if (ProtectVar.m_PTFX.m_Notify)
		{
			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_PTFX), ProtectVar.m_PTFX.m_Redirect, ProtectVar.m_PTFX.m_Block);
		}

		if (ProtectVar.m_PTFX.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case SCRIPT_ENTITY_STATE_CHANGE_EVENT:
	{
		int16_t NetworkID = 0;
		BitBuffer->ReadShort(&NetworkID, 0xD);

		uint32_t StateIndex = 0;
		BitBuffer->ReadUint(&StateIndex, 0x4);

		if (StateIndex > 9)
		{
			Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
			Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
			Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
			NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, Sender->m_ID);

			if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash)
			{
				Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_SESCECrash), false, true);
			}

			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case SCRIPT_WORLD_STATE_EVENT:
	{
		uint32_t StateIndex = 0;
		BitBuffer->ReadUint(&StateIndex, 0x4);

		bool UnkBool = false;
		BitBuffer->ReadBool(&UnkBool);

		if (StateIndex > 8 || (StateIndex == 2 && UnkBool))
		{
			Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
			Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
			Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
			NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, Sender->m_ID);

			if (NetworkProtectionNotificationsMenuVars::m_Vars.m_Crash)
			{
				Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_SWSECrash), false, true);
			}

			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case SCRIPTED_GAME_EVENT:
	{
		uint32_t Count = 0;
		BitBuffer->ReadUint(&Count, 0x20);

		int *Arguments = new int[54];
		if (Count - 1 <= 0x1AF)
		{
			int PaddedArguments[107];
			if (BitBuffer->ReadIntArray(PaddedArguments, Count << 3, 0))
			{
				FixScriptedGameEvent(Count >> 2, PaddedArguments, Arguments);

				bool Block = false;

				if (Arguments[1] >= 0 && Arguments[1] <= 32)
				{
					switch (Arguments[0])
					{
					case Rage::Global::BanFromCEO:
						Block = HandleScriptEvent(Sender, "Ban from CEO", Count, Arguments);
						break;

					case Rage::Global::KickFromCEO:
						Block = HandleScriptEvent(Sender, "Kick from CEO", Count, Arguments);
						break;

					case Rage::Global::ApartmentTeleport:
						if ((Count >> 3) < 6 || (Arguments[4] != Arguments[5]) || Arguments[4] < 0 || Arguments[4] > 116)
						{
							Block = HandleScriptEvent(Sender, "Black Screen", Count, Arguments);
							break;
						}

						Block = HandleScriptEvent(Sender, "Apartment Invite", Count, Arguments);
						break;

					case Rage::Global::NetworkBail:
						Block = HandleScriptEvent(Sender, "Network Bail", Count, Arguments);
						break;

					case Rage::Global::SetWeather:
						Block = HandleScriptEvent(Sender, "Set Weather", Count, Arguments);
						break;

					case Rage::Global::MessageHandler:
						Block = HandleScriptEvent(Sender, "Notification", Count, Arguments);
						break;

					case Rage::Global::Invite:
						Block = HandleScriptEvent(Sender, "Invite", Count, Arguments);
						break;

					case Rage::Global::RemoveWantedLevel:
						if (Menu::GetLocalPlayer().m_InVehicle)
						{
							if (Native::IsPedInVehicle(Menu::GetPlayer(Sender->m_ID).m_Ped, Menu::GetLocalPlayer().m_Vehicle, true))
							{
								Block = false;
								break;
							}
						}

						Block = HandleScriptEvent(Sender, "Remove Wanted Level", Count, Arguments);
						break;

					case Rage::Global::GiveCopsTurnBlindEye:
						if (Menu::GetLocalPlayer().m_InVehicle)
						{
							if (Native::IsPedInVehicle(Menu::GetPlayer(Sender->m_ID).m_Ped, Menu::GetLocalPlayer().m_Vehicle, true))
							{
								Block = false;
								break;
							}
						}

						Block = HandleScriptEvent(Sender, "Cops Turn Blind Eye", Count, Arguments);
						break;

					case Rage::Global::GiveOffTheRadar:
						if (Menu::GetLocalPlayer().m_InVehicle)
						{
							if (Native::IsPedInVehicle(Menu::GetPlayer(Sender->m_ID).m_Ped, Menu::GetLocalPlayer().m_Vehicle, true))
							{
								Block = false;
								break;
							}
						}

						Block = HandleScriptEvent(Sender, "Off the Radar", Count, Arguments);
						break;

					case Rage::Global::SetVehicleCanBeLockedOn:
						Block = HandleScriptEvent(Sender, "Vehicle Lock On", Count, Arguments);
						break;

					case Rage::Global::DestroyedPersonalVehicle:
						Block = HandleScriptEvent(Sender, "Destroyed Personal Vehicle", Count, Arguments);
						break;

					case Rage::Global::KickFromVehicle:
						Block = HandleScriptEvent(Sender, "Kick from Vehicle", Count, Arguments);
						break;

					case Rage::Global::ForceIntoMission:
						Block = HandleScriptEvent(Sender, "Force Into Mission", Count, Arguments);
						break;

					case -977515445: // updated
						Block = HandleScriptEvent(Sender, "Crash", Count, Arguments);
						break;

					case 767605081: // updated
						Block = HandleScriptEvent(Sender, "Crash", Count, Arguments);
						break;

					case -1730227041: // updated
						Block = true;
						break;

					case -545396442: // updated
						Block = HandleScriptEvent(Sender, "Force Into MOC Mission", Count, Arguments);
						break;

					case 1437305856: // updated
						Block = HandleScriptEvent(Sender, "Screen Flicker", Count, Arguments);
						break;

					case 1300962917: // updated
						Block = HandleScriptEvent(Sender, "Beach Party TP", Count, Arguments);
						break;

					case 1302185744: // updated
						Block = HandleScriptEvent(Sender, "Transaction Error", Count, Arguments);
						break;

					case -1162153263: // updated
						Block = true; // Sound Spam
						break;
					}
				}
				else
				{
					Block = true;
				}

#ifdef DEV
				if (Sender && Sender->m_ID >= 0)
				{
					std::stringstream Str;

					for (int i = 0; i < Count >> 2; i++)
					{
						Str << Arguments[i] << " ";
					}

					LOG_DEV("%s: %s", Sender->m_PlayerInfo->m_Name, Str.str().c_str());
				}
#endif

				if (Block)
				{
					delete[] Arguments;
					return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
				}
				else
				{
					if (Arguments[0] > 1000 && Arguments[0] < 75000)
					{
						Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
						Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_MENU_CHERAX] = true;
					}
				}
			}
		}

		delete[] Arguments;
		break;
	}

	case NETWORK_INCREMENT_STAT_EVENT:
	{
		uint32_t Hash = 0;
		BitBuffer->ReadUint(&Hash, 0x20);

		int Value = 0;
		BitBuffer->ReadInt(&Value, 0x20);

		bool Report = false;
		std::string Reason = GetReportNameFromStatHash(Hash, &Report);

		if (ProtectVar.m_IncrementStat.m_Notify)
		{
			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, std::string(TRANSLATE(T_IncrementStat)) + " - " + Reason, ProtectVar.m_IncrementStat.m_Redirect, ProtectVar.m_IncrementStat.m_Block);
		}

		if (ProtectVar.m_IncrementStat.m_Redirect)
		{
			Rage::Engine::SendIncrementStatEvent(Hash, Value, Sender);
		}

		if (Report)
		{
			NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_REPORT, Sender->m_ID);
		}

		if (ProtectVar.m_IncrementStat.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case FIRE_EVENT:
	{
		if (ProtectVar.m_Fire.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case EXPLOSION_EVENT:
	{
		int16_t Unk = 0;
		BitBuffer->ReadShort(&Unk, 0x10);
		BitBuffer->ReadShort(&Unk, 0xD);

		int16_t OwnerNetworkID = 0;
		BitBuffer->ReadShort(&OwnerNetworkID, 0xD);
		BitBuffer->ReadShort(&Unk, 0xD);

		int ExplosionType = 0;
		BitBuffer->ReadInt(&ExplosionType, 0x8);

		uint32_t Scale = 0;
		BitBuffer->ReadUint(&Scale, 0x8);

		Math::Vector3_<float> XYZ;

		auto Instance = BitBuffer->CreateSyncReaderInstance();
		BitBuffer->ReadVector3(&Instance, &XYZ, 0x0, 0x16);

		float FixedScale = Scale * 0.0039215689f;
		float Distance = XYZ.GetMathmaticalDistance(Math::Vector3<float>::ToSerialized(Menu::GetLocalPlayer().m_Coords));

		bool DontNotify = false;
		if (NetworkProtectionNetEventsMenuVars::m_Vars.m_Blame)
		{
			Rage::Network::NetObj *OwnerObject = Rage::Engine::GetNetworkObjectFromNetworkID(*(uint64_t *)Global::Vars::g_NetworkObjectManager, OwnerNetworkID, true);
			if (OwnerObject)
			{
				if (OwnerObject->m_Owner != Sender->m_ID)
				{
					if (OwnerObject->m_Owner >= 0 && OwnerObject->m_Owner < 34)
					{
						if (Menu::GetPlayer(OwnerObject->m_Owner).m_Connected)
						{
							DontNotify = true;
							Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, Utils::VA::VA("%s %s", TRANSLATE(T_ExplosionBlaming), OwnerObject->m_Owner == Menu::GetLocalPlayer().m_ID ? "you" : Menu::GetPlayer(OwnerObject->m_Owner).m_Name), false, false);
						}
					}
				}
			}
		}

		if (Distance <= 2.f || (Distance <= 4.f && FixedScale >= 2.f))
		{
			if (ProtectVar.m_Explosion.m_Notify)
			{
				if (!DontNotify)
					Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_Explosion), ProtectVar.m_Explosion.m_Redirect, ProtectVar.m_Explosion.m_Block);
			}

			if (ProtectVar.m_Explosion.m_Redirect)
			{
				Utils::GetFiberPool()->Push([=] {
					float X = Menu::GetPlayer(Sender->m_ID).m_Coords.m_X + fabs(Menu::GetLocalPlayer().m_Coords.m_X - XYZ.m_X);
					float Y = Menu::GetPlayer(Sender->m_ID).m_Coords.m_Y + fabs(Menu::GetLocalPlayer().m_Coords.m_Y - XYZ.m_Y);
					float Z = Menu::GetPlayer(Sender->m_ID).m_Coords.m_Z + fabs(Menu::GetLocalPlayer().m_Coords.m_Z - XYZ.m_Z);

					Native::AddExplosion(X, Y, Z, ExplosionType, FixedScale, true, false, 1.f, true);
				});
			}
		}

		if (ProtectVar.m_Explosion.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case GAME_WEATHER_EVENT:
	{
		bool CircleAroundSession = false;
		BitBuffer->ReadBool(&CircleAroundSession);

		uint32_t WeatherID = 0;
		BitBuffer->ReadUint(&WeatherID, 0x5);

		if (WeatherID > 14)
		{
			if (!CircleAroundSession)
			{
				Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
				Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
				Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
				NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, Sender->m_ID);

				if (ProtectVar.m_Weather.m_Notify)
				{
					Menu::GetNotify()->NotifyStackedProtectionEvent(CircleAroundSession ? "~c~Unknown~w~" : Sender->m_PlayerInfo->m_Name, TRANSLATE(T_WeatherCrash), false, true);
				}
			}

			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		if (ProtectVar.m_Weather.m_Notify && !CircleAroundSession)
		{
			Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
			Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_WEATHER_CHANGE] = true;

			Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_WeatherUpdate), false, ProtectVar.m_Weather.m_Block);
		}

		if (ProtectVar.m_Weather.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case GAME_CLOCK_EVENT:
	{
		uint32_t Year = 0;
		BitBuffer->ReadUint(&Year, 0x5);

		uint32_t Month = 0;
		BitBuffer->ReadUint(&Month, 0x4);

		uint32_t Day = 0;
		BitBuffer->ReadUint(&Day, 0xC);

		uint32_t Hour = 0;
		BitBuffer->ReadUint(&Hour, 0x6);

		uint32_t Minute = 0;
		BitBuffer->ReadUint(&Minute, 0x7);

		uint32_t Second = 0;
		BitBuffer->ReadUint(&Second, 0x7);

		if (Hour > 23 || Minute > 59 || Second > 59)
		{
			Menu::GetPlayer(Sender->m_ID).m_BlacklistRequests = GetTickCount() + 15000;
			Menu::GetPlayer(Sender->m_ID).m_IsModder = true;
			Menu::GetPlayer(Sender->m_ID).m_ModderFlags[Menu::REASON_CRASH_ATTEMPT] = true;
			NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_CRASH_ATTEMPT, Sender->m_ID);

			if (ProtectVar.m_Clock.m_Notify)
			{
				Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_ClockCrash), false, true);
			}

			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		if (ProtectVar.m_Clock.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case NETWORK_PLAY_SOUND_EVENT:
	{
		bool OnEntity = false;
		BitBuffer->ReadBool(&OnEntity);

		int16_t NetworkID = 0;
		Math::Vector3_<float> XYZ;

		if (OnEntity)
		{
			BitBuffer->ReadShort(&NetworkID, 0xD);

			Rage::Network::NetObj *Object = Rage::Engine::GetNetworkObjectFromNetworkID(*(uint64_t *)Global::Vars::g_NetworkObjectManager, NetworkID, true);
			if (!Object)
			{
				/*if (ProtectVar.m_Sound.m_Notify) {
						Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, "Sound - Crash", false, true);
					}*/

				return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
			}

			if (Global::Vars::g_PedFactory && Global::Vars::g_PedFactory->m_LocalPed)
			{
				if (Object->m_Entity == (uint64_t)Global::Vars::g_PedFactory->m_LocalPed || Object->m_Entity == (uint64_t)Global::Vars::g_PedFactory->m_LocalPed->m_Vehicle)
				{
					if (ProtectVar.m_Sound.m_Notify)
					{
						Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_Sound), false, ProtectVar.m_Sound.m_Block);
					}
				}
			}
		}
		else
		{
			auto Instance = BitBuffer->CreateSyncReaderInstance();
			BitBuffer->ReadVector3(&Instance, &XYZ, 0x0, 0x13);

			float Distance = XYZ.GetMathmaticalDistance(Math::Vector3<float>::ToSerialized(Menu::GetLocalPlayer().m_Coords));
			if (Distance <= 10.f)
			{
				Menu::GetNotify()->NotifyStackedProtectionEvent(Sender->m_PlayerInfo->m_Name, TRANSLATE(T_Sound), false, ProtectVar.m_Sound.m_Block);
			}
		}

		if (ProtectVar.m_Sound.m_Block)
		{
			return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
		}

		break;
	}

	case RAGDOLL_REQUEST_EVENT:
	case REMOTE_SCRIPT_LEAVE_EVENT:
	case BLOW_UP_VEHICLE_EVENT:
	case NETWORK_PED_SEEN_DEAD_PED_EVENT:
	case NETWORK_START_SYNCED_SCENE_EVENT:
	case GIVE_PICKUP_REWARDS_EVENT:
	case KICK_VOTES_EVENT:
	{
		return HandleEventAck(EventID, Sender, Receiver, EventIndex, EventBitset);
	}
	}

	memcpy(BitBuffer, &Cache, sizeof(Cache));
	OriginalHandleNetworkEvent(This, Sender, Receiver, EventID, EventIndex, EventBitset, BitbufferSize, BitBuffer);
}