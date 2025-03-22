#include "security.hpp"
#include "utils/thread_pool.hpp"
#include "utils/caller.hpp"
#include "utils/va.hpp"
#include "utils/config.hpp"
#include "utils/log.hpp"
#include "global/variables.hpp"
#include "wallpapers/not_that_easy.hpp"
#include "menu/init.hpp"
#include "auth/crypto/crypto.hpp"
#include "menu/auth.hpp"
#include "utils/memory/memory.hpp"
#include <tlhelp32.h>
#include <algorithm>
#include <VersionHelpers.h>
#include <filesystem>

namespace security {
#define UNLINK(x)                   \
    (x).Flink->Blink = (x).Blink;   \
    (x).Blink->Flink = (x).Flink;
#define RELINK(x, real)         \
    (x).Flink->Blink = (real);  \
    (x).Blink->Flink = (real);  \
    (real)->Blink = (x).Blink;  \
    (real)->Flink = (x).Flink;

	typedef struct _PEB_LDR_DATA {
		BYTE _0x0000[12];
		LIST_ENTRY inLoadOrderModuleList;
		LIST_ENTRY inMemoryOrderModuleList;
		LIST_ENTRY inInitializationOrderModuleList;
	} PEB_LDR_DATA;

	typedef struct _PEB {
		BYTE _0x0000[24];
		PEB_LDR_DATA* ldr;
	} PEB;

	void mainSecurityThread() {
		BYTE currentHash[64];
		memcpy(currentHash, getSecurity()->moduleDigest, 64);

		if (getSecurity()->getModuleMemoryIntegrityHash(global::vars::g_menuHandle, global::vars::m_menuPE)) {
			if (memcmp(getSecurity()->moduleDigest, currentHash, 64)) {
			}
		} else {
			customError("S"_Protect, "Failed to initialize (#4)"_Protect);
			menu::getInit()->unload();
		}

		auto table = getSecurity()->getIntegrityItems();
		for (auto& item : table) {
			if (memcmp((void*)item.address, item.originalBytes, 5)) {
				BYTE* newBytes = (BYTE*)item.address;
				break;
			}
		}

		auto threads = utils::getThreadPool()->getThreads();
		for (auto& thread : threads) {
			CONTEXT cxt = { 0 };
			cxt.ContextFlags = CONTEXT_DEBUG_REGISTERS;

			if (GetThreadContext(thread->handle, &cxt) == 0) {
				continue;
			}

			if (cxt.Dr0 != 0 || cxt.Dr1 != 0 || cxt.Dr2 != 0 || cxt.Dr3 != 0) {
				break;
			}
		}

		Sleep(5000);
	}

	void debuggerDetectionThread() {
		BOOL isRemoteDebuggerPresent;
		CheckRemoteDebuggerPresent(GetCurrentProcess(), &isRemoteDebuggerPresent);

		uint64_t isRemotePresent = 0;
		uint32_t noDebugInherit = 0;
		HANDLE debugObject = 0;

		static uint64_t ntQueryInformationProcess = 0;

		if (ntQueryInformationProcess == 0) {
			if (global::vars::g_windows7) {
				ntQueryInformationProcess = (uint64_t)GetProcAddress(global::vars::g_ntdll, "NtQueryInformationProcess"_Protect);
			} else {
				ntQueryInformationProcess = getSecurity()->generateSyscallCaller(479);
			}
		}

		NTSTATUS ntStatusProcessDebugPort = caller::call<NTSTATUS>(ntQueryInformationProcess, GetCurrentProcess(), 7, &isRemotePresent, sizeof(isRemotePresent), 0);
		NTSTATUS ntStatusProcessDebugFlags = caller::call<NTSTATUS>(ntQueryInformationProcess, GetCurrentProcess(), 31, &noDebugInherit, sizeof(noDebugInherit), 0);
		NTSTATUS ntStatusProcessDebugObject = caller::call<NTSTATUS>(ntQueryInformationProcess, GetCurrentProcess(), 30, &debugObject, sizeof(debugObject), 0);

		if (IsDebuggerPresent()
			|| isRemoteDebuggerPresent
			//|| heapFlags > 2
			//|| heapForceFlags > 0
			//|| ntGlobalFlag & 0x00000070
			|| (ntStatusProcessDebugPort == 0 && isRemotePresent != 0)
			|| (ntStatusProcessDebugFlags == 0 && noDebugInherit == 0)
			|| (ntStatusProcessDebugObject == 0 && debugObject)) {
		}

		Sleep(5000);
	}

