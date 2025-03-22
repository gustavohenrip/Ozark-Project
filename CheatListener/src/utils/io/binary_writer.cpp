#include "binary_writer.hpp"
#include <stack>

namespace IO {
	BinaryWriter::BinaryWriter(uint8_t* Buffer, uint32_t MaxSize) {
		m_Buffer = Buffer;
		m_Size = MaxSize;
		m_CurrentIndex = 0;
	}

	uint8_t* BinaryWriter::RegisterAllocation(uint32_t Size) {
		uint8_t* Allocation = (uint8_t*)malloc(Size);
		m_Allocations.push_back(Allocation);
		memset(Allocation, 0, Size);
		return Allocation;
	}

	bool BinaryWriter::IsReadValid(uint32_t ReadSize) {
		return !((m_CurrentIndex + ReadSize) > m_Size);
	}

	void BinaryWriter::ReverseBuffer(uint8_t* Buffer, uint32_t Size) {
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

	void BinaryWriter::CleanAllocations() {
		for (auto& Allocation : m_Allocations) {
			if (Allocation) {
				free(Allocation);
			}
		}
	}

	void BinaryWriter::WriteDouble(double Value) {
		if (IsReadValid(sizeof(double))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(double));

			memcpy(Buffer, &Value, sizeof(double));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(double));

			m_CurrentIndex += (uint32_t)sizeof(double);
		}
	}

	void BinaryWriter::WriteInt16(short Value) {
		if (IsReadValid(sizeof(short))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(short));

			memcpy(Buffer, &Value, sizeof(short));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(short));

			m_CurrentIndex += (uint32_t)sizeof(short);
		}
	}

	void BinaryWriter::WriteUInt16(ushort Value) {
		if (IsReadValid(sizeof(ushort))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(ushort));

			memcpy(Buffer, &Value, sizeof(ushort));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(ushort));

			m_CurrentIndex += (uint32_t)sizeof(ushort);
		}
	}

	void BinaryWriter::WriteInt32(int Value) {
		if (IsReadValid(sizeof(int))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(int));

			memcpy(Buffer, &Value, sizeof(int));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(int));

			m_CurrentIndex += (uint32_t)sizeof(int);
		}
	}

	void BinaryWriter::WriteUInt32(uint32_t Value) {
		if (IsReadValid(sizeof(uint32_t))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(uint32_t));

			memcpy(Buffer, &Value, sizeof(uint32_t));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(uint32_t));

			m_CurrentIndex += (uint32_t)sizeof(uint32_t);
		}
	}

	void BinaryWriter::WriteInt64(int64_t Value) {
		if (IsReadValid(sizeof(int64_t))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(int64_t));

			memcpy(Buffer, &Value, sizeof(int64_t));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(int64_t));

			m_CurrentIndex += (uint32_t)sizeof(int64_t);
		}
	}

	void BinaryWriter::WriteUInt64(uint64_t Value) {
		if (IsReadValid(sizeof(uint64_t))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(uint64_t));

			memcpy(Buffer, &Value, sizeof(uint64_t));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(uint64_t));

			m_CurrentIndex += (uint32_t)sizeof(uint64_t);
		}
	}

	void BinaryWriter::WriteFloat(float Value) {
		if (IsReadValid(sizeof(float))) {
			uint8_t* Buffer = RegisterAllocation(sizeof(float));

			memcpy(Buffer, &Value, sizeof(float));
			memcpy((m_Buffer + m_CurrentIndex), Buffer, sizeof(float));

			m_CurrentIndex += (uint32_t)sizeof(float);
		}
	}

	void BinaryWriter::WriteByte(uint8_t Value) {
		if (IsReadValid(sizeof(uint8_t))) {
			*(uint8_t*)(m_Buffer + m_CurrentIndex) = Value;
			m_CurrentIndex += (uint32_t)sizeof(uint8_t);
		}
	}

	void BinaryWriter::WriteBytes(uint8_t* Value, uint32_t Size) {
		if (IsReadValid(Size)) {
			memcpy(m_Buffer + m_CurrentIndex, Value, Size);
			m_CurrentIndex += Size;
		} else {
			printf("read not valid\n");
		}
	}
}