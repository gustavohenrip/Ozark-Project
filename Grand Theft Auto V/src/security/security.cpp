#include "security.hpp"
#include "utils/log.hpp"
#include "syscalls.hpp"
#include "module.hpp"
#include "global/security_vars.hpp"
#include "global/variables.hpp"
#include "auth/crypto/crypto.hpp"
#include "utils/thread_pool.hpp"
#include "menu/auth.hpp"
#include "menu/init.hpp"
#include "utils/memory/memory.hpp"
#include <VersionHelpers.h>
#include <tlhelp32.h> 

namespace Security {
#define UNLINK(x)                   \
    (x).Flink->Blink = (x).Blink;   \
    (x).Blink->Flink = (x).Flink;

#define RELINK(x, real)         \
    (x).Flink->Blink = (real);  \
    (x).Blink->Flink = (real);  \
    (real)->Blink = (x).Blink;  \
    (real)->Flink = (x).Flink;

	struct UnlinkedModule {
		PLIST_ENTRY m_InLoadOrderLinks;
		PLIST_ENTRY m_InMemoryOrderLinks;
		PLIST_ENTRY m_InInitializationOrderLinks;
		Structs::PLDR_DATA_TABLE_ENTRY m_Entry;
		Structs::UNICODE_STRING m_FullDllName;
		Structs::UNICODE_STRING m_BaseDllName;
	}; UnlinkedModule g_Unlinked;

	namespace Metrics {
		eMetricID Metric::GetMetricID() {
			return METRIC_BAD;
		}

		void Metric::OnEvent() {
			LOG_DEV("Metric %i called with no OnEvent", GetMetricID());
		}

		eMetricID MetricThreadCreatedInUnmappedMemory::GetMetricID() {
			return METRIC_THREAD_CREATED_IN_UNMAPPED_MEMORY;
		}

		eMetricID MetricDebugging::GetMetricID() {
			return METRIC_DEBUGGING;
		}

		void MetricDebugging::OnEvent() {
			return;

			Bluescreen();
			CloseProcess();
			Menu::GetInit()->Unload();
		}

		eMetricID MetricThreadBeingDebugged::GetMetricID() {
			return METRIC_THREAD_BEING_DEBUGGED;
		}

		void MetricThreadBeingDebugged::OnEvent() {
			return;

			Bluescreen();
			CloseProcess();
			Menu::GetInit()->Unload();
		}

		eMetricID MetricFunctionIntegrityFailed::GetMetricID() {
			return METRIC_FUNCTION_INTEGRITY_FAILED;
		}

		void MetricFunctionIntegrityFailed::OnEvent() {
			Menu::GetInit()->Unload();
		}

		eMetricID MetricBlacklistedProcess::GetMetricID() {
			return METRIC_BLACKLISTED_PROCESS;
		}

		eMetricID MetricModuleDigestMismatch::GetMetricID() {
			return METRIC_MODULE_DIGEST_MISMATCH;
		}

		void MetricModuleDigestMismatch::OnEvent() {
			return;

			Bluescreen();
			CloseProcess();
			Menu::GetInit()->Unload();
		}

		eMetricID MetricCrashed::GetMetricID() {
			return METRIC_CRASHED;
		}
	}

	void MainSecurityThread() {
		uint8_t CurrentHash[64];
		memcpy(CurrentHash, Global::Security::g_ModuleDigest, 64);

		if (UpdateModuleDigest(Global::Vars::g_ModuleHandle, Global::Security::g_PEHeader)) {
			if (memcmp(Global::Security::g_ModuleDigest, CurrentHash, 64)) {
				Auth::ProcessMetric(new Metrics::MetricModuleDigestMismatch(), "");
			}
		} else {
			LOG_CUSTOM_ERROR("S"_Protect, "Failed to initialize (#4)"_Protect);
			Menu::GetInit()->Unload();
		}

		auto Threads = Utils::GetThreadPool()->GetThreads();
		for (auto& Thread : Threads) {
			CONTEXT Context = { 0 };
			Context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

			if (GetThreadContext(Thread->m_Handle, &Context) == 0) {
				continue;
			}

			if (Context.Dr0 != 0 || Context.Dr1 != 0 || Context.Dr2 != 0 || Context.Dr3 != 0) {
				Auth::ProcessMetric(new Metrics::MetricThreadBeingDebugged(), Thread->m_Name);
				break;
			}
		}

		Sleep(5000);
	}

