#include "network_global_chat.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "chat/network_global_chat_presets.hpp"
#include "chat/network_global_chat_targets.hpp"
#include "menu/base/util/notify.hpp"
#include "rage/engine.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"

using namespace NetworkGlobalChatMenuVars;

namespace NetworkGlobalChatMenuVars {
	Vars_ m_Vars;

	uint64_t GetSenderChatToken(uint64_t Default) {
		uint64_t ChatToken = Default; // Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_ChatToken;

		if (Default == 0) {
			if (Menu::GetLocalPlayer().m_NetGamePlayer) {
				if (Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo) {
					ChatToken = Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_ChatToken;
				}
			}
		}
		
		if (m_Vars.m_RandomizeSender) {
			static int Current = 0;
			if (Current == 32) Current = 0;

			if (Menu::GetPlayer(Current).m_Exists && Menu::GetPlayer(Current).m_NetGamePlayer) {
				ChatToken = Menu::GetPlayer(Current).m_NetGamePlayer->m_PlayerInfo->m_ChatToken;
			}

			Current++;
		}

		if (m_Vars.m_SenderToggle) {
			if (Menu::GetPlayer(m_Vars.m_SenderID).m_NetGamePlayer) {
				if (Menu::GetPlayer(m_Vars.m_SenderID).m_NetGamePlayer->m_PlayerInfo) {
					return Menu::GetPlayer(m_Vars.m_SenderID).m_NetGamePlayer->m_PlayerInfo->m_ChatToken;
				}
			}

			Menu::GetNotify()->NotifyStacked("Network -> Global Chat -> Spoof sender disabled, player no longer valid", Global::UIVars::g_NotificationError);
			m_Vars.m_SenderToggle = false;
			m_Vars.m_SenderID = 0;
		}

		return ChatToken;
	}

	Player GetPlayerIDFromIP(uint32_t ExternalIP) {
		Player ID = -1;
		
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			if (ID != -1) return;

			if (Player.m_NetGamePlayer) {
				if (Player.m_NetGamePlayer->m_PlayerInfo) {
					if (Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP == ExternalIP) {
						ID = Player.m_ID;
					}
				}
			}
		});

		return ID;
	}

	Player GetPlayerIDFromPlayerKey(int Key) {
		Player ID = -1;

		Rage::Network::GlobalChatRaw* Data = (Rage::Network::GlobalChatRaw*)(*(uint64_t*)(Global::Vars::g_GlobalChatPtr) + 0x78);

		if (Data->m_PlayerCount > 0) {
			int Increment = 0;
			do {
				Rage::Network::PlayerPeer* Peer = Data->m_PlayerPeers[Increment];
				if (Peer) {
					if (!(Peer->m_Bitset & 2) && (Peer->m_Bitset & 1)) {
						if (Data->m_Flag0 != -1 || Peer->m_Flag0) {
							if (Peer->m_PlayerKey < 0) {
								int PlayerFlag = (Data->m_Bitset & 1) ? Data->m_Flag2 : Data->m_Flag1;
								Peer->m_PlayerKey = Rage::Engine::GetClientPlayerKey(Data->m_ManagerPtr, &Peer->m_SecondaryKey, PlayerFlag);
							}

							if (Peer->m_PlayerKey >= 0) {
								if (Peer->m_PlayerKey == Key) {
									ID = GetPlayerIDFromIP(Peer->m_ExternalIP);
								}
							}
						}
					}
				}

				Increment++;
			} while (Increment < Data->m_PlayerCount);
		}

		return ID;
	}

	bool CanSendToPlayer(Player ID) {
		if (!NetworkGlobalChatTargetsMenuVars::m_Vars.m_Toggle) return true;
		return Menu::GetPlayer(ID).m_TargetedGlobalChat;
	}

	void SetupGlobalChatMessageContext(Rage::Network::GlobalMsgContext* Context, uint64_t ChatToken, const char* Message, bool Flag0) {
		strncpy(Context->m_Message, Message, 0x100);

		Context->m_Message[0xFF] = 0;
		Context->m_Flag0IsNeg1 = Flag0;
		Context->m_SenderChatToken = ChatToken;
	}

	void SendGlobalMessage() {
		Rage::Network::GlobalChatRaw* Data = (Rage::Network::GlobalChatRaw*)(*(uint64_t*)(Global::Vars::g_GlobalChatPtr) + 0x78);

		Rage::Network::GlobalMsgContext* Context = new Rage::Network::GlobalMsgContext();
		SetupGlobalChatMessageContext(Context, GetSenderChatToken(0), m_Vars.m_TempName.c_str(), Data->m_Flag0 == -1);

		bool Sent = false;
		if (Data->m_PlayerCount > 0) {
			int Increment = 0;
			do {
				Rage::Network::PlayerPeer* Peer = Data->m_PlayerPeers[Increment];
				if (Peer) {
					Player Recv = GetPlayerIDFromIP(Peer->m_ExternalIP);
					if (Recv == -1) continue;

					if (CanSendToPlayer(Recv)) {
						if (!(Peer->m_Bitset & 2) && (Peer->m_Bitset & 1)) {
							if (Data->m_Flag0 != -1 || Peer->m_Flag0) {
								if (Peer->m_PlayerKey < 0) {
									int PlayerFlag = (Data->m_Bitset & 1) ? Data->m_Flag2 : Data->m_Flag1;
									Peer->m_PlayerKey = Rage::Engine::GetClientPlayerKey(Data->m_ManagerPtr, &Peer->m_SecondaryKey, PlayerFlag);
								}

								if (Peer->m_PlayerKey >= 0) {
									Sent = true;
									printf("Sending message to %X - %X - %i times\n", Peer->m_ExternalIP, Peer->m_PlayerKey, m_Vars.m_MessageCount);

									for (int i = 0; i < m_Vars.m_MessageCount; i++) {
										Rage::Engine::SendGlobalChatMessage(Data->m_ManagerPtr, Peer->m_PlayerKey, Context);
									}
								}
							}
						}
					}
				}

				Increment++;
			} while (Increment < Data->m_PlayerCount);
		}

		delete Context;

		if (Sent) {
			Menu::GetNotify()->NotifyStacked("Successfully sent message!", Global::UIVars::g_NotificationSuccess);
		} else {
			Menu::GetNotify()->NotifyStacked("Failed to send message to other players!", Global::UIVars::g_NotificationError);
		}
	}
}

