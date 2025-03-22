#include "ped_local.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../ped.hpp"
#include "spawner/ped_spawner_edit.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace PedLocalMenuVars;

namespace PedLocalMenuVars {
	Vars_ m_Vars;
}

void PedLocalMenu::Init() {
	SetName("Local NPCs");
	SetParentSubmenu<PedMenu>();

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<PedLocalMenu>();
			ESPMenuVars::m_Vars.m_ESP = &m_Vars.m_ESP;
			ESPMenuVars::m_Vars.m_Player = false;
		}));

	addOption(ButtonOption("Revive")
		.addTranslation()
		.addOnClick([] { PedSpawnerEditMenuVars::Run(2, false, true, nullptr, 0); }));

	addOption(ButtonOption("Kill")
		.addTranslation()
		.addOnClick([] { PedSpawnerEditMenuVars::Run(3, false, true, nullptr, 0); }));

	addOption(ButtonOption("Explode")
		.addTranslation()
		.addOnClick([] { PedSpawnerEditMenuVars::Run(4, false, true, nullptr, 0); }));

	addOption(ButtonOption("Clone")
		.addTranslation()
		.addOnClick([] { PedSpawnerEditMenuVars::Run(5, false, true, nullptr, 0); }));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] { PedSpawnerEditMenuVars::Run(1, false, true, nullptr, 0); }));
}

void PedLocalMenu::Update() {}

/*Called once on submenu open*/
void PedLocalMenu::UpdateOnce() {}

/*Called always*/
void PedLocalMenu::FeatureUpdate() {
	if (m_Vars.m_ESP.m_NameLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_NameRainbowToggle
		|| m_Vars.m_ESP.m_Name
		|| m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_SnaplineRainbowToggle
		|| m_Vars.m_ESP.m_Snapline
		|| m_Vars.m_ESP.m_3DLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_3DRainbowToggle
		|| m_Vars.m_ESP.m_3D) {
		Menu::Pools::GetPedPool()->Run([](Object Obj) {
			// Name ESP
			if (m_Vars.m_ESP.m_NameLineOfSightRainbowToggle) {
				m_Vars.m_ESP.m_NameLineOfSightRainbow.Run();
			}

			if (m_Vars.m_ESP.m_NameRainbowToggle) {
				m_Vars.m_ESP.m_NameRainbow.Run();
			}

			if (m_Vars.m_ESP.m_Name) {
				ESPMenuVars::NameESP(m_Vars.m_ESP, Obj);
			}

			// Snapline ESP
			if (m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle) {
				m_Vars.m_ESP.m_SnaplineLineOfSightRainbow.Run();
			}

			if (m_Vars.m_ESP.m_SnaplineRainbowToggle) {
				m_Vars.m_ESP.m_SnaplineRainbow.Run();
			}

			if (m_Vars.m_ESP.m_Snapline) {
				ESPMenuVars::SnaplineESP(m_Vars.m_ESP, Obj);
			}

			// 3D ESP
			if (m_Vars.m_ESP.m_3DLineOfSightRainbowToggle) {
				m_Vars.m_ESP.m_3DLineOfSightRainbow.Run();
			}

			if (m_Vars.m_ESP.m_3DRainbowToggle) {
				m_Vars.m_ESP.m_3DRainbow.Run();
			}

			if (m_Vars.m_ESP.m_3D) {
				ESPMenuVars::_3DESP(m_Vars.m_ESP, Obj);
			}
		});
	}
}

PedLocalMenu* _instance;
PedLocalMenu* PedLocalMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PedLocalMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PedLocalMenu::~PedLocalMenu() { delete _instance; }