	void DebuggerDetectionThread() {
		BOOL IsRemoteDebuggerPresent;
		CheckRemoteDebuggerPresent(GetCurrentProcess(), &IsRemoteDebuggerPresent);

		uint64_t IsRemotePresent = 0;
		uint32_t NoDebugInherit = 0;
		HANDLE DebugObject = 0;

		LONG ntStatusProcessDebugPort = GetSyscalls()->NtQueryInformationProcess(GetCurrentProcess(), 7, &IsRemotePresent, sizeof(IsRemotePresent), 0);
		LONG ntStatusProcessDebugFlags = GetSyscalls()->NtQueryInformationProcess(GetCurrentProcess(), 31, &NoDebugInherit, sizeof(NoDebugInherit), 0);
		LONG ntStatusProcessDebugObject = GetSyscalls()->NtQueryInformationProcess(GetCurrentProcess(), 30, &DebugObject, sizeof(DebugObject), 0);

		if (IsDebuggerPresent()
			|| IsRemoteDebuggerPresent
			|| (ntStatusProcessDebugPort == 0 && IsRemotePresent != 0)
			|| (ntStatusProcessDebugFlags == 0 && NoDebugInherit == 0)
			|| (ntStatusProcessDebugObject == 0 && DebugObject)) {
			Auth::ProcessMetric(new Metrics::MetricDebugging(), "");
		}

		Sleep(5000);
	}

