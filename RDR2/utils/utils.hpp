#pragma once
#include "stdafx.hpp"
#include <tlhelp32.h>

namespace utils {
	struct secondsToTime {
		int years;
		int days;
		int hours;
		int minutes;
		int seconds;

		secondsToTime(int iSeconds_) {
			years = abs(iSeconds_ / (60 * 60 * 24 * 365));
			days = iSeconds_ / 86400;
			hours = (iSeconds_ % 86400) / 3600;
			minutes = ((iSeconds_ % 86400) % 3600) / 60;
			seconds = (((iSeconds_ % 86400) % 3600) % 60) / 1;
		}
	};

	template<typename T>
	T* allocateMemoryType() {
		void* allocated = malloc(sizeof(T));
		memset(allocated, 0, sizeof(T));
		return (T*)allocated;
	}

	void* allocateMemory(int len);
	std::string createRandomString(size_t length);
	std::vector<std::string> split(std::string s, std::string splitter);
	std::vector<BYTE> convertNumberToBytes(int paramInt);
	std::vector<char> convertStringToBytes(std::string data);
	std::string convertBytesToString(BYTE* data, int len, bool space = false);
	std::string rot13(std::string input);
	std::string rot13Number(std::string input);
	std::string rot13NumberReverse(std::string input);
	void replaceString(std::string& str, const std::string& from, const std::string& to);
	void createRandomBytes(BYTE* data, int len);
	DWORD findProcessId(std::string processName);
}