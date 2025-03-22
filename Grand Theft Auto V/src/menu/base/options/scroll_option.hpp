#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "../util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "global/ui_vars.hpp"
#include "utils/config.hpp"
#include "utils/va.hpp"
#include "menu/base/util/menu_input.hpp"
#include "menu/base/util/notify.hpp"

template<typename Type>
struct ScrollStruct {
	TranslationString m_Name;
	Type m_Result;
};

template<typename Type>
class ScrollOption : public Option {
public:
	ScrollOption(ScrollOptionType typ, std::string m_Name)
		: Option(m_Name, false, true), m_Type(typ), m_Number(nullptr), m_Toggle(nullptr), m_HasMin(false), m_HasMax(false), m_Loop(false), m_ScrollSpeed(130), m_OnUpdateCallback([](ScrollOption*) {}), m_OnClickCallback([]() {}), m_OnHoverCallback([]() {}) {}

	ScrollOption& addScroll(int& num, int min_, int max_, ScrollStruct<Type>* structure) {
		m_HasMin = true;
		m_HasMax = true;
		m_Min = min_;
		m_Max = max_ - 1;
		m_Number = &num;
		m_Scroll = structure;
		return *this;
	}

	ScrollOption& addToggle(bool& t) {
		m_Toggle = &t;
		return *this;
	}

	ScrollOption& addOnClick(std::function<void()> func) {
		m_OnClickCallback = func;
		return *this;
	}

	ScrollOption& addRequirement(std::function<bool()> func) {
		m_RequirementCallback = func;
		return *this;
	}

	ScrollOption& addOnUpdate(std::function<void(ScrollOption*)> func) {
		m_OnUpdateCallback = func;
		return *this;
	}

	ScrollOption& addOnHover(std::function<void()> func) {
		m_OnHoverCallback = func;
		return *this;
	}

	ScrollOption& addTooltip(std::string tip) {
		m_Tooltip = tip;
		return *this;
	}

	ScrollOption& setScrollSpeed(DWORD speed) {
		m_ScrollSpeed = speed;
		return *this;
	}

	ScrollOption& canLoop() {
		m_Loop = true;
		return *this;
	}

	ScrollOption& addTranslation() {
		m_Name.SetHasTranslation(true);
		m_Tooltip.SetHasTranslation(true);
		return *this;
	}

	ScrollOption& canBeSaved(std::stack<std::string> MenuStack) {
		m_CanBeSaved = true;
		if (IsValidPtr(m_Number) && m_RequirementCallback()) {
			*m_Number = Utils::GetConfig()->ReadInt(MenuStack, m_Name.GetOriginal().c_str(), (int)*m_Number, { "Values" });
			if (m_Type == TOGGLE && IsValidPtr(m_Toggle)) {
				*m_Toggle = Utils::GetConfig()->ReadBool(MenuStack, m_Name.GetOriginal().c_str(), *m_Toggle);
				
				if (*m_Toggle) {
					m_OnClickCallback();
				}
			}
		}

		return *this;
	}

	ScrollOption& addHotkey() {
		m_HasHotkey = true;
		return *this;
	}

	void Render(int pos) {
		m_OnUpdateCallback(this);

		char nameBuffer[128];
		sprintf(nameBuffer, "%s ~m~[%i/%i]", m_Scroll[*m_Number].m_Name.Get().c_str(), (*m_Number) + 1, m_Max + 1);

		float WrapDistance = 0.f;

		switch (m_Type) {
			case TOGGLE:
				if (m_Toggle == nullptr) {
					Menu::GetRenderer()->DrawText("~r~ERROR", { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor);
					return;
				}

				if (Menu::GetBase()->IsOptionSelected(pos)) {
					WrapDistance = 0.017f;
					Menu::GetRenderer()->DrawSprite("commonmenu", "shop_arrows_upanddown", { Global::UIVars::g_MenuX + 0.217f - 0.010f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.023f, 0.037f }, 90.f, Global::UIVars::g_SelectedTextColor);
				}

				Menu::GetRenderer()->DrawSprite("commonmenu", "common_medal", { Global::UIVars::g_MenuX + 0.221f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.02112f * 0.8f, 0.03744f * 0.8f }, 0.f, *m_Toggle ? Global::UIVars::g_ToggleOnColor : Global::UIVars::g_ToggleOffColor);
				Menu::GetRenderer()->DrawText(nameBuffer, { Global::UIVars::g_MenuX + 0.002f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - (Global::UIVars::g_MenuWrap - 0.011f))) - WrapDistance });
				break;
			case SCROLL:
			case SCROLLSELECT:
				if (Menu::GetBase()->IsOptionSelected(pos)) {
					WrapDistance = 0.017f;
					Menu::GetRenderer()->DrawSprite("commonmenu", "shop_arrows_upanddown", { Global::UIVars::g_MenuX + 0.217f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.023f, 0.037f }, 90.f, Global::UIVars::g_SelectedTextColor);
				}

				Menu::GetRenderer()->DrawText(nameBuffer, { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - Global::UIVars::g_MenuWrap)) - WrapDistance });
				break;
		}

		Menu::GetRenderer()->DrawText(m_Name.Get().c_str(), { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor);
	}

	void RenderSelected(int pos, std::stack<std::string> MenuStack) {
		m_OnHoverCallback();

		if (m_RequirementCallback() && Menu::Util::GetInput()->IsOptionPressed()) {
			switch (m_Type) {
				case TOGGLE: *m_Toggle = !*m_Toggle; break;
				case SCROLL: /*Open custom input*/ break;
			}

			m_OnClickCallback();
		}

		if (m_CanBeSaved) {
			Utils::GetConfig()->WriteInt(MenuStack, m_Name.Get().c_str(), (int)*m_Number, { "Values" });
			if (m_Type == TOGGLE) Utils::GetConfig()->WriteBool(MenuStack, m_Name.GetOriginal().c_str(), *m_Toggle);
		}

		if (m_HasHotkey && m_RequirementCallback()) {
			if (Menu::Util::GetInput()->IsJustReleased(true, VK_F12)) {
				Native::PlaySoundFrontend(-1, "ATM_WINDOW", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);

				Menu::Util::GetMenuInput()->Push([this] {
					Menu::Util::GetMenuInput()->Hotkey(m_Name.Get(), this);
				});
			}
		}

		if (Menu::Util::GetInput()->IsLeftJustPressed()) {
			if (!m_LeftDisabled) {
				*m_Number -= 1;
				if (m_Loop) {
					if (*m_Number < m_Min) *m_Number = m_Max;
				} else if (*m_Number < m_Min && m_HasMin) *m_Number = m_Min;
				if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();

				Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
			}

			m_LeftDisabled = false;
			m_LeftTimer = 0;
		}

		if (Menu::Util::GetInput()->IsRightJustPressed()) {
			if (!m_RightDisabled) {
				*m_Number += 1;
				if (m_Loop) {
					if (*m_Number > m_Max) *m_Number = m_Min;
				} else if (*m_Number > m_Max && m_HasMax) *m_Number = m_Max;

				if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();

				Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
			}

			m_RightDisabled = false;
			m_RightTimer = 0;
		}

		static DWORD timer = 0;

		if (Menu::Util::GetInput()->IsLeftPressed()) {
			if (++m_LeftTimer > 20) {
				m_LeftDisabled = true;
				if ((timeGetTime() - timer) > m_ScrollSpeed) {
					*m_Number -= 1;
					if (m_Loop) {
						if (*m_Number < m_Min) *m_Number = m_Max;
					} else if (*m_Number < m_Min && m_HasMin) *m_Number = m_Min;

					if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();
					timer = timeGetTime();

					Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
				}
			}
		}

		if (Menu::Util::GetInput()->IsRightPressed()) {
			if (++m_RightTimer > 20) {
				m_RightDisabled = true;
				if ((timeGetTime() - timer) > m_ScrollSpeed) {
					*m_Number += 1;
					if (m_Loop) {
						if (*m_Number > m_Max) *m_Number = m_Min;
					} else if (*m_Number > m_Max && m_HasMax) *m_Number = m_Max;

					if (m_Type == SCROLL || m_Type == TOGGLE) m_OnClickCallback();
					timer = timeGetTime();

					Native::PlaySoundFrontend(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
				}
			}
		}
	}

	void InvokeSave(std::stack<std::string> MenuStack) {}
	
	void InvokeHotkey() {
		if (!m_RequirementCallback()) return;

		if (m_Type == TOGGLE) {
			*m_Toggle = !*m_Toggle;
		}

		m_OnClickCallback();

		static std::unordered_map<uint32_t, uint32_t> Notifies;
		if (Notifies[Native::GetHashKey(m_Name.Get().c_str())] > GetTickCount()) return;

		Notifies[Native::GetHashKey(m_Name.Get().c_str())] = GetTickCount() + 5000;
		Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", TRANSLATE(T_UsedHotkeyFor), m_Name.Get().c_str()));
	}

	void WriteTranslation(nlohmann::json& JsonObject) {
		for (int i = 0; i < m_Max + 1; i++) {
			if (m_Scroll[i].m_Name.HasTranslation()) {
				if (JsonObject["Scrollables"][m_Name.GetOriginal()][m_Scroll[i].m_Name.GetOriginal()].empty()) {
					JsonObject["Scrollables"][m_Name.GetOriginal()][m_Scroll[i].m_Name.GetOriginal()] = m_Scroll[i].m_Name.GetOriginal();
				}
			}
		}
	}

	void ReadTranslation(nlohmann::json& JsonObject) {
		for (int i = 0; i < m_Max + 1; i++) {
			if (m_Scroll[i].m_Name.HasTranslation()) {
				auto Name = m_Scroll[i].m_Name.GetOriginal();
				m_Scroll[i].m_Name.SetMapped(JsonGet<std::string>(JsonObject["Scrollables"][m_Name.GetOriginal()], Name, Name).c_str());
			}
		}
	}

	void ResetTranslation() {
		for (int i = 0; i < m_Max + 1; i++) {
			if (m_Scroll[i].m_Name.HasTranslation()) {
				m_Scroll[i].m_Name.Reset();
			}
		}
	}
private:
	std::function<void(ScrollOption*)> m_OnUpdateCallback;
	std::function<void()> m_OnClickCallback;
	std::function<void()> m_OnHoverCallback;

	ScrollOptionType m_Type;
	int* m_Number;
	bool* m_Toggle;
	int m_Min;
	int m_Max;
	int m_LeftTimer = 0;
	int m_RightTimer = 0;
	uint32_t m_ScrollSpeed;

	bool m_HasMin;
	bool m_HasMax;
	bool m_Loop;
	bool m_LeftDisabled = false;
	bool m_RightDisabled = false;

	ScrollStruct<Type>* m_Scroll;
};