	void processDetectionThread() {
#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

		static std::string strings[] = {
			"ollydbg.exe"_Protect, "processhacker.exe"_Protect, "tcpview.exe"_Protect, "autoruns.exe"_Protect,
			"filemon.exe"_Protect, "procmon.exe"_Protect, "regmon.exe"_Protect, "procexp.exe"_Protect,
			"ida.exe"_Protect, "immunitydebugger.exe"_Protect, "wireshark.exe"_Protect, "dumpcap.exe"_Protect,
			"hookexplorer.exe"_Protect, "importrec.exe"_Protect, "petools.exe"_Protect, "lordpe.exe"_Protect,
			"sysinspector.exe"_Protect, "proc_analyzer.exe"_Protect, "sysanalyzer.exe"_Protect, "sniff_hit.exe"_Protect,
			"windbg.exe"_Protect, "joeboxcontrol.exe"_Protect, "joeboxserver.exe"_Protect, "apimonitor.exe"_Protect,
			"apimonitor-x86.exe"_Protect, "apimonitor-x64.exe"_Protect, "x32dbg.exe"_Protect, "x64dbg.exe"_Protect, 
			"x96dbg.exe"_Protect, "cheatengine.exe"_Protect, "scylla.exe"_Protect, "charles.exe"_Protect,
			"proxifier.exe"_Protect, "netmon.exe"_Protect,
		};

		HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapShot != INVALID_HANDLE_VALUE) {
			PROCESSENTRY32 procEntry;
			procEntry.dwSize = sizeof(PROCESSENTRY32W);

			if (Process32First(snapShot, &procEntry)) {
				do {
					for (auto& str : strings) {
						std::string name(procEntry.szExeFile);
						std::transform(name.begin(), name.end(), name.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

						if (!strcmp(name.c_str(), str.c_str())) {
							return;
						}
					}
				} while (Process32Next(snapShot, &procEntry));
			}

			CloseHandle(snapShot);
		}
			
		Sleep(15000);
	}

	bool securityHelpers::initialize() {
		global::vars::g_windows7 = std::filesystem::exists(utils::getConfig()->getWindowsFilePath());

		customInfo("S"_Protect, "Running Windows 7: %s"_Protect, global::vars::g_windows7 ? "YES"_Protect : "NO"_Protect);

		if (!global::vars::g_ntdll) {
			customError("S"_Protect, "Failed to initialize (#1)"_Protect);
			return false;
		}

		if (!getModuleMemoryIntegrityHash(global::vars::g_menuHandle, global::vars::m_menuPE)) return false;

		utils::getThreadPool()->add("ST"_Protect, mainSecurityThread);
		utils::getThreadPool()->add("DT"_Protect, debuggerDetectionThread);
		utils::getThreadPool()->add("PDT"_Protect, processDetectionThread);
		return true;
	}

	bool securityHelpers::registerIntegrity(const char* name, uint64_t address, BYTE expectedFirstByte) {
		if (address == 0) return true; // :shrug:
		integrityItem item;
		item.name = name;
		item.address = address;
		memcpy(item.originalBytes, (void*)address, 5);
		
		if (expectedFirstByte != 0) {
			if (*(BYTE*)(address) != expectedFirstByte) {
				customError("S"_Protect, "Failed to initialize %s (#2)"_Protect, name);
				return false;
			}
		}

		integrityItems.push_back(item);
		return true;
	}

	void securityHelpers::closeProcess() {
		// first try the traditional ways
		exit(0);
		ExitProcess(0);
		TerminateProcess(GetCurrentProcess(), 0);

		// if we get here, prolly patched the above
		static uint64_t ntSuspendProcess = 0;
		if (ntSuspendProcess == 0) {
			if (global::vars::g_windows7) {
				ntSuspendProcess = (uint64_t)GetProcAddress(global::vars::g_ntdll, "NtSuspendProcess"_Protect);
			} else {
				ntSuspendProcess = generateSyscallCaller(625);
			}
		}

		caller::call<int>(ntSuspendProcess, GetCurrentProcess());

		int i = 0;
		while (true) {
			*(uint64_t*)((uint64_t)global::vars::g_ntdll + i) = 0xCCCCCCCCCCCCCCCC;
			i += 8;
		}
	}

