#include "hooks.hpp"
#include "utils/caller.hpp"
#include "rage/invoker/natives.hpp"
#include "utils/log.hpp"
#include "menu/base/util/player_manager.hpp"

uint64_t Menu::Hooks::sub_141358C18(int Index, uint64_t Override) {
	if (Override) {
		if (!IsValidPtr(Override)) {
			return 0;
		}
	}

	uint64_t Ptr = *(uint64_t*)(g_GraphicCrashTable + (Index * 8));
	if (!IsValidPtr(Ptr)) {
		return 0;
	}

	return Originalsub_141358C18(Index, Override);
}

uint64_t Menu::Hooks::sub_1415BBB74(uint64_t Rcx, uint64_t Rdx) {
	if (!IsValidPtr(Rcx)) {
		LOG_WARN("[Crash] 0x15BBB74 (1)");
		return 0;
	}

	if (*(uint64_t*)Rcx != Rdx) {
		if (*(uint64_t*)(Rcx + 8) != Rdx) {
			if (!IsValidPtr(*(uint64_t*)(Rdx + 0x18))) {
				LOG_WARN("[Crash] 0x15BBB74 (2)");
				return 0;
			}

			if (!IsValidPtr(*(uint64_t*)(Rdx + 0x20))) {
				LOG_WARN("[Crash] 0x15BBB74 (3)");
				return 0;
			}
		} else {
			if (!IsValidPtr(*(uint64_t*)(Rcx + 8))) {
				LOG_WARN("[Crash] 0x15BBB74 (4)");
				return 0;
			}
		}
	}

	return Originalsub_1415BBB74(Rcx, Rdx);
}

void Menu::Hooks::ScriptWorldStateCrashHook(uint64_t Rcx, uint32_t Edx, uint32_t R8d) {
	if (!IsValidPtr(Rcx)) {
		LOG_WARN("[Crash] 0xSWSC (1)");
		return;
	}

	return OriginalScriptWorldStateCrash(Rcx, Edx, R8d);
}

uint64_t Menu::Hooks::sub_14113A75C(uint64_t Rcx, uint64_t Rdx) {
	if (*(BYTE*)(Rdx + 281)) {
		auto v2 = *(__int64*)(Rcx - 432);
		auto t = *(uint64_t*)v2;
		if (!IsValidPtr(*(uint64_t*)(t + 0x620))) {
			LOG_WARN("[Crash] 0x113A75C (1)");
			return 0;
		}

		uint64_t addy = Caller::Call<uint64_t>(*(uint64_t*)(t + 0x620), v2);

		if (!IsValidPtr(addy)) {
			LOG_WARN("[Crash] 0x113A75C (2)");
			return 0;
		}
	}

	return Originalsub_14113A75C(Rcx, Rdx);
}

uint64_t sub_14162E398(uint64_t** Rcx, uint32_t Rdx) {
	unsigned int v2; // edi
	int v3; // eax
	uint64_t* i; // rbx
	uint64_t v5; // rsi

	v2 = Rdx;
	if (Rdx >= 0x20 || (v3 = *((unsigned int*)Rcx + ((unsigned __int64)Rdx >> 5) + 2), _bittest((LONG*)&v3, Rdx & 0x1F))) {
		for (i = *Rcx; i; i = (uint64_t*)i[1]) {
			v5 = *i;
			if ((*(unsigned int(**)(uint64_t))(*(uint64_t*)*i + 24))(*i) == v2)
				return v5;
		}
	}
	return 0;
}

void Menu::Hooks::sub_140A1BDF4(uint64_t Rcx, uint64_t Rdx) {
	if (!IsValidPtr(Rdx)) {
		LOG_WARN("[Crash] 0xA1BDF4 (1)");
		return;
	}

	uint64_t RdxVirtualTable = *(uint64_t*)Rdx;
	uint64_t Result = Caller::Call<uint64_t>(*(uint64_t*)(RdxVirtualTable + 0x80), Rdx);
	if (IsValidPtr(Result)) {
		Result = sub_14162E398((uint64_t**)(Result + 0x10), 8);
		if (IsValidPtr(Result)) {
			uint64_t Pointer = Result + 8;
			uint64_t Virtual = *(uint64_t*)(Pointer);

			Result = Caller::Call<uint64_t>(*(uint64_t*)(Virtual + 0x28), Pointer);
			if (!IsValidPtr(Result)) {
				LOG_WARN("[Crash] 0xA1BDF4 (2)");
				return;
			}
		}
	}

	return Originalsub_140A1BDF4(Rcx, Rdx);
}

char m_Stack[0xFFFF];

