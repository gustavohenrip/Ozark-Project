#pragma once
#include "stdafx.hpp"

namespace rage::script {
	struct scriptHeader {
		char pad_0000[16];
		void* codeBlocks;
		char pad_0018[4];
		uint32_t codeCount;
		char pad1[4];
		uint32_t localCount;
		char pad2[4];
		uint32_t nativeCount;
		char pad_0030[16];
		uint64_t* natives;
		char pad_0048[16];
		uint32_t nameHash;
		char pad_005C[4];
		char* name;
		char** strings;
		uint32_t stringCount;
		char pad_0074[12];
	};

	struct scriptTableItem {
		scriptHeader* header;
		char _0x0008[0x8];
		uint32_t nameHash;
		char _0x000C[0x4];
	};

	struct scriptTable {
		scriptTableItem* table;
		char _0x0008[0x10];
		int count;

		scriptTableItem* find(Hash hash) {
			if (!table) return nullptr;

			for (int i = 0; i < count; i++) {
				if (table[i].nameHash == hash) {
					return &table[i];
				}
			}

			return nullptr;
		}
	};
}