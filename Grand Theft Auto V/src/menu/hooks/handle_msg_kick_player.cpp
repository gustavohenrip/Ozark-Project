#include "hooks.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main.hpp"

TranslationString T_YouBeenKickedByHost("You've been kicked by the host. Prevented transition", true, true);
TranslationString T_YouBeenDesyncKicked("You've been desync kicked", true, true);

bool Menu::Hooks::HandleMsgKickPlayerHook(uint64_t Rcx, uint64_t Rdx) {
	OriginalHandleMsgKickPlayer(Rcx, Rdx); // Read the data

	uint32_t KickType = *(uint32_t*)(Rdx + 0x10);
	switch (KickType) {
		case 0:
		case 4: // Admin
			Menu::GetNotify()->NotifyStacked(TRANSLATE(T_YouBeenKickedByHost), Global::UIVars::g_NotificationError);
			break;

		case 1:
			Menu::GetNotify()->NotifyStacked(TRANSLATE(T_YouBeenDesyncKicked), Global::UIVars::g_NotificationError);
			break;

		default:
			LOG_CUSTOM_DEV("ReadMsgKickPlayer", "Unknown kick: %i", KickType);
			break;
	}

	return false;
}