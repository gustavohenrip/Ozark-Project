#include "binary_reader.hpp"
#include <stack>

namespace IO {
	BinaryReader::BinaryReader(uint8_t* Buffer, uint32_t MaxSize) {
		m_Buffer = Buffer;
		m_Size = MaxSize;
		m_CurrentIndex = 0;
	}

	uint8_t* BinaryReader::RegisterAllocation(uint32_t Size) {
		uint8_t* Allocation = (uint8_t*)malloc(Size);
		m_Allocations.push_back(Allocation);
		memset(Allocation, 0, Size);
		return Allocation;
	}

	bool BinaryReader::IsReadValid(uint32_t ReadSize) {
		return !((m_CurrentIndex + ReadSize) > m_Size);
	}

	void BinaryReader::ReverseBuffer(uint8_t* Buffer, uint32_t Size) {
		std::stack<uint8_t> Stack;

		for (uint32_t i = 0; i < Size; i++) {
			Stack.push(Buffer[i]);
		}

		int Index = 0;

		while (!Stack.empty()) {
			Buffer[Index++] = Stack.top();
			Stack.pop();
		}
	}

	void BinaryReader::CleanAllocations() {
		for (auto& Allocation : m_Allocations) {
			if (Allocation) {
				free(Allocation);
			}
		}
	}

	double BinaryReader::ReadDouble() {
		if (IsReadValid(sizeof(double))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(double));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(double));

			m_CurrentIndex += (uint32_t)sizeof(double);
			return *(double*)Buffer;
		}

		return 0;
	}

	short BinaryReader::ReadInt16() {
		if (IsReadValid(sizeof(short))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(short));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(short));

			m_CurrentIndex += (uint32_t)sizeof(short);
			return *(short*)Buffer;
		}

		return 0;
	}

	uint16_t BinaryReader::ReadUInt16() {
		if (IsReadValid(sizeof(ushort))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(ushort));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(ushort));

			m_CurrentIndex += (uint32_t)sizeof(ushort);
			return *(ushort*)Buffer;
		}

		return 0;
	}

	int BinaryReader::ReadInt24() {
		if (IsReadValid(3)) {
			uint8_t* Buffer = RegisterAllocation(3);
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), 3);

			m_CurrentIndex += 3;
			return (((Buffer[2] << 0x10) | (Buffer[1] << 8)) | Buffer[0]);
		}

		return 0;
	}

	int BinaryReader::ReadInt32() {
		if (IsReadValid(sizeof(int))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(int));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(int));

			m_CurrentIndex += (uint32_t)sizeof(int);
			return *(int*)Buffer;
		}

		return 0;
	}

	uint32_t BinaryReader::ReadUInt32() {
		if (IsReadValid(sizeof(uint32_t))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(uint32_t));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(uint32_t));

			m_CurrentIndex += (uint32_t)sizeof(uint32_t);
			return *(uint32_t*)Buffer;
		}

		return 0;
	}

	int64_t BinaryReader::ReadInt64() {
		if (IsReadValid(sizeof(int64_t))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(int64_t));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(int64_t));

			m_CurrentIndex += (uint32_t)sizeof(int64_t);
			return *(long long*)Buffer;
		}

		return 0;
	}

	uint64_t BinaryReader::ReadUInt64() {
		if (IsReadValid(sizeof(uint64_t))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(uint64_t));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(uint64_t));

			m_CurrentIndex += (uint32_t)sizeof(uint64_t);
			return *(uint64_t*)Buffer;
		}

		return 0;
	}

	float BinaryReader::ReadFloat() {
		if (IsReadValid(sizeof(float))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(float));
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), sizeof(float));

			m_CurrentIndex += (uint32_t)sizeof(float);
			return *(float*)Buffer;
		}

		return 0;
	}

	uint8_t BinaryReader::ReadByte() {
		if (IsReadValid(sizeof(uint8_t))) {
			uint8_t Buffer = *(uint8_t*)(m_Buffer + m_CurrentIndex);
			m_CurrentIndex += (uint32_t)sizeof(uint8_t);
			return Buffer;
		}

		return 0;
	}

	bool BinaryReader::ReadBool() {
		if (IsReadValid(sizeof(bool))) {
			bool Buffer = *(bool*)(m_Buffer + m_CurrentIndex);
			m_CurrentIndex += (uint32_t)sizeof(bool);
			return Buffer;
		}

		return 0;
	}

	const char* BinaryReader::ReadChars(uint32_t Size) {
		if (IsReadValid(Size)) {
			char* Buffer = (char*)RegisterAllocation(Size);
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), Size);

			m_CurrentIndex += Size;
			return (const char*)Buffer;
		}

		return 0;
	}

	uint8_t* BinaryReader::ReadBytes(uint32_t Size) {
		if (IsReadValid(Size)) {
			uint8_t* Buffer = RegisterAllocation(Size);
			memcpy(Buffer, (m_Buffer + m_CurrentIndex), Size);

			m_CurrentIndex += Size;
			return Buffer;
		}

		return 0;
	}

	void BinaryReader::CopyBytes(uint8_t* Destination, uint32_t Size) {
		uint8_t* Buffer = ReadBytes(Size);
		if (Buffer) {
			memcpy(Destination, Buffer, Size);
		}
	}
}