#pragma once
#include "stdafx.hpp"

namespace Memory {
	template<typename A, typename V>
	void Write(A Address, V Value) {
		DWORD OldProtect;
		VirtualProtect((void*)Address, sizeof(Value), PAGE_EXECUTE_READWRITE, &OldProtect);
		memcpy((void*)Address, &Value, sizeof(Value));
		VirtualProtect((void*)Address, sizeof(Value), OldProtect, &OldProtect);
	}

	template<typename A, typename V>
	void WriteMemcpy(A Address, V Value, int Size) {
		DWORD OldProtect;
		VirtualProtect((void*)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
		memcpy((void*)Address, Value, Size);
		VirtualProtect((void*)Address, Size, OldProtect, &OldProtect);
	}

	template<typename T>
	T* GetFormattedAddressWithOffset(uint64_t Address, int Offset = 0) {
		return (T*)(Address + Offset);
	}

	template<typename A>
	void Nop(A Address, int Count) {
		DWORD OldProtect;
		VirtualProtect((void*)Address, Count, PAGE_EXECUTE_READWRITE, &OldProtect);
		memset((void*)Address, 0x90, Count);
		VirtualProtect((void*)Address, Count, OldProtect, &OldProtect);
	}

	void WriteVector(uint64_t Address, std::vector<uint8_t> Vec);
	uint64_t GetAddressFromInstruction(uint64_t Address, int OpcodeSize = 3, int OpcodeLength = 7);
}