#include "world_advanced_ui.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "ui/world_advanced_ui_sky.hpp"
#include "ui/world_advanced_ui_traffic_lights.hpp"
#include "ui/world_advanced_ui_vehicle.hpp"
#include "ui/world_advanced_ui_puddles.hpp"
#include "ui/world_advanced_ui_cross_multiplier.hpp"
#include "ui/world_advanced_ui_vehicle_neon.hpp"
#include "ui/world_advanced_ui_liquid.hpp"

using namespace WorldAdvancedUIMenuVars;

namespace WorldAdvancedUIMenuVars {
	Vars_ m_Vars;
}

void WorldAdvancedUIMenu::Init() {
	SetName("Advanced UI");
	SetParentSubmenu<WorldMenu>();

	addOption(SubmenuOption("Sky")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUISkyMenu>());

	addOption(SubmenuOption("Traffic Lights")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUITrafficLightMenu>());

	addOption(SubmenuOption("Vehicle")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUIVehicleMenu>());

	addOption(SubmenuOption("Vehicle Neons")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUIVehicleNeonsMenu>());

	addOption(SubmenuOption("Puddles")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUIPuddlesMenu>());

	addOption(SubmenuOption("Cross Multiplier")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUICrossMultiplierMenu>());

	addOption(SubmenuOption("Liquid")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUILiquidMenu>());

	/*
		init_weather/sub_140E12D50 Clouds
		sub_140E12480 Distant Lights
	*/
}

void WorldAdvancedUIMenu::Update() {}

/*Called once on submenu open*/
void WorldAdvancedUIMenu::UpdateOnce() {}

/*Called always*/
void WorldAdvancedUIMenu::FeatureUpdate() {}

WorldAdvancedUIMenu* _instance;
WorldAdvancedUIMenu* WorldAdvancedUIMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUIMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUIMenu::~WorldAdvancedUIMenu() { delete _instance; }