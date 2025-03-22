#include "security.h"
#include "PPCVM/virtual_machine.h"
#include "PPCVM/payloads.h"
#include "syscalls/syscall.h"
#include "syscalls/wrapper.h"
#include "util/log.h"
#include "auth/crypto/crypto.h"
#include "global/auth_vars.h"
#include "global/vars.h"
#include "menu/init.h"
#include <tlhelp32.h> 

namespace structs {
	typedef struct _LSA_UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR  Buffer;
	} LSA_UNICODE_STRING, * PLSA_UNICODE_STRING, UNICODE_STRING, * PUNICODE_STRING;

	typedef struct _PEB_LDR_DATA {
		BYTE _0x0000[12];
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
	} PEB_LDR_DATA, * PPEB_LDR_DATA;

	typedef struct _LDR_DATA_TABLE_ENTRY {
		LIST_ENTRY     InLoadOrderLinks;
		LIST_ENTRY     InMemoryOrderLinks;
		LIST_ENTRY     InInitializationOrderLinks;
		LPVOID         DllBase;
		LPVOID         EntryPoint;
		ULONG          SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
	} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

	typedef struct _RTL_USER_PROCESS_PARAMETERS {
		BYTE           Reserved1[16];
		PVOID          Reserved2[10];
		UNICODE_STRING ImagePathName;
		UNICODE_STRING CommandLine;
	} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

	typedef struct _PEB {
		BYTE						 _0x0000[24];

		PPEB_LDR_DATA                Ldr;
		PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
		LPVOID                       SubSystemData;
		LPVOID                       ProcessHeap;
		LPVOID                       FastPebLock;
		LPVOID                       _SYSTEM_DEPENDENT_02;
		LPVOID                       _SYSTEM_DEPENDENT_03;
		LPVOID                       _SYSTEM_DEPENDENT_04;
		union {
			LPVOID                     KernelCallbackTable;
			LPVOID                     UserSharedInfoPtr;
		};
		DWORD                        SystemReserved;
		DWORD                        _SYSTEM_DEPENDENT_05;
		LPVOID                       _SYSTEM_DEPENDENT_06;
		LPVOID                       TlsExpansionCounter;
		LPVOID                       TlsBitmap;
		DWORD                        TlsBitmapBits[2];
		LPVOID                       ReadOnlySharedMemoryBase;
		LPVOID                       _SYSTEM_DEPENDENT_07;
		LPVOID                       ReadOnlyStaticServerData;
		LPVOID                       AnsiCodePageData;
		LPVOID                       OemCodePageData;
		LPVOID                       UnicodeCaseTableData;
		DWORD                        NumberOfProcessors;
		union {
			DWORD                      NtGlobalFlag;
			LPVOID                     dummy02;
		};
		LARGE_INTEGER                CriticalSectionTimeout;
		LPVOID                       HeapSegmentReserve;
		LPVOID                       HeapSegmentCommit;
		LPVOID                       HeapDeCommitTotalFreeThreshold;
		LPVOID                       HeapDeCommitFreeBlockThreshold;
		DWORD                        NumberOfHeaps;
		DWORD                        MaximumNumberOfHeaps;
		LPVOID                       ProcessHeaps;
		LPVOID                       GdiSharedHandleTable;
		LPVOID                       ProcessStarterHelper;
		LPVOID                       GdiDCAttributeList;
		LPVOID                       LoaderLock;
		DWORD                        OSMajorVersion;
		DWORD                        OSMinorVersion;
		WORD                         OSBuildNumber;
		WORD                         OSCSDVersion;
		DWORD                        OSPlatformId;
		DWORD                        ImageSubsystem;
		DWORD                        ImageSubsystemMajorVersion;
		LPVOID                       ImageSubsystemMinorVersion;
		union {
			LPVOID                     ImageProcessAffinityMask;
			LPVOID                     ActiveProcessAffinityMask;
		};
#ifdef _WIN64
		LPVOID                       GdiHandleBuffer[64];
#else
		LPVOID                       GdiHandleBuffer[32];
#endif  
		LPVOID                       PostProcessInitRoutine;
		LPVOID                       TlsExpansionBitmap;
		DWORD                        TlsExpansionBitmapBits[32];
		LPVOID                       SessionId;
		ULARGE_INTEGER               AppCompatFlags;
		ULARGE_INTEGER               AppCompatFlagsUser;
		LPVOID                       pShimData;
		LPVOID                       AppCompatInfo;
		PUNICODE_STRING              CSDVersion;
		LPVOID                       ActivationContextData;
		LPVOID                       ProcessAssemblyStorageMap;
		LPVOID                       SystemDefaultActivationContextData;
		LPVOID                       SystemAssemblyStorageMap;
		LPVOID                       MinimumStackCommit;
	} PEB, * PPEB;

	struct UnlinkedModule {
		PLIST_ENTRY m_InLoadOrderLinks;
		PLIST_ENTRY m_InMemoryOrderLinks;
		PLIST_ENTRY m_InInitializationOrderLinks;
		PLDR_DATA_TABLE_ENTRY entry;
		UNICODE_STRING m_FullDllName;
		UNICODE_STRING m_BaseDllName;
	};

