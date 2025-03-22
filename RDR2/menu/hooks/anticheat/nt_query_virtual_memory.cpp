#include "menu/hooks/hooks.hpp"
#include "utils/hooking/hooking.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/scripthook.hpp"
#include "utils/va.hpp"
#include "menu/auth.hpp"
#include "utils/memory/pattern.hpp"

NTSTATUS menu::hooks::ntQueryVirtualMemoryHook(HANDLE processHandle, void* baseAddress, int memoryInformationClass, void* memoryInformation, SIZE_T memoryInformationLength, SIZE_T* returnLength) {
   // uint64_t baseScan = (uint64_t)baseAddress;

    HMODULE module = GetModuleHandleA(0);
    MODULEINFO info;
    GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(info));
    uint64_t base = reinterpret_cast<uint64_t>(info.lpBaseOfDll);
    uint64_t size = static_cast<uint64_t>(info.SizeOfImage);
    static int64_t count = 0;
    if ((uint64_t)_ReturnAddress() > base && (uint64_t)_ReturnAddress() < (base + size)) {
   // if (baseScan >= global::vars::g_menuAddress && baseScan <= (global::vars::g_menuAddress + global::vars::g_menuSize)) {
        if (count % 1000 == 0) {
            customDev("AC", "Blocked sig scan | %lli", count++);
        }

        if (memoryInformationClass == 0 && memoryInformation) {
            MEMORY_BASIC_INFORMATION* info = (MEMORY_BASIC_INFORMATION*)memoryInformation;
            info->Type = MEM_PRIVATE;
            info->Protect = PAGE_NOACCESS;
            info->AllocationProtect = PAGE_NOACCESS;
        }

        return STATUS_INVALID_PARAMETER;
    }

    return originalNtQueryVirtualMemory(processHandle, baseAddress, memoryInformationClass, memoryInformation, memoryInformationLength, returnLength);
}