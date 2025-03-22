#include "input.hpp"
#include "utils/memory/pattern.hpp"
#include "utils/hooking/hooking.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/base.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/submenus/main/scripthook.hpp"
#include "utils/thread_pool.hpp"

namespace menu::util {
	struct keyboardState {
		DWORD time;
		bool isWithAlt;
		bool wasDownBefore;
		bool isUpNow;
		bool justUp;
		bool justDown;
	}; keyboardState keyStates[255];

	LRESULT APIENTRY input::windowProcessCallback(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
		switch (msg) {
			case WM_SIZE: return DefWindowProc(hwnd, msg, param, lparam);
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP: {
				if (param < 255) {
					keyStates[param].time = GetTickCount();
					keyStates[param].isWithAlt = (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP);
					keyStates[param].wasDownBefore = (lparam >> 30) & 1;
					keyStates[param].isUpNow = (msg == WM_SYSKEYUP || msg == WM_KEYUP);
					if (!keyStates[param].wasDownBefore) keyStates[param].justDown = !(msg == WM_SYSKEYUP || msg == WM_KEYUP);
				}

				for (auto function : scriptHookMenuVars::vars.keyboards) {
					function((uint32_t)param, lparam & 0xFFFF, (lparam >> 16) & 0xFF, (lparam >> 24) & 1, (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP), (lparam >> 30) & 1, (msg == WM_SYSKEYUP || msg == WM_KEYUP));
				}

				break;
			}
		}

		return CallWindowProc(getInput()->windowProcess, hwnd, msg, param, lparam);
	}

	void input::unload() {
		SetWindowLongPtrA(global::vars::g_gameWindow, GWLP_WNDPROC, (LONG_PTR)windowProcess);
	}

	void input::scrollUp() {
		if (menu::getSubmenuHandler()->getTotalOptions() == 0) return;
		menu::getBase()->currentOption--;
		if (menu::getBase()->scrollOffset > 0 && menu::getBase()->currentOption - menu::getBase()->scrollOffset == -1) menu::getBase()->scrollOffset--;
		native::playSoundFrontend("NAV_UP_DOWN", "HUD_SHOP_SOUNDSET", FALSE, FALSE);
		menu::getBase()->breakScroll = 1;
	}

	void input::scrollDown() {
		if (menu::getSubmenuHandler()->getTotalOptions() == 0) return;
		menu::getBase()->currentOption++;
		if (menu::getBase()->scrollOffset < menu::getSubmenuHandler()->getTotalOptions() - menu::getBase()->maxOptions && menu::getBase()->currentOption - menu::getBase()->scrollOffset == menu::getBase()->maxOptions) menu::getBase()->scrollOffset++;
		native::playSoundFrontend("NAV_UP_DOWN", "HUD_SHOP_SOUNDSET", FALSE, FALSE);
		menu::getBase()->breakScroll = 2;
	}

	void input::scrollBottom() {
		if (menu::getSubmenuHandler()->getTotalOptions() == 0) return;
		menu::getBase()->currentOption = menu::getSubmenuHandler()->getTotalOptions() - 1;
		if (menu::getSubmenuHandler()->getTotalOptions() >= menu::getBase()->maxOptions) menu::getBase()->scrollOffset = menu::getSubmenuHandler()->getTotalOptions() - menu::getBase()->maxOptions;
		native::playSoundFrontend("NAV_UP_DOWN", "HUD_SHOP_SOUNDSET", FALSE, FALSE);
		menu::getBase()->breakScroll = 3;
	}

