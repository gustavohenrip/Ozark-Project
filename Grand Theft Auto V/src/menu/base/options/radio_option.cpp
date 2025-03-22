#include "radio_option.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/renderer.hpp"

void RadioOption::Render(int pos) {
	m_OnUpdateCallback(this);

	Menu::GetRenderer()->DrawText(m_Name.Get().c_str(), { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor);

	if (m_Context->m_Toggles[m_Index]) {
		Color Col = Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor;

		std::string Sprite = m_Context->m_Sprite.second;
		if (strstr(Sprite.c_str(), "shop_") && !strstr(Sprite.c_str(), "shop_tick_icon")) {
			if (Menu::GetBase()->IsOptionSelected(pos)) {
				Sprite += "_b";
			} else {
				Sprite += "_a";
			}

			Col = { 255, 255, 255, 255 };
		}

		Menu::GetRenderer()->DrawSprite(m_Context->m_Sprite.first, Sprite.c_str(), { Global::UIVars::g_MenuX + 0.2195f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.02112f, 0.03744f }, 0.f, Col);
	}
}

void RadioOption::RenderSelected(int pos, std::stack<std::string> MenuStack) {
	m_OnHoverCallback();

	if (m_RequirementCallback() && Menu::Util::GetInput()->IsOptionPressed()) {
		m_OnClickCallback();

		if (!m_Context->m_Toggles[m_Index]) {
			m_Context->m_Toggles[m_Index] = true;
			m_Context->m_Toggles[m_Context->m_Selected] = false;
			m_Context->m_Selected = m_ID;
		}
	}
}