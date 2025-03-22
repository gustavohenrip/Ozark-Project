#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"

namespace Rage::Script {
	struct ScriptHeader {
		char _0x0000[0x10];
		unsigned char** m_CodeBlocksOffset;
		char _0x0018[0x4];
		int m_CodeLength;
		char _0x0020[0x4];
		int m_LocalCount;
		char _0x0028[0x4];
		int m_NativeCount;
		uint64_t* m_LocalOffset;
		char _0x0038[0x8];
		uint64_t* m_Natives;
		char _0x0048[0x10];
		uint32_t m_NameHash;
		char _0x005C[0x4];
		const char* m_Name;
		char** m_StringsOffset;
		int m_StringSize;
		char _0x0074[0xC];

		const char* GetString(int StringPosition) const {
			return StringPosition < 0 || StringPosition >= m_StringSize ? NULL : &m_StringsOffset[StringPosition >> 14][StringPosition & 0x3FFF];
		}
	};

	struct ScriptTableItem {
		ScriptHeader* m_Header;
		char _0x0008[0x4];
		uint32_t m_ScriptHash;
	};

	struct ScriptTable {
		ScriptTableItem* m_Table;
		char _0x0008[0x10];
		int m_Count;

		ScriptTableItem* Find(uint32_t ScriptHash) {
			if (m_Table == nullptr) {
				return nullptr;
			}

			for (int i = 0; i < m_Count; i++) {
				if (m_Table[i].m_ScriptHash == ScriptHash) {
					return &m_Table[i];
				}
			}

			return nullptr;
		}
	};

	struct GameScriptHandler;
	struct GameScriptHandlerNetworkComponent;

	struct ScriptThreadContext {
		uint32_t m_ThreadID;
		uint32_t m_ScriptHash;
		eThreadState m_State;
		uint32_t m_InstructionPointer;
		uint32_t m_FramePointer;
		uint32_t m_StackPointer;
		float m_TimerA;
		float m_TimerB;
		float m_TimerC;
		char _0x0024[0x2C];
		uint32_t m_StackSize;
		char _0x0054[0x54];
	};

	struct ScriptThread {
		uint64_t m_VTable;
		ScriptThreadContext m_Context;
		void* m_Stack;
		char _0x00B8[0x10];
		const char* m_ExitMessage;
		char m_Name[0x40];
		GameScriptHandler* m_Handler;
		GameScriptHandlerNetworkComponent* m_NetComponent;
	};

	struct GtaThread : ScriptThread {
		uint32_t m_ScriptHash;
		char _0x0124[0x14];
		uint32_t m_InstanceID;
		char _0x013C[0x4];
		uint8_t m_Flag;
		bool m_SafeForNetwork;
		char _0x0142[0x2];
		bool m_IsMinigame;
		char _0x0145[0x2];
		bool m_CanBePaused;
		bool m_CanRemoveBlips;
		char _0x0149[0xF];
	};

	struct GameScriptId {
		char _0x0000[0x8];
		uint32_t m_Hash;
		char m_Name[0x20];
		char _0x002C[0x4];
		uint32_t m_Timestamp;
		uint32_t m_ScriptId;
		uint32_t m_InstanceId;
		uint32_t m_InstanceHash;
		char _0x0040[0x10];
	};

	struct GameScriptHandlerNetworkComponent {
		char _0x0000[0x8];
		GameScriptHandler* m_Component;
		int m_State;
		int m_PendingPlayerJoinBitset;
	};

	struct GameScriptHandler {
		char _0x0000[0x18];
		GtaThread* m_Thread;
		char _0x0020[0x28];
		GameScriptHandlerNetworkComponent* m_NetComponent;
		char _0x0050[0x10];
		GameScriptId m_ScriptId;
	};
}