void NetworkGlobalChatMenu::Init() {
	SetName("Global Chat");
	SetParentSubmenu<NetworkMenu>();

	/*addOption(SubmenuOption("Presets")
		.addTranslation()
		.addSubmenu<NetworkGlobalChatPresetsMenu>()
		.addOnClick([] { NetworkGlobalChatPresetsMenuVars::m_Vars.m_Editing = &m_Vars.m_TempName; }));*/

	addOption(SubmenuOption("Targets")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkGlobalChatTargetsMenu>()
		.addTooltip("Global chat and messages sent from here will only be sent to these specific players"));

	/*addOption(ButtonOption("Input Message")
		.addTranslation()
		.addKeyboard("Enter your message", 0x100, [] (const char* Message) { m_Vars.m_TempName = Message; }));*/

	addOption(ScrollOption<int>(TOGGLE, "Spoof Sender")
		.addTranslation()
		.addToggle(m_Vars.m_SenderToggle)
		.addOnClick([] {
			m_Vars.m_RandomizeSender = false;
			m_Vars.m_SenderID = m_Vars.m_PlayerList[m_Vars.m_Sender].m_Result;
		})
		.addScroll(m_Vars.m_Sender, 0, NUMOF(m_Vars.m_PlayerList), m_Vars.m_PlayerList)
		.addOnUpdate([] (ScrollOption<int>* Option) { Option->addScroll(m_Vars.m_Sender, 0, m_Vars.m_ListSize, m_Vars.m_PlayerList); })
		.addRequirement([] { return m_Vars.m_ListSize >= 1; }));

	addOption(ToggleOption("Randomize Sender")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RandomizeSender)
		.addOnClick([] { m_Vars.m_SenderToggle = false; })
		.addRequirement([] { return m_Vars.m_ListSize >= 1; }));

	/*addOption(NumberOption<int>(SCROLLSELECT, "Send Message")
		.addTranslation()
		.addNumber(m_Vars.m_MessageCount, "%i", 1).addMin(1)
		.addOnClick(SendGlobalMessage)
		.addTooltip("Sends x amount of messages"));*/

	addString("Network -> Global Chat -> Spoof sender disabled, player no longer valid");
}

void NetworkGlobalChatMenu::Update() {
	if (m_Vars.m_TempName.length()) {
		Menu::GetNotify()->Subtitle(m_Vars.m_TempName.c_str());
	}
}

/*Called once on submenu open*/
void NetworkGlobalChatMenu::UpdateOnce() {}

/*Called always*/
void NetworkGlobalChatMenu::FeatureUpdate() {
	m_Vars.m_ListSize = 0;
	Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
		m_Vars.m_PlayerList[m_Vars.m_ListSize].m_Name.Set(Player.m_Name);
		m_Vars.m_PlayerList[m_Vars.m_ListSize].m_Result = Player.m_ID;

		if (m_Vars.m_SenderToggle) {
			if (m_Vars.m_SenderID == Player.m_ID) {
				m_Vars.m_Sender = m_Vars.m_ListSize;
			}
		}

		m_Vars.m_ListSize++;
	});

	if (m_Vars.m_SenderToggle) {
		if (!Menu::GetPlayer(m_Vars.m_SenderID).m_Connected) {
			m_Vars.m_SenderToggle = false;
			m_Vars.m_Sender = 0;
			Menu::GetNotify()->NotifyStacked(getString("Network -> Global Chat -> Spoof sender disabled, player no longer valid"), Global::UIVars::g_NotificationError);
		}
	}

	if (m_Vars.m_ListSize == 0) {
		m_Vars.m_SenderToggle = false;
		m_Vars.m_RandomizeSender = false;
	}
}

NetworkGlobalChatMenu* _instance;
NetworkGlobalChatMenu* NetworkGlobalChatMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkGlobalChatMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkGlobalChatMenu::~NetworkGlobalChatMenu() { delete _instance; }