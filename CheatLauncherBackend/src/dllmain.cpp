// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.hpp"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
#ifdef DEV
	FILE* stream;
	AllocConsole();
	SetConsoleTitleA("Ozark");

	freopen_s(&stream, "CONIN$", "w", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);
#endif
	return TRUE;
}