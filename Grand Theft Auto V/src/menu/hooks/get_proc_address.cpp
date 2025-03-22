#include "hooks.hpp"
#include "menu/base/submenus/main/misc/misc_scripthook.hpp"
#include "utils/log.hpp"
#include "utils/log.hpp"

FARPROC Menu::Hooks::GetProcAddressHook(HMODULE moduleHandle, LPCSTR lpProcName) {
	if (moduleHandle == (HMODULE)0x63686970) {
		if (!strcmp(lpProcName, "scriptRegister")) {
			LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom scriptRegister");
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::ScriptRegister;
		}

		if (!strcmp(lpProcName, "scriptRegisterAdditionalThread")) {
			LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom scriptRegisterAdditionalThread");
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::ScriptRegisterAdditionalThread;
		}

		if (!strcmp(lpProcName, "scriptUnregister")) {
			LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom scriptUnregister");
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::ScriptUnregister;
		}

		if (!strcmp(lpProcName, "nativeInit")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::NativeInit;
		}

		if (!strcmp(lpProcName, "nativePush64")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::NativePush64;
		}

		if (!strcmp(lpProcName, "nativeCall")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::NativeCall;
		}

		if (!strcmp(lpProcName, "getGlobalPtr")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::GetGlobalPtr;
		}

		if (!strcmp(lpProcName, "scriptWait")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::ScriptWait;
		}

		if (!strcmp(lpProcName, "keyboardHandlerRegister")) {
			LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom keyboardHandlerRegister");
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::KeyboardHandlerRegister;
		}

		if (!strcmp(lpProcName, "keyboardHandlerUnregister")) {
			LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom keyboardHandlerUnregister");
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::KeyboardHandlerUnregister;
		}

		if (!strcmp(lpProcName, "getScriptHandleBaseAddress")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::GetScriptHandleBaseAddress;
		}

		if (!strcmp(lpProcName, "presentCallbackRegister")) {
			LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom presentCallbackRegister");
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::PresentCallbackRegister;
		}

		if (!strcmp(lpProcName, "presentCallbackUnregister")) {
			LOG_CUSTOM_SUCCESS("ScriptHook", "Gave script custom presentCallbackUnregister");
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::PresentCallbackUnregister;
		}

		if (!strcmp(lpProcName, "worldGetAllVehicles")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::WorldGetAllVehicles;
		}

		if (!strcmp(lpProcName, "worldGetAllPeds")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::WorldGetAllPeds;
		}

		if (!strcmp(lpProcName, "worldGetAllObjects")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::WorldGetAllObjects;
		}

		if (!strcmp(lpProcName, "worldGetAllPickups")) {
			return (FARPROC)&MiscScriptHookMenuVars::Proxy::WorldGetAllPickups;
		}

		LOG_CUSTOM_WARN("ScriptHook", "Failed to verify %s", lpProcName);
	}

	return OriginalGetProcAddress(moduleHandle, lpProcName);
}