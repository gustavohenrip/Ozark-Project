#include "network_presence.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/caller.hpp"
#include "rage/api/social.hpp"
#include "menu/base/util/presence.hpp"
#include "utils/va.hpp"
#include "utils/memory/memory.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_rid.hpp"

using namespace NetworkPresenceMenuVars;

namespace NetworkPresenceMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Types[] = {
		{ TranslationString("Generic", true), 0x8000 },
		{ TranslationString("Spectator", true), 0x8001 },
	};

	void JoinPlayer(uint64_t RockstarID) {
		m_Vars.m_PendingGsInfoRequest = false;

		m_Vars.m_GsInfo.clear();
		m_Vars.m_Type = 2;
		m_Vars.m_PendingGsInfoRequestID = RockstarID;
		m_Vars.m_PendingGsInfoRequest = true;
		m_Vars.m_PendingTimeout = GetTickCount() + 10000;

		Rage::Network::RockstarIdentifier* Identifier = new Rage::Network::RockstarIdentifier();
		Identifier->m_RockstarID = RockstarID;
		Identifier->m_Type = 3;
		
		// call into get info
		Native::ClearGamerTaskData();
		Rage::Engine::SendSessionInfoRequest(Identifier, 1);
		delete Identifier;

		Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %i", NetworkPresenceMenu::GetInstance()->getString("Successfully sent request to join").c_str(), RockstarID), Global::UIVars::g_NotificationSuccess);
	}

	void JoinPlayerCallback(const char* Input) {
		std::string StringName(Input);
		if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
			// RID
			JoinPlayer(atoll(StringName.c_str()));
		} else {
			Rage::API::GetSocialClub()->GetProfileFromName(StringName, [] (bool Success, Rage::API::RockstarAccount Account) {
				if (Success) {
					JoinPlayer(Account.m_RockstarID);
				} else {
					Menu::GetNotify()->NotifyStacked(NetworkPresenceMenu::GetInstance()->getString("Failed resolving Rockstar ID"), Global::UIVars::g_NotificationError);
				}
			});
		}
	}

	void InputPlayerCallback(const char* Input) {
		std::string StringName(Input);
		if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
			// RID
			m_Vars.m_Target = atoll(StringName.c_str());
		} else {
			Rage::API::GetSocialClub()->GetProfileFromName(StringName, [] (bool Success, Rage::API::RockstarAccount Account) {
				if (Success) {
					m_Vars.m_Target = Account.m_RockstarID;
				} else {
					Menu::GetNotify()->NotifyStacked(NetworkPresenceMenu::GetInstance()->getString("Failed resolving Rockstar ID"), Global::UIVars::g_NotificationError);
				}
			});
		}
	}

	void JoinPlayerViaSessionInfo(Rage::Network::NetMsgIdentifier* Identifier, Rage::Network::GsSession* SessionInfo, int Flag) {
		Rage::Network::NetworkHandle EmptyHandle;
		Caller::Call<int>(Global::Vars::g_GameAddress + 0x10E0C9C, Identifier, 1, SessionInfo, "", &EmptyHandle, "", -1, -1, 0, 0, Flag, 1);
	}

	void HandleJoinPlayer() {
		m_Vars.m_PendingGsInfoRequest = false;

		Rage::Network::GameInvite* Invite = new Rage::Network::GameInvite();
		Invite->h = NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID;
		Invite->h2 = 3;
		Invite->if_ = -1;
		Invite->it = -1;
		Invite->f = 0xA000;
		Invite->gm = 0;
		Invite->cc = NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID;
		Invite->cc2 = 3;
		Invite->l = 0; // 0
		Invite->p = 0;
		Invite->cr = 0;
		Invite->u = 0;
		Invite->d = 889455970;
		Invite->jq = 0;
		strcpy(Invite->s, m_Vars.m_GsInfo.c_str());
		strcpy(Invite->c, "");
		strcpy(Invite->mid, "");
		strcpy(Invite->n, "sdsdsdsds");

		LOG_DEV("Getting info from %s", m_Vars.m_GsInfo.c_str());
		Rage::Engine::GetSessionInfoFromGs(&Invite->gssession_start, Invite->s, 0, 1);

		Memory::WriteVector(Global::Vars::g_FunctionPatches[5].m_Address, Global::Vars::g_FunctionPatches[5].m_Patch);
		
		char* x2 = new char[500];
		memset(x2, 0, 500);

		Caller::Call<int>(Global::Vars::g_CreateGameInvitePresence, x2, Global::Vars::g_SendGameInvitePresenceTable);
		Caller::Call<int>(Global::Vars::g_SendGameInvitePresence, Invite, x2);

		Memory::WriteVector(Global::Vars::g_FunctionPatches[5].m_Address, Global::Vars::g_FunctionPatches[5].m_Original);

		delete Invite;
		delete[] x2;

		Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %i...", NetworkPresenceMenu::GetInstance()->getString("Joining").c_str(), m_Vars.m_PendingGsInfoRequestID), Global::UIVars::g_NotificationSuccess);
	}
}

