#include "object_local.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../object.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "spawner/object_spawner_edit.hpp"

using namespace ObjectLocalMenuVars;

namespace ObjectLocalMenuVars {
	Vars_ m_Vars;
}

void ObjectLocalMenu::Init() {
	SetName("Local Objects");
	SetParentSubmenu<ObjectMenu>();

	addOption(SubmenuOption("ESP")
		.addTranslation()
		.addSubmenu<ESPMenu>()
		.addOnClick([] {
			ESPMenu::GetInstance()->SetParentSubmenu<ObjectLocalMenu>();
			ESPMenuVars::m_Vars.m_ESP = &m_Vars.m_ESP;
			ESPMenuVars::m_Vars.m_Player = false;
		}));

	addOption(ButtonOption("Freeze Position")
		.addTranslation()
		.addOnClick([] { ObjectSpawnerEditMenuVars::Run(2, false, true, 0, 0); }));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] { ObjectSpawnerEditMenuVars::Run(1, false, true, 0, 0); }));
}

void ObjectLocalMenu::Update() {}

/*Called once on submenu open*/
void ObjectLocalMenu::UpdateOnce() {}

/*Called always*/
void ObjectLocalMenu::FeatureUpdate() {
	if (m_Vars.m_ESP.m_NameLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_NameRainbowToggle
		|| m_Vars.m_ESP.m_Name
		|| m_Vars.m_ESP.m_SnaplineLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_SnaplineRainbowToggle
		|| m_Vars.m_ESP.m_Snapline
		|| m_Vars.m_ESP.m_3DLineOfSightRainbowToggle
		|| m_Vars.m_ESP.m_3DRainbowToggle
		|| m_Vars.m_ESP.m_3D) {
		Menu::Pools::GetObjectPool()->Run([](Object Obj) {
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

ObjectLocalMenu* _instance;
ObjectLocalMenu* ObjectLocalMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectLocalMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectLocalMenu::~ObjectLocalMenu() { delete _instance; }