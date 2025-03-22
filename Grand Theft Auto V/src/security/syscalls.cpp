#include "syscalls.hpp"
#include "auth/crypto/crypto.hpp"
#include "structs.hpp"
#include "module.hpp"
#include "utils/caller.hpp"
#include "global/variables.hpp"

namespace Security {
	void EncryptAllocation(Syscalls::CryptedAllocItem* Address) {
		if (!Address->m_Encrypted) {
			uint8_t Key[23] = {
				0x6F, 0x7A, 0x61, 0x72, 0x6B, 0x20, 0x72, 0x75, 0x6C, 0x65, 0x7A, 0x20,
				0x6C, 0x75, 0x6E, 0x61, 0x20, 0x64, 0x72, 0x75, 0x6C, 0x65, 0x7A
			};

			Auth::Crypto::RC4(Key, sizeof(Key), Address->m_ShellCode, Address->m_Size);

			// random key each time
			Address->m_XorKey = (uint8_t)(rand() % 255);

			for (uint8_t i = 0; i < Address->m_Size; i++) {
				Address->m_ShellCode[i] ^= Address->m_XorKey;
			}

			Address->m_Encrypted = true;
		}
	}

	void DecryptAllocation(Syscalls::CryptedAllocItem* Address) {
		if (Address->m_Encrypted) {
			// random key each time
			for (uint8_t i = 0; i < Address->m_Size; i++) {
				Address->m_ShellCode[i] ^= Address->m_XorKey;
			}

			uint8_t Key[23] = {
				0x6F, 0x7A, 0x61, 0x72, 0x6B, 0x20, 0x72, 0x75, 0x6C, 0x65, 0x7A, 0x20,
				0x6C, 0x75, 0x6E, 0x61, 0x20, 0x64, 0x72, 0x75, 0x6C, 0x65, 0x7A
			};

			Auth::Crypto::RC4(Key, sizeof(Key), Address->m_ShellCode, Address->m_Size);

			Address->m_Encrypted = false;
		}
	}

	void SetupAllocation(std::pair<LPVOID, int> lpAddress, const char* SyscallShellcode, uint8_t ShellcodeSize, uint8_t ShellcodeIndexOffset) {
		Syscalls::CryptedAllocItem* Address = nullptr;

		Address = (Syscalls::CryptedAllocItem*)lpAddress.first;

		memcpy(&Address->m_ShellCode, SyscallShellcode, ShellcodeSize);
		*(int*)(&Address->m_ShellCode[ShellcodeIndexOffset]) = lpAddress.second;

		Address->m_Size = ShellcodeSize;
		EncryptAllocation(Address);
	}

