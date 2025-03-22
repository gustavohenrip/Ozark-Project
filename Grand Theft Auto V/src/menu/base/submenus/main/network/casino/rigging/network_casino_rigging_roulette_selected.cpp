#include "network_casino_rigging_roulette_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_casino_rigging_roulette.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/global.hpp"
#include "../../network_host_toolkit.hpp"

using namespace NetworkCasinoRiggingRouletteSelectedMenuVars;

namespace NetworkCasinoRiggingRouletteSelectedMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Outcomes[] = {
		{ TranslationString("~g~Zero", true), 0 },
		{ TranslationString("~g~Double Zero", true), 37 },
		{ TranslationString("~r~1", true), 1 },
		{ TranslationString("~u~2", true), 2 },
		{ TranslationString("~r~3", true), 3 },
		{ TranslationString("~u~4", true), 4 },
		{ TranslationString("~r~5", true), 5 },
		{ TranslationString("~u~6", true), 6 },
		{ TranslationString("~r~7", true), 7 },
		{ TranslationString("~u~8", true), 8 },
		{ TranslationString("~r~9", true), 9 },
		{ TranslationString("~u~10", true), 10 },
		{ TranslationString("~u~11", true), 11 },
		{ TranslationString("~r~12", true), 12 },
		{ TranslationString("~u~13", true), 13 },
		{ TranslationString("~r~14", true), 14 },
		{ TranslationString("~u~15", true), 15 },
		{ TranslationString("~r~16", true), 16 },
		{ TranslationString("~u~17", true), 17 },
		{ TranslationString("~r~18", true), 18 },
		{ TranslationString("~r~19", true), 19 },
		{ TranslationString("~u~20", true), 20 },
		{ TranslationString("~r~21", true), 21 },
		{ TranslationString("~u~22", true), 22 },
		{ TranslationString("~r~23", true), 23 },
		{ TranslationString("~u~24", true), 24 },
		{ TranslationString("~r~25", true), 25 },
		{ TranslationString("~u~26", true), 26 },
		{ TranslationString("~r~27", true), 27 },
		{ TranslationString("~u~28", true), 28 },
		{ TranslationString("~u~29", true), 29 },
		{ TranslationString("~r~30", true), 30 },
		{ TranslationString("~u~31", true), 31 },
		{ TranslationString("~r~32", true), 32 },
		{ TranslationString("~u~33", true), 33 },
		{ TranslationString("~r~34", true), 34 },
		{ TranslationString("~u~35", true), 35 },
		{ TranslationString("~r~36", true), 36 },
	};

	bool MigrateRouletteHost() {
		return NetworkHostToolkitMenuVars::ForceHostOfScript("casinoroulette", Menu::GetLocalPlayer().m_NetGamePlayer, false);
	}

	void BreakGame(NetworkCasinoRiggingRouletteSelectedMenu* This) {
		uint64_t RouletteLocals = Rage::Engine::GetScriptLocals("casinoroulette");
		if (RouletteLocals) {
			if (MigrateRouletteHost()) {
				Menu::ScriptLocal(RouletteLocals, 130).At(1357).At(153).At(m_Vars.m_Selected, 1).As<int>() = 38;
			}
		}
	}
}

void NetworkCasinoRiggingRouletteSelectedMenu::Init() {
	SetName("Roulette");
	SetParentSubmenu<NetworkCasinoRiggingRouletteMenu>();

	addString("The game needs to be in progress to end it");
	addString("Winning Outcome");

	addOption(ButtonOption("Break Game")
		.addTranslation()
		.addOnClick([this] { BreakGame(this); })
		.addTooltip("Players at the table will be unable to leave"));
}

void NetworkCasinoRiggingRouletteSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkCasinoRiggingRouletteSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(1);
	
	addOption(ScrollOption<int>(TOGGLE, getString("Winning Outcome"))
		.addToggle(m_Vars.m_Tables[m_Vars.m_Selected].m_Enabled)
		.addScroll(m_Vars.m_Tables[m_Vars.m_Selected].m_Rigged, 0, NUMOF(Outcomes), Outcomes));
}

/*Called always*/
void NetworkCasinoRiggingRouletteSelectedMenu::FeatureUpdate() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("casinoroulette")) > 0) {
		for (int i = 0; i < 6; i++) {
			if (m_Vars.m_Tables[i].m_Enabled) {
				uint64_t RouletteLocals = Rage::Engine::GetScriptLocals("casinoroulette");
				if (RouletteLocals) {
					if (Menu::ScriptLocal(RouletteLocals, 130).At(1357).At(7).At(i, 1).As<int>() == 4) {
						if (MigrateRouletteHost()) {
							Menu::ScriptLocal(RouletteLocals, 130).At(1357).At(153).At(i, 1).As<int>() = Outcomes[m_Vars.m_Tables[i].m_Rigged].m_Result;
						}
					}
				}
			}
		}
	}
}

NetworkCasinoRiggingRouletteSelectedMenu* _instance;
NetworkCasinoRiggingRouletteSelectedMenu* NetworkCasinoRiggingRouletteSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoRiggingRouletteSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoRiggingRouletteSelectedMenu::~NetworkCasinoRiggingRouletteSelectedMenu() { delete _instance; }