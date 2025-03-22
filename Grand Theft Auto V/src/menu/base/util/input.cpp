#include "input.hpp"
#include "global/variables.hpp"
#include "menu/base/base.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/submenu_handler.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/misc/misc_scripthook.hpp"

namespace Menu::Util {
	struct KeyboardState {
		uint32_t m_Time;
		bool m_IsWithAlt;
		bool m_WasDownBefore;
		bool m_IsUpNow;
		bool m_JustUp;
		bool m_JustDown;
	}; KeyboardState g_KeyStates[255];

	LRESULT APIENTRY Input::WindowProcessCallback(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
		switch (msg) {
			case WM_SIZE: return DefWindowProc(hwnd, msg, param, lparam);
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP: {
				if (param < 255) {
					g_KeyStates[param].m_Time = GetTickCount();
					g_KeyStates[param].m_IsWithAlt = (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP);
					g_KeyStates[param].m_WasDownBefore = (lparam >> 30) & 1;
					g_KeyStates[param].m_IsUpNow = (msg == WM_SYSKEYUP || msg == WM_KEYUP);
					if (!g_KeyStates[param].m_WasDownBefore) g_KeyStates[param].m_JustDown = !(msg == WM_SYSKEYUP || msg == WM_KEYUP);

					if (g_KeyStates[param].m_IsUpNow) {
						GetInput()->m_LastKey = param;
					}
				}

				for (auto Function : MiscScriptHookMenuVars::m_Vars.m_Keyboards) {
					Function((uint32_t)param, lparam & 0xFFFF, (lparam >> 16) & 0xFF, (lparam >> 24) & 1, (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP), (lparam >> 30) & 1, (msg == WM_SYSKEYUP || msg == WM_KEYUP));
				}

				break;
			}
		}

		return CallWindowProc(GetInput()->GetWindowProcess(), hwnd, msg, param, lparam);
	}

	void Input::Unload() {
		SetWindowLongPtrA(Global::Vars::g_GameWindow, GWLP_WNDPROC, (LONG_PTR)m_WindowProcess);
	}

