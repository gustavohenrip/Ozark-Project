#include "memory.hpp"
#include <Psapi.h>

namespace memory {
	uint64_t memory::getAddressFromInstruction(uint64_t address, int opcodeSize, int opcodeLength) {
		return (uint64_t)(*(int*)(address + opcodeSize) + address + opcodeLength);
	}

	std::vector<uint64_t> memory::scanForString(std::string str) {
		// scans for strings in the games memory and returns their addresses
		std::vector<uint64_t> addresses;

		std::string mask = "";
		for (std::size_t i = 0; i < str.length(); i++) mask += "x";

		for (uint64_t start = getGameStart(); start < getGameEnd(); start++) {
			if (compare((uint8_t*)start, (uint8_t*)str.c_str(), mask.c_str())) {
				addresses.push_back(start);
			}
		}

		return addresses;
	}

	uint64_t memory::getLinkedPointer(uint64_t address, std::vector<DWORD> offsets) {
		// loops thru pointers
		uint64_t ptr = *(uint64_t*)address;
		if (!ptr) return 0;

		for (auto const& value : offsets) {
			if (&value == &offsets.back()) {
				ptr += value;
				if (!ptr) return 0;
				return ptr;
			} else {
				ptr = *(uint64_t*)(ptr + value);
				if (!ptr) return 0;
			}
		}

		return ptr;
	}

	uint64_t memory::getGameStart() {
		initialize();
		return gameStart;
	}

	uint64_t memory::getGameEnd() {
		initialize();
		return gameEnd;
	}

	uint64_t memory::getGameSize() {
		initialize();
		return gameSize;
	}

	bool memory::isGameMemory(uint64_t address) {
		initialize();
		return address >= gameStart && address <= (gameStart + gameSize);
	}

	void memory::initialize() {
		// initializes the start and size vars
		if (gameStart) return;

		gameStart = (uint64_t)GetModuleHandleA(NULL);
		auto nt = (IMAGE_NT_HEADERS64*)(((IMAGE_DOS_HEADER*)gameStart) + ((IMAGE_DOS_HEADER*)gameStart)->e_lfanew);

		gameEnd = gameStart + nt->OptionalHeader.SizeOfCode;
		gameSize = nt->OptionalHeader.SizeOfImage;
	}

	bool memory::compare(uint8_t* data, uint8_t* mask, const char* smask) {
		for (; *smask; ++smask, ++data, ++mask)
			if (*smask == 'x' && *data != *mask)
				return false;

		return *smask == NULL;
	}

	memory* getMemory() {
		static memory instance;
		return &instance;
	}
}