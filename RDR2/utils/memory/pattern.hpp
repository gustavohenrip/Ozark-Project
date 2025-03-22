#pragma once
#include "stdafx.hpp"
#include "memory.hpp"
#include "utils/singleton.hpp"
#include "global/auth_vars.hpp"

namespace memory {
	struct patternByte {
		patternByte()
			: ignore(true), data(0)
		{};

		patternByte(std::string pattern, bool ignorethis = false) {
			data = stringToUint8(pattern);
			ignore = ignorethis;
		};

		bool ignore;
		uint8_t data;
	private:
		uint8_t stringToUint8(std::string str) {
			std::istringstream iss(str);
			uint32_t ret = 0;

			if (iss >> std::hex >> ret) {
				return (uint8_t)ret;
			}
		
			return 0;
		}
	};

	struct batchedPattern {
		patternInfo pattern;
		std::function<void(uint64_t)> callback;
		std::vector<patternByte> bytes;
		bool found;
		uint64_t address;
	};

	class pattern {
	public:
		uint64_t scan(const char* moduleName, std::string pattern);
		bool serverScanProcess(const char* name, std::function<void(uint64_t)> callback);
		bool scanProcess(patternInfo pattern, std::function<void(uint64_t)> callback);
		void scanModule(const char* moduleName, patternInfo pattern, std::function<void(uint64_t)> callback);
	}; pattern* getPattern();
}