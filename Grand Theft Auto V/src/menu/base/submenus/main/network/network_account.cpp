#include "network_account.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "menu/base/util/global.hpp"
#include "account/network_account_level.hpp"
#include "account/network_account_unlocks.hpp"
#include "account/network_account_times.hpp"
#include "account/network_account_outfits.hpp"
#include "account/network_account_win_loss.hpp"
#include "account/network_account_money.hpp"
#include "account/network_account_kd.hpp"
#include "menu/base/util/stats.hpp"

using namespace NetworkAccountMenuVars;

namespace NetworkAccountMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Characters[] = {
		{ TranslationString("PLACEHOLDER", true), 0 },
		{ TranslationString("PLACEHOLDER", true), 1 }
	};

	int GetCharacterCount() {
		const char* CharacterTwo = Native::StatGetString(0xD2AB0EC6, -1);
		if (!CharacterTwo || strlen(CharacterTwo) < 1) return 1;
		return 2;
	}
}

void NetworkAccountMenu::Init() {
	SetName("Account");
	SetParentSubmenu<NetworkMenu>();

	int CurrentChar = 0;
	Native::StatGetInt(0x2F2F120F, &CurrentChar, -1);

	m_Vars.m_SelectedCharacter = CurrentChar ? 1 : 0;

	addOption(ScrollOption<int>(SCROLL, "Selected Character")
		.addTranslation()
		.addScroll(m_Vars.m_SelectedCharacter, 0, NUMOF(Characters), Characters)
		.addOnUpdate([] (ScrollOption<int>* Option) { Option->addScroll(m_Vars.m_SelectedCharacter, 0, GetCharacterCount(), Characters); }));

	/*addOption(SubmenuOption("Money")
		.addTranslation()
		.addSubmenu<NetworkAccountMoneyMenu>());*/

	addOption(SubmenuOption("Level")
		.addTranslation()
		.addSubmenu<NetworkAccountLevelMenu>());

	addOption(SubmenuOption("Unlocks")
		.addTranslation()
		.addSubmenu<NetworkAccountUnlocksMenu>());

	addOption(SubmenuOption("Times")
		.addTranslation()
		.addSubmenu<NetworkAccountTimesMenu>());

	addOption(SubmenuOption("Outfits")
		.addTranslation()
		.addSubmenu<NetworkAccountOutfitsMenu>());

	addOption(SubmenuOption("Wins and Losses")
		.addTranslation()
		.addSubmenu<NetworkAccountWinsLossesMenu>());

	addOption(SubmenuOption("K/D")
		.addTranslation()
		.addSubmenu<NetworkAccountKDMenu>());

	addOption(ButtonOption("Redesign Character")
		.addTranslation()
		.addOnClick([] { Stats::Interface<bool>::Set("FM_CHANGECHAR_ASKED", false); }));

	addOption(ButtonOption("Clear Mental State")
		.addTranslation()
		.addOnClick([] { Stats::Interface<float>::Set("PLAYER_MENTAL_STATE", 0.0f); }));

	addString("Unavailable");
}

void NetworkAccountMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountMenu::UpdateOnce() {
	Global::UIVars::g_InsideCasino = false;

	Characters[0].m_Name.Set(Native::StatGetString(0x4A211FC8, -1));
	Characters[1].m_Name.Set(Native::StatGetString(0xD2AB0EC6, -1));

	if (!Native::NetworkIsInSession()) {
		Characters[0].m_Name.Set("~c~" + getString("Unavailable"));
		Characters[1].m_Name.Set("~c~" + getString("Unavailable"));
	}
}

/*Called always*/
void NetworkAccountMenu::FeatureUpdate() {}

NetworkAccountMenu* _instance;
NetworkAccountMenu* NetworkAccountMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountMenu::~NetworkAccountMenu() { delete _instance; }