	uint64_t securityHelpers::generateSyscallCaller(int syscall) {
		int sys = 0;

		BYTE syscallAsmWindows10[] = { 0x4C, 0x8B, 0xD1, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x05, 0xC3 };

		if (getSyscall(syscall, &sys)) {
			void* syscallMemory = VirtualAlloc(NULL, 11, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			memcpy(syscallMemory, syscallAsmWindows10, 11);
			*(int*)((uint64_t)syscallMemory + 4) = sys;
			return (uint64_t)syscallMemory;
		}

		return 0;
	}

	void securityHelpers::bluescreen() {
		ULONG retLen = 0;
		HANDLE handle;
		TOKEN_PRIVILEGES prevState;
		TOKEN_PRIVILEGES newState;

		static uint64_t ntAdjustPrivilegesToken = 0;
		static uint64_t ntOpenProcessToken = 0;
		static uint64_t ntClose = 0;
		static uint64_t ntRaiseHardError = 0;

		if (ntAdjustPrivilegesToken == 0) {
			if (global::vars::g_windows7) {
				ntOpenProcessToken = (uint64_t)GetProcAddress(global::vars::g_ntdll, "NtOpenProcessToken"_Protect);
				ntAdjustPrivilegesToken = (uint64_t)GetProcAddress(global::vars::g_ntdll, "NtAdjustPrivilegesToken"_Protect);
				ntClose = (uint64_t)GetProcAddress(global::vars::g_ntdll, "NtClose"_Protect);
				ntRaiseHardError = (uint64_t)GetProcAddress(global::vars::g_ntdll, "NtRaiseHardError"_Protect);
			} else {
				ntOpenProcessToken = generateSyscallCaller(433);
				ntAdjustPrivilegesToken = generateSyscallCaller(213);
				ntClose = generateSyscallCaller(260);
				ntRaiseHardError = generateSyscallCaller(521);
			}
		}

		if (caller::call<int>(ntOpenProcessToken, GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &handle) >= 0) {
			prevState.PrivilegeCount = 1;
			newState.PrivilegeCount = 1;
			newState.Privileges[0].Luid.LowPart = 19;
			newState.Privileges[0].Luid.HighPart = 0;
			newState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			caller::call<int>(ntAdjustPrivilegesToken, handle, FALSE, &newState, sizeof(TOKEN_PRIVILEGES), &prevState, &retLen);
			caller::call<int>(ntClose, handle);

			retLen = 0;
			caller::call<int>(ntRaiseHardError, STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &retLen);
		}
	}

	bool securityHelpers::getSyscall(int ordinal, int* syscall) {
		if (uint64_t proc = (uint64_t)GetProcAddress(global::vars::g_ntdll, MAKEINTRESOURCEA(ordinal))) {
			// if the first byte is the byte for { mov r10, rcx } - the start of every syscall func
			if (*(BYTE*)proc == 0x4C) {
				*syscall = *(int*)(proc + 4);
				return true;
			}
		}

		return false;
	}

	void securityHelpers::changeWallpaper() {
		FILE* pFile = fopen(utils::va::va("%s\\why_lol.png"_Protect, utils::getConfig()->getDocumentsPath()), "wb"_Protect);
		fwrite(g_notThatEasyWallpaper, 1, sizeof(g_notThatEasyWallpaper), pFile);
		fclose(pFile);

		SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (char*)utils::va::va("%s\\why_lol.png"_Protect, utils::getConfig()->getDocumentsPath()), SPIF_UPDATEINIFILE);
	}

	std::vector<imageSection> securityHelpers::enumMemorySections(HMODULE hModule, std::vector<BYTE> originalPE) {
		std::vector<imageSection> output;

		PIMAGE_DOS_HEADER pDOSHeader = NULL;
		PIMAGE_NT_HEADERS pNTHeader = NULL;

		if (originalPE.empty()) {
			pDOSHeader = static_cast<PIMAGE_DOS_HEADER>((void*)hModule);
			pNTHeader = reinterpret_cast<PIMAGE_NT_HEADERS>((PBYTE)hModule + pDOSHeader->e_lfanew);
		} else {
			pDOSHeader = static_cast<PIMAGE_DOS_HEADER>((void*)originalPE.data());
			pNTHeader = reinterpret_cast<PIMAGE_NT_HEADERS>((PBYTE)originalPE.data() + pDOSHeader->e_lfanew);
		}

		IMAGE_SECTION_HEADER* pSectionHeader = reinterpret_cast<IMAGE_SECTION_HEADER*>(pNTHeader + 1);
		for (int i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++) {
			char* section = reinterpret_cast<char*>(pSectionHeader->Name);
			uint64_t start = reinterpret_cast<uint64_t>(hModule) + pSectionHeader->VirtualAddress;

			imageSection current = { section, start, pSectionHeader->Misc.VirtualSize };
			output.push_back(current);
			pSectionHeader += 1;
		}

		return output;
	}

