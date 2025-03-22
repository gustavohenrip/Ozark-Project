#include "number_option.hpp"
#include "../util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "utils/va.hpp"
#include "global/ui_vars.hpp"
#include "utils/va.hpp"
#include "menu/base/util/menu_input.hpp"
#include "menu/base/util/notify.hpp"

static bool g_LeftDisabled = false;
static bool g_RightDisabled = false;
static int g_LeftTimer = 0;
static int g_RightTimer = 0;

template<class Type>
void NumberOption<Type>::Render(int pos) {
	m_OnUpdateCallback(this);

	float WrapDistance = 0.f;
	const char* String = m_ShowMax ? Utils::VA::VA((m_Format + " / " + m_Format).c_str(), *m_Number, m_Max) : Utils::VA::VA(m_Format.c_str(), *m_Number);

	switch (m_Type) {
		case TOGGLE:
			if (Menu::GetBase()->IsOptionSelected(pos)) {
				WrapDistance = 0.017f;
				Menu::GetRenderer()->DrawSprite("commonmenu", "shop_arrows_upanddown", { Global::UIVars::g_MenuX + 0.217f - 0.010f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.023f, 0.037f }, 90.f, Global::UIVars::g_SelectedTextColor);
			}

			Menu::GetRenderer()->DrawSprite("commonmenu", "common_medal", { Global::UIVars::g_MenuX + 0.221f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.02112f * 0.8f, 0.03744f * 0.8f }, 0.f, *m_Toggle ? Global::UIVars::g_ToggleOnColor : Global::UIVars::g_ToggleOffColor);
			Menu::GetRenderer()->DrawText(String, { Global::UIVars::g_MenuX + 0.002f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - (Global::UIVars::g_MenuWrap - 0.011f))) - WrapDistance });
			break;
		case SCROLL:
		case SCROLLSELECT:
			if (Menu::GetBase()->IsOptionSelected(pos)) {
				WrapDistance = 0.017f;
				Menu::GetRenderer()->DrawSprite("commonmenu", "shop_arrows_upanddown", { Global::UIVars::g_MenuX + 0.217f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.023f, 0.037f }, 90.f, Global::UIVars::g_SelectedTextColor);
			}

			Menu::GetRenderer()->DrawText(String, { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - Global::UIVars::g_MenuWrap)) - WrapDistance });
			break;
	}

	Menu::GetRenderer()->DrawText(m_Name.Get().c_str(), { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor);
}

template<class Type>
void NumberOption<Type>::InvokeSave(std::stack<std::string> MenuStack) {
	if (m_CanBeSaved) {
		if (std::is_same<Type, int>::value) Utils::GetConfig()->WriteInt(MenuStack, m_Name.GetOriginal().c_str(), (int)*m_Number, { "Values" });
		else if (std::is_same<Type, float>::value) Utils::GetConfig()->WriteFloat(MenuStack, m_Name.GetOriginal().c_str(), (float)*m_Number, { "Values" });
		if (m_Type == TOGGLE) Utils::GetConfig()->WriteBool(MenuStack, m_Name.GetOriginal().c_str(), *m_Toggle);
	}
}