eThreadState Menu::Hooks::ScriptThreadTickHook(Rage::Script::GtaThread* Thread, uint32_t Opcodes) {
	eThreadState State = Thread->m_Context.m_State;

	static uint8_t ThreadBackup[sizeof(Rage::Script::GtaThread)];
	static float Timers[3];
	static uint8_t Handler[sizeof(Rage::Script::GameScriptHandler)];

	if (State == STATE_RUNNING && !Global::Vars::g_FreemodeTerminated && Thread->m_ScriptHash == 0xc875557d) {
		memcpy(ThreadBackup, Thread, sizeof(Rage::Script::GtaThread));
		memcpy(m_Stack, Thread->m_Stack, Thread->m_Context.m_StackSize);

		if (Thread->m_Handler) {
			memcpy(Handler, Thread->m_Handler, sizeof(Rage::Script::GameScriptHandler));
		}
	}

	eThreadState ResultState = OriginalScriptThreadTick(Thread, Opcodes);

	if (ResultState == STATE_KILLED && State == STATE_RUNNING && Thread->m_ScriptHash == 0xc875557d) {
		if (!Global::Vars::g_FreemodeTerminated) {
			memcpy(Timers, &Thread->m_Context.m_TimerA, sizeof(Timers));

			ResultState = STATE_RUNNING;
			memcpy(Thread, ThreadBackup, sizeof(ThreadBackup));

			if (Thread->m_Stack) {
				memcpy(Thread->m_Stack, m_Stack, Thread->m_Context.m_StackSize);
			}

			if (Thread->m_Handler) {
				memcpy(Thread->m_Handler, Handler, sizeof(Handler));
			}

			memcpy(&Thread->m_Context.m_TimerA, Timers, sizeof(Timers));
		} else {
			Global::Vars::g_FreemodeTerminated = false;
		}
	}

	return ResultState;
}

void Menu::Hooks::TerminateThisThreadHook(Rage::Invoker::NativeContext* Context) {
	Global::Vars::g_FreemodeTerminated = true;
	Native::TerminateThisThread();
}

uint64_t Menu::Hooks::sub_140E72620(uint64_t Rcx) {
	uint64_t Ptr = *(uint64_t*)(Rcx + 0x10);
	if (Ptr) {
		Ptr = *(uint64_t*)(Ptr + 0xBD0);
		if (Ptr) {
			Ptr = *(uint64_t*)(Ptr + 0x710);
			if (Ptr) {
				// og
				return Originalsub_140E72620(Rcx);
			}
		}
	}

	LOG_WARN("[Crash] 0xE72620 (1)");
	return 0;
}

void Menu::Hooks::sub_140E606D8(uint64_t Rcx, uint64_t Rdx) {
	uint64_t Ptr = *(uint64_t*)(Rdx + 0xBD0);
	if (Ptr) {
		Ptr = *(uint64_t*)(Ptr + 0x710);
		if (Ptr) {
			// og
			return Originalsub_140E606D8(Rcx, Rdx);
		}
	}

	LOG_WARN("[Crash] 0xE606D8 (1)");
}

bool Menu::Hooks::sub_140AEA62C(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint32_t R9d, uint64_t XMM6) {
	if (R8 == 0) {
		LOG_WARN("[Crash] 0xAEA62C (1)");
		return false;
	}

	return Originalsub_140AEA62C(Rcx, Rdx, R8, R9d, XMM6);
}

uint64_t Menu::Hooks::sub_140AFB59C(uint64_t Rcx, int Edx, int R8d) {
	if (!IsValidPtr(*(uint64_t*)(Rcx + 0x100))) {
		LOG_WARN("[Crash] 0xAFB59C (1)");
		return 0;
	}

	return Originalsub_140AFB59C(Rcx, Edx, R8d);
}

void Menu::Hooks::sub_140E8407C(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	if (!IsValidPtr(Rdx)) {
		LOG_WARN("[Crash] 0xE8407C (1)");
		return;
	}

	uint64_t Table = *(uint64_t*)Rdx;
	uint64_t Ptr = Caller::Call<uint64_t>(*(uint64_t*)(Table + 0x620), Rdx);

	if (!IsValidPtr(Ptr)) {
		LOG_WARN("[Crash] 0xE8407C (2)");
		return;
	}

	Originalsub_140E8407C(Rcx, Rdx, R8, R9);
}

uint64_t Menu::Hooks::sub_140730894(uint64_t Rcx, uint64_t Rdx) {
	if (!IsValidPtr(Rdx)) {
		LOG_WARN("[Crash] 0x730894 (1)");
		return 0;
	}

	return Originalsub_140730894(Rcx, Rdx);
}

uint64_t Menu::Hooks::AccessNetObjHook(uint16_t* Rcx) {
	auto ret = _ReturnAddress();

	if (ret == (void*)g_2Take1CrashLocation) {
		if (!Rcx) {
			LOG_DEV("2T1 Crash");
			return 0;
		}

		auto Return = OriginalAccessNetObj(Rcx);

		if (!Return) {
			return Return;
		}

		if (!*(uint64_t*)(Return + 72)) {
			return 0;
		}

		return Return;
	}

	return OriginalAccessNetObj(Rcx);
}

