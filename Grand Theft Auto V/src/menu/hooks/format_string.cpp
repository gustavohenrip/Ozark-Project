#include "hooks.hpp"
#include "global/ui_vars.hpp"
#include "menu/base/util/notify.hpp"

uint64_t Menu::Hooks::FormatGameEngineStringHook(uint64_t Rcx, uint64_t Rdx, uint32_t R8d, const char* R9, uint32_t Stack) {
	if (R9) {
		if (strlen(R9) >= 98) {
			if (!Global::UIVars::g_RenderingTooltip.empty()) {
				if (strstr(Global::UIVars::g_RenderingTooltip.c_str(), R9)) {
					return OriginalFormatGameEngineString(Rcx, Rdx, R8d, Global::UIVars::g_RenderingTooltip.c_str(), Stack);
				}
			}

			auto Contexts = Menu::GetNotify()->GetTextContext();
			if (!Contexts.empty()) {
				for (auto& Text : Contexts) {
					if (strstr(Text.c_str(), R9)) {
						return OriginalFormatGameEngineString(Rcx, Rdx, R8d, Text.c_str(), Stack);
					}
				}
			}
		}
	}

	return OriginalFormatGameEngineString(Rcx, Rdx, R8d, R9, Stack);
}

int Menu::Hooks::GetGameEngineStringLineCountHook(uint64_t Rcx, const char* Rdx, float Xmm2, uint64_t R9, bool Stack) {
	if (Rdx) {
		if (strlen(Rdx) >= 98) {
			if (!Global::UIVars::g_RenderingTooltip.empty()) {
				if (strstr(Global::UIVars::g_RenderingTooltip.c_str(), Rdx)) {
					return OriginalGetGameEngineStringLineCount(Rcx, Global::UIVars::g_RenderingTooltip.c_str(), Xmm2, R9, Stack);
				}
			}

			auto Contexts = Menu::GetNotify()->GetTextContext();
			if (!Contexts.empty()) {
				for (auto& Text : Contexts) {
					if (strstr(Text.c_str(), Rdx)) {
						return OriginalGetGameEngineStringLineCount(Rcx, Text.c_str(), Xmm2, R9, Stack);
					}
				}
			}
		}
	}

	return OriginalGetGameEngineStringLineCount(Rcx, Rdx, Xmm2, R9, Stack);
}