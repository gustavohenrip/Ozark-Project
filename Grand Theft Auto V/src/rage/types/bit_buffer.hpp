#pragma once
#include "stdafx.hpp"
#include "utils/math.hpp"

#pragma pack(push)
#pragma pack(1)
namespace Rage {
	struct BitBufferSyncInstance;

	struct BitBuffer {
		uint64_t m_RawBuffer;	// 0x0
		uint32_t m_Unk1;		// 0x8
		uint32_t m_MaxBit;		// 0xC
		uint32_t m_NextBit;		// 0x10
		uint32_t m_CurrentBit;	// 0x14
		uint32_t m_Unk2;		// 0x18
		uint8_t m_BitFlag;		// 0x1C

		BitBuffer() {
			memset(this, 0, sizeof(BitBuffer));
			m_BitFlag &= 0xFC;
		}

		void Prepare(uint64_t RawBuffer, uint32_t Size) {
			m_BitFlag &= 0xFD;
			m_BitFlag |= 1;
			m_RawBuffer = RawBuffer;
			m_MaxBit = 8 * Size;
		}

		uint32_t GetPosition() {
			return m_NextBit;
		}

		bool SetPosition(uint32_t Bits) {
			if (Bits >= 0) {
				uint32_t Length = (m_BitFlag & 1) ? m_MaxBit : m_CurrentBit;

				if (Bits <= Length) {
					m_NextBit = Bits;
					return true;
				}
			}

			return false;
		}

		void AddOffset(uint32_t Offset) {
			m_NextBit += Offset;
		}

		size_t GetDataLength() {
			return (m_CurrentBit / 8) + (m_CurrentBit % 8) ? 1 : 0;
		}

		BitBufferSyncInstance CreateSyncReaderInstance();
		void UpdateBit(uint32_t LastSize);

		bool ReadBool(bool* Output);
		bool ReadInt(int* Output, uint32_t BitOffset);
		bool ReadUint(uint32_t* Output, uint32_t BitOffset);
		bool ReadULongLong(uint64_t* Output, uint32_t BitOffset);
		bool ReadShort(int16_t* Output, uint32_t BitOffset);
		void ReadVector3(BitBufferSyncInstance* Instance, Math::Vector3_<float>* Output, uint32_t Unk, uint32_t BitOffset);
		bool ReadIntArray(int* Output, uint32_t Count, uint32_t Unk);
		bool ReadNetMsgHeader(int* OutID);

		void WriteUint(uint32_t Input, uint32_t BitSize, uint32_t BitOffset);
	};

	struct BitBufferSyncInstance {
		void* m_VTable;
		uint8_t m_Type; // 1 = reader, 2 = writer, 3 = calculator, 4 = logger
		char _0x0009[0x7];
		uint64_t m_SubType; // 0 = reader, 2 = writer, 3 = calculator, 4 = logger
		BitBuffer* m_Buffer;
	};
}
#pragma pack(pop)