#define UNLINK(x)                   \
    (x).Flink->Blink = (x).Blink;   \
    (x).Blink->Flink = (x).Flink;

#define RELINK(x, real)         \
    (x).Flink->Blink = (real);  \
    (x).Blink->Flink = (real);  \
    (real)->Blink = (x).Blink;  \
    (real)->Flink = (x).Flink;
}

namespace security {
	std::unordered_map<HMODULE, structs::UnlinkedModule> g_unlinked;
#define ProcessEnvironmentBlock ((structs::PEB*)__readgsqword(0x60))

	struct image_section {
		std::string m_name;
		uint64_t m_pointer;
		uint64_t m_size;
	};

	void close_process() {
		exit(0);
		ExitProcess(0);
		TerminateProcess(GetCurrentProcess(), 0);

		int i = 0;
		while (true) {
			*(uint64_t*)((uint64_t)global::vars::g_module_handle + i) = 0xCCCCCCCCCCCCCCCC;
			i += 8;
		}
	}

	void security_::bsod() {
		return;

		ULONG return_length = 0;
		HANDLE handle;
		TOKEN_PRIVILEGES prev_state;
		TOKEN_PRIVILEGES new_state;

		if (syscalls::NtOpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &handle) >= 0) {
			prev_state.PrivilegeCount = 1;
			new_state.PrivilegeCount = 1;
			new_state.Privileges[0].Luid.LowPart = 19;
			new_state.Privileges[0].Luid.HighPart = 0;
			new_state.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			syscalls::NtAdjustPrivilegesToken(handle, FALSE, &new_state, sizeof(TOKEN_PRIVILEGES), &prev_state, &return_length);
			syscalls::NtClose(handle);

			return_length = 0;
			syscalls::NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &return_length);
		}
	}

	void security_::relink(HMODULE handle) {
		RELINK(g_unlinked[handle].entry->InLoadOrderLinks, g_unlinked[handle].m_InLoadOrderLinks);
		RELINK(g_unlinked[handle].entry->InInitializationOrderLinks, g_unlinked[handle].m_InInitializationOrderLinks);
		RELINK(g_unlinked[handle].entry->InMemoryOrderLinks, g_unlinked[handle].m_InMemoryOrderLinks);
	}

	void security_::unlink(HMODULE handle) {
		LIST_ENTRY* CurrentEntry = ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList.Flink;
		structs::LDR_DATA_TABLE_ENTRY* Current = nullptr;

		while (CurrentEntry != &ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList && CurrentEntry != nullptr) {
			Current = CONTAINING_RECORD(CurrentEntry, structs::LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (Current->DllBase == handle) {
				g_unlinked[handle].m_InLoadOrderLinks = Current->InLoadOrderLinks.Blink->Flink;
				g_unlinked[handle].m_InInitializationOrderLinks = Current->InInitializationOrderLinks.Blink->Flink;
				g_unlinked[handle].m_InMemoryOrderLinks = Current->InMemoryOrderLinks.Blink->Flink;
				g_unlinked[handle].m_FullDllName = Current->FullDllName;
				g_unlinked[handle].m_BaseDllName = Current->BaseDllName;
				g_unlinked[handle].entry = Current;

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

	std::vector<image_section> enum_memory_sections(HMODULE hmodule, std::vector<uint8_t> pe) {
		std::vector<image_section> output;

		PIMAGE_DOS_HEADER dosheader = nullptr;
		PIMAGE_NT_HEADERS ntheader = nullptr;

		if (pe.empty()) {
			dosheader = (PIMAGE_DOS_HEADER)hmodule;
			ntheader = (PIMAGE_NT_HEADERS)(hmodule + dosheader->e_lfanew);
		} else {
			dosheader = (PIMAGE_DOS_HEADER)pe.data();
			ntheader = (PIMAGE_NT_HEADERS)(pe.data() + dosheader->e_lfanew);
		}

		PIMAGE_SECTION_HEADER sectionheader = (PIMAGE_SECTION_HEADER)(ntheader + 1);
		for (int i = 0; i < ntheader->FileHeader.NumberOfSections; i++) {
			image_section current = { (char*)sectionheader->Name, (uint64_t)(hmodule + sectionheader->VirtualAddress), sectionheader->Misc.VirtualSize };
			output.push_back(current);
			sectionheader += 1;
		}

		return output;
	}

	bool update_module_digest(HMODULE handle, std::vector<uint8_t> header) {
		std::vector<image_section> section_list = enum_memory_sections(handle, header);

		for (int i = 0; i < section_list.size(); i++) {
			image_section img = section_list[i];
			if (!img.m_name.compare(XOR(".text"))) {
				auth::crypto::XECRYPT_SHA512_STATE state;
				auth::crypto::sha_init(&state);
				auth::crypto::sha_update(&state, (uint8_t*)img.m_pointer, (uint32_t)(img.m_size - 1));
				auth::crypto::sha_final(&state, global::auth::g_module_digest, 0x40);
				return true;
			}
		}

		return false;
	}

	void setup_syscalls() {
		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// set r3 to the address of ram
			vm->get_current_context()->gpr[3] = (uint64_t)vm::get_core()->get_ram();
		}, 0);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// memset
			memset((void*)vm->get_current_context()->gpr[3], (int)vm->get_current_context()->gpr[4], vm->get_current_context()->gpr[5]);
		}, 1);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// get execution address
			vm->get_current_context()->gpr[3] = (uint64_t)vm->get_payload();
		}, 2);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// "create thread" r3=func, r4=arg, r5=out thread id
			vm::virtual_machine* payload = vm::get_core()->create_vm((uint32_t*)vm->get_current_context()->gpr[5]);
			if (payload) {
				payload->execute((uint8_t*)vm->get_current_context()->gpr[3], (void*)vm->get_current_context()->gpr[4]);
			}
		}, 3);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// "create thread" - wait for completion - r3=func, r4=arg, r5=out thread id
			vm::virtual_machine* payload = vm::get_core()->create_vm((uint32_t*)vm->get_current_context()->gpr[5]);
			if (payload) {
				payload->execute((uint8_t*)vm->get_current_context()->gpr[3], (void*)vm->get_current_context()->gpr[4]);

				while (!payload->get_current_context()->complete)
					Sleep(0);
			}
		}, 4);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// memcpy
			memcpy((void*)vm->get_current_context()->gpr[3], (void*)vm->get_current_context()->gpr[4], vm->get_current_context()->gpr[5]);
		}, 5);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// sleep
			Sleep((uint32_t)vm->get_current_context()->gpr[3]);
		}, 6);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// init nt syscalls
			vm->get_current_context()->gpr[3] = syscalls::generate({ XOR("C:\\Windows\\System32\\ntdll.dll") }, {
				XOR("NtQueryVirtualMemory"),
				XOR("NtQuerySystemInformation"),
				XOR("NtQueryInformationProcess"),
				XOR("NtSetInformationThread"),
				XOR("NtSuspendProcess"),
				XOR("NtOpenProcessToken"),
				XOR("NtAdjustPrivilegesToken"),
				XOR("NtClose"),
				XOR("NtRaiseHardError")
			});
		}, 7);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// REPORT TO AUTH SERVER
		}, 8);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// bsod :weary:
			bsod();
		}, 9);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// dev
		}, 10);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// RC4
			auth::crypto::rc4((uint8_t*)vm->get_current_context()->gpr[3], (uint32_t)vm->get_current_context()->gpr[4], (uint8_t*)vm->get_current_context()->gpr[5], (uint32_t)vm->get_current_context()->gpr[6], (uint32_t)vm->get_current_context()->gpr[7]);
		}, 11);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// RC4 modified
			auth::crypto::rc4_modified((uint8_t*)vm->get_current_context()->gpr[3], (uint32_t)vm->get_current_context()->gpr[4], (uint8_t*)vm->get_current_context()->gpr[5], (uint32_t)vm->get_current_context()->gpr[6], (uint32_t)vm->get_current_context()->gpr[7]);
		}, 12);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// debugger thread
			BOOL is_remote_debugger_present = FALSE;
			CheckRemoteDebuggerPresent(GetCurrentProcess(), &is_remote_debugger_present);

			if (IsDebuggerPresent() || is_remote_debugger_present) {
				menu::init::unload();
				close_process();
			}
		}, 13);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			return;

			// process thread
			LOG("PROCESS THREAD CALLED");

