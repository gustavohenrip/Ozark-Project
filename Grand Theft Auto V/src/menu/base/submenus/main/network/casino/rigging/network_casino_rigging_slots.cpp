#include "network_casino_rigging_slots.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_casino_rigging.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/global.hpp"
#include "../../../network.hpp"

using namespace NetworkCasinoRiggingSlotsMenuVars;

namespace NetworkCasinoRiggingSlotsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<eCasinoSlots> Slots[] = {
		{ TranslationString("Cherry", true), WHEEL_SYMBOL_CHERRY },
		{ TranslationString("Plum", true), WHEEL_SYMBOL_PLUM },
		{ TranslationString("Melon", true), WHEEL_SYMBOL_MELON },
		{ TranslationString("Bell", true), WHEEL_SYMBOL_BELL },
		{ TranslationString("Seven", true), WHEEL_SYMBOL_SEVEN },
		{ TranslationString("Jackpot", true), WHEEL_SYMBOL_JACKPOT },
		{ TranslationString("Bottle", true), WHEEL_SYMBOL_ANKH }
	};

	void SetSymbolOutcomeForWheel(int Wheel, int Symbol) {
		uint64_t Locals = Rage::Engine::GetScriptLocals("casino_slots");
		if (Locals) {
			for (int i = 0; i < 64; i++) {
				Menu::ScriptLocal(Locals, 1354).At(1).At(Wheel, 65).At(i, 1).As<int>() = Symbol;
			}
		}
	}
}

void NetworkCasinoRiggingSlotsMenu::Init() {
	SetName("Slot Machines");
	SetParentSubmenu<NetworkCasinoRiggingMenu>();

	addOption(ScrollOption<eCasinoSlots>(TOGGLE, "First Wheel")
		.addTranslation()
		.addToggle(m_Vars.m_First)
		.addScroll(m_Vars.m_FirstSlot, 0, NUMOF(Slots), Slots));

	addOption(ScrollOption<eCasinoSlots>(TOGGLE, "Second Wheel")
		.addTranslation()
		.addToggle(m_Vars.m_Second)
		.addScroll(m_Vars.m_SecondSlot, 0, NUMOF(Slots), Slots));

	addOption(ScrollOption<eCasinoSlots>(TOGGLE, "Third Wheel")
		.addTranslation()
		.addToggle(m_Vars.m_Third)
		.addScroll(m_Vars.m_ThirdSlot, 0, NUMOF(Slots), Slots));
}

void NetworkCasinoRiggingSlotsMenu::Update() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("am_mp_casino")) <= 0) {
		Menu::GetSubmenuHandler()->SetSubmenu(NetworkMenu::GetInstance());
		return;
	}
}

/*Called once on submenu open*/
void NetworkCasinoRiggingSlotsMenu::UpdateOnce() {}

/*Called always*/
void NetworkCasinoRiggingSlotsMenu::FeatureUpdate() {
	if (m_Vars.m_First) {
		SetSymbolOutcomeForWheel(0, Slots[m_Vars.m_FirstSlot].m_Result);
	}

	if (m_Vars.m_Second) {
		SetSymbolOutcomeForWheel(1, Slots[m_Vars.m_SecondSlot].m_Result);
	}

	if (m_Vars.m_Third) {
		SetSymbolOutcomeForWheel(2, Slots[m_Vars.m_ThirdSlot].m_Result);
	}
}

NetworkCasinoRiggingSlotsMenu* _instance;
NetworkCasinoRiggingSlotsMenu* NetworkCasinoRiggingSlotsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoRiggingSlotsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoRiggingSlotsMenu::~NetworkCasinoRiggingSlotsMenu() { delete _instance; }