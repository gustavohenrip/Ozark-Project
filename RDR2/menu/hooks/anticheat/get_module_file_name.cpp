#include "../hooks.hpp"
#include "utils/log.hpp"
#include <algorithm>

DWORD menu::hooks::getModuleFileNameAHook(HMODULE hModule, const char* filename, DWORD size) {
	std::string str(filename);
	std::transform(str.begin(), str.end(), str.begin(), [] (unsigned char c) -> unsigned char { return std::tolower(c); });

	if (strstr(str.c_str(), "scripthook")
		|| strstr(str.c_str(), "trainer")
		|| strstr(str.c_str(), "cheat")
		|| strstr(str.c_str(), "ozark")
		|| strstr(str.c_str(), "RDR2.dll")
		|| strstr(str.c_str(), "RDR2_")) {
		customDev("AC", "Blocking file name resolve for %s (A)", filename);
		return 0;
	}

	return originalGetModuleFileNameA(hModule, filename, size);
}

DWORD menu::hooks::getModuleFileNameWHook(HMODULE hModule, const wchar_t* filename, DWORD size) {
	std::wstring str(filename);
	std::transform(str.begin(), str.end(), str.begin(), [] (wchar_t c) -> wchar_t { return std::tolower(c); });

	if (wcsstr(str.c_str(), L"scripthook")
		|| wcsstr(str.c_str(), L"trainer")
		|| wcsstr(str.c_str(), L"cheat")
		|| wcsstr(str.c_str(), L"ozark")
		|| wcsstr(str.c_str(), L"RDR2.dll")
		|| wcsstr(str.c_str(), L"RDR2_")) {
		customDev("AC", "Blocking file name resolve for %ls (W)", filename);
		return 0;
	}

	return originalGetModuleFileNameW(hModule, filename, size);
}