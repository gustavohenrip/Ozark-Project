#include "hotkeys_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../hotkey_menu.hpp"
#include "menu/base/util/menu_input.hpp"
#include "menu/base/util/hotkeys.hpp"

using namespace HotkeysSelectedMenuVars;

namespace HotkeysSelectedMenuVars {
	Vars_ m_Vars;
}

void HotkeysSelectedMenu::Init() {
	SetName("Hotkey");
	SetParentSubmenu<HotkeysMenu>();

	addString("Rebind");
	addString("Delete");
}

void HotkeysSelectedMenu::Update() {}

/*Called once on submenu open*/
void HotkeysSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	addOption(ButtonOption(getString("Rebind"))
		.addOnClick([=] {
			Menu::Util::GetMenuInput()->Push([] {
				Menu::Util::GetMenuInput()->Hotkey(((Menu::Util::RegisteredHotkey*)m_Vars.m_Hotkey)->m_Parent->GetName().Get(), ((Menu::Util::RegisteredHotkey*)m_Vars.m_Hotkey)->m_Parent);
			});
		}));

	addOption(ButtonOption(getString("Delete"))
		.addOnClick([] {
			Menu::Util::GetHotkey()->UnregisterHotkey(((Menu::Util::RegisteredHotkey*)m_Vars.m_Hotkey)->m_Parent);
			Menu::GetSubmenuHandler()->SetSubmenuPrevious(true);
		}));
}

/*Called always*/
void HotkeysSelectedMenu::FeatureUpdate() {}

HotkeysSelectedMenu* _instance;
HotkeysSelectedMenu* HotkeysSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new HotkeysSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

HotkeysSelectedMenu::~HotkeysSelectedMenu() { delete _instance; }