#include "network_protection_ghost_mode_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/util/timers.hpp"

using namespace NetworkProtectionGhostModeSelectedMenuVars;

namespace NetworkProtectionGhostModeSelectedMenuVars {
	Vars_ m_Vars;

	void WriteUpdate() {
		Utils::GetConfig()->WriteBool(NetworkProtectionGhostModeMenu::GetInstance()->m_ParentNameStack, "Enabled", m_Vars.m_Selected->m_Enabled, { m_Vars.m_Selected->m_Name });
		Utils::GetConfig()->WriteBool(NetworkProtectionGhostModeMenu::GetInstance()->m_ParentNameStack, "Ghost for All Players", m_Vars.m_Selected->m_DisableForSession, { m_Vars.m_Selected->m_Name });
	}
}

void NetworkProtectionGhostModeSelectedMenu::Init() {
	SetName("Ghost Mode");
	SetParentSubmenu<NetworkProtectionGhostModeMenu>();

	addString("Enabled");
	addString("Ghost for Player");
	addString("All Players");
}

void NetworkProtectionGhostModeSelectedMenu::Update() {
	if (!m_Vars.m_Populated) return;
	if (m_Vars.m_Selected->m_Type == NetObjectPlayer) return;

	static int Timer = 0;
	Menu::Timers::RunTimedFunction(&Timer, 250, [this] {
		ClearOptionsOffset(3);

		Menu::GetPlayerManager()->Foreach([this] (Menu::PlayerVars& Player) {
			addOption(ToggleOption(Player.m_Name)
				.addToggle(Player.m_GhostDisable[m_Vars.m_Selected->m_Type]));
		});
	});
}

/*Called once on submenu open*/
void NetworkProtectionGhostModeSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	addOption(ToggleOption(getString("Enabled"))
		.addToggle(m_Vars.m_Selected->m_Enabled)
		.addOnClick(WriteUpdate));

	if (m_Vars.m_Selected->m_Type == NetObjectPlayer) return;

	addOption(ToggleOption(getString("Ghost for All Players"))
		.addToggle(m_Vars.m_Selected->m_DisableForSession)
		.addOnClick(WriteUpdate));

	addOption(BreakOption(getString("Ghost for Player")));

	m_Vars.m_Populated = true;
}

/*Called always*/
void NetworkProtectionGhostModeSelectedMenu::FeatureUpdate() {}

NetworkProtectionGhostModeSelectedMenu* _instance;
NetworkProtectionGhostModeSelectedMenu* NetworkProtectionGhostModeSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionGhostModeSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionGhostModeSelectedMenu::~NetworkProtectionGhostModeSelectedMenu() { delete _instance; }