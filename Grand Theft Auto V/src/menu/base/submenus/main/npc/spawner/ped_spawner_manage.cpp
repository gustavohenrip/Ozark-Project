#include "ped_spawner_manage.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../ped_spawner.hpp"
#include "utils/va.hpp"
#include "ped_spawner_edit.hpp"

using namespace PedSpawnerManageMenuVars;

namespace PedSpawnerManageMenuVars {
	Vars_ m_Vars;
}

void PedSpawnerManageMenu::Init() {
	SetName("Manage");
	SetParentSubmenu<PedSpawnerMenu>();

	addOption(SubmenuOption("All NPCs")
		.addTranslation()
		.addOnClick([] { PedSpawnerEditMenuVars::m_Vars.m_EditingAll = true; })
		.addSubmenu<PedSpawnerEditMenu>());

	addOption(ButtonOption("Clear Manager")
		.addTranslation()
		.addOnClick([this] { m_Vars.m_SpawnedPeds.clear(); UpdateOnce(); })
		.addTooltip("Does NOT delete the NPCs from the world, only from the manager"));

	addOption(BreakOption("NPCs")
		.addTranslation());

	addString("~m~None");
}

void PedSpawnerManageMenu::Update() {}

/*Called once on submenu open*/
void PedSpawnerManageMenu::UpdateOnce() {
	ClearOptionsOffset(3);

	for (std::size_t i = 0; i < m_Vars.m_SpawnedPeds.size(); i++) {
		if (!Native::DoesEntityExist(m_Vars.m_SpawnedPeds[i].m_Handle)) {
			m_Vars.m_SpawnedPeds.erase(m_Vars.m_SpawnedPeds.begin() + i);
		}
	}

	if (!m_Vars.m_SpawnedPeds.empty()) {
		for (std::size_t i = 0; i < m_Vars.m_SpawnedPeds.size(); i++) {
			addOption(SubmenuOption(Utils::VA::VA("[%i] %s", i + 1, m_Vars.m_SpawnedPeds[i].m_Name.c_str()))
				.addOnClick([=] {
					PedSpawnerEditMenuVars::m_Vars.m_EditingAll = false;
					PedSpawnerEditMenuVars::m_Vars.m_SelectedPed = i;
				})
				.addSubmenu<PedSpawnerEditMenu>());
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void PedSpawnerManageMenu::FeatureUpdate() {}

PedSpawnerManageMenu* _instance;
PedSpawnerManageMenu* PedSpawnerManageMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PedSpawnerManageMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PedSpawnerManageMenu::~PedSpawnerManageMenu() { delete _instance; }