#include "hooking.hpp"
#include "global/variables.hpp"
#include "rage/invoker/native_invoker.hpp"
#include "utils/caller.hpp"

namespace Hook {
	bool Hooking::Remove(void* Address) {
		return MH_RemoveHook(Address) == MH_OK;
	}

	bool Hooking::Disable(void* Address) {
		return MH_DisableHook(Address) == MH_OK;
	}

	bool Hooking::GetOriginalBytes(std::string Name, uint64_t* Address, uint8_t* Buffer) {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			if (!Name.compare(Name)) {
				uint64_t LocalEnc = Hook.m_EncryptedAddress;
				uint8_t* Bytes = (uint8_t*)&LocalEnc;
				for (int i = 0; i < 8; i++) {
					Bytes[i] ^= Hook.m_EncryptionKey;
				}

				*Address = LocalEnc;
				memcpy(Buffer, Hook.m_Original, 20);
				return true;
			}
		}

		return false;
	}

	bool Hooking::GetHookedBytes(std::string Name, uint64_t* Address, uint8_t* Buffer) {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			if (!Name.compare(Name)) {
				uint64_t LocalEnc = Hook.m_EncryptedAddress;
				uint8_t* Bytes = (uint8_t*)&LocalEnc;
				for (int i = 0; i < 8; i++) {
					Bytes[i] ^= Hook.m_EncryptionKey;
				}

				*Address = LocalEnc;
				memcpy(Buffer, Hook.m_Hooked, 20);
				return true;
			}
		}

		return false;
	}

	uint64_t Hooking::GetHookAddress(std::string Name) {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			if (!Name.compare(Name)) {
				uint64_t LocalEnc = Hook.m_EncryptedAddress;
				uint8_t* Bytes = (uint8_t*)&LocalEnc;
				for (int i = 0; i < 8; i++) {
					Bytes[i] ^= Hook.m_EncryptionKey;
				}

				return LocalEnc;
			}
		}

		return 0;
	}

	void Hooking::Unload() {
		for (auto& Hook : m_Hooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			LOG_CUSTOM_INFO("Hook"_Protect, "Unhooked %s"_Protect, Name.c_str());

			uint8_t* Bytes = (uint8_t*)&Hook.m_EncryptedAddress;
			for (int i = 0; i < 8; i++) {
				Bytes[i] ^= Hook.m_EncryptionKey;
			}

			MH_QueueDisableHook((void*)Hook.m_EncryptedAddress);
		}

		MH_ApplyQueued();
		MH_Uninitialize();

		for (auto& Hook : m_VirtualHooks) {
			std::string Name = Hook.m_EncryptedName;
			for (std::size_t i = 0; i < Name.length(); i++) {
				Name[i] ^= Hook.m_EncryptionKey;
			}

			uint8_t* Bytes = (uint8_t*)&Hook.m_EncryptedAddress;
			for (int i = 0; i < 8; i++) {
				Bytes[i] ^= Hook.m_EncryptionKey;
			}

			Bytes = (uint8_t*)&Hook.m_EncryptedOriginal;
			for (int i = 0; i < 8; i++) {
				Bytes[i] ^= Hook.m_EncryptionKey;
			}

			LOG_CUSTOM_INFO("Hook"_Protect, "Unhooked %s"_Protect, Name.c_str());
			Memory::Write(Hook.m_EncryptedAddress, Hook.m_EncryptedOriginal);
		}

		for (auto& Script : m_ScriptHooks) {
			if (IsValidPtr(Global::Vars::g_GameStreamedScripts)) {
				uint64_t Table = *(uint64_t*)Global::Vars::g_GameStreamedScripts;

				int Index = 0;
				Caller::Call<int>(Global::Vars::g_GetStreamedScript, Global::Vars::g_GameStreamedScripts, &Index, Script.m_ScriptName);

				if (Index != -1) {
					Rage::Script::ScriptHeader* Program = Caller::Call<Rage::Script::ScriptHeader*>(*(uint64_t*)(Table + 0x68), Global::Vars::g_GameStreamedScripts, Index);
					if (Program) {
						Program->m_Natives[Script.m_NativePosition] = Script.m_OriginalFunction;
						LOG_CUSTOM_INFO("Hook"_Protect, "Unhooked function in %s"_Protect, Script.m_ScriptName);
					}
				}
			}
		}
	}

	bool Hooking::CreateScript(const char* Name, const char* ScriptName, uint64_t NativeHash, void* Function, int* Error) {
		if (IsValidPtr(Global::Vars::g_GameStreamedScripts)) {
			uint64_t NativeFunction = (uint64_t)Rage::Invoker::GetNativeHandler(NativeHash);
			if (IsValidPtr(NativeFunction)) {
				uint64_t Table = *(uint64_t*)Global::Vars::g_GameStreamedScripts;

				int Index = 0;
				Caller::Call<int>(Global::Vars::g_GetStreamedScript, Global::Vars::g_GameStreamedScripts, &Index, ScriptName);

				if (Index != -1) {
					Rage::Script::ScriptHeader* Program = Caller::Call<Rage::Script::ScriptHeader*>(*(uint64_t*)(Table + 0x68), Global::Vars::g_GameStreamedScripts, Index);
					if (Program) {
						for (int j = 0; j < Program->m_NativeCount; j++) {
							if (Program->m_Natives[j] == NativeFunction) {
								m_ScriptHooks.push_back({ ScriptName, Program->m_Natives[j], j });
								Program->m_Natives[j] = (uint64_t)Function;
								LOG_CUSTOM_INFO("Hook"_Protect, "Hooked %s"_Protect, Name);
								return true;
							}
						}
					} else { if (Error) *Error = 4; }
				} else { if (Error) *Error = 3; }
			} else { if (Error) *Error = 2; }
		} else { if (Error) *Error = 1; }

		return false;
	}

	void Hooking::CreateScriptAll(uint64_t NativeHash, void* Function) {
		for (int i = 0; i < Global::Vars::g_ScriptTable->m_Count; i++) {
			auto Header = Global::Vars::g_ScriptTable->m_Table[i].m_Header;
			if (Header) {
				for (int j = 0; j < Header->m_NativeCount; j++) {
					if (Header->m_Natives[j] == NativeHash) {
						Header->m_Natives[j] = (uint64_t)Function;
					}
				}
			}
		}
	}

	Hooking* GetHooking() {
		static Hooking Instance;
		return &Instance;
	}
}