#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

			// strcmp
			static std::string process_names[] = {
				XOR("ollydbg.exe"), XOR("processhacker.exe"), XOR("tcpview.exe"), XOR("autoruns.exe"),
				XOR("filemon.exe"), XOR("procmon.exe"), XOR("regmon.exe"), XOR("procexp.exe"),
				XOR("ida.exe"), XOR("immunitydebugger.exe"), XOR("wireshark.exe"), XOR("dumpcap.exe"),
				XOR("hookexplorer.exe"), XOR("importrec.exe"), XOR("petools.exe"), XOR("lordpe.exe"),
				XOR("sysinspector.exe"), XOR("proc_analyzer.exe"), XOR("sysanalyzer.exe"), XOR("sniff_hit.exe"),
				XOR("windbg.exe"), XOR("joeboxcontrol.exe"), XOR("joeboxserver.exe"), XOR("apimonitor.exe"),
				XOR("apimonitor-x86.exe"), XOR("apimonitor-x64.exe"), XOR("x32dbg.exe"), XOR("x64dbg.exe"),
				XOR("x96dbg.exe"), XOR("cheatengine.exe"), XOR("scylla.exe"), XOR("charles.exe"),
				XOR("proxifier.exe"), XOR("netmon.exe"), XOR("cheatengine-x86_64.exe"), XOR("ReClass.NET.exe"),
				XOR("HTTPDebuggerUI.exe"), XOR("ida64.exe")
			};

			// strstr
			static std::string window_names[] = {
				XOR("Scylla x64 v"), XOR("x64dbg"), XOR("ReClass.NET (x64)"), XOR("Cheat Engine"), XOR("The Wireshark"),
				XOR("HTTP Debugger")
			};

			for (HWND hwnd = GetTopWindow(0); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT)) {
				char window_name[100];
				char process_name[100];
				DWORD owning_process_id = 0;

				GetWindowTextA(hwnd, window_name, 100);
				GetWindowThreadProcessId(hwnd, &owning_process_id);

				if (std::string(window_name).empty()) continue;

				PROCESSENTRY32 entry;
				auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				if (snapshot != INVALID_HANDLE_VALUE) {
					entry.dwSize = sizeof(entry);

					if (Process32First(snapshot, &entry)) {
						do {
							if (entry.th32ProcessID == owning_process_id) {
								strcpy(process_name, entry.szExeFile);
								break;
							}
						} while (Process32Next(snapshot, &entry));
					}

					CloseHandle(snapshot);
				}

				std::string lower_process_name(process_name);
				std::transform(lower_process_name.begin(), lower_process_name.end(), lower_process_name.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

				std::string lower_window_name(window_name);
				std::transform(lower_window_name.begin(), lower_window_name.end(), lower_window_name.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

				bool processed = false;
				for (auto& name : process_names) {
					std::string lower_name(name);
					std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

					if (!strcmp(lower_name.c_str(), lower_process_name.c_str())) {
						LOG(XOR("Failed security (please don't open %s)"), lower_process_name.c_str());
						HANDLE process_handle = OpenProcess(PROCESS_TERMINATE, FALSE, owning_process_id);
						TerminateProcess(process_handle, 0);
						CloseHandle(process_handle);

						menu::init::unload();
						close_process();
					}
				}

				if (!processed) {
					for (auto& name : window_names) {
						std::string lower_name(name);
						std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

						if (strstr(lower_window_name.c_str(), lower_name.c_str())) {
							LOG(XOR("Failed security (please don't open %s)"), lower_name.c_str());
							HANDLE process_handle = OpenProcess(PROCESS_TERMINATE, FALSE, owning_process_id);
							TerminateProcess(process_handle, 0);
							CloseHandle(process_handle);

							menu::init::unload();
							close_process();
						}
					}
				}
			}
		}, 14);

		vm::get_core()->add_syscall([] (vm::virtual_machine* vm) {
			// hash thread
			return;

			if (!global::auth::g_calculated_digest) {
				if (!update_module_digest(global::vars::g_module_handle, global::auth::g_pe_header)) {
					return;
				}

				global::auth::g_calculated_digest = true;
			}

			uint8_t current_hash[64];
			memcpy(current_hash, global::auth::g_module_digest, 64);

			if (update_module_digest(global::vars::g_module_handle, global::auth::g_pe_header)) {
				if (memcmp(global::auth::g_module_digest, current_hash, 64)) {
					LOG_DEV("HASH MISMATCH");
					menu::init::unload();
					close_process();
				}
			} else {
				LOG_DEV(XOR("Failed security (1)"));
				menu::init::unload();
				close_process();
			}
		}, 15);
	}

	bool security_::load() {
#ifdef DEV_MODE
		return true;
#endif

		setup_syscalls();

		vm::virtual_machine* machine = vm::get_core()->create_vm();
		if (machine) {
			machine->execute(vm::payloads::payload_init);

			while (!machine->get_current_context()->complete) 
				Sleep(0);

			int status = (int)machine->get_current_context()->gpr[3];
			delete machine;

			switch (status) {
				case 1:
					LOG_CUSTOM_ERROR(XOR("Init"), XOR("Failed to load security (1)"));
					return false;

				case 1337:
					m_debugger_thread_id = *(uint32_t*)&vm::get_core()->get_ram()[0];
					m_process_check_thread_id = *(uint32_t*)&vm::get_core()->get_ram()[4];
					m_module_hash_check_thread_id = *(uint32_t*)&vm::get_core()->get_ram()[8];
					return true;
			}
		}

		LOG_CUSTOM_ERROR(XOR("Init"), XOR("Failed to load security (2)"));
		return false;

		return true;
	}

	security_* get_security() {
		static security_ instance;
		return &instance;
	}
}