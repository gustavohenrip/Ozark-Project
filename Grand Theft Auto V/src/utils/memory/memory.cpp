#include "memory.hpp"

namespace Memory {
	void WriteVector(uint64_t Address, std::vector<uint8_t> Vec) {
		for (std::size_t i = 0; i < Vec.size(); i++) {
			Write(Address + i, Vec[i]);
		}
	}

	uint64_t GetAddressFromInstruction(uint64_t Address, int OpcodeSize, int OpcodeLength) {
		return (uint64_t)(*(int*)(Address + OpcodeSize) + Address + OpcodeLength);
	}
}