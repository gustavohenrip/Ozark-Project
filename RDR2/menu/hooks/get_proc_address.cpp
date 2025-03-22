#include "hooks.hpp"
#include "menu/base/submenus/main/scripthook.hpp"
#include "utils/log.hpp"

FARPROC menu::hooks::getProcAddressHook(HMODULE moduleHandle, LPCSTR lpProcName) {
	if (moduleHandle == (HMODULE)0x63686970) {
		if (!strcmp(lpProcName, "scriptRegister")) {
			customSuccess("ScriptHook"_Protect, "Gave script custom scriptRegister"_Protect);
			return (FARPROC)&scriptHookMenuVars::scriptRegister;
		}

		if (!strcmp(lpProcName, "scriptRegisterAdditionalThread")) {
			customSuccess("ScriptHook"_Protect, "Gave script custom scriptRegisterAdditionalThread"_Protect);
			return (FARPROC)&scriptHookMenuVars::scriptRegisterAdditionalThread;
		}

		if (!strcmp(lpProcName, "scriptUnregister")) {
			customSuccess("ScriptHook"_Protect, "Gave script custom scriptUnregister"_Protect);
			return (FARPROC)&scriptHookMenuVars::scriptUnregister;
		}

		if (!strcmp(lpProcName, "nativeInit")) {
			return (FARPROC)&scriptHookMenuVars::nativeInit;
		}

		if (!strcmp(lpProcName, "nativePush64")) {
			return (FARPROC)&scriptHookMenuVars::nativePush64;
		}

		if (!strcmp(lpProcName, "nativeCall")) {
			return (FARPROC)&scriptHookMenuVars::nativeCall;
		}

		if (!strcmp(lpProcName, "getGlobalPtr")) {
			return (FARPROC)&scriptHookMenuVars::getGlobalPtr;
		}

		if (!strcmp(lpProcName, "scriptWait")) {
			return (FARPROC)&scriptHookMenuVars::scriptWait;
		}

		if (!strcmp(lpProcName, "keyboardHandlerRegister")) {
			customSuccess("ScriptHook"_Protect, "Gave script custom keyboardHandlerRegister"_Protect);
			return (FARPROC)&scriptHookMenuVars::keyboardHandlerRegister;
		}

		if (!strcmp(lpProcName, "keyboardHandlerUnregister")) {
			customSuccess("ScriptHook"_Protect, "Gave script custom keyboardHandlerUnregister"_Protect);
			return (FARPROC)&scriptHookMenuVars::keyboardHandlerUnregister;
		}

		if (!strcmp(lpProcName, "getScriptHandleBaseAddress")) {
			return (FARPROC)&scriptHookMenuVars::getScriptHandleBaseAddress;
		}

		if (!strcmp(lpProcName, "presentCallbackRegister")) {
			customSuccess("ScriptHook"_Protect, "Gave script custom presentCallbackRegister"_Protect);
			return (FARPROC)&scriptHookMenuVars::presentCallbackRegister;
		}

		if (!strcmp(lpProcName, "presentCallbackUnregister")) {
			customSuccess("ScriptHook"_Protect, "Gave script custom presentCallbackUnregister"_Protect);
			return (FARPROC)&scriptHookMenuVars::presentCallbackUnregister;
		}

		if (!strcmp(lpProcName, "worldGetAllVehicles")) {
			return (FARPROC)&scriptHookMenuVars::worldGetAllVehicles;
		}

		if (!strcmp(lpProcName, "worldGetAllPeds")) {
			return (FARPROC)&scriptHookMenuVars::worldGetAllPeds;
		}

		if (!strcmp(lpProcName, "worldGetAllObjects")) {
			return (FARPROC)&scriptHookMenuVars::worldGetAllObjects;
		}

		if (!strcmp(lpProcName, "worldGetAllPickups")) {
			return (FARPROC)&scriptHookMenuVars::worldGetAllPickups;
		}

		customWarn("ScriptHook"_Protect, "Failed to verify %s"_Protect, lpProcName);
	}

	return originalGetProcAddress(moduleHandle, lpProcName);
}