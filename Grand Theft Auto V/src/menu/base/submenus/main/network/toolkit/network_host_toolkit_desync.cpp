#include "network_host_toolkit_desync.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_host_toolkit.hpp"

using namespace NetworkHostToolkitDesyncMenuVars;

namespace NetworkHostToolkitDesyncMenuVars {
	Vars_ m_Vars;
}

void NetworkHostToolkitDesyncMenu::Init() {
	SetName("Desync Kick");
	SetParentSubmenu<NetworkHostToolkitMenu>();

	addOption(ToggleOption("Block")
		.addTranslation()
		.addToggle(m_Vars.m_Block).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Redirect")
		.addTranslation()
		.addToggle(m_Vars.m_Redirect).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Notify")
		.addTranslation()
		.addToggle(m_Vars.m_Notify).canBeSaved(m_ParentNameStack));
}

void NetworkHostToolkitDesyncMenu::Update() {}

/*Called once on submenu open*/
void NetworkHostToolkitDesyncMenu::UpdateOnce() {}

/*Called always*/
void NetworkHostToolkitDesyncMenu::FeatureUpdate() {}

NetworkHostToolkitDesyncMenu* _instance;
NetworkHostToolkitDesyncMenu* NetworkHostToolkitDesyncMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkHostToolkitDesyncMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkHostToolkitDesyncMenu::~NetworkHostToolkitDesyncMenu() { delete _instance; }