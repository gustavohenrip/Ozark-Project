#pragma once
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
#include "../Dependencies/VMProtectSDK.h"

#pragma comment(lib, "libMinHook.x64.lib")
#pragma comment(lib, "VMProtectSDK64.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#pragma warning(disable : 4244)
     
template<typename T, int N>
constexpr int NUMOF(T(&)[N]) { return N; }

template<typename T>
bool isValidPtr(T ptr) {
	if ((uint64_t)ptr == 0) return false;
	if ((uint64_t)ptr <= 0xFFFFFFFF) return false;
	if (((uint64_t)ptr & 0xFFFFFFFF) < 0x100) return false;
	if (((uint64_t)ptr & 0xFFFFFFFF) == 0xFFFFFFFF) return false;
	return true;
}

#define DEV
#define TESTER1
#define MENU_VERSION_INTERNAL 9
__forceinline const char* operator""_Protect(const char* string, std::size_t) {
#ifndef DEV
	return VMProtectDecryptStringA(string);
#else
	return string;
#endif
}