#pragma once
#include "stdafx.hpp"

namespace IO {
	class BinaryWriter {
	public:
		BinaryWriter() {}
		BinaryWriter(uint8_t* Buffer, uint32_t MaxSize);

		bool IsReadValid(uint32_t ReadSize);
		void ReverseBuffer(uint8_t* Buffer, uint32_t Size);
		void CleanAllocations();

		void WriteDouble(double Value);
		void WriteInt16(short Value);
		void WriteUInt16(ushort Value);
		void WriteInt32(int Value);
		void WriteUInt32(uint32_t Value);
		void WriteInt64(int64_t Value);
		void WriteUInt64(uint64_t Value);
		void WriteFloat(float Value);
		void WriteByte(uint8_t Value);
		void WriteBytes(uint8_t* Value, uint32_t Size);
	private:
		uint8_t* m_Buffer;
		uint32_t m_Size;
		int m_CurrentIndex;
		std::vector<uint8_t*> m_Allocations;

		uint8_t* RegisterAllocation(uint32_t Size);
	};
}