	void ProcessDetectionThread() {
#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

		// strcmp
		static std::string ProcessNames[] = {
			"ollydbg.exe"_Protect, "processhacker.exe"_Protect, "tcpview.exe"_Protect, "autoruns.exe"_Protect,
			"filemon.exe"_Protect, "procmon.exe"_Protect, "regmon.exe"_Protect, "procexp.exe"_Protect,
			"ida.exe"_Protect, "immunitydebugger.exe"_Protect, "wireshark.exe"_Protect, "dumpcap.exe"_Protect,
			"hookexplorer.exe"_Protect, "importrec.exe"_Protect, "petools.exe"_Protect, "lordpe.exe"_Protect,
			"sysinspector.exe"_Protect, "proc_analyzer.exe"_Protect, "sysanalyzer.exe"_Protect, "sniff_hit.exe"_Protect,
			"windbg.exe"_Protect, "joeboxcontrol.exe"_Protect, "joeboxserver.exe"_Protect, "apimonitor.exe"_Protect,
			"apimonitor-x86.exe"_Protect, "apimonitor-x64.exe"_Protect, "x32dbg.exe"_Protect, "x64dbg.exe"_Protect,
			"x96dbg.exe"_Protect, "cheatengine.exe"_Protect, "scylla.exe"_Protect, "charles.exe"_Protect,
			"proxifier.exe"_Protect, "netmon.exe"_Protect, "cheatengine-x86_64.exe"_Protect, "ReClass.NET.exe"_Protect
		};

		// strstr
		static std::string WindowNames[] = {
			"Scylla x64 v"_Protect, "x64dbg"_Protect, "ReClass.NET (x64)"_Protect, "Cheat Engine"_Protect, "The Wireshark"_Protect
		};

		for (HWND hwnd = GetTopWindow(0); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT)) {
			char WindowName[100];
			char ProcessName[100];
			DWORD OwningProcessID = 0;

			GetWindowTextA(hwnd, WindowName, 100);
			GetWindowThreadProcessId(hwnd, &OwningProcessID);

			if (std::string(WindowName).empty()) continue;

			PROCESSENTRY32 m_Entry;
			auto m_Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (m_Snapshot != INVALID_HANDLE_VALUE) {
				m_Entry.dwSize = sizeof(m_Entry);

				if (Process32First(m_Snapshot, &m_Entry)) {
					do {
						if (m_Entry.th32ProcessID == OwningProcessID) {
							strcpy(ProcessName, m_Entry.szExeFile);
							break;
						}
					} while (Process32Next(m_Snapshot, &m_Entry));
				}

				CloseHandle(m_Snapshot);
			}

			std::string LowerProcessName(ProcessName);
			std::transform(LowerProcessName.begin(), LowerProcessName.end(), LowerProcessName.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

			std::string LowerWindowName(WindowName);
			std::transform(LowerWindowName.begin(), LowerWindowName.end(), LowerWindowName.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

			bool Processed = false;
			for (auto& Name : ProcessNames) {
				std::string LowerName(Name);
				std::transform(LowerName.begin(), LowerName.end(), LowerName.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

				if (!strcmp(LowerName.c_str(), LowerProcessName.c_str())) {
					HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, FALSE, OwningProcessID);
					TerminateProcess(ProcessHandle, 0);
					CloseHandle(ProcessHandle);

					if (!Processed) {
						Auth::ProcessMetric(new Metrics::MetricBlacklistedProcess(), Name);
					}

					Processed = true;
				}
			}

			if (!Processed) {
				for (auto& Name : WindowNames) {
					std::string LowerName(Name);
					std::transform(LowerName.begin(), LowerName.end(), LowerName.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

					if (strstr(LowerWindowName.c_str(), LowerName.c_str())) {
						HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, FALSE, OwningProcessID);
						TerminateProcess(ProcessHandle, 0);
						CloseHandle(ProcessHandle);

						if (!Processed) {
							Auth::ProcessMetric(new Metrics::MetricBlacklistedProcess(), Name);
						}

						Processed = true;
					}
				}
			}
		}

		Sleep(1500);
	}

	bool Initialize() {
		Global::Vars::g_Windows7 = IsWindows7OrGreater() && !IsWindows8OrGreater();

		if (!GetSyscalls()->Initialize()) {
			LOG_CUSTOM_ERROR("Security"_Protect, "Failed to initialize (1)"_Protect);
			return false;
		}

		if (!UpdateModuleDigest(Global::Vars::g_ModuleHandle, Global::Security::g_PEHeader)) {
			LOG_CUSTOM_ERROR("Security"_Protect, "Failed to initialize (2)"_Protect);
			return false;
		}

		Utils::GetThreadPool()->Add("ST"_Protect, MainSecurityThread);
		Utils::GetThreadPool()->Add("DT"_Protect, DebuggerDetectionThread);
		Utils::GetThreadPool()->Add("PDT"_Protect, ProcessDetectionThread);

		return true;
 	}

	bool UpdateModuleDigest(HMODULE hModule, std::vector<uint8_t> PE) {
		std::vector<Structs::ImageSection> SectionList = EnumMemorySections(hModule, PE);

		for (int i = 0; i < SectionList.size(); i++) {
			Structs::ImageSection Img = SectionList[i];
			if (!Img.m_Name.compare(".text"_Protect)) {
				Auth::Crypto::XECRYPT_SHA512_STATE State;
				Auth::Crypto::Sha512Init(&State);
				Auth::Crypto::Sha512Update(&State, (uint8_t*)Img.m_Pointer, (uint32_t)(Img.m_Size - 1));
				Auth::Crypto::Sha512Final(&State, Global::Security::g_ModuleDigest, 64);
				return true;
			}
		}

		LOG_CUSTOM_ERROR("S"_Protect, "Failed to initialize (#3)"_Protect);
		return false;
	}

	std::vector<Structs::ImageSection> EnumMemorySections(HMODULE hModule, std::vector<uint8_t> PE) {
		std::vector<Structs::ImageSection> Output;

		PIMAGE_DOS_HEADER DOSHeader = nullptr;
		PIMAGE_NT_HEADERS NTHeader = nullptr;

		if (PE.empty()) {
			DOSHeader = (PIMAGE_DOS_HEADER)hModule;
			NTHeader = (PIMAGE_NT_HEADERS)(hModule + DOSHeader->e_lfanew);
		} else {
			DOSHeader = (PIMAGE_DOS_HEADER)PE.data();
			NTHeader = (PIMAGE_NT_HEADERS)(PE.data() + DOSHeader->e_lfanew);
		}

		PIMAGE_SECTION_HEADER SectionHeader = (PIMAGE_SECTION_HEADER)(NTHeader + 1);
		for (int i = 0; i < NTHeader->FileHeader.NumberOfSections; i++) {
			Structs::ImageSection current = { (char*)SectionHeader->Name, (uint64_t)(hModule + SectionHeader->VirtualAddress), SectionHeader->Misc.VirtualSize };
			Output.push_back(current);
			SectionHeader += 1;
		}

		return Output;
	}

	void CloseProcess() {
		// first try the traditional ways
		exit(0);
		ExitProcess(0);
		TerminateProcess(GetCurrentProcess(), 0);

		// if we get here, prolly patched the above
		GetSyscalls()->NtSuspendProcess(GetCurrentProcess());

		int i = 0;
		while (true) {
			*(uint64_t*)((uint64_t)Global::Vars::g_ModuleHandle + i) = 0xCCCCCCCCCCCCCCCC;
			i += 8;
		}
	}

	void Bluescreen() {
		ULONG RetLen = 0;
		HANDLE Handle;
		TOKEN_PRIVILEGES PrevState;
		TOKEN_PRIVILEGES NewState;

		if (GetSyscalls()->NtOpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &Handle) >= 0) {
			PrevState.PrivilegeCount = 1;
			NewState.PrivilegeCount = 1;
			NewState.Privileges[0].Luid.LowPart = 19;
			NewState.Privileges[0].Luid.HighPart = 0;
			NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			GetSyscalls()->NtAdjustPrivilegesToken(Handle, FALSE, &NewState, sizeof(TOKEN_PRIVILEGES), &PrevState, &RetLen);
			GetSyscalls()->NtClose(Handle);

			RetLen = 0;
			GetSyscalls()->NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &RetLen);
		}
	}

	void Unlink() {
		if ((uint64_t)Global::Vars::g_ModuleHandle == 0x180000000) return;

		LIST_ENTRY* CurrentEntry = ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList.Flink;
		Structs::LDR_DATA_TABLE_ENTRY* Current = nullptr;

		while (CurrentEntry != &ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList && CurrentEntry != nullptr) {
			Current = CONTAINING_RECORD(CurrentEntry, Structs::LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (Current->DllBase == Global::Vars::g_ModuleHandle) {
				g_Unlinked.m_InLoadOrderLinks = Current->InLoadOrderLinks.Blink->Flink;
				g_Unlinked.m_InInitializationOrderLinks = Current->InInitializationOrderLinks.Blink->Flink;
				g_Unlinked.m_InMemoryOrderLinks = Current->InMemoryOrderLinks.Blink->Flink;
				g_Unlinked.m_FullDllName = Current->FullDllName;
				g_Unlinked.m_BaseDllName = Current->BaseDllName;
				g_Unlinked.m_Entry = Current;

				UNLINK(Current->InLoadOrderLinks);
				UNLINK(Current->InInitializationOrderLinks);
				UNLINK(Current->InMemoryOrderLinks);

				Current->FullDllName.Buffer = L"";
				Current->FullDllName.Length = 0;
				Current->BaseDllName.Buffer = L"";
				Current->BaseDllName.Length = 0;
				break;
			}

			CurrentEntry = CurrentEntry->Flink;
		}
	}

	void Relink() {
		return;
		RELINK(g_Unlinked.m_Entry->InLoadOrderLinks, g_Unlinked.m_InLoadOrderLinks);
		RELINK(g_Unlinked.m_Entry->InInitializationOrderLinks, g_Unlinked.m_InInitializationOrderLinks);
		RELINK(g_Unlinked.m_Entry->InMemoryOrderLinks, g_Unlinked.m_InMemoryOrderLinks);
		g_Unlinked.m_Entry->FullDllName = g_Unlinked.m_FullDllName;
		g_Unlinked.m_Entry->BaseDllName = g_Unlinked.m_BaseDllName;
	}

	void FakePE() {
		if ((uint64_t)Global::Vars::g_ModuleHandle == 0x180000000) return;

		void* Ps = Module::GetModuleHandle(L"psapi.dll");
		if (Ps) {
			memcpy(Global::Vars::g_ModuleHandle, Module::GetModuleHandle(L"psapi.dll"), 0x1000);
		}
	}
}