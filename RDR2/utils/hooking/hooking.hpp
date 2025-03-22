#pragma once
#include "stdafx.hpp"
#include "minhook.hpp"
#include "utils/log.hpp"
#include "global/variables.hpp"
#include "rage/types/base_types.hpp"
#include "rage/types/script_types.hpp"
#include "rage/invoker/native_invoker.hpp"

namespace hooking {
	class hooking {
	public:
		bool isHooking = true;

		template <typename T>
		bool create(const char* name, uint64_t address, void* function, T** trampoline) {
			auto initStatus = MH_Initialize(); // if it's already initialized it returns MH_ERROR_ALREADY_INITIALIZED
			if (initStatus == MH_OK || initStatus == MH_ERROR_ALREADY_INITIALIZED) {
				if (address && function) {
					auto status = MH_CreateHook((LPVOID)address, function, (void**)trampoline);
					bool enableHook = MH_EnableHook((LPVOID)address) == MH_OK;
					if ((status == MH_OK || status == MH_ERROR_ALREADY_CREATED) && enableHook) {
						detourHooks h;
						h.encryptionKey = (BYTE)(rand() % 255);
						h.encryptedName = name;
						h.encryptedAddress = address;

						for (std::size_t i = 0; i < h.encryptedName.length(); i++) {
							h.encryptedName[i] ^= h.encryptionKey;
						}

						BYTE* bytes = (BYTE*)&h.encryptedAddress;
						for (int i = 0; i < 8; i++) {
							bytes[i] ^= h.encryptionKey;
						}

						hooks.push_back(h);
						customInfo("Hook"_Protect, "Hooked %s"_Protect, name);
						return true;
					} else {
					}
				} else {
				}
			} else {
			}

			return false;
		}

		bool remove(void* address) {
			return MH_RemoveHook(address) == MH_OK;
		}

		bool disable(void* address) {
			return MH_DisableHook(address) == MH_OK;
		}

		void unload() {
			for (auto& hook : hooks) {
				std::string name = hook.encryptedName;
				for (std::size_t i = 0; i < name.length(); i++) {
					name[i] ^= hook.encryptionKey;
				}

				customInfo("Hook"_Protect, "Unhooked %s"_Protect, name.c_str());

				BYTE* bytes = (BYTE*)&hook.encryptedAddress;
				for (int i = 0; i < 8; i++) {
					bytes[i] ^= hook.encryptionKey;
				}

				MH_QueueDisableHook((void*)hook.encryptedAddress);
			}

			MH_ApplyQueued();
			MH_Uninitialize();

			for (auto& vhook : virtualHooks) {
				std::string name = vhook.encryptedName;
				for (std::size_t i = 0; i < name.length(); i++) {
					name[i] ^= vhook.encryptionKey;
				}

				BYTE* bytes = (BYTE*)&vhook.encryptedAddress;
				for (int i = 0; i < 8; i++) {
					bytes[i] ^= vhook.encryptionKey;
				}

				bytes = (BYTE*)&vhook.encryptedOriginal;
				for (int i = 0; i < 8; i++) {
					bytes[i] ^= vhook.encryptionKey;
				}

				customInfo("Hook"_Protect, "Unhooked %s"_Protect, name.c_str());
				memory::getMemory()->write(vhook.encryptedAddress, vhook.encryptedOriginal);
			}

			for (auto& script : scriptHook) {
				if (script.script->header != nullptr) {
					if (script.script->header->natives[script.nativePosition] != 0) {
						if (script.originalFunction != 0) {
							script.script->header->natives[script.nativePosition] = script.originalFunction;
						}
					}
				}
			}
		}

		template <typename T>
		bool createVirtual(const char* name, uint64_t table, int index, void* function, T** trampoline) {
			if (!table || !function) return false;

			uint64_t address = table + (8 * index);
			uint64_t original = memory::getMemory()->read<uint64_t, uint64_t>(address);

			*((void**)trampoline) = (void*)original;

			vmtHooks h;
			h.encryptionKey = (BYTE)(rand() % 255);
			h.encryptedName = name;
			h.encryptedAddress = address;
			h.encryptedOriginal = original;

			for (std::size_t i = 0; i < h.encryptedName.length(); i++) {
				h.encryptedName[i] ^= h.encryptionKey;
			}

			BYTE* bytes = (BYTE*)&h.encryptedAddress;
			for (int i = 0; i < 8; i++) {
				bytes[i] ^= h.encryptionKey;
			}

			bytes = (BYTE*)&h.encryptedOriginal;
			for (int i = 0; i < 8; i++) {
				bytes[i] ^= h.encryptionKey;
			}

			virtualHooks.push_back(h);
			memory::getMemory()->write(address, (uint64_t)function);
			customInfo("Hook"_Protect, "Hooked %s"_Protect, name);
			return true;
		}

		bool createScript(const char* name, Hash scriptNameHash, uint64_t nativeHash, void* function) {
			auto script = global::vars::g_scriptTable->find(scriptNameHash);
			if (script) {
				if (script->header) {
					uint64_t nativeFunction = (uint64_t)rage::invoker::getNativeHandler(nativeHash);
					if (nativeFunction) {
						for (uint32_t i = 0; i < script->header->nativeCount; i++) {
							if (script->header->natives[i] == nativeFunction) {
								script->header->natives[i] = (uint64_t)function;
								customInfo("Hook"_Protect, "Hooked %s"_Protect, name);
								return true;
							}
						}
					}
				}
			}

			return false;
		}

		void createScriptAll(uint64_t nativeHash, void* function) {
			if (nativeHash) {
				if (global::vars::g_scriptTable) {
					for (int i = 0; i < global::vars::g_scriptTable->count; i++) {
						auto header = global::vars::g_scriptTable->table[i].header;
						if (header) {
							for (uint32_t j = 0; j < header->nativeCount; j++) {
								if (header->natives[j] == nativeHash) {
									scriptHook.push_back({ &global::vars::g_scriptTable->table[i], header->natives[j], j });
									header->natives[j] = (uint64_t)function;
								}
							}
						}
					}
				}
			}
		}
	private:
		struct detourHooks {
			BYTE encryptionKey;
			std::string encryptedName;
			uint64_t encryptedAddress;
		};

		struct vmtHooks {
			BYTE encryptionKey;
			std::string encryptedName;
			uint64_t encryptedAddress;
			uint64_t encryptedOriginal;
		};

		struct scriptHooks {
			rage::script::scriptTableItem* script;
			uint64_t originalFunction;
			uint32_t nativePosition;
		};

		std::vector<detourHooks> hooks;
		std::vector<vmtHooks> virtualHooks;
		std::vector<scriptHooks> scriptHook;
	};
	
	static hooking* getHooking() {
		static hooking instance;
		return &instance;
	}
}