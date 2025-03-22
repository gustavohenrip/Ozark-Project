#include "network_casino.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/util/stats.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "casino/network_casino_rigging.hpp"

using namespace NetworkCasinoMenuVars;

namespace NetworkCasinoMenuVars {
	Vars_ m_Vars;

	int func_22(int iParam0) {
		int iVar0;
		int iVar1;

		iVar0 = iParam0;
		if (iVar0 == -1) {
			iVar1 = Menu::Global(1312763).As<int>();
			if (iVar1 > -1) {
				Menu::Global(2551772).As<int>() = 0;
				iVar0 = iVar1;
			} else {
				iVar0 = 0;
				Menu::Global(2551772).As<int>() = 1;
			}
		}

		return iVar0;
	}
}

void NetworkCasinoMenu::Init() {
	SetName("Casino");
	SetParentSubmenu<NetworkMenu>();

	addOption(SubmenuOption("Rigging")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkCasinoRiggingMenu>());

	addOption(ButtonOption("Remove Ban")
		.addTranslation().addHotkey()
		.addOnClick([] {
			if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("casino_lucky_wheel"))) {
				uint64_t Local = Rage::Engine::GetScriptLocals("casino_lucky_wheel");
				if (Local) {
					Native::ClearBit(Menu::ScriptLocal(Local, 107).Get<int>(), 25);
				}

				Menu::Global(1696932).As<int>() = 0;
				Stats::Interface<int>::Set("MPPLY_CASINO_CHIPS_WON_GD", 0);
				Stats::Interface<int>::Set("MPPLY_CASINO_CHIPS_WONTIM", 0);
			}
		}));

	addOption(ButtonOption("Reset Lucky Wheel Usage")
		.addTranslation().addHotkey()
		.addOnClick([] {
			if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("casino_lucky_wheel"))) {
				Rage::Engine::ProcessBasketTransaction(1888666031, 1668906031, 4, { { 147735386, -1, 0, 0, 0 } });

				uint64_t Local = Rage::Engine::GetScriptLocals("casino_lucky_wheel");
				if (Local) {
					Native::SetBit(Menu::ScriptLocal(Local, 107).Get<int>(), 20);
				}

				Menu::Global(2462410).As<int>() = 0;
				Stats::Interface<int>::Set(Menu::Global(2552060).At(8350, 3).At(func_22(-1) + 1).As<uint32_t>(), 0);
				Stats::Interface<int>::Set("MPPLY_LUCKY_WHEEL_USAGE", 0);
			}
		}));
}

void NetworkCasinoMenu::Update() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("am_mp_casino")) <= 0) {
		Menu::GetSubmenuHandler()->SetSubmenu(NetworkMenu::GetInstance());
	}
}

/*Called once on submenu open*/
void NetworkCasinoMenu::UpdateOnce() {
	Global::UIVars::g_InsideCasino = true;
	Global::UIVars::g_InsideCasinoLuckyWheel = false;
	Global::UIVars::g_InsideCasinoSlotMachines = false;
	Global::UIVars::g_InsideCasinoBlackjack = false;
	Global::UIVars::g_InsideCasinoBlackjackHigh = false;
	Global::UIVars::g_InsideCasinoPoker = false;
	Global::UIVars::g_InsideCasinoPokerHigh = false;
	Global::UIVars::g_InsideCasinoRoulette = false;
	Global::UIVars::g_InsideCasinoRouletteHigh = false;
}

/*Called always*/
void NetworkCasinoMenu::FeatureUpdate() {}

NetworkCasinoMenu* _instance;
NetworkCasinoMenu* NetworkCasinoMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoMenu::~NetworkCasinoMenu() { delete _instance; }