	void input::scrollTop() {
		if (menu::getSubmenuHandler()->getTotalOptions() == 0) return;
		menu::getBase()->currentOption = 0;
		menu::getBase()->scrollOffset = 0;
		native::playSoundFrontend("NAV_UP_DOWN", "HUD_SHOP_SOUNDSET", FALSE, FALSE);
		menu::getBase()->breakScroll = 4;
	}
	//LRESULT APIENTRY input::windowProcessCallback(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam)
	inline LRESULT (*originalWindowLongPtrFunction)(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
	LRESULT windowLongPtrFunctionHook(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {

		//menu::util::getInput()->windowProcessCallback(hwnd, msg, param, lparam);

		switch (msg) {
		case WM_SIZE: return DefWindowProc(hwnd, msg, param, lparam);
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP: {
			if (param < 255) {
				keyStates[param].time = GetTickCount();
				keyStates[param].isWithAlt = (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP);
				keyStates[param].wasDownBefore = (lparam >> 30) & 1;
				keyStates[param].isUpNow = (msg == WM_SYSKEYUP || msg == WM_KEYUP);
				if (!keyStates[param].wasDownBefore) keyStates[param].justDown = !(msg == WM_SYSKEYUP || msg == WM_KEYUP);
			}

			for (auto function : scriptHookMenuVars::vars.keyboards) {
				function((uint32_t)param, lparam & 0xFFFF, (lparam >> 16) & 0xFF, (lparam >> 24) & 1, (msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP), (lparam >> 30) & 1, (msg == WM_SYSKEYUP || msg == WM_KEYUP));
			}

			break;
		}
		}

		return originalWindowLongPtrFunction(hwnd, msg, param, lparam);
	}

	void wpcThread() {		
		//printf("doing the dirty\n");
		static int flag = 0;
		auto longPtr = GetWindowLongPtrA(global::vars::g_gameWindow, GWLP_WNDPROC);
		if (longPtr != (LONG_PTR)input::windowProcessCallback && flag < 2) {
			printf("setting ptr in the menu %llX\n", longPtr);
			if (flag == 1) {
				if (longPtr) {
					printf("hacking.................\n");
					hooking::getHooking()->create("idkwhatimdoing", longPtr, &windowLongPtrFunctionHook, &originalWindowLongPtrFunction);
				} else {
					printf("longPtr == 0 THIS IS BAD\n");
				}
			} else {
				menu::util::getInput()->windowProcess = (WNDPROC)SetWindowLongPtrA(global::vars::g_gameWindow, GWLP_WNDPROC, (LONG_PTR)input::windowProcessCallback);
			}
			printf("flag is %i\n", flag);
			flag++;
		}
		Sleep(1);
	}

	void input::update() {
		//static bool isThreadSet = false;
		//if (!isThreadSet) {
		//	utils::getThreadPool()->add("idkwhatimdoing"_Protect, wpcThread);
		//	isThreadSet = true;
		//}
		if (!windowProcess) {
			//windowProcess = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(global::vars::g_gameWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&windowProcessCallback)));
			//windowProcess = (WNDPROC)SetWindowLongPtrA(global::vars::g_gameWindow, GWLP_WNDPROC, (LONG_PTR)windowProcessCallback);
		}

		if (menu::getBase()->isInputDisabled) return;

		static unsigned int counter = 0;
		static int delay = 150;

		if (counter < GetTickCount()) {
			if (isOpenBindPressed()) {
				menu::getBase()->open = !menu::getBase()->open;
				counter = GetTickCount() + delay;
				if (!menu::getBase()->open)
					native::clearAllHelpMessages();
			} else {
				if (menu::getBase()->open) {
					if (isPressed(true, VK_NUMPAD8) || isPressed(false, INPUT_FRONTEND_UP)) {
						if (menu::getBase()->currentOption == 0) {
							scrollBottom();
						} else {
							scrollUp();
						}

						if (delay > 120) delay -= 15;
					} else if (isPressed(true, VK_NUMPAD2) || isPressed(false, INPUT_FRONTEND_DOWN)) {
						if (menu::getBase()->currentOption == menu::getSubmenuHandler()->getTotalOptions() - 1) {
							scrollTop();
						} else {
							scrollDown();
						}
					} else if (isBJustPressed()) {
						native::playSoundFrontend("BACK", "HUD_SHOP_SOUNDSET", FALSE, FALSE);
						menu::getSubmenuHandler()->setSubmenuPrevious();
					} else {
						delay = 150;
						return;
					}

					if (delay > 80) delay -= 15;
					counter = GetTickCount() + delay;
				}
			}
		}
	}

	bool input::isJustPressed(bool keyboard, int key) {
		if (keyboard) {
			return ((GetAsyncKeyState(key) & 0xFFFF) == 0x8001);

			/*bool result = (GetTickCount() - 100) < keyStates[key].time && !keyStates[key].isUpNow && keyStates[key].justDown;
			if (result) keyStates[key].justDown = false;
			return result;*/
		}

		return native::isDisabledControlJustPressed(0, key);
	}

	bool input::isPressed(bool keyboard, int key) {
		if (keyboard) {
			return (GetAsyncKeyState(key) & 0x8000);

			// return GetTickCount() < (keyStates[key].time + 5000) && !keyStates[key].isUpNow;
		}

		return native::isDisabledControlPressed(0, key);
	}

	bool input::isJustReleased(bool keyboard, int key) {
		if (keyboard) {
			bool result = GetTickCount() < (keyStates[key].time + 100) && keyStates[key].isUpNow;
			if (result) memset(&keyStates[key], 0, sizeof(keyboardState));
			return result;
		}

		return native::isDisabledControlJustReleased(0, key);
	}

	bool input::isOpenBindPressed() {
		return (isPressed(false, INPUT_FRONTEND_RB) && isPressed(false, INPUT_FRONTEND_RIGHT)) || isPressed(true, VK_F4);
	}

	bool input::isAJustPressed() {
		return native::isDisabledControlJustReleased(0, INPUT_FRONTEND_ACCEPT) || isJustPressed(true, VK_NUMPAD5);
	}

	bool input::isBJustPressed() {
		return native::isDisabledControlJustReleased(0, INPUT_FRONTEND_CANCEL) || isJustPressed(true, VK_NUMPAD0);
	}

	bool input::isOptionPressed() {
		if (isAJustPressed()) {
			native::playSoundFrontend("SELECT", "HUD_SHOP_SOUNDSET", FALSE, FALSE);
			return true;
		}

		return false;
	}

	bool input::isLeftJustPressed() {
		return isJustPressed(false, INPUT_FRONTEND_LEFT) || isJustPressed(true, VK_NUMPAD4);
	}

	bool input::isRightJustPressed() {
		return isJustPressed(false, INPUT_FRONTEND_RIGHT) || isJustPressed(true, VK_NUMPAD6);
	}

	bool input::isLeftPressed() {
		return isPressed(false, INPUT_FRONTEND_LEFT) || isPressed(true, VK_NUMPAD4);
	}

	bool input::isRightPressed() {
		return isPressed(false, INPUT_FRONTEND_RIGHT) || isPressed(true, VK_NUMPAD6);
	}

	input* getInput() {
		static input instance;
		return &instance;
	}
}