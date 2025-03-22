#pragma once
#include "stdafx.hpp"

namespace IO {
	class BinaryReader {
	public:
		BinaryReader(uint8_t* Buffer, uint32_t MaxSize);

		bool IsReadValid(uint32_t ReadSize);
		void ReverseBuffer(uint8_t* Buffer, uint32_t Size);
		void CleanAllocations();

		double ReadDouble();
		short ReadInt16();
		uint16_t ReadUInt16();
		int ReadInt24();
		int ReadInt32();
		uint32_t ReadUInt32();
		int64_t ReadInt64();
		uint64_t ReadUInt64();
		float ReadFloat();
		uint8_t ReadByte();
		bool ReadBool();
		const char* ReadChars(uint32_t Size);
		uint8_t* ReadBytes(uint32_t Size);

		void CopyBytes(uint8_t* Destination, uint32_t Size);
		uint32_t GetCurrentIndex() { return m_CurrentIndex; }
	private:
		uint8_t* m_Buffer;
		uint32_t m_Size;
		uint32_t m_CurrentIndex;
		std::vector<uint8_t*> m_Allocations;

		uint8_t* RegisterAllocation(uint32_t Size);
	};
}