	bool securityHelpers::getModuleMemoryIntegrityHash(HMODULE hModule, std::vector<BYTE> originalPE) {
		std::vector<imageSection> sectionList = enumMemorySections(hModule, originalPE);

		for (int i = 0; i < sectionList.size(); i++) {
			imageSection is = sectionList[i];
			if (is.name == ".text"_Protect) {
				crypto::XECRYPT_SHA512_STATE state;
				crypto::sha512Init(&state);
				crypto::sha512Update(&state, (BYTE*)is.pointer, (DWORD)(is.size - 1));
				crypto::sha512Final(&state, moduleDigest, 64);
				return true;
			}
		}

		customError("S"_Protect, "Failed to initialize (#3)"_Protect);
		return false;
	}

	void securityHelpers::unlink() {
		PEB* peb = (PEB*)__readgsqword(0x60);

		LIST_ENTRY* currentEntry = peb->ldr->inLoadOrderModuleList.Flink;
		LDR_DATA_TABLE_ENTRY* current = nullptr;

		while (currentEntry != &peb->ldr->inLoadOrderModuleList && currentEntry != NULL) {
			current = CONTAINING_RECORD(currentEntry, LDR_DATA_TABLE_ENTRY, inLoadOrderLinks);
			if (current->dllBase == global::vars::g_menuHandle) {
				UNLINK(current->inLoadOrderLinks);
				UNLINK(current->inInitializationOrderLinks);
				UNLINK(current->inMemoryOrderLinks);

				current->fullDllName.buffer = L"";
				current->fullDllName.length = 0;
				current->baseDllName.buffer = L"";
				current->baseDllName.length = 0;
				break;
			}

			currentEntry = currentEntry->Flink;
		}
	}

	void securityHelpers::printModules() {
		PEB* peb = (PEB*)__readgsqword(0x60);

		LIST_ENTRY* currentEntry = peb->ldr->inLoadOrderModuleList.Flink;
		LDR_DATA_TABLE_ENTRY* current = nullptr;

		while (currentEntry != &peb->ldr->inLoadOrderModuleList && currentEntry != NULL) {
			current = CONTAINING_RECORD(currentEntry, LDR_DATA_TABLE_ENTRY, inLoadOrderLinks);
			if (current) {
				printf("%wZ\n", current->baseDllName);
			}

			currentEntry = currentEntry->Flink;
		}
	}

	void securityHelpers::relink() {}

	void securityHelpers::fakePE(uint64_t moduleBase) {
		for (int i = 0; i < 0x1000; i++) {
			memory::getMemory()->write(moduleBase + i, 0);
		}

		DWORD oldProtect = 0;
		VirtualProtect((void*)moduleBase, 0x30, PAGE_NOACCESS, &oldProtect);
	}

	securityHelpers* getSecurity() {
		static securityHelpers instance;
		return &instance;
	}

	namespace metrics {
		eMetrics metric::getMetric() {
			return (eMetrics)0x1337;
		}

		void metric::onEvent() {
			dev("Metric %i called with no onEvent handler", getMetric());
		}

		eMetrics metricInfo::getMetric() {
			return METRIC_INFO;
		}

		eMetrics metricDebugger::getMetric() {
			return METRIC_DEBUGGER;
		}

		void metricDebugger::onEvent() {
			return;

			getSecurity()->changeWallpaper();
			getSecurity()->bluescreen();
			menu::getInit()->unload(); // sanity
		}


		eMetrics metricThreadDebugger::getMetric() {
			return METRIC_THREAD_DEBUGGER;
		}

		void metricThreadDebugger::onEvent() {
			return;

			getSecurity()->changeWallpaper();
			getSecurity()->bluescreen();
			getSecurity()->closeProcess();
			menu::getInit()->unload(); // sanity
		}

		eMetrics metricIntegrityCheckFailed::getMetric() {
			return METRIC_INTEGRITY_CHECK_FAILED;
		}

		void metricIntegrityCheckFailed::onEvent() {
			return;

			getSecurity()->changeWallpaper();
			getSecurity()->bluescreen();
			getSecurity()->closeProcess();
			menu::getInit()->unload(); // sanity
		}

		eMetrics metricBlacklistedProcess::getMetric() {
			return METRIC_BLACKLISTED_PROCESS;
		}

		void metricBlacklistedProcess::onEvent() {
			return;

			menu::getInit()->unload();
		}

		eMetrics metricModuleDigestMismatch::getMetric() {
			return METRIC_MODULE_DIGEST_MISMATCH;
		}

		void metricModuleDigestMismatch::onEvent() {
			return;

			getSecurity()->changeWallpaper();
			getSecurity()->bluescreen();
			getSecurity()->closeProcess();
			menu::getInit()->unload(); // sanity
		}

		eMetrics metricClientCrash::getMetric() {
			return METRIC_CLIENT_CRASH;
		}
	}
}