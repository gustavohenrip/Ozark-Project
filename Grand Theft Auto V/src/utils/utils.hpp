#pragma once
#include "stdafx.hpp"

namespace Utils {
	struct SecondsToTime {
		int Years;
		int Days;
		int Hours;
		int Minutes;
		int Seconds;

		SecondsToTime(int iSeconds_) {
			Years = abs(iSeconds_ / (60 * 60 * 24 * 365));
			Days = iSeconds_ / 86400;
			Hours = (iSeconds_ % 86400) / 3600;
			Minutes = ((iSeconds_ % 86400) % 3600) / 60;
			Seconds = (((iSeconds_ % 86400) % 3600) % 60) / 1;
		}
	};

	std::string CreateRandomString(size_t Length);
	std::vector<std::string> Split(std::string Str, std::string Splitter);
	std::vector<uint8_t> ConvertNumberToBytes(int ParamInt);
	std::vector<char> ConvertStringToBytes(std::string Data);
	std::string ConvertBytesToString(uint8_t* Data, int Length, bool Space = false);
	std::string Rot13(std::string Input);
	std::string Rot13Number(std::string Input);
	std::string Rot13NumberReverse(std::string Input);
	void ReplaceString(std::string& Str, const std::string& From, const std::string& To);
	void CreateRandomBytes(uint8_t* Data, int Length);
	std::string GetStackTrace();
	
	template<typename T>
	T* AllocateMemoryType() {
		void* Allocated = malloc(sizeof(T));
		memset(Allocated, 0, sizeof(T));
		return (T*)Allocated;
	}
}