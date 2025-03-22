#pragma once
#include "stdafx.hpp"

namespace Utils {
	uint32_t GetTickCount();
	void CreateThread(void* Thread, void* Param);
	void GenerateRandomBytes(uint8_t* Buffer, uint32_t Length);
	std::string ConvertBytesToString(uint8_t* Buffer, uint32_t Length);
	std::vector<uint8_t> ConvertIntegerToBytes(uint32_t Integer);
	std::string GetCurrentPath();
	std::vector<std::string> Split(std::string& s, char c);
}