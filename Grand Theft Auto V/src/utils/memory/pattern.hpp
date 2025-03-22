#pragma once
#include "stdafx.hpp"
#include "global/auth_vars.hpp"

namespace Memory {
	struct PatternByte {
		PatternByte() : m_Ignore(true), m_Data(0) {};

		PatternByte(std::string Pattern, bool Ignorethis = false) {
			m_Data = StringToUint8(Pattern);
			m_Ignore = Ignorethis;
		};

		bool m_Ignore;
		uint8_t m_Data;
	private:
		uint8_t StringToUint8(std::string str) {
			std::istringstream StringStream(str);
			uint32_t ReturnValue = 0;

			if (StringStream >> std::hex >> ReturnValue) {
				return (uint8_t)ReturnValue;
			}

			return 0;
		}
	};

	class PatternBatch {
	public:
		void Submit(PatternInfo Pattern, std::function<void(uint64_t)> Callback);
		void Submit(const char* Name, std::function<void(uint64_t)> Callback);
		void Flush();
	private:
		struct BatchedPattern {
			PatternInfo m_Pattern;
			std::vector<PatternByte> m_Bytes;
			std::function<void(uint64_t)> m_Callback;
			bool m_Found;
			uint64_t m_Address;
		};

		std::vector<BatchedPattern> m_Buffer;
	};

	uint64_t Scan(std::string Pattern, const char* Name = nullptr);
	uint64_t ScanFromBase(std::string Pattern, uint32_t Offset);
	bool ServerScanProcess(const char* Name, std::function<void(uint64_t)> Callback);
	bool ScanProcess(PatternInfo Pattern, std::function<void(uint64_t)> Callback);
	bool ScanProcessModule(const char* Name, PatternInfo Pattern, std::function<void(uint64_t)> Callback);
}