#include "hooks.hpp"
#include "menu/base/submenus/main/network/network_global_chat.hpp"

void Menu::Hooks::SendGameMessageHook(uint64_t Manager, int PlayerKey, Rage::Network::GlobalMsgContext* Context, uint64_t Unk1, uint64_t Unk2) {
	Context->m_SenderChatToken = NetworkGlobalChatMenuVars::GetSenderChatToken(Context->m_SenderChatToken);

	Player Recv = NetworkGlobalChatMenuVars::GetPlayerIDFromPlayerKey(PlayerKey);
	if (Recv != -1) {
		if (!NetworkGlobalChatMenuVars::CanSendToPlayer(Recv)) {
			return;
		}
	}

	return OriginalSendGameMessage(Manager, PlayerKey, Context, Unk1, Unk2);
}