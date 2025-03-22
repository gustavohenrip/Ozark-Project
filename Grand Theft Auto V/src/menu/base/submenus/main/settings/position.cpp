#include "position.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "global/ui_vars.hpp"


namespace PositionMenuVars {
	Vars_ m_Vars;
}

void PositionMenu::Init() {
	SetName("Positions");
	SetParentSubmenu<SettingsMenu>();

	addOption(NumberOption<float>(SCROLL, "Position X")
		.addTranslation()
		.addNumber(Global::UIVars::g_MenuX, "%.3f", .001f).addMin(0.f).addMax(1.f).setScrollSpeed(1).canLoop());

	addOption(NumberOption<float>(SCROLL, "Position Y")
		.addTranslation()
		.addNumber(Global::UIVars::g_MenuY, "%.3f", .001f).addMin(0.f).addMax(1.f).setScrollSpeed(1).canLoop());
}

void PositionMenu::Update() {}

/*Called once on submenu open*/
void PositionMenu::UpdateOnce() {}

/*Called always*/
void PositionMenu::FeatureUpdate() {}

PositionMenu* _instance;
PositionMenu* PositionMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PositionMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PositionMenu::~PositionMenu() { delete _instance; }