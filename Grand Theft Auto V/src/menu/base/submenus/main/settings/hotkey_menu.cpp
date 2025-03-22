#include "hotkey_menu.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "menu/base/util/hotkeys.hpp"
#include "utils/va.hpp"
#include "menu/base/util/menu_input.hpp"
#include "hk/hotkeys_selected.hpp"

using namespace HotkeysMenuVars;

namespace HotkeysMenuVars {
	Vars_ m_Vars;
}

void HotkeysMenu::Init() {
	SetName("Hotkeys");
	SetParentSubmenu<SettingsMenu>();

	HotkeysSelectedMenu::GetInstance();
	addString("~m~None");
}

void HotkeysMenu::Update() {}

/*Called once on submenu open*/
void HotkeysMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Menu::Util::GetHotkey()->GetHotkeys().size()) {
		for (auto& Hotkey : Menu::Util::GetHotkey()->GetHotkeys()) {
			addOption(SubmenuOption(Utils::VA::VA("[%s] %s", Menu::Util::g_KeyNames[Hotkey.m_Key], Hotkey.m_Parent->GetName().Get().c_str()))
				.addSubmenu<HotkeysSelectedMenu>()
				.addOnClick([=] { HotkeysSelectedMenuVars::m_Vars.m_Hotkey = (void*)&Hotkey; }));
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void HotkeysMenu::FeatureUpdate() {}

HotkeysMenu* _instance;
HotkeysMenu* HotkeysMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new HotkeysMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

HotkeysMenu::~HotkeysMenu() { delete _instance; }