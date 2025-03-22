#include "misc_tunables.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/global.hpp"

using namespace MiscTunablesMenuVars;

namespace MiscTunablesMenuVars {
	Vars_ m_Vars;

	template <typename TYPE>
	void SetTunable(int Tunable, TYPE Value) {
		Menu::Global(262145).At(Tunable).As<TYPE>() = Value;
	}

	void ToggleXmasDLC() {
		SetTunable(4734, (int)m_Vars.m_XmasDLC);
		SetTunable(8931, (int)!m_Vars.m_XmasDLC);
		SetTunable(8932, (int)!m_Vars.m_XmasDLC);
		SetTunable(8982, (int)m_Vars.m_XmasDLC);
	}

	void ToggleValentinesDLC() {
		SetTunable(6599, (int)m_Vars.m_ValentinesDLC);
		SetTunable(12909, (int)m_Vars.m_ValentinesDLC);
		SetTunable(12910, (int)m_Vars.m_ValentinesDLC);
		SetTunable(12961, (int)m_Vars.m_ValentinesDLC);
	}

	void ToggleIndependenceDLC() {
		SetTunable(7794, (int)m_Vars.m_IndependenceDLC);
		SetTunable(7803, (int)m_Vars.m_IndependenceDLC);
		SetTunable(7808, (int)m_Vars.m_IndependenceDLC);
		SetTunable(7809, (int)m_Vars.m_IndependenceDLC);
	}

	void ToggleFreeShopping() {
		for (int i = 73; i < 84; i++) {
			SetTunable(i, m_Vars.m_FreeShopping ? 0 : 0x3F800000);
		}
	}

	void ToggleSCTV() {
		SetTunable(8269, (int)m_Vars.m_DisableSCTV);
		SetTunable(8270, (int)m_Vars.m_DisableSCTV);
		SetTunable(8271, (int)m_Vars.m_DisableSCTV);
	}
}

void MiscTunablesMenu::Init() {
	SetName("Tunables");
	SetParentSubmenu<MiscMenu>();

	addOption(ToggleOption("Disable Idle Kick")
		.addTranslation()
		.addToggle(m_Vars.m_DisableIdleKick).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Utility Fees")
		.addTranslation()
		.addToggle(m_Vars.m_DisableUtilityFees).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable SCTV")
		.addTranslation()
		.addToggle(m_Vars.m_DisableSCTV).canBeSaved(m_ParentNameStack)
		.addOnClick(ToggleSCTV));

	addOption(ToggleOption("Christmas DLC")
		.addTranslation()
		.addToggle(m_Vars.m_XmasDLC).canBeSaved(m_ParentNameStack)
		.addOnClick(ToggleXmasDLC));

	addOption(ToggleOption("Valentines DLC")
		.addTranslation()
		.addToggle(m_Vars.m_ValentinesDLC).canBeSaved(m_ParentNameStack)
		.addOnClick(ToggleValentinesDLC));

	addOption(ToggleOption("Independence Day DLC")
		.addTranslation()
		.addToggle(m_Vars.m_IndependenceDLC).canBeSaved(m_ParentNameStack)
		.addOnClick(ToggleIndependenceDLC));

	addOption(ToggleOption("Free Shopping")
		.addTranslation()
		.addToggle(m_Vars.m_FreeShopping)
		.addOnClick(ToggleFreeShopping).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Unlimited Fireworks")
		.addTranslation()
		.addToggle(m_Vars.m_UnlimitedFireworks)
		.addOnClick([] { SetTunable(7795, m_Vars.m_UnlimitedFireworks ? std::numeric_limits<int>::max() : 10); }).canBeSaved(m_ParentNameStack));
}

void MiscTunablesMenu::Update() {}

/*Called once on submenu open*/
void MiscTunablesMenu::UpdateOnce() {}

/*Called always*/
void MiscTunablesMenu::FeatureUpdate() {
	if (m_Vars.m_DisableUtilityFees) {
		SetTunable(157, 0);
		SetTunable(158, 0);
		SetTunable(159, 0);
		SetTunable(6647, 0);
		SetTunable(7769, 0);
		SetTunable(7221, 0);
		SetTunable(6646, 0);
		SetTunable(6649, 0);
		SetTunable(6645, 0);
		SetTunable(7797, 0);
		SetTunable(7688, 0);
	}

	if (m_Vars.m_DisableIdleKick) {
		SetTunable(87, std::numeric_limits<int>::max());
		SetTunable(88, std::numeric_limits<int>::max());
		SetTunable(89, std::numeric_limits<int>::max());
		SetTunable(90, std::numeric_limits<int>::max());
	}

	if (m_Vars.m_XmasDLC) {
		ToggleXmasDLC();
	}

	if (m_Vars.m_ValentinesDLC) {
		ToggleValentinesDLC();
	}

	if (m_Vars.m_IndependenceDLC) {
		ToggleIndependenceDLC();
	}

	if (m_Vars.m_DisableSCTV) {
		ToggleSCTV();
	}
}

MiscTunablesMenu* _instance;
MiscTunablesMenu* MiscTunablesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscTunablesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscTunablesMenu::~MiscTunablesMenu() { delete _instance; }