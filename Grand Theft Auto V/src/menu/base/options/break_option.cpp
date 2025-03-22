#include "break_option.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/ui_vars.hpp"

void BreakOption::Render(int pos) {
	m_OnUpdateCallback(this);

	Menu::GetRenderer()->DrawText(("~italic~" + m_Name.Get()).c_str(), { Global::UIVars::g_MenuX + .004f + ((0.23f - (0.004f * 2.f)) / 2.f), Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.310f, Global::UIVars::g_BreakFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_BreakTextColor, JUSTIFY_CENTER);
}

void BreakOption::RenderSelected(int pos, std::stack<std::string> MenuStack) {
	switch (Menu::GetBase()->m_BreakScroll) {
		case 1: // up
			if (Menu::GetBase()->m_ScrollOffset > 0 && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset == 0) Menu::GetBase()->m_ScrollOffset--;
			if (Menu::GetBase()->m_CurrentOption - 1 < 0) Menu::Util::GetInput()->ScrollBottom();
			else Menu::GetBase()->m_CurrentOption--;
			break;
		case 2: // down
			if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption + 1 - Menu::GetBase()->m_ScrollOffset == Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
			if (Menu::GetBase()->m_CurrentOption + 1 >= Menu::GetSubmenuHandler()->GetTotalOptions()) Menu::Util::GetInput()->ScrollTop();
			else Menu::GetBase()->m_CurrentOption++;
			break;
		case 3: // bottom
			if (Menu::GetBase()->m_ScrollOffset > 0 && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset == 1) Menu::GetBase()->m_ScrollOffset--;
			Menu::GetBase()->m_CurrentOption--;
			break;
		case 4: // top
			if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetSubmenuHandler()->GetTotalOptions() >= Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset == Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
			Menu::GetBase()->m_CurrentOption++;
			break;
	}
}