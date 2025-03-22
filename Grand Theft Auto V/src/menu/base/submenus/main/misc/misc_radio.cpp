#include "misc_radio.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "utils/va.hpp"
#include "menu/base/util/panels.hpp"

using namespace MiscRadioMenuVars;

namespace MiscRadioMenuVars {
	Vars_ m_Vars;
}

void MiscRadioMenu::Init() {
	SetName("Radio");
	SetParentSubmenu<MiscMenu>();
	
	addOption(ToggleOption("Mobile Radio")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_MobileRadio)
		.addOnClick([] {
			Native::SetMobileRadioEnabledDuringGameplay(m_Vars.m_MobileRadio);
			Native::SetMobilePhoneRadioState(m_Vars.m_MobileRadio);
		}).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Skip Radio Track")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::SkipRadioForward(); }));

	addOption(BreakOption("Radio Stations")
		.addTranslation());

	addString("Change to");
}

void MiscRadioMenu::Update() {}

/*Called once on submenu open*/
void MiscRadioMenu::UpdateOnce() {
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_RADIO", true);

	ClearOptionsOffset(3);

	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	std::vector<std::shared_ptr<RadioOption>> TempOptions;

	for (int i = 0; i < Native::_MaxRadioStationIndex(); i++) {
		TempOptions.push_back(addOption(RadioOption(Utils::VA::VA("%s %s", getString("Change to").c_str(), Native::_GetLabelText(Native::GetRadioStationName(i))))
			.addRadio(m_Vars.m_Context)
			.addOnClick([=] { Native::SetRadioToStationIndex(i); })));
	}

	int Index = Native::GetPlayerRadioStationIndex();
	if (Index >= 0 && Index < Native::_MaxRadioStationIndex()) {
		TempOptions[Index]->UpdateSelected();
	} else {
		m_Vars.m_Context.m_Toggles[m_Vars.m_Context.m_Selected] = false;
		m_Vars.m_Context.m_Selected = 99;
		m_Vars.m_Context.m_Toggles[99] = true;
	}
}

/*Called always*/
void MiscRadioMenu::FeatureUpdate() {}

MiscRadioMenu* _instance;
MiscRadioMenu* MiscRadioMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscRadioMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscRadioMenu::~MiscRadioMenu() { delete _instance; }