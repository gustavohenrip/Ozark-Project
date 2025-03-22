#include "injector.hpp"
#include <Tlhelp32.h> 
#include <ShlObj.h>
#pragma comment (lib, "shlwapi.lib")
#pragma comment(lib, "BlackBone.lib")

#include "../Dependencies/BlackBone/BlackBone/Process/Process.h"

#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

namespace Injector {
	DWORD GetProcessIdByName(std::string name) {
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE) return false;

		BOOL loop = Process32First(snapshot, &entry);
		while (loop) {
			if (!strcmp(entry.szExeFile, name.c_str())) {
				return entry.th32ProcessID;
			}

			loop = Process32Next(snapshot, &entry);
		}

		return 0;
	}

	bool ManualMap(DWORD ProcessID, uint8_t* Buffer, int Size) {
		blackbone::Process proc;
		NTSTATUS status = proc.Attach(ProcessID);
		return proc.mmap().MapImage(Size, Buffer, false, blackbone::ManualImports).success();
	}

	bool NativeInject(DWORD ProcessID, std::string path) {
		std::wstring w(path.begin(), path.end());

		blackbone::Process proc;
		NTSTATUS status = proc.Attach(ProcessID);
		proc.EnsureInit();
		return proc.modules().Inject(w).success();
	}
}