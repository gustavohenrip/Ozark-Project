#include "network_spoofing.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "spoofing/network_spoofing_player_rotation.hpp"
#include "spoofing/network_spoofing_vehicle_rotation.hpp"
#include "spoofing/network_spoofing_ip.hpp"
#include "spoofing/network_spoofing_name.hpp"
#include "spoofing/network_spoofing_crew.hpp"
#include "spoofing/network_spoofing_rid.hpp"
#include "spoofing/network_spoofing_sync_intervals.hpp"
#include "utils/memory/memory.hpp"
#include "utils/va.hpp"
#include "utils/memory/pattern.hpp"
#include "spoofing/network_spoofing_profiles.hpp"
#include "utils/log.hpp"

using namespace NetworkSpoofingMenuVars;

namespace NetworkSpoofingMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Positions[] = {
		{ TranslationString("~c~Cycle Players", true), 0 },
		{ TranslationString("Los Santos Airport", true), 1 },
		{ TranslationString("In Place", true), 2 }
	};

	void SetPeerSpoofing() {
		if (m_Vars.m_PeerToken) {
			if (!Native::NetworkIsInSession()) {
				Memory::PatternBatch Batch;

				/*Batch.Submit({ "48 8B 05 ? ? ? ? 48 83 F8 FF 75 17 48 8D 0D ? ? ? ? E8"_Protect, 0, "PEER_1"_Protect, true }, [](uint64_t Address) {
					*(uint64_t*)(Memory::GetAddressFromInstruction(Address)) = Global::Vars::g_SpoofedPeer; // 
				});*/

				Batch.Submit({ "48 8D 0D ? ? ? ? E8 ? ? ? ? 84 C0 74 22 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D"_Protect, 0, "PEER_2"_Protect, true }, [](uint64_t Address) {
					*(uint64_t*)(Memory::GetAddressFromInstruction(Address) + 0x60) = Global::Vars::g_SpoofedPeer; // 0x40 //
				});

				Batch.Submit({ "48 8D 0D ? ? ? ? 48 69 C0 ? ? ? ? F6 84 08 ? ? ? ? ? 74 08 8B 84 08"_Protect, 0, "PEER_3"_Protect, true }, [](uint64_t Address) {
					*(uint64_t*)(Memory::GetAddressFromInstruction(Address) + 0x60) = Global::Vars::g_SpoofedPeer; // 0x40
				});

				//Batch.Submit({ "48 8B 1D ? ? ? ? 48 8D 4E 20 E8 ? ? ? ? 48 8B CB 8B D0 E8 ? ? ? ? F3 0F 10 1D ? ? ? ? 44 0F 28 C8 F3 44 0F 59 CB EB 08 F3 0F 10 1D"_Protect, 0, "PEER_4"_Protect, true }, [](uint64_t Address) {
				//	*(uint64_t*)(*(uint64_t*)(Memory::GetAddressFromInstruction(Address)) + 0x13D0) = Global::Vars::g_SpoofedPeer;
				//});

				Batch.Flush();

				if (Global::Vars::g_GetPeerToken) {
					// *(uint32_t*)(Memory::GetAddressFromInstruction(Global::Vars::g_GetPeerToken + 0xF4)) = 0;
					*(uint64_t*)(Memory::GetAddressFromInstruction(Global::Vars::g_GetPeerToken + 0x128)) = Global::Vars::g_SpoofedPeer; // 0x10e
				}

				if (IsValidPtr(Global::Vars::g_PedFactory)) {
					if (IsValidPtr(Global::Vars::g_PedFactory->m_LocalPed)) {
						if (IsValidPtr(Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo)) {
							Global::Vars::g_PedFactory->m_LocalPed->m_PlayerInfo->m_HostToken = Global::Vars::g_SpoofedPeer;
						}
					}
				}
			}
		}
	}
}

void NetworkSpoofingMenu::Init() {
	SetName("Spoofing");
	SetParentSubmenu<NetworkMenu>();

	if (Global::Vars::g_GetPeerToken) {
		m_Vars.m_Peer = *(uint64_t*)(Memory::GetAddressFromInstruction(Global::Vars::g_GetPeerToken + 0x128));
	}

	addOption(SubmenuOption("Profiles")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingProfilesMenu>());

	addOption(SubmenuOption("Rockstar ID")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingRockstarIDMenu>());

	addOption(SubmenuOption("Rotation")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingPlayerRotationMenu>());

	addOption(SubmenuOption("Vehicle Rotation")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingVehicleRotationMenu>());

	addOption(SubmenuOption("IP Address")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingIPMenu>());

	addOption(SubmenuOption("Name")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingNameMenu>());

	addOption(SubmenuOption("Crew")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingCrewMenu>());

	addOption(SubmenuOption("Sync Intervals")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkSpoofingSyncIntervalsMenu>()
		.addTooltip("Spoof the interval your player syncs with each client"));

	addOption(ToggleOption("Movement Animation")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_MoveAnimation).canBeSaved(m_ParentNameStack)
		.addTooltip("Stops your player doing a move animation"));

	addOption(ToggleOption("Rockstar Developer")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RockstarDeveloper).canBeSaved(m_ParentNameStack)
		.addTooltip("You NEED to use RID spoofing for this, otherwise you have the chance to get banned"));

	/*addOption(ToggleOption("Force Next Session Host")
		.addTranslation()
		.addToggle(m_Vars.m_PeerToken)
		.addOnClick(SetPeerSpoofing).canBeSaved(m_ParentNameStack)
		.addTooltip("Spoofs to gain next session host. YOU HAVE TO LOAD INTO SP FIRST OTHERWISE YOU WILL HAVE ISSUES. If you want to disable, you have to restart"));*/
}

void NetworkSpoofingMenu::Update() {}

/*Called once on submenu open*/
void NetworkSpoofingMenu::UpdateOnce() {
	if (Native::DoesEntityExist(NetworkSpoofingVehicleRotationMenuVars::m_Vars.m_GhostHandle)) {
		Native::DetachEntity(NetworkSpoofingVehicleRotationMenuVars::m_Vars.m_GhostHandle, true, false);
		Native::DeleteEntity(&NetworkSpoofingVehicleRotationMenuVars::m_Vars.m_GhostHandle);
		NetworkSpoofingVehicleRotationMenuVars::m_Vars.m_GhostHandle = 0;
	}
}

/*Called always*/
void NetworkSpoofingMenu::FeatureUpdate() {}

NetworkSpoofingMenu* _instance;
NetworkSpoofingMenu* NetworkSpoofingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingMenu::~NetworkSpoofingMenu() { delete _instance; }