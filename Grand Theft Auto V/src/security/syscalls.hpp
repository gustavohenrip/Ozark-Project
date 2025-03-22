#pragma once
#include "stdafx.hpp"
#include <mutex>

namespace Security {
	class Syscalls {
	public:
		struct CryptedAllocItem {
			uint8_t m_Size;
			bool m_Encrypted;
			uint8_t m_XorKey;
			uint8_t m_ShellCode[1];
		};

		bool Initialize();
		void Clean();
		std::vector<uint64_t> GetSyscallAddresses();
		CryptedAllocItem* GetAllocatedPage() { return m_AllocatedPage; }

		LONG NtQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, int MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);
		LONG NtQuerySystemInformation(int SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
		LONG NtQueryInformationProcess(HANDLE ProcessHandle, int ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
		LONG NtSetInformationThread(HANDLE ThreadHandle, int ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength);
		LONG NtSuspendProcess(HANDLE ProcessHandle);
		LONG NtOpenProcessToken(HANDLE ProcessHandle, ACCESS_MASK DesiredAccess, PHANDLE TokenHandle);
		LONG NtAdjustPrivilegesToken(HANDLE TokenHandle, BOOLEAN DisableAllPrivileges, PTOKEN_PRIVILEGES TokenPrivileges, ULONG PreviousPrivilegesLength, PTOKEN_PRIVILEGES PreviousPrivileges, PULONG RequiredLength);
		LONG NtClose(HANDLE ProcessHandle);
		LONG NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, int UnicodeStringParameterMask, PVOID* Parameters, int ResponseOption, PULONG Response);
	private:
		enum eFunctions {
			_NtQueryVirtualMemory,
			_NtQuerySystemInformation,
			_NtQueryInformationProcess,
			_NtSetInformationThread,
			_NtSuspendProcess,
			_NtOpenProcessToken,
			_NtAdjustPrivilegesToken,
			_NtClose,
			_NtRaiseHardError
		};

		LPVOID m_NtdllDisk = nullptr;
		CryptedAllocItem* m_AllocatedPage;
		std::unordered_map<eFunctions, std::pair<CryptedAllocItem*, int>> m_Functions;
		std::unordered_map<eFunctions, std::mutex> m_Mutexs;

		int GetSyscallIndex(const char* pFunction, LPVOID pDisk = nullptr);
	};

	Syscalls* GetSyscalls();
}