uint64_t Menu::Hooks::CutscenePedWriteHook(bool Reset) {
	Rage::Types::PedFactory* Factory = Global::Vars::g_PedFactory;
	if (IsValidPtr(Factory)) {
		Rage::Types::Ped* Ped = Factory->m_LocalPed;
		if (IsValidPtr(Ped)) {
			uint64_t UnkPtr = *(uint64_t*)((uint64_t)Ped + 0x10c8);
			if (IsValidPtr(UnkPtr)) {
				return OriginalCutscenePedWrite(Reset);
			}
		}
	}

	LOG_WARN("[Crash] 0xCSPW (1)");
	return 0;
}

uint64_t Menu::Hooks::sub_140632DD4(uint64_t Rcx, uint64_t Rdx, uint32_t R8d, uint32_t R9d, uint32_t Stack_28, bool Stack_20, bool Stack_18) {
	if (!IsValidPtr(Rcx)) {
		LOG_WARN("[Crash] 0x632DD4 (1)");
		return 0;
	}

	return Originalsub_140632DD4(Rcx, Rdx, R8d, R9d, Stack_28, Stack_20, Stack_18);
}

bool Menu::Hooks::sub_140E7C4E8(uint64_t Rcx, uint64_t Rdx, uint64_t R8d, uint32_t R9d, uint64_t Stack_210, uint64_t Stack_208, uint64_t Stack_200, uint64_t Stack_1F8, bool Stack_1F0, bool Stack_1E8) {
	if (!IsValidPtr(Rcx)) {
		LOG_WARN("[Crash] 0xE7C4E8 (1)");
		return false;
	}

	if (!IsValidPtr(*(uint64_t*)Rcx)) {
		LOG_WARN("[Crash] 0xE7C4E8 (2)");
		return false;
	}

	return Originalsub_140E7C4E8(Rcx, Rdx, R8d, R9d, Stack_210, Stack_208, Stack_200, Stack_1F8, Stack_1F0, Stack_1E8);
}

void Menu::Hooks::TrailerDetachmentCrashHook(uint64_t Trailer, uint64_t Flag) {
	uint64_t Entity = *(uint64_t*)(Trailer + 0x50);
	if (Entity) {
		uint64_t EntityDrawHandler = *(uint64_t*)(Entity + 0x48);
		if (EntityDrawHandler) {
			if ((*(uint32_t*)(EntityDrawHandler + 0x5C) & 0xF) >= 2) {
				uint64_t Info = *(uint64_t*)EntityDrawHandler;
				if (Info) {
					if (*(uint16_t*)(Info + 0xC50) > 0) {
						if (!IsValidPtr(*(uint64_t*)(Info + 0xC48))) {
							*(uint16_t*)(Info + 0xC50) = 0;
							LOG_DEV("Crash 19");
							return;
						}
					}
				}
			}
		}
	}

	return OriginalTrailerDetachmentCrash(Trailer, Flag);
}

void Menu::Hooks::UpdateScriptForCloneHook(uint64_t Rcx, Rage::Network::NetObj* Rdx, uint32_t R8d, uint8_t R9b) {
	if (IsValidPtr(Rdx)) {
		if (IsValidPtr(Rdx->m_VTable)) {
			return OriginalUpdateScriptForClone(Rcx, Rdx, R8d, R9b);
		}
	}

	LOG_WARN("[Crash] 0xUSFC (1)");
}

void Menu::Hooks::sub_141644B58(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
	if (!IsValidPtr(Rcx)) {
		LOG_WARN("[Crash] 0x1644B58 (1)");
		return;
	}

	return Originalsub_141644B58(Rcx, Rdx, R8);
}

uint64_t Menu::Hooks::sub_1411175B0(uint64_t Rcx) {
	uint64_t Address = *(uint64_t*)(Rcx + 0x2B0);
	if (Address) {
		uint64_t VTable = *(uint64_t*)Address;
		if (VTable > Global::Vars::g_GameAddress && VTable < (Global::Vars::g_GameAddress + Global::Vars::g_GameSize)) {
			return Originalsub_1411175B0(Rcx);
		}
	}

	LOG_WARN("[Crash] 0x11175B0 (1)");
	return 0;
}

uint64_t Menu::Hooks::RockstarIDWriterHook(uint64_t Rcx, uint64_t Rdx) {
	if (Rdx) {
		uint64_t RockstarID = *(uint64_t*)(Rdx + 0x50);
		if (RockstarID == 0) {
			*(uint64_t*)(Rdx + 0x50) = 212121;
		}
	}

	return OriginalRockstarIDWriter(Rcx, Rdx);
}