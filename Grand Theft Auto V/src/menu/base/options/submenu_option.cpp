#include "submenu_option.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/ui_vars.hpp"

void SubmenuOption::Render(int pos) {
	m_OnUpdateCallback(this, pos);

	Color BarColor = Global::UIVars::g_TitleHeaderColor.WithOpacity(200);
	if (Global::UIVars::g_InsideLSC 
		|| Global::UIVars::g_InsideCasinoLuckyWheel
		|| Global::UIVars::g_InsideCasinoSlotMachines
		|| Global::UIVars::g_InsideCasinoBlackjack
		|| Global::UIVars::g_InsideCasinoBlackjackHigh
		|| Global::UIVars::g_InsideCasinoPoker
		|| Global::UIVars::g_InsideCasinoPokerHigh
		|| Global::UIVars::g_InsideCasinoRoulette
		|| Global::UIVars::g_InsideCasinoRouletteHigh
		|| Global::UIVars::g_InsideCasino) {
		BarColor = Color(0, 0, 0, 200);
	} else if (Global::UIVars::g_InsideBennys) {
		BarColor = Color(0x523D06C8);
	} else if (Global::UIVars::g_InsideWeapon) {
		BarColor = Color(0x9C2E21C8);
	}

	Menu::GetRenderer()->DrawText(m_Name.Get().c_str(), { Global::UIVars::g_MenuX + 0.004f + m_Offset, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor);
	Menu::GetRenderer()->DrawRect({ Global::UIVars::g_MenuX + 0.23f - 0.005f, Global::UIVars::g_MenuY + (pos * 0.0315f) }, { 0.005f, 0.0315f }, BarColor);

	if (m_HasSideText) {
		Menu::GetRenderer()->DrawText(m_SideText.c_str(), { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - Global::UIVars::g_MenuWrap)) - 0.005f });
	}
}

void SubmenuOption::RenderSelected(int pos, std::stack<std::string> MenuStack) {
	m_OnHoverCallback();

	if (m_HasHotkey && m_RequirementCallback()) {
		if (Menu::Util::GetInput()->IsJustReleased(true, VK_F12)) {
			Native::PlaySoundFrontend(-1, "ATM_WINDOW", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);

			Menu::Util::GetMenuInput()->Push([this] {
				Menu::Util::GetMenuInput()->Hotkey(m_Name.Get(), this);
			});
		}
	}

	if (m_RequirementCallback() && Menu::Util::GetInput()->IsOptionPressed()) {
		m_OnClickCallback();
		Menu::GetSubmenuHandler()->SetSubmenu(m_Submenu);
	}
}

void SubmenuOption::InvokeHotkey() {
	if (!m_RequirementCallback()) return;

	m_OnClickCallback();
	
	if (!Menu::GetBase()->m_Open) Menu::GetBase()->m_Open = true;
	Menu::GetSubmenuHandler()->SetSubmenu(m_Submenu);

	static std::unordered_map<uint32_t, uint32_t> Notifies;
	if (Notifies[Native::GetHashKey(m_Name.Get().c_str())] > GetTickCount()) return;

	Notifies[Native::GetHashKey(m_Name.Get().c_str())] = GetTickCount() + 5000;
	Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", TRANSLATE(T_UsedHotkeyFor), m_Name.Get().c_str()));
}