	void Input::Update() {
		if (!m_WindowProcess) {
			m_WindowProcess = (WNDPROC)SetWindowLongPtrA(Global::Vars::g_GameWindow, GWLP_WNDPROC, (LONG_PTR)WindowProcessCallback);
		}

		if (Menu::GetBase()->m_IsInputDisabled) return;

		static uint32_t Counter = 0;
		static int Delay = 150;

		if (Counter < GetTickCount()) {
			if (IsOpenBindPressed()) {
				Native::PlaySoundFrontend(-1, "Back", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
				Menu::GetRenderer()->ResetGlobe();
				Menu::GetBase()->m_Open = !Menu::GetBase()->m_Open;
				Counter = GetTickCount() + 200;
			} else {
				if (Menu::GetBase()->m_Open) {
					if ((IsPressed(true, VK_NUMPAD8) || IsPressed(false, ControlFrontendUp))) {
						if (Menu::GetBase()->m_CurrentOption == 0) {
							ScrollBottom();
						} else {
							ScrollUp();
						}

						if (Delay > 120) Delay -= 15;
					} else if (IsPressed(true, VK_NUMPAD2) || IsPressed(false, ControlFrontendDown)) {
						if (Menu::GetBase()->m_CurrentOption == Menu::GetSubmenuHandler()->GetTotalOptions() - 1) {
							ScrollTop();
						} else {
							ScrollDown();
						}
					} else if (IsBJustPressed()) {
						Menu::GetSubmenuHandler()->SetSubmenuPrevious();
						Native::PlaySoundFrontend(-1, "Back", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
					} else {
						Delay = 150;
						return;
					}

					if (Delay > 80) Delay -= 15;
					Counter = GetTickCount() + Delay;
				}
			}
		}
	}

	void Input::ScrollUp() {
		if (Menu::GetSubmenuHandler()->GetTotalOptions() == 0) return;
		Menu::GetBase()->m_CurrentOption--;
		if (Menu::GetBase()->m_ScrollOffset > 0 && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset == -1) Menu::GetBase()->m_ScrollOffset--;
		Native::PlaySoundFrontend(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		Menu::GetBase()->m_BreakScroll = 1;
	}

	void Input::ScrollDown(bool disableSound) {
		if (Menu::GetSubmenuHandler()->GetTotalOptions() == 0) return;
		Menu::GetBase()->m_CurrentOption++;
		if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset == Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
		if (!disableSound) Native::PlaySoundFrontend(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		Menu::GetBase()->m_BreakScroll = 2;
	}

	void Input::ScrollBottom() {
		if (Menu::GetSubmenuHandler()->GetTotalOptions() == 0) return;
		Menu::GetBase()->m_CurrentOption = Menu::GetSubmenuHandler()->GetTotalOptions() - 1;
		if (Menu::GetSubmenuHandler()->GetTotalOptions() >= Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset = Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions;
		Native::PlaySoundFrontend(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		Menu::GetBase()->m_BreakScroll = 3;
	}

	void Input::ScrollTop() {
		if (Menu::GetSubmenuHandler()->GetTotalOptions() == 0) return;
		Menu::GetBase()->m_CurrentOption = 0;
		Menu::GetBase()->m_ScrollOffset = 0;
		Native::PlaySoundFrontend(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
		Menu::GetBase()->m_BreakScroll = 4;
	}

	bool Input::IsJustReleased(bool Keyboard, int Key, bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;

		if (Keyboard) {
			bool result = GetTickCount() < (g_KeyStates[Key].m_Time + 100) && g_KeyStates[Key].m_IsUpNow;
			memset(&g_KeyStates[Key], 0, sizeof(g_KeyStates[0]));
			return result;
		}

		return Native::IsDisabledControlJustReleased(0, Key);
	}

	bool Input::IsJustPressed(bool Keyboard, int Key, bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;

		if (Keyboard) {
			return ((GetAsyncKeyState(Key) & 0xFFFF) == 0x8001);
		}

		return Native::IsDisabledControlJustPressed(0, Key);
	}

	bool Input::IsPressed(bool Keyboard, int Key, bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;

		if (Keyboard) {
			return (GetAsyncKeyState(Key) & 0x8000);
			// return GetTickCount() < (g_KeyStates[Key].m_Time + 600000) && !g_KeyStates[Key].m_IsUpNow;
		}

		return Native::IsDisabledControlPressed(0, Key);
	}

	bool Input::IsOpenBindPressed(bool OverrideInput) {
		return (IsPressed(false, ControlFrontendRb) && IsPressed(false, ControlFrontendRight)) || IsJustPressed(true, VK_F4);
	}

	bool Input::IsAJustPressed(bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;
		return Native::IsDisabledControlJustReleased(0, ControlFrontendAccept) || IsJustPressed(true, VK_NUMPAD5);
	}

	bool Input::IsBJustPressed(bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;
		return Native::IsDisabledControlJustReleased(0, ControlFrontendCancel) || IsJustPressed(true, VK_NUMPAD0);
	}

	bool Input::IsOptionPressed(bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;

		if (IsAJustPressed()) {
			Native::PlaySoundFrontend(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
			return true;
		}

		return false;
	}

	bool Input::IsLeftJustPressed(bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;
		return IsJustPressed(false, ControlFrontendLeft) || IsJustPressed(true, VK_NUMPAD4);
	}

	bool Input::IsRightJustPressed(bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;
		return IsJustPressed(false, ControlFrontendRight) || IsJustPressed(true, VK_NUMPAD6);
	}

	bool Input::IsLeftPressed(bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;
		return IsPressed(false, ControlFrontendLeft) || IsPressed(true, VK_NUMPAD4);
	}

	bool Input::IsRightPressed(bool OverrideInput) {
		if (Menu::GetBase()->m_IsInputDisabled && !OverrideInput) return false;
		return IsPressed(false, ControlFrontendRight) || IsPressed(true, VK_NUMPAD6);
	}

	Input* GetInput() {
		static Input Instance;
		return &Instance;
	}
}