#include "network_spoofing_sync_intervals.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"
#include "menu/base/util/timers.hpp"

using namespace NetworkSpoofingSyncIntervalsMenuVars;

namespace NetworkSpoofingSyncIntervalsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Intervals[] = {
		{ TranslationString("Very Low", true), 0, },
		{ TranslationString("Low", true), 1, },
		{ TranslationString("Medium", true), 2, },
		{ TranslationString("High", true), 3, },
		{ TranslationString("Very High", true), 4, },
	};
}

void NetworkSpoofingSyncIntervalsMenu::Init() {
	SetName("Sync Intervals");
	SetParentSubmenu<NetworkSpoofingMenu>();
}

void NetworkSpoofingSyncIntervalsMenu::Update() {
	static int Timer = 0;

	Menu::Timers::RunTimedFunction(&Timer, 250, [this] {
		ClearOptionsOffset(0);

		Menu::GetPlayerManager()->Foreach([this](Menu::PlayerVars& Player) {
			if (!Player.IsValidName()) return;

			if (!Player.m_SyncIntervalToggle) {
				if (Menu::GetLocalPlayer().m_PedPointer && Menu::GetLocalPlayer().m_PedPointer->GetNetObj()) {
					Player.m_SyncInterval = Menu::GetLocalPlayer().m_PedPointer->GetNetObj()->m_PlayerSyncIntervals[Player.m_ID];
				} else Player.m_SyncInterval = 0;
			}

			if (Player.m_SyncInterval > 4 || Player.m_SyncInterval < 0) Player.m_SyncInterval = 0;

			addOption(ScrollOption<int>(TOGGLE, Player.m_Name)
				.addToggle(Player.m_SyncIntervalToggle)
				.addScroll(Player.m_SyncInterval, 0, NUMOF(Intervals), Intervals));
		}, false, true);
	});

	if (Menu::GetBase()->m_CurrentOption >= m_Options.size()) {
		Menu::GetBase()->m_CurrentOption = (int)m_Options.size() - 1;
	}
}

/*Called once on submenu open*/
void NetworkSpoofingSyncIntervalsMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingSyncIntervalsMenu::FeatureUpdate() {
	Menu::GetPlayerManager()->Foreach([this](Menu::PlayerVars& Player) {
		if (Player.m_SyncIntervalToggle) {
			if (Menu::GetLocalPlayer().m_PedPointer && Menu::GetLocalPlayer().m_PedPointer->GetNetObj()) {
				Menu::GetLocalPlayer().m_PedPointer->GetNetObj()->m_PlayerSyncIntervals[Player.m_ID] = Player.m_SyncInterval;
			}
		}
	}, false, true);
}

NetworkSpoofingSyncIntervalsMenu* _instance;
NetworkSpoofingSyncIntervalsMenu* NetworkSpoofingSyncIntervalsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingSyncIntervalsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingSyncIntervalsMenu::~NetworkSpoofingSyncIntervalsMenu() { delete _instance; }