#include "toggle_option.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/ui_vars.hpp"

void ToggleOption::Render(int pos) {
	m_OnUpdateCallback(this);

	Menu::GetRenderer()->DrawText(m_Name.Get().c_str(), { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor);
	Menu::GetRenderer()->DrawSprite("commonmenu", "common_medal", { Global::UIVars::g_MenuX + 0.221f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.02112f * 0.8f , 0.03744f * 0.8f }, 0.f, * m_Toggle ? Global::UIVars::g_ToggleOnColor : Global::UIVars::g_ToggleOffColor);
}

void ToggleOption::RenderSelected(int pos, std::stack<std::string> MenuStack) {
	m_OnHoverCallback();

	if (m_HasHotkey && m_RequirementCallback()) {
		if (Menu::Util::GetInput()->IsJustReleased(true, VK_F12)) {
			Native::PlaySoundFrontend(-1, "ATM_WINDOW", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);

			Menu::Util::GetMenuInput()->Push([this] {
				Menu::Util::GetMenuInput()->Hotkey(m_Name.Get(), this);
			});
		}
	}

	if (m_Toggle && m_RequirementCallback() && Menu::Util::GetInput()->IsOptionPressed()) {
		*m_Toggle = !*m_Toggle;
		m_OnClickCallback();

		if (m_CanBeSaved) {
			Utils::GetConfig()->WriteBool(MenuStack, m_Name.GetOriginal().c_str(), *m_Toggle);
		}
	}
}

void ToggleOption::InvokeHotkey() {
	if (!m_RequirementCallback()) return;

	*m_Toggle = !*m_Toggle;
	m_OnClickCallback();

	static std::unordered_map<uint32_t, uint32_t> Notifies;
	if (Notifies[Native::GetHashKey(m_Name.Get().c_str())] > GetTickCount()) return;

	Notifies[Native::GetHashKey(m_Name.Get().c_str())] = GetTickCount() + 5000;
	Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", TRANSLATE(T_UsedHotkeyFor), m_Name.Get().c_str()));
}