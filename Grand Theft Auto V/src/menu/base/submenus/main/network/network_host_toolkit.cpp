#include "network_host_toolkit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "rage/types/global_types.hpp"
#include "menu/base/util/global.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "rage/types/generic_types.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "toolkit/network_host_toolkit_blacklist.hpp"
#include "toolkit/network_host_toolkit_desync.hpp"
#include "toolkit/network_host_toolkit_join_block.hpp"
#include "menu/base/util/panels.hpp"
#include "utils/caller.hpp"
#include "utils/log.hpp"

using namespace NetworkHostToolkitMenuVars;

namespace NetworkHostToolkitMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Transitions[] = {
		{ TranslationString("Public", true), 1 },
		{ TranslationString("Closed Crew", true), 2 },
		{ TranslationString("Crew", true), 3 },
		{ TranslationString("Closed Friend", true), 6 },
		{ TranslationString("Solo", true), 10 },
		{ TranslationString("Invite Only", true), 11 }
	};

	ScrollStruct<int> WhoCanJoin[] = {
		{ TranslationString("Anybody", true), 0 },
		{ TranslationString("Nobody", true), 1 },
		{ TranslationString("Friends Only", true), 2 },
		{ TranslationString("Ozark Only", true), 3 }
	};

	void TransitionToNewLobby(int ID) {
		Utils::GetFiberPool()->Push([=] {
			if (ID == -1) {
				Menu::Global(Rage::Global::TransitionSessionFlag + 2).As<int>() = ID;
			} else {
				Menu::Global(Rage::Global::TransitionSession).As<int>() = ID;
			}

			Menu::Global(Rage::Global::TransitionSessionFlag).As<int>() = 1;
			Utils::WaitFiber(200);
			Menu::Global(Rage::Global::TransitionSessionFlag).As<int>() = 0;
		});
	}

	bool ForceHostOfScript(const char* Script, Rage::Network::NetGamePlayer* Player, bool Notify) {
		if (!IsValidPtr(Player)) {
			LOG_WARN("Failed to force host of %s, player isn't valid", Script);
			return false;
		}

		if (Native::NetworkGetHostOfScript(Script, -1, 0) == Player->m_ID) {
			LOG_WARN("Failed to force host of %s, player is already host", Script);
			return true;
		}

		if (!IsValidPtr(Player->m_PlayerInfo)) {
			LOG_WARN("Failed to force host of %s, player info isn't valid", Script);
			return false;
		}

		bool Forced = false;

		if (Notify) Menu::GetNotify()->NotifyStacked(NetworkHostToolkitMenu::GetInstance()->getString("Attempting to migrate script host..."));

		for (uint16_t i = 0; i < *(uint16_t*)(Global::Vars::g_ScriptHandlerPoolCount); i++) {
			Rage::Script::GtaThread* Thread = *(Rage::Script::GtaThread**)(Global::Vars::g_ScriptHandlerPool + (i * 8));
			if (IsValidPtr(Thread)) {
				if (!strcmp(Thread->m_Name, Script)) {
					Rage::Script::GameScriptHandlerNetworkComponent* Component = Thread->m_NetComponent;
					if (IsValidPtr(Component)) {
						Rage::Engine::MigrateScriptHost(Component, Player);
						Forced = true;
					}
				}
			}
		}

		if (Forced) {
			if (Notify) Menu::GetNotify()->NotifyStacked(NetworkHostToolkitMenu::GetInstance()->getString("Successfully migrated script host!"), Global::UIVars::g_NotificationSuccess);
			return true;
		} else if (Notify) Menu::GetNotify()->NotifyStacked(NetworkHostToolkitMenu::GetInstance()->getString("Failed migrating script host!"), Global::UIVars::g_NotificationError);

		return false;
	}
}

void NetworkHostToolkitMenu::Init() {
	SetName("Host Toolkit");
	SetParentSubmenu<NetworkMenu>();

	addString("Attempting to migrate script host...");
	addString("Successfully migrated script host!");
	addString("Failed migrating script host!");

	addOption(SubmenuOption("Blacklisted Players")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitBlacklistMenu>()
		.addTooltip("View and modify players who have been host kicked and are blacklisted from joining back"));

	addOption(SubmenuOption("Desync Kick")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitDesyncMenu>()
		.addTooltip("Block/redirect desync & netlimiter kicks in your session"));

	addOption(SubmenuOption("Join Blocking")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitJoinBlockingMenu>()
		.addTooltip("Prevent players joining your sessions"));

	addOption(ToggleOption("Open Extra Slot")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ExtraSlot)
		.addOnClick([] { Native::_0x49EC8030F5015F8B(m_Vars.m_ExtraSlot ? 4 : 0); }).canBeSaved(m_ParentNameStack)
		.addTooltip("Allows an extra player to join past the usual limit"));

	addOption(ButtonOption("Migrate Script Host")
		.addTranslation().addHotkey()
		.addOnClick([] { ForceHostOfScript("freemode", Menu::GetLocalPlayer().m_NetGamePlayer); }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Create Session")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Transition, 0, NUMOF(Transitions), Transitions)
		.addOnClick([] { TransitionToNewLobby(Transitions[m_Vars.m_Transition].m_Result); }));
}

void NetworkHostToolkitMenu::Update() {}

/*Called once on submenu open*/
void NetworkHostToolkitMenu::UpdateOnce() {
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_BLACKLISTED_PLAYERS", false);
}

/*Called always*/
void NetworkHostToolkitMenu::FeatureUpdate() {}

NetworkHostToolkitMenu* _instance;
NetworkHostToolkitMenu* NetworkHostToolkitMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkHostToolkitMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkHostToolkitMenu::~NetworkHostToolkitMenu() { delete _instance; }