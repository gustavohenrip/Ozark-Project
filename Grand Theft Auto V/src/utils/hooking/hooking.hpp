#pragma once
#include "stdafx.hpp"
#include "minhook.hpp"
#include "utils/log.hpp"
#include "utils/memory/memory.hpp"
#include "rage/types/script_types.hpp"

namespace Hook {
	class Hooking {
	public:
		template <typename T>
		bool Create(const char* Name, uint64_t Address, void* Function, T** Trampoline) {
			MH_STATUS InitStatus = MH_Initialize();
			if (InitStatus == MH_OK || InitStatus == MH_ERROR_ALREADY_INITIALIZED) {
				if (Address && Function) {
					uint8_t Backup[20];
					memcpy(Backup, (void*)Address, 20);

					auto status = MH_CreateHook((LPVOID)Address, Function, (void**)Trampoline);
					if ((status == MH_OK || status == MH_ERROR_ALREADY_CREATED) && MH_EnableHook((LPVOID)Address) == MH_OK) {
						DetourHooks Hook;
						Hook.m_EncryptionKey = (uint8_t)(rand() % 255);
						Hook.m_EncryptedName = Name;
						Hook.m_EncryptedAddress = Address;

						memcpy(Hook.m_Original, Backup, 20);
						memcpy(Hook.m_Hooked, (void*)Address, 20);

						for (std::size_t i = 0; i < Hook.m_EncryptedName.length(); i++) {
							Hook.m_EncryptedName[i] ^= Hook.m_EncryptionKey;
						}

						uint8_t* Bytes = (uint8_t*)&Hook.m_EncryptedAddress;
						for (int i = 0; i < 8; i++) {
							Bytes[i] ^= Hook.m_EncryptionKey;
						}

						m_Hooks.push_back(Hook);
#ifdef DEV
						LOG_CUSTOM_INFO("Hook"_Protect, "Hooked %s @ %llx"_Protect, Name, Address);
#else
						LOG_CUSTOM_INFO("Hook"_Protect, "Hooked %s"_Protect, Name);
#endif
						return true;
					}
				}
			}

			return false;
		}

		template <typename T>
		bool CreateVirtual(const char* Name, uint64_t Table, int Index, void* Function, T** Trampoline) {
			if (!Table || !Function) {
				LOG_DEV("Virtual hook failed - %s %llx %llx", Name, Table, Function);
				return false;
			}

			uint64_t Address = Table + (8 * Index);
			uint64_t Original = *(uint64_t*)Address;

			if (*((void**)Trampoline) == nullptr) {
				*((void**)Trampoline) = (void*)Original;
			}

			VMTHooks Hook;
			Hook.m_EncryptionKey = (BYTE)(rand() % 255);
			Hook.m_EncryptedName = Name;
			Hook.m_EncryptedAddress = Address;
			Hook.m_EncryptedOriginal = Original;

			for (std::size_t i = 0; i < Hook.m_EncryptedName.length(); i++) {
				Hook.m_EncryptedName[i] ^= Hook.m_EncryptionKey;
			}

			BYTE* Bytes = (BYTE*)&Hook.m_EncryptedAddress;
			for (int i = 0; i < 8; i++) {
				Bytes[i] ^= Hook.m_EncryptionKey;
			}

			Bytes = (BYTE*)&Hook.m_EncryptedOriginal;
			for (int i = 0; i < 8; i++) {
				Bytes[i] ^= Hook.m_EncryptionKey;
			}

			m_VirtualHooks.push_back(Hook);
			Memory::Write(Address, (uint64_t)Function);
			LOG_CUSTOM_INFO("Hook"_Protect, "Hooked %s"_Protect, Name);
			return true;
		}

		bool CreateScript(const char* Name, const char* ScriptName, uint64_t NativeHash, void* Function, int* Error = nullptr);
		void CreateScriptAll(uint64_t NativeHash, void* Function);
		bool Remove(void* Address);
		bool Disable(void* Address);
		bool GetOriginalBytes(std::string Name, uint64_t* Address, uint8_t* Buffer);
		bool GetHookedBytes(std::string Name, uint64_t* Address, uint8_t* Buffer);
		uint64_t GetHookAddress(std::string Name);
		void Unload();
	private:
		struct DetourHooks {
			uint8_t m_EncryptionKey;
			std::string m_EncryptedName;
			uint64_t m_EncryptedAddress;
			uint8_t m_Original[20];
			uint8_t m_Hooked[20];
		};

		struct VMTHooks {
			uint8_t m_EncryptionKey;
			std::string m_EncryptedName;
			uint64_t m_EncryptedAddress;
			uint64_t m_EncryptedOriginal;
		};

		struct ScriptHooks {
			const char* m_ScriptName;
			uint64_t m_OriginalFunction;
			int m_NativePosition;
		};

		std::vector<DetourHooks> m_Hooks;
		std::vector<VMTHooks> m_VirtualHooks;
		std::vector<ScriptHooks> m_ScriptHooks;
	};

	Hooking* GetHooking();
}