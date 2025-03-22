#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"

namespace memory {
	class memory {
	public:
		template<typename A, typename V>
		void write(A address, V value) {
			// VirtualProtect changes the permissions on the page we're writing to, to make it writeable
			DWORD oldProtect;
			VirtualProtect((void*)address, sizeof(value), PAGE_READWRITE, &oldProtect);
			memcpy((void*)address, &value, sizeof(value));
			VirtualProtect((void*)address, sizeof(value), oldProtect, &oldProtect);
		}

		template<typename A>
		void writeVector(A address, std::vector<byte> vec) {
			// write an array of vector<V>
			for (std::size_t i = 0; i < vec.size(); i++) {
				write(address + i, vec[i]);
			}
		}

		template<typename A, typename R>
		R read(A address) {
			return *(R*)address;
		}

		template<typename T>
		T* getFormattedAddressWithOffset(uint64_t address, int offset = 0) {
			return (T*)(address + offset);
		}

		template<typename A>
		void nop(A address, int count) {
			DWORD oldProtect;
			VirtualProtect((void*)address, count, PAGE_EXECUTE_READWRITE, &oldProtect);
			memset((void*)address, 0x90, count);
			VirtualProtect((void*)address, count, oldProtect, &oldProtect);
		}

		uint64_t getAddressFromInstruction(uint64_t address, int opcodeSize = 3, int opcodeLength = 7);
		std::vector<uint64_t> scanForString(std::string str);
		uint64_t getLinkedPointer(uint64_t address, std::vector<DWORD> offsets);
		uint64_t getGameStart();
		uint64_t getGameEnd();
		uint64_t getGameSize();
		bool isGameMemory(uint64_t address);
	private:
		uint64_t gameStart;
		uint64_t gameEnd;
		uint64_t gameSize;

		void initialize();
		bool compare(uint8_t* data, uint8_t* mask, const char* smask);
	}; memory* getMemory();
}