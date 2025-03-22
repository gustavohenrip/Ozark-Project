#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <Windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <intrin.h>
#include <Psapi.h>
#include <timeapi.h>
#include <intrin.h>
#include <stack>
#include "../Dependencies/VMProtectSDK.h"

#pragma comment(lib, "libMinHook.x64.lib")
#pragma comment(lib, "VMProtectSDK64.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#pragma warning(disable : 4244)

template<typename T, int N>
constexpr int NUMOF(T(&)[N]) { return N; }

template<typename T>
inline bool IsValidPtr(T Ptr) {
	uint64_t Address = (uint64_t)Ptr;
	if (Address < 0x5000) return false;
	if ((Address & 0xFFFFFFFF) == 0xFFFFFFFF) return false;
	return true;
}

#define IsValidVTable(Address) \
	[=]() -> bool { \
		if (!IsValidPtr<uint64_t>(Address)) return false; \
		return Address > Global::Vars::g_GameAddress && Address < (Global::Vars::g_GameAddress + Global::Vars::g_GameSize); \
	}()


constexpr char CharacterMap[] = {
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '.', '/', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'_', '_',  '_', '_', '_', '_',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
};

constexpr static __forceinline uint32_t JenkinsHash32(const char* String, uint32_t CurrentHashValue = NULL) {
	while (*String != NULL) {
		CurrentHashValue += CharacterMap[*(String++)];
		CurrentHashValue += (CurrentHashValue << 10);
		CurrentHashValue ^= (CurrentHashValue >> 6);
	}

	CurrentHashValue += (CurrentHashValue << 3);
	CurrentHashValue ^= (CurrentHashValue >> 11);
	CurrentHashValue += (CurrentHashValue << 15);

	return CurrentHashValue;
}

#define joaat(String) \
    []( ) -> auto { \
        constexpr auto HashValue = JenkinsHash32( String ); \
        \
        return HashValue; \
    }( )

#define ProcessEnvironmentBlock ((Security::Structs::PEB*)__readgsqword(0x60))
#define TRANSLATE(name) name.Get().c_str()

#undef min
#undef max
#undef GetTempPath

#undef GetCurrentProcess
#define GetCurrentProcess() (HANDLE)-1

#define DEV1
#define MENU_VERSION "BETA"
#define MENU_VERSION_INTERNAL 26
#define MENU_PTB 0
#define BETA "~g~[BETA]~s~ "

template<typename T, typename T1>
T* Cast(T1 Type) {
	return (T*)(Type);
}

__forceinline const char* operator""_Protect(const char* string, std::size_t) {
#ifndef DEV
	return VMProtectDecryptStringA(string);
#else
	return string;
#endif
}