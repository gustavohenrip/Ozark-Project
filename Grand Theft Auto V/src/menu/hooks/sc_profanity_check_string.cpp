#include "hooks.hpp"
#include "menu/base/submenus/main/misc/misc_disables.hpp"

enum eProfanity {
	PROFANITY_CHAT_CLEAN = -1,
	PROFANITY_CLEAN,
	PROFANITY_DIRTY,
	PROFANITY_UNK,
	PROFANITY_ERROR
};

int Menu::Hooks::ScProfanityCheckStringStatusHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
	if (Global::Vars::g_BypassProfanityChecks) {
		Global::Vars::g_BypassProfanityChecks = false;
		return OriginalScProfanityCheckStringStatus(Rcx, Rdx, R8);
	}

	if (MiscDisablesMenuVars::m_Vars.m_DisableProfanityFilter) {
		return PROFANITY_CLEAN;
	}

	return OriginalScProfanityCheckStringStatus(Rcx, Rdx, R8);
}

int Menu::Hooks::GetChatProfanityStatusHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8) {
	if (MiscDisablesMenuVars::m_Vars.m_DisableProfanityFilter) {
		return PROFANITY_CHAT_CLEAN;
	}

	return OriginalGetChatProfanityStatus(Rcx, Rdx, R8);
}