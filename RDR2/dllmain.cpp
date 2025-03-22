#include "stdafx.hpp"
#include "menu/init.hpp"
#include "global/variables.hpp"
#include "security/security.hpp"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        global::vars::g_menuHandle = hModule;
        std::copy((BYTE*)hModule, (BYTE*)(hModule + 0x1000), std::back_inserter(global::vars::m_menuPE));

        MODULEINFO myModuleInfo;
        GetModuleInformation(GetCurrentProcess(), hModule, &myModuleInfo, sizeof(MODULEINFO));

        global::vars::g_menuAddress = (uint64_t)myModuleInfo.lpBaseOfDll;
        global::vars::g_menuSize = (uint64_t)myModuleInfo.SizeOfImage;

        DisableThreadLibraryCalls(hModule);
        security::getSecurity()->unlink();
        security::getSecurity()->fakePE((uint64_t)hModule);

        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)menu::init::entry, hModule, 0, 0);
    } else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        global::vars::g_unloading = true;
        CloseHandle(global::vars::g_explorerHandle);
    }

    return TRUE;
}