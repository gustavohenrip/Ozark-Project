#include "network_account_level.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account.hpp"
#include "menu/base/util/stats.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/log.hpp"

using namespace NetworkAccountLevelMenuVars;

namespace NetworkAccountLevelMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Types[] = {
		{ TranslationString("RP", true), 0 },
		{ TranslationString("Crew", true), 1 }
	};

	int Levels[] = { 1, 25, 50, 69, 75, 100, 120, 250, 420, 500, 666, 1000, 1234, 1337, 2000, 4000, 6969, 8000 };

	int LevelToRP(int Level) {
		return Menu::Global(292403).At(Level + 1).As<int>(); //
	}

	int GetRPLevel() {
		return Stats::Interface<int>::Get("CHAR_SET_RP_GIFT_ADMIN");
	}

	int GetCrewLevel() {
		return Stats::Interface<int>::Get("MPPLY_CREW_LOCAL_XP_0");
	}

	void SetRPLevel(int Level) {
		if (Level > 8000) Level = 8000;

		Stats::Interface<int>::Set("CHAR_SET_RP_GIFT_ADMIN", LevelToRP(Level));
		Menu::GetNotify()->NotifyStacked(NetworkAccountLevelMenu::GetInstance()->getString("Rank set! Change lobbies for it to take effect :)"));
	}

	void SetCrewLevel(int Level) {
		if (Level > 8000) Level = 8000;
		Stats::Interface<int>::Set("MPPLY_CREW_LOCAL_XP_0", LevelToRP(Level));
		Stats::Interface<int>::Set("MPPLY_CREW_LOCAL_XP_1", LevelToRP(Level));
		Stats::Interface<int>::Set("MPPLY_CREW_LOCAL_XP_2", LevelToRP(Level));
		Stats::Interface<int>::Set("MPPLY_CREW_LOCAL_XP_3", LevelToRP(Level));
		Stats::Interface<int>::Set("MPPLY_CREW_LOCAL_XP_4", LevelToRP(Level));
	}

	void SetLevel(int Level) {
		if (m_Vars.m_Type == 0) SetRPLevel(Level);
		else SetCrewLevel(Level);
	}

	int GetLevel() {
		if (m_Vars.m_Type == 0) return GetRPLevel();
		else return GetCrewLevel();
	}
}

void NetworkAccountLevelMenu::Init() {
	SetName("Level");
	SetParentSubmenu<NetworkAccountMenu>();

	addOption(ScrollOption<int>(SCROLL, "Type")
		.addTranslation()
		.addScroll(m_Vars.m_Type, 0, NUMOF(Types), Types));

	addOption(ButtonOption("Input Level")
		.addTranslation()
		.addKeyboard("Enter level", 4, [] (const char* Level) { SetLevel(std::stoi(Level)); }));

	addOption(BreakOption("Levels")
		.addTranslation());

	for (int Level : Levels) {
		addOption(ButtonOption("Level " + std::to_string(Level))
			.addTranslation()
			.addOnClick([=] { SetLevel(Level); }));
	}

	addString("Rank set! Change lobbies for it to take effect :)");
}

void NetworkAccountLevelMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountLevelMenu::UpdateOnce() {}

/*Called always*/
void NetworkAccountLevelMenu::FeatureUpdate() {}

NetworkAccountLevelMenu* _instance;
NetworkAccountLevelMenu* NetworkAccountLevelMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountLevelMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountLevelMenu::~NetworkAccountLevelMenu() { delete _instance; }