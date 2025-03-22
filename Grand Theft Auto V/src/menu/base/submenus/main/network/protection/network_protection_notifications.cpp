#include "network_protection_notifications.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"

using namespace NetworkProtectionNotificationsMenuVars;

namespace NetworkProtectionNotificationsMenuVars {
	Vars_ m_Vars;
}

void NetworkProtectionNotificationsMenu::Init() {
	SetName("Notifications");
	SetParentSubmenu<NetworkProtectionMenu>();

	addOption(ToggleOption("Spawns")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_PlayerSpawn).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player spawns"));

	addOption(ToggleOption("Spectates")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Spectating).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player spectates another player"));

	addOption(ToggleOption("Desync Kicks")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Desyncs).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player is desync kicking another player"));

	addOption(ToggleOption("Host Blacklists")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_HostKicks).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when the host is blacklisting another player, usually due to a host kick"));

	addOption(ToggleOption("Drops")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_MoneyDrop).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player is dropping money/rp"));

	addOption(ToggleOption("Crash")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Crash).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player tries to crash you"));

	addOption(ToggleOption("Script Host Migration")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ScriptHostMigration).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player forces script host"));

	addOption(ToggleOption("Rockstar Admin")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RockstarDeveloper).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player is detected as a real R* admin"));

	addOption(ToggleOption("Join Timeout")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_JoinTimeout).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player is trying to time you out of joining the session"));

	addOption(ToggleOption("Spoofed Chat")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SpoofedChat).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player is spoofing chat sender"));

	addOption(ToggleOption("Spoofed Text")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SpoofedText).canBeSaved(m_ParentNameStack)
		.addTooltip("Notification when a player is spoofing text message sender"));

	addString("is spectating");
	addString("you");
}

void NetworkProtectionNotificationsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionNotificationsMenu::UpdateOnce() {}

/*Called always*/
void NetworkProtectionNotificationsMenu::FeatureUpdate() {
	if (m_Vars.m_Spectating) {
		Menu::GetPlayerManager()->Foreach([this](Menu::PlayerVars& Player) {
			if (Player.m_PedPointer) {
				if (Player.m_PedPointer->GetNetObj()) {
					if (Player.m_PedPointer->GetNetObj()->m_SpectatingNetworkID) {
						Rage::Network::NetObj* Object = Rage::Engine::GetNetworkObjectFromNetworkID(*(uint64_t*)Global::Vars::g_NetworkObjectManager, Player.m_PedPointer->GetNetObj()->m_SpectatingNetworkID, true);
						if (Object) {
							if (Object->m_Owner >= 0 && Object->m_Owner < 32) {
								if (!Native::IsEntityDead(Player.m_Ped, 0)) {
									Player.m_IsModder = true;

									if ((GetTickCount() - m_Vars.m_SpectateMap[Player.m_ID][Object->m_Owner]) > 5000) {
										m_Vars.m_SpectateMap[Player.m_ID][Object->m_Owner] = GetTickCount();

										std::string Target = std::string(Menu::GetPlayer(Object->m_Owner).m_Name);
										if (!Target.compare(Menu::GetLocalPlayer().m_Name)) {
											if (!Player.m_ModderFlags[Menu::REASON_SPECTATING]) {
												NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_SPECTATING_YOU, Object->m_Owner);
											}

											Target = getString("you");
										}

										Menu::GetNotify()->NotifyStacked(Menu::Helpers::CleanPlayerName(Player.m_Name) + " " + getString("is spectating") + " " + Target);
									}

									Player.m_ModderFlags[Menu::REASON_SPECTATING] = true;
								}
							}
						}
					}
				}
			}
		});
	}
}

NetworkProtectionNotificationsMenu* _instance;
NetworkProtectionNotificationsMenu* NetworkProtectionNotificationsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionNotificationsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionNotificationsMenu::~NetworkProtectionNotificationsMenu() { delete _instance; }