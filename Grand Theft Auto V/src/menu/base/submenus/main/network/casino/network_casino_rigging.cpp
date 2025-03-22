#include "network_casino_rigging.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_casino.hpp"
#include "menu/base/util/global.hpp"
#include "rigging/network_casino_rigging_blackjack.hpp"
#include "rigging/network_casino_rigging_slots.hpp"
#include "rigging/network_casino_rigging_roulette.hpp"
#include "menu/base/submenus/main/network.hpp"

using namespace NetworkCasinoRiggingMenuVars;

namespace NetworkCasinoRiggingMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> LuckyWheels[] = {
		{ TranslationString("Default", true), 0 },
		{ TranslationString("Clothing", true), 0 },
		{ TranslationString("Mystery", true), 0 },
		{ TranslationString("Podium Vehicle", true), 0 },
		{ TranslationString("Discount Voucher", true), 0 },
		{ TranslationString("2500 RP", true), 0 },
		{ TranslationString("5000 RP", true), 0 },
		{ TranslationString("7500 RP", true), 0 },
		{ TranslationString("10,000 RP", true), 0 },
		{ TranslationString("15,000 RP", true), 0 },
		{ TranslationString("20,000 Cash", true), 0 },
		{ TranslationString("30,000 Cash", true), 0 },
		{ TranslationString("40,000 Cash", true), 0 },
		{ TranslationString("50,000 Cash", true), 0 },
		{ TranslationString("10,000 Chips", true), 0 },
		{ TranslationString("15,000 Chips", true), 0 },
		{ TranslationString("20,000 Chips", true), 0 },
		{ TranslationString("25,000 Chips", true), 0 }
	};

	void ResetLuckyWheel() {
		for (int i = 26228; i <= 26235; i++) {
			Menu::Global(262145).At(i).As<int>() = 0;
		}

		for (int i = 26119; i <= 26138; i++) {
			Menu::Global(262145).At(i).As<int>() = 5;
		}
	}

	void HandleLuckyWheel() {
		ResetLuckyWheel();

		switch (m_Vars.m_LuckyWheel) {
			case 1: Menu::Global(262145).At(26119).As<int>() = 0xFFFF; break; // 
			case 2: Menu::Global(262145).At(26136).As<int>() = 0xFFFF; break; // 
			case 3: Menu::Global(262145).At(26137).As<int>() = 0xFFFF; break; // 
			case 4: Menu::Global(262145).At(26138).As<int>() = 0xFFFF; break; // 
			case 5: Menu::Global(262145).At(26123).As<int>() = 0xFFFF; break; // 
			case 6: Menu::Global(262145).At(26124).As<int>() = 0xFFFF; break; // 
			case 7: Menu::Global(262145).At(26125).As<int>() = 0xFFFF; break; // 
			case 8: Menu::Global(262145).At(26126).As<int>() = 0xFFFF; break; // 
			case 9: Menu::Global(262145).At(26127).As<int>() = 0xFFFF; break; // 
			case 10: Menu::Global(262145).At(26128).As<int>() = 0xFFFF; break; // 
			case 11: Menu::Global(262145).At(26129).As<int>() = 0xFFFF; break; // 
			case 12: Menu::Global(262145).At(26130).As<int>() = 0xFFFF; break; // 
			case 13: Menu::Global(262145).At(26131).As<int>() = 0xFFFF; break; // 
			case 14: Menu::Global(262145).At(26132).As<int>() = 0xFFFF; break; // 
			case 15: Menu::Global(262145).At(26133).As<int>() = 0xFFFF; break; // 
			case 16: Menu::Global(262145).At(26134).As<int>() = 0xFFFF; break; // 
			case 17: Menu::Global(262145).At(26135).As<int>() = 0xFFFF; break; // 
		}
	}
}

void NetworkCasinoRiggingMenu::Init() {
	SetName("Rigging");
	SetParentSubmenu<NetworkCasinoMenu>();

	addOption(SubmenuOption("Slot Machines")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkCasinoRiggingSlotsMenu>());

	addOption(SubmenuOption("Blackjack")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkCasinoRiggingBlackjackMenu>());

	/*addOption(SubmenuOption("Poker")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkCasinoRiggingMenu>()); // TODO*/

	addOption(SubmenuOption("Roulette")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkCasinoRiggingRouletteMenu>());

	addOption(ScrollOption<int>(SCROLL, "Lucky Wheel")
		.addTranslation()
		.addScroll(m_Vars.m_LuckyWheel, 0, NUMOF(LuckyWheels), LuckyWheels)
		.addOnClick(HandleLuckyWheel));
}

void NetworkCasinoRiggingMenu::Update() {
	Global::UIVars::g_InsideCasino = false;
	Global::UIVars::g_InsideCasinoLuckyWheel = false;
	Global::UIVars::g_InsideCasinoSlotMachines = false;
	Global::UIVars::g_InsideCasinoBlackjack = false;
	Global::UIVars::g_InsideCasinoBlackjackHigh = false;
	Global::UIVars::g_InsideCasinoPoker = false;
	Global::UIVars::g_InsideCasinoPokerHigh = false;
	Global::UIVars::g_InsideCasinoRoulette = false;
	Global::UIVars::g_InsideCasinoRouletteHigh = false;

	switch (Menu::GetBase()->m_CurrentOption) {
		case 0:
			Global::UIVars::g_InsideCasinoSlotMachines = true;
			break;

		case 1:
			Global::UIVars::g_InsideCasinoBlackjack = true;
			break;

		/*case 2:
			Global::UIVars::g_InsideCasinoPoker = true;
			break;*/

		case 2:
			Global::UIVars::g_InsideCasinoRoulette = true;
			break;

		case 3:
			Global::UIVars::g_InsideCasinoLuckyWheel = true;
			break;
	}


	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("am_mp_casino")) <= 0) {
		Menu::GetSubmenuHandler()->SetSubmenu(NetworkMenu::GetInstance());
	}
}

/*Called once on submenu open*/
void NetworkCasinoRiggingMenu::UpdateOnce() {}

/*Called always*/
void NetworkCasinoRiggingMenu::FeatureUpdate() {}

NetworkCasinoRiggingMenu* _instance;
NetworkCasinoRiggingMenu* NetworkCasinoRiggingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoRiggingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoRiggingMenu::~NetworkCasinoRiggingMenu() { delete _instance; }