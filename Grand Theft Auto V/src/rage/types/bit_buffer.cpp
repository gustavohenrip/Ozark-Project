#include "bit_buffer.hpp"
#include "rage/engine.hpp"
#include "utils/caller.hpp"
#include "global/variables.hpp"

namespace Rage {
	BitBufferSyncInstance BitBuffer::CreateSyncReaderInstance() {
		BitBufferSyncInstance Instance;
		Instance.m_VTable = (void*)Global::Vars::g_SyncDataReaderVtable;
		Instance.m_Type = 1;
		Instance.m_SubType = 0;
		Instance.m_Buffer = this;
		return Instance;
	}

	void BitBuffer::UpdateBit(uint32_t LastSize) {
		m_NextBit += LastSize;

		if (m_NextBit > m_CurrentBit) {
			m_CurrentBit = m_NextBit;
		}
	}

	bool BitBuffer::ReadBool(bool* Output) {
		return Caller::Call<bool>(Global::Vars::g_ReadBoolFromBitBuffer, this, Output);
	}

	bool BitBuffer::ReadInt(int* Output, uint32_t BitOffset) {
		return Caller::Call<bool>(Global::Vars::g_ReadIntFromBitBuffer, this, Output, BitOffset);
	}

	bool BitBuffer::ReadUint(uint32_t* Output, uint32_t BitOffset) {
		return Caller::Call<bool>(Global::Vars::g_ReadUnsignedIntFromBitBuffer, this, Output, BitOffset);
	}

	bool BitBuffer::ReadULongLong(uint64_t* Output, uint32_t BitOffset) {
		return Caller::Call<bool>(Global::Vars::g_ReadUnsignedLongLongBuffer, this, Output, BitOffset);
	}

	bool BitBuffer::ReadShort(int16_t* Output, uint32_t BitOffset) {
		return Caller::Call<bool>(Global::Vars::g_ReadShortFromBitBuffer, this, Output, BitOffset);
	}

	void BitBuffer::ReadVector3(BitBufferSyncInstance* Instance, Math::Vector3_<float>* Output, uint32_t Unk, uint32_t BitOffset) {
		Caller::Call<int>(Global::Vars::g_ReadVector3FromBitBuffer, Instance, Output, Unk, BitOffset);
	}

	bool BitBuffer::ReadIntArray(int* Output, uint32_t Count, uint32_t Unk) {
		return Caller::Call<bool>(Global::Vars::g_ReadUintArrayFromBitBuffer, this, Output, Count, Unk);
	}

	bool BitBuffer::ReadNetMsgHeader(int* OutID) {
		return Caller::Call<bool>(Global::Vars::g_ReadNetMsgHeaderFromBitBuffer, OutID, this);
	}

	void BitBuffer::WriteUint(uint32_t Input, uint32_t BitSize, uint32_t BitOffset) {
		Caller::Call<int>(Global::Vars::g_WriteUintToBitBuffer, this, Input, BitSize, BitOffset);
		UpdateBit(BitSize);
	}
}