template<class Type>
void NumberOption<Type>::RenderSelected(int pos, std::stack<std::string> MenuStack) {
	m_OnHoverCallback();

	if (m_RequirementCallback() && Menu::Util::GetInput()->IsOptionPressed()) {
		bool DontProcess = false;
		switch (m_Type) {
			case TOGGLE: *m_Toggle = !*m_Toggle; break;
			case SCROLL: 
				m_IsKeyboardActive = true;
				DontProcess = true;
				Menu::GetBase()->m_KeyboardTitle = "Enter input";
				Native::DisplayOnscreenKeyboard(0, "OzarkKeyboardTitle", "", "", "", "", "", 10);
				break;
		}

		if (!DontProcess) {
			m_OnClickCallback();
		}
	}

	if (m_HasHotkey && m_RequirementCallback()) {
		if (Menu::Util::GetInput()->IsJustReleased(true, VK_F12)) {
			Native::PlaySoundFrontend(-1, "ATM_WINDOW", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);

			Menu::Util::GetMenuInput()->Push([this] {
				Menu::Util::GetMenuInput()->Hotkey(m_Name.Get(), this);
			});
		}
	}

	if (m_IsKeyboardActive) {
		int Status = Native::UpdateOnscreenKeyboard();
		if (Status == 0) {
			Menu::GetBase()->m_DisableInputThisFrame = true;
		} else if (Status == 1) {
			std::string StringName(Native::GetOnscreenKeyboardResult());

			if (!StringName.empty()) {
				bool Bad = false;
				static char Valid[] = {
					'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
					'.', '-'
				};

				for (auto& C : StringName) {
					Bad = true;

					for (int i = 0; i < NUMOF(Valid); i++) {
						if (C == Valid[i]) {
							Bad = false;
							break;
						}
					}

					if (Bad) break;
				}

				if (!Bad) {
					Type out = 0;

					if (std::is_same<Type, float>::value) out = (Type)std::stof(StringName);
					else if (std::is_same<Type, int64_t>::value) out = (Type)std::stoll(StringName);
					else if (std::is_same<Type, int>::value) out = (Type)std::stol(StringName);
					else out = (Type)std::stoi(StringName);

					if (m_HasMin && out < m_Min) *m_Number = m_Min;
					else if (m_HasMax && out > m_Max) *m_Number = m_Max;
					else *m_Number = out;

					m_OnClickCallback();
				}
			}

			m_IsKeyboardActive = false;
		} else if (Status > 1) {
			m_IsKeyboardActive = false;
		}
	}

	if (m_CanBeSaved) {
		if (std::is_same<Type, int>::value) Utils::GetConfig()->WriteInt(MenuStack, m_Name.GetOriginal().c_str(), (int)*m_Number, { "Values" });
		else if (std::is_same<Type, float>::value) Utils::GetConfig()->WriteFloat(MenuStack, m_Name.GetOriginal().c_str(), (float)*m_Number, { "Values" });
		if (m_Type == TOGGLE) Utils::GetConfig()->WriteBool(MenuStack, m_Name.GetOriginal().c_str(), *m_Toggle);
	}

	if (Menu::Util::GetInput()->IsLeftJustPressed()) {
		if (!g_LeftDisabled) {
			*m_Number -= m_Step;
			if (m_Loop) {
				if (*m_Number < m_Min) *m_Number = m_Max;
			} else if (*m_Number < m_Min && m_HasMin) *m_Number = m_Min;

			if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();

			Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		}

		g_LeftDisabled = false;
		g_LeftTimer = 0;
	}

	if (Menu::Util::GetInput()->IsRightJustPressed()) {
		if (!g_RightDisabled) {
			*m_Number += m_Step;
			if (m_Loop) {
				if (*m_Number > m_Max) *m_Number = m_Min;
			} else if (*m_Number > m_Max && m_HasMax) *m_Number = m_Max;

			if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();

			Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		}

		g_RightDisabled = false;
		g_RightTimer = 0;
	}

	static uint32_t Timer = 0;

	if (Menu::Util::GetInput()->IsLeftPressed()) {
		if (++g_LeftTimer > 20) {
			g_LeftDisabled = true;
			if ((timeGetTime() - Timer) > m_ScrollSpeed) {
				*m_Number -= m_Step;
				if (m_Loop) {
					if (*m_Number < m_Min) *m_Number = m_Max;
				} else if (*m_Number < m_Min && m_HasMin) *m_Number = m_Min;

				if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();
				Timer = timeGetTime();

				Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
			}
		}
	}

	if (Menu::Util::GetInput()->IsRightPressed()) {
		if (++g_RightTimer > 20) {
			g_RightDisabled = true;
			if ((timeGetTime() - Timer) > m_ScrollSpeed) {
				*m_Number += m_Step;
				if (m_Loop) {
					if (*m_Number > m_Max) *m_Number = m_Min;
				} else if (*m_Number > m_Max && m_HasMax) *m_Number = m_Max;

				if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();
				Timer = timeGetTime();

				Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
			}
		}
	}
}

template<class Type>
void NumberOption<Type>::InvokeHotkey() {
	if (!m_RequirementCallback()) return;

	bool DontProcess = false;
	switch (m_Type) {
		case TOGGLE: *m_Toggle = !*m_Toggle; break;
		case SCROLL:
			m_IsKeyboardActive = true;
			DontProcess = true;
			Menu::GetBase()->m_KeyboardTitle = "Enter input";
			Native::DisplayOnscreenKeyboard(0, "OzarkKeyboardTitle", "", "", "", "", "", 10);
			break;
	}

	if (!DontProcess) {
		m_OnClickCallback();
	}

	static std::unordered_map<uint32_t, uint32_t> Notifies;
	if (Notifies[Native::GetHashKey(m_Name.Get().c_str())] > GetTickCount()) return;

	Notifies[Native::GetHashKey(m_Name.Get().c_str())] = GetTickCount() + 5000;
	Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", TRANSLATE(T_UsedHotkeyFor), m_Name.Get().c_str()));
}

template class NumberOption<int>;
template class NumberOption<short>;
template class NumberOption<float>;
template class NumberOption<uint8_t>;