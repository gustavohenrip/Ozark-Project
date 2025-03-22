#include "world.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "world/world_weather.hpp"
#include "world/world_time.hpp"
#include "world/world_ocean.hpp"
#include "world/world_map.hpp"
#include "world/world_dispatch.hpp"
#include "world/world_advanced_ui.hpp"
#include "world/world_trains.hpp"
#include "menu/base/util/global.hpp"
#include "utils/memory/memory.hpp"

using namespace WorldMenuVars;

namespace WorldMenuVars {
	Vars_ m_Vars;
}

void WorldMenu::Init() {
	SetName("World");
	SetParentSubmenu<MainMenu>();

	addString("Bullet Tracers");

	addOption(SubmenuOption("Advanced UI")
		.addTranslation().addHotkey()
		.addSubmenu<WorldAdvancedUIMenu>());

	addOption(SubmenuOption("Weather")
		.addTranslation().addHotkey()
		.addSubmenu<WorldWeatherMenu>());

	addOption(SubmenuOption("Time")
		.addTranslation().addHotkey()
		.addSubmenu<WorldTimeMenu>());

	addOption(SubmenuOption("Ocean")
		.addTranslation().addHotkey()
		.addSubmenu<WorldOceanMenu>());

	addOption(SubmenuOption("Dispatch")
		.addTranslation().addHotkey()
		.addSubmenu<WorldDispatchMenu>());

	addOption(SubmenuOption("Train")
		.addTranslation().addHotkey()
		.addSubmenu<WorldTrainMenu>());

	addOption(ToggleOption(getString("Bullet Tracers"))
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_WorldTracers).canBeSaved(m_ParentNameStack)
		.addTooltip("Show bullets being shot in the world"));

	addOption(ToggleOption("World Waypoint")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_WorldWaypoint).canBeSaved(m_ParentNameStack)
		.addTooltip("Show markers in world to your waypoint destination"));

	addOption(ToggleOption("Disable Lights")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableLights)
		.addOnClick([] { Native::_SetBlackout(false); }).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Enable Sky")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableSky).canBeSaved(m_ParentNameStack)
		.addOnClick([] { *(bool*)((uint64_t)Global::Vars::g_UIWeather - 0x60) = m_Vars.m_DisableSky; }));

	addOption(ToggleOption("Render Water on Map")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RenderWaterOnMap)
		.addOnClick([] { Memory::Write(Global::Vars::g_RenderWaterOnMapIntensity, 0.f); }).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Interior Refresh")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::RefreshInterior(Native::GetInteriorFromEntity(Menu::GetLocalPlayer().m_Ped)); }));

	addOption(ButtonOption("Clear Area")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::ClearArea(Menu::GetLocalPlayer().m_Coords.m_X, Menu::GetLocalPlayer().m_Coords.m_Y, Menu::GetLocalPlayer().m_Coords.m_Z, 1000.f, false, false, false, false); }));

	addOption(NumberOption<float>(SCROLL, "Fog on Map")
		.addTranslation()
		.addNumber(m_Vars.m_Fog, "%.4f", 0.0001f).addMin(0.f).addMax(10.f).setScrollSpeed(20).canBeSaved(m_ParentNameStack)
		.addOnClick([] { *(float*)Global::Vars::g_MapFogIntensity = m_Vars.m_Fog; }));
}

void WorldMenu::Update() {}

/*Called once on submenu open*/
void WorldMenu::UpdateOnce() {
	m_Vars.m_Fog = *(float*)Global::Vars::g_MapFogIntensity;
	m_Vars.m_DisableSky = *(bool*)((uint64_t)Global::Vars::g_UIWeather - 0x60);
}

/*Called always*/
void WorldMenu::FeatureUpdate() {
	if (m_Vars.m_WorldWaypoint) {
		if (Global::Vars::g_WaypointData) {
			if (Global::Vars::g_WaypointData->m_Points && Global::Vars::g_WaypointData->m_Count) {
				for (uint32_t i = 0; i < Global::Vars::g_WaypointData->m_Count; i++) {
					if (i > 40) break;

					Math::Vector4<float> Point = Global::Vars::g_WaypointData->m_Points[i];
					Native::DrawMarker(28, Point.m_X, Point.m_Y, Point.m_Z + 0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0);
				}
			}
		}
	}

	if (m_Vars.m_WorldTracers) {
		for (int i = 0; i < 500; i++) {
			Tracer Trace = m_Vars.m_Tracers[i];
			if (Trace.m_Populated) {
				if (GetTickCount() - Trace.m_SpawnTime > 3000) {
					Trace.m_Populated = false;
					continue;
				}

				Native::DrawLine(Trace.m_SpawnPosition.m_X, Trace.m_SpawnPosition.m_Y, Trace.m_SpawnPosition.m_Z, Trace.m_EndPosition.m_X, Trace.m_EndPosition.m_Y, Trace.m_EndPosition.m_Z, rand() % 255, rand() % 255, rand() % 255, 255);;
			}
		}
	}

	if (m_Vars.m_DisableLights) {
		Native::_SetBlackout(true);
	}

	if (m_Vars.m_RenderWaterOnMap) {
		Memory::Write(Global::Vars::g_RenderWaterOnMapIntensity, 1.0f);
	}
}

WorldMenu* _instance;
WorldMenu* WorldMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldMenu::~WorldMenu() { delete _instance; }