#include "network_friends_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_friends.hpp"
#include "menu/base/submenus/main/network/network_presence.hpp"
#include "menu/base/util/presence.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "rage/engine.hpp"

using namespace NetworkFriendsSelectedMenuVars;

namespace NetworkFriendsSelectedMenuVars {
	Vars_ m_Vars;
}

void NetworkFriendsSelectedMenu::Init() {
	SetName("Friends");
	SetParentSubmenu<NetworkFriendsMenu>();

	addOption(ButtonOption("Show Profile")
		.addTranslation()
		.addOnClick([] {
			Rage::Network::NetworkHandle Handle;
			Native::NetworkHandleFromUserId(std::to_string(m_Vars.m_Selected->m_RockstarID).c_str(), &Handle.m_Handle, (sizeof(Handle) / 4));
			Native::NetworkShowProfileUi(&Handle.m_Handle);
		}));

	addOption(ButtonOption("Send Friend Request")
		.addTranslation()
		.addOnClick([] { Rage::Engine::SendFriendRequest(m_Vars.m_Selected->m_RockstarID); }));

	addOption(ButtonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] { Menu::Presence::InvitePlayer(m_Vars.m_Selected->m_RockstarID); }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Join Player")
		.addTranslation()
		.addScroll(NetworkPresenceMenuVars::m_Vars.m_JoinThemType, 0, NUMOF(NetworkPresenceMenuVars::Types), NetworkPresenceMenuVars::Types)
		.addOnClick([] { NetworkPresenceMenuVars::JoinPlayer(m_Vars.m_Selected->m_RockstarID); })
		.addTooltip("Force join this player, even if they're in an invite only!"));
}

void NetworkFriendsSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkFriendsSelectedMenu::UpdateOnce() {}

/*Called always*/
void NetworkFriendsSelectedMenu::FeatureUpdate() {}

NetworkFriendsSelectedMenu* _instance;
NetworkFriendsSelectedMenu* NetworkFriendsSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkFriendsSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkFriendsSelectedMenu::~NetworkFriendsSelectedMenu() { delete _instance; }