void NetworkPresenceMenu::Init() {
	SetName("Presence");
	SetParentSubmenu<NetworkMenu>();

	addString("Successfully sent request to join");
	addString("Failed resolving Rockstar ID");
	addString("Joining");
	addString("Target");
	addString("Failed to resolve players session, they may be offline");

	addOption(ButtonOption("Input Player")
		.addTranslation()
		.addKeyboard("Enter players name or rid", 30, InputPlayerCallback));

	addOption(ButtonOption("Show Profile")
		.addTranslation()
		.addOnClick([] {
			Rage::Network::NetworkHandle Handle;
			Native::NetworkHandleFromUserId(std::to_string(m_Vars.m_Target).c_str(), &Handle.m_Handle, (sizeof(Handle) / 4));
			Native::NetworkShowProfileUi(&Handle.m_Handle);
		})
		.addRequirement([] { return m_Vars.m_Target > 0; }));

	addOption(ButtonOption("Send Friend Request")
		.addTranslation()
		.addOnClick([] { Rage::Engine::SendFriendRequest(m_Vars.m_Target); })
		.addRequirement([] { return m_Vars.m_Target > 0; }));

	addOption(ButtonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] { Menu::Presence::InvitePlayer(m_Vars.m_Target); })
		.addRequirement([] { return m_Vars.m_Target > 0; }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Join Player")
		.addTranslation()
		.addScroll(m_Vars.m_JoinThemType, 0, NUMOF(Types), Types)
		.addOnClick([] { JoinPlayer(m_Vars.m_Target); })
		.addTooltip("Force join this player, even if they're in an invite only!")
		.addRequirement([] { return m_Vars.m_Target > 0; }));
}

void NetworkPresenceMenu::Update() {
	if (m_Vars.m_Target > 0) {
		Menu::GetNotify()->Subtitle(Utils::VA::VA("%s: %i", getString("Target").c_str(), m_Vars.m_Target));
	}
}

/*Called once on submenu open*/
void NetworkPresenceMenu::UpdateOnce() {}

/*Called always*/
void NetworkPresenceMenu::FeatureUpdate() {
	if (m_Vars.m_PendingGsInfoRequest) {
		if (m_Vars.m_PendingTimeout < GetTickCount()) {
			m_Vars.m_PendingGsInfoRequest = false;
			Menu::GetNotify()->NotifyStacked(getString("Failed to resolve players session, they may be offline"), Global::UIVars::g_NotificationError);
			return;
		}

		if (m_Vars.m_Type == 2) {
			if (!m_Vars.m_GsInfo.empty()) {
				HandleJoinPlayer();
				m_Vars.m_GsInfo.clear();
			}
		}
	}
}

NetworkPresenceMenu* _instance;
NetworkPresenceMenu* NetworkPresenceMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPresenceMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPresenceMenu::~NetworkPresenceMenu() { delete _instance; }