	bool Syscalls::Initialize() {
		const char* SyscallShellcode = nullptr;
		uint8_t ShellcodeIndexOffset = 0;
		uint8_t ShellcodeSize = 0;

		if (!Global::Vars::g_Windows7) { // Windows 10
			SyscallShellcode = "\x49\x89\xCA\xB8\x99\x00\x00\x00\x0F\x05\xC3";
			ShellcodeIndexOffset = 4;
			ShellcodeSize = 11;
		} else if (Global::Vars::g_Windows7) { // Windows 7
			SyscallShellcode = "\xB8\x00\x00\x00\x00\x33\xC9\x8D\x54\x24\x04\x64\xFF\x15\xC0\x00\x00\x00\x83\xC4\x04\xC2\x08\x00"; // NOT TESTED
			ShellcodeIndexOffset = 1;
			ShellcodeSize = 24;
		} else {
			return false;
		}

		auto CheckAllocation = [this] (LPVOID lpAddress) {
			if (!lpAddress) {
				Clean();
				return false;
			}

			return true;
		};

		char name[MAX_PATH];
		if (GetModuleFileNameA(Module::GetModuleHandle(L"ntdll.dll"), name, MAX_PATH)) {
			FILE* fp;
			fopen_s(&fp, name, "rb"_Protect);
			if (fp) {
				fseek(fp, 0, SEEK_END);
				int size = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				m_NtdllDisk = ::VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
				if (m_NtdllDisk) {
					fread(m_NtdllDisk, 1, size, fp);
					fclose(fp);
				}
			}
		}

		m_Functions[_NtQueryVirtualMemory].second = GetSyscallIndex("NtQueryVirtualMemory"_Protect, m_NtdllDisk);
		m_Functions[_NtQuerySystemInformation].second = GetSyscallIndex("NtQuerySystemInformation"_Protect, m_NtdllDisk);
		m_Functions[_NtQueryInformationProcess].second = GetSyscallIndex("NtQueryInformationProcess"_Protect, m_NtdllDisk);
		m_Functions[_NtSetInformationThread].second = GetSyscallIndex("NtSetInformationThread"_Protect, m_NtdllDisk);
		m_Functions[_NtSuspendProcess].second = GetSyscallIndex("NtSuspendProcess"_Protect, m_NtdllDisk);
		m_Functions[_NtOpenProcessToken].second = GetSyscallIndex("NtOpenProcessToken"_Protect, m_NtdllDisk);
		m_Functions[_NtAdjustPrivilegesToken].second = GetSyscallIndex("NtAdjustPrivilegesToken"_Protect, m_NtdllDisk);
		m_Functions[_NtClose].second = GetSyscallIndex("NtClose"_Protect, m_NtdllDisk);
		m_Functions[_NtRaiseHardError].second = GetSyscallIndex("NtRaiseHardError"_Protect, m_NtdllDisk);

		if (m_NtdllDisk) ::VirtualFree(m_NtdllDisk, 0, MEM_RELEASE);

		// Allocate the syscall invoke memory
		int BlockSize = ShellcodeSize + sizeof(CryptedAllocItem) - 1;
		m_AllocatedPage = (CryptedAllocItem*)::VirtualAlloc(0, BlockSize * m_Functions.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		// now that the above syscall is resolved, we can use the secure ptrs
		for (auto& Element : m_Functions) {
			Element.second.first = (CryptedAllocItem*)&m_AllocatedPage[Element.first * BlockSize];
			if (!CheckAllocation(Element.second.first)) return false;
			SetupAllocation(Element.second, SyscallShellcode, ShellcodeSize, ShellcodeIndexOffset);
		}

		// encode the page pointer
		m_AllocatedPage = (CryptedAllocItem*)m_AllocatedPage;
		return true;
	}


	int Syscalls::GetSyscallIndex(const char* pFunction, LPVOID pDisk) {
		FARPROC FunctionAddress = 0;

		if (pDisk) {
			FunctionAddress = Module::GetProcAddressDisk((HMODULE)pDisk, pFunction);
		} else FunctionAddress = Module::GetProcAddress(Module::GetModuleHandle(L"ntdll.dll"), pFunction);

		if (FunctionAddress) {
			uint64_t Address = (uint64_t)FunctionAddress;

			if (!Global::Vars::g_Windows7) {
				if (*(uint8_t*)Address == 0x49 || *(uint8_t*)Address == 0x4C) {
					return *(int*)(Address + 4);
				}
			} else if (Global::Vars::g_Windows7) {
				if (*(uint8_t*)Address == 0xB8) {
					return *(int*)(Address + 1);
				}
			}
		}

		return 0;
	}

	void Syscalls::Clean() {
		::VirtualFree(GetAllocatedPage(), 0, MEM_RELEASE);
	}

	std::vector<uint64_t> Syscalls::GetSyscallAddresses() {
		std::vector<uint64_t> Addresses;

		for (auto Element : m_Functions) {
			Addresses.push_back((uint64_t)Element.second.first);
		}

		return Addresses;
	}

	LONG Syscalls::NtQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, int MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtQueryVirtualMemory]);
		CryptedAllocItem* Address = m_Functions[_NtQueryVirtualMemory].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtQuerySystemInformation(int SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtQuerySystemInformation]);
		CryptedAllocItem* Address = m_Functions[_NtQuerySystemInformation].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtQueryInformationProcess(HANDLE ProcessHandle, int ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength) {
		static bool FirstCall = true;

		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtQueryInformationProcess]);
		CryptedAllocItem* Address = m_Functions[_NtQueryInformationProcess].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtSetInformationThread(HANDLE ThreadHandle, int ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtSetInformationThread]);
		CryptedAllocItem* Address = m_Functions[_NtSetInformationThread].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtSuspendProcess(HANDLE ProcessHandle) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtSuspendProcess]);
		CryptedAllocItem* Address = m_Functions[_NtSuspendProcess].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, ProcessHandle);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtOpenProcessToken(HANDLE ProcessHandle, ACCESS_MASK DesiredAccess, PHANDLE TokenHandle) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtOpenProcessToken]);
		CryptedAllocItem* Address = m_Functions[_NtOpenProcessToken].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, ProcessHandle, DesiredAccess, TokenHandle);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtAdjustPrivilegesToken(HANDLE TokenHandle, BOOLEAN DisableAllPrivileges, PTOKEN_PRIVILEGES TokenPrivileges, ULONG PreviousPrivilegesLength, PTOKEN_PRIVILEGES PreviousPrivileges, PULONG RequiredLength) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtAdjustPrivilegesToken]);
		CryptedAllocItem* Address = m_Functions[_NtAdjustPrivilegesToken].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, TokenHandle, DisableAllPrivileges, TokenPrivileges, PreviousPrivilegesLength, PreviousPrivileges, RequiredLength);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtClose(HANDLE ProcessHandle) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtClose]);
		CryptedAllocItem* Address = m_Functions[_NtClose].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, ProcessHandle);
		EncryptAllocation(Address);

		return Return;
	}

	LONG Syscalls::NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, int UnicodeStringParameterMask, PVOID* Parameters, int ResponseOption, PULONG Response) {
		LONG Return = 0;

		std::unique_lock<std::mutex> l(m_Mutexs[_NtRaiseHardError]);
		CryptedAllocItem* Address = m_Functions[_NtRaiseHardError].first;

		DecryptAllocation(Address);
		Return = Caller::Call<LONG>((uint64_t)&Address->m_ShellCode, ErrorStatus, NumberOfParameters, UnicodeStringParameterMask, Parameters, ResponseOption, Response);
		EncryptAllocation(Address);

		return Return;
	}

	Syscalls* GetSyscalls() {
		static Syscalls Instance;
		return &Instance;
	}
}