#include "button_option.hpp"
#include "menu/base/util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "global/ui_vars.hpp"
#include "menu/base/util/instructionals.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/util/menu_input.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"

void ButtonOption::Render(int pos) {
	m_OnUpdateCallback(this);

	Menu::GetRenderer()->DrawText(m_Name.Get().c_str(), { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor);

	if (m_HasKeyboard) {
		Menu::GetRenderer()->DrawSprite("Ozark", Menu::GetBase()->IsOptionSelected(pos) ? "keyboard_black" : "keyboard_white", { Global::UIVars::g_MenuX + 0.210f + 0.004f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, { 0.025f, 0.03f }, 0.f, { 255, 255, 255, 255 });
	} else if (m_HasSprite && m_OnSpriteRenderRequirement()) {
		Color Col = Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor;

		std::string Sprite = m_Sprite.second;
		if (strstr(Sprite.c_str(), "shop_") && !strstr(Sprite.c_str(), "shop_tick_icon")) {
			if (Menu::GetBase()->IsOptionSelected(pos)) {
				Sprite += "_b";
			} else {
				Sprite += "_a";
			}

			Col = { 255, 255, 255, 255 };
		}

		if (m_SpriteRotate) {
			m_SpriteRotation += 1.f;
			if (m_SpriteRotation > 360.f) m_SpriteRotation = 0.f;
		}

		Menu::GetRenderer()->DrawSprite(m_Sprite.first, Sprite.c_str(), { Global::UIVars::g_MenuX + 0.2195f, (Global::UIVars::g_MenuY + 0.016f) + (pos * 0.0315f) }, m_SpriteScale, m_SpriteRotation, Col);
	}

	if (m_HasSideText) {
		Menu::GetRenderer()->DrawText(m_SideText.c_str(), { Global::UIVars::g_MenuX + 0.004f, Global::UIVars::g_MenuY + (pos * 0.0315f) + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, Menu::GetBase()->IsOptionSelected(pos) ? Global::UIVars::g_SelectedTextColor : Global::UIVars::g_MainTextColor, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - Global::UIVars::g_MenuWrap))  });
	}
}

void ButtonOption::RenderSelected(int pos, std::stack<std::string> MenuStack) {
	m_OnHoverCallback();

	if (m_Instructionals.size()) {
		Instructionals::GetInstructionals()->Setup();

		for (auto& Instructional : m_Instructionals) {
			if (std::get<2>(Instructional)) {
				Instructionals::GetInstructionals()->AddInstructional(std::get<0>(Instructional), (eControls)std::get<1>(Instructional));
			} else {
				Instructionals::GetInstructionals()->AddInstructional(std::get<0>(Instructional), (eScaleformButtons)std::get<1>(Instructional));
			}
		}

		Instructionals::GetInstructionals()->Close();
	}

	if (m_RequirementCallback() && Menu::Util::GetInput()->IsOptionPressed()) {
		m_OnClickCallback();

		if (m_HasKeyboard) {
			m_IsKeyboardActive = true;
			Menu::GetBase()->m_KeyboardTitle = m_KeyboardTitle;
			Native::DisplayOnscreenKeyboard(0, "OzarkKeyboardTitle", "", "", "", "", "", m_KeyboardMaxChars);
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
			if (m_OnKeyboardCallbackThis) {
				m_OnKeyboardCallbackThis(this, Native::GetOnscreenKeyboardResult());
			}

			if (m_OnKeyboardCallback) {
				m_OnKeyboardCallback(Native::GetOnscreenKeyboardResult());
			}

			m_IsKeyboardActive = false;
		} else if (Status > 1) {
			m_IsKeyboardActive = false;
		}
	}
}

void ButtonOption::InvokeHotkey() {
	if (!m_RequirementCallback()) return;
	m_OnClickCallback();

	if (m_HasKeyboard) {
		m_IsKeyboardActive = true;
		Menu::GetBase()->m_KeyboardTitle = m_KeyboardTitle;

		Utils::GetFiberPool()->Push([this] {
			Native::DisplayOnscreenKeyboard(0, "OzarkKeyboardTitle", "", "", "", "", "", m_KeyboardMaxChars);

			while (m_IsKeyboardActive) {
				Utils::GetFiberManager()->GoToMainFiber();

				int Status = Native::UpdateOnscreenKeyboard();
				if (Status == 0) {
					Menu::GetBase()->m_DisableInputThisFrame = true;
				} else if (Status == 1) {
					if (m_OnKeyboardCallbackThis) {
						m_OnKeyboardCallbackThis(this, Native::GetOnscreenKeyboardResult());
					}

					if (m_OnKeyboardCallback) {
						m_OnKeyboardCallback(Native::GetOnscreenKeyboardResult());
					}

					m_IsKeyboardActive = false;
				} else if (Status > 1) {
					m_IsKeyboardActive = false;
				}
			}
		});
	}

	static std::unordered_map<uint32_t, uint32_t> Notifies;
	if (Notifies[Native::GetHashKey(m_Name.Get().c_str())] > GetTickCount()) return;

	Notifies[Native::GetHashKey(m_Name.Get().c_str())] = GetTickCount() + 5000;
	Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", TRANSLATE(T_UsedHotkeyFor), m_Name.Get().c_str()));
}