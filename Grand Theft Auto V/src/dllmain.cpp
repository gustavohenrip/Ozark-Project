#include "stdafx.hpp"
#include "menu/init.hpp"
#include "global/variables.hpp"
#include "global/security_vars.hpp"
#include "security/security.hpp"

BOOL APIENTRY DllMain(HMODULE hModule, uint32_t ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        Global::Vars::g_ModuleHandle = hModule;
        // std::copy((uint8_t*)hModule, (uint8_t*)(hModule + 0x1000), std::back_inserter(Global::Security::g_PEHeader));
        
        if ((uint64_t)hModule != 0) {
            MODULEINFO ModuleInfo;
            GetModuleInformation(GetCurrentProcess(), hModule, &ModuleInfo, sizeof(MODULEINFO));

            Global::Vars::g_MenuAddress = (uint64_t)ModuleInfo.lpBaseOfDll;
            Global::Vars::g_MenuSize = (uint64_t)ModuleInfo.SizeOfImage;
        }

#ifndef DEV
        Security::FakePE();
        Security::Unlink();
#endif

        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Menu::Init::Entry, hModule, 0, 0);
    } else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        Global::Vars::g_Unloading = true;
    }

    return TRUE;
}