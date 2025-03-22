#include "hooks.hpp"
#include "utils/caller.hpp"

bool Menu::Hooks::RenderScriptTexturesHook(uint64_t Table, uint32_t Edx, uint32_t R8d) {
	uint32_t Count = *(uint32_t*)(Table + 0x9C40);
	if (Count) {
		for (uint32_t i = 0; i < (Count < 500 ? Count : 500); i++) {
			Caller::Call<int>(Global::Vars::g_GameAddress + 0xA2B174, Table + (i * 0x50), Edx, R8d);
		}

		if (Count >= 500) {
			for (uint32_t i = 500; i < Count; i++) {
				Caller::Call<int>(Global::Vars::g_GameAddress + 0xA2B174, Global::Vars::g_TextureBypass + ((i - 500) * 0x50), Edx, R8d);
			}
		}
	}

	return true;
}

uint64_t Menu::Hooks::GetRenderTextureHook(uint64_t Table) {
	uint32_t Count = *(uint32_t*)(Table + 0x9C40);
	if (Count >= 500 && Count < 1500) {
		*(uint32_t*)(Table + 0x9C40) = Count + 1;
		return Global::Vars::g_TextureBypass + ((Count - 500) * 0x50);
	}

	return OriginalGetRenderTexture(Table);
}