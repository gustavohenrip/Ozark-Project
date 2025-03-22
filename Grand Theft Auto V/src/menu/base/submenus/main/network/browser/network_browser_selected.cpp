#include "network_browser_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_browser.hpp"
#include "../network_presence.hpp"
#include "menu/base/util/presence.hpp"
#include "rage/engine.hpp"

using namespace NetworkBrowserSelectedMenuVars;

namespace NetworkBrowserSelectedMenuVars {
	Vars_ m_Vars;
}

void NetworkBrowserSelectedMenu::Init() {
	SetName("Browser");
	SetParentSubmenu<NetworkBrowserMenu>();

	addOption(ButtonOption("Show Profile")
		.addTranslation()
		.addOnClick([] {
			Rage::Network::NetworkHandle Handle;
			Native::NetworkHandleFromUserId(std::to_string(Global::Vars::g_Sessions[m_Vars.m_Selected].m_RockstarID).c_str(), &Handle.m_Handle, (sizeof(Handle) / 4));
			Native::NetworkShowProfileUi(&Handle.m_Handle);
		}));

	addOption(ButtonOption("Send Friend Request")
		.addTranslation()
		.addOnClick([] { Rage::Engine::SendFriendRequest(Global::Vars::g_Sessions[m_Vars.m_Selected].m_RockstarID); }));

	addOption(ButtonOption("Invite to Session")
		.addTranslation()
		.addOnClick([] { Menu::Presence::InvitePlayer(Global::Vars::g_Sessions[m_Vars.m_Selected].m_RockstarID); }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Join Player")
		.addTranslation()
		.addScroll(NetworkPresenceMenuVars::m_Vars.m_JoinThemType, 0, NUMOF(NetworkPresenceMenuVars::Types), NetworkPresenceMenuVars::Types)
		.addOnClick([] { NetworkPresenceMenuVars::JoinPlayer(Global::Vars::g_Sessions[m_Vars.m_Selected].m_RockstarID); })
		.addTooltip("Force join this player, even if they're in an invite only!"));
}

void NetworkBrowserSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkBrowserSelectedMenu::UpdateOnce() {}

/*Called always*/
void NetworkBrowserSelectedMenu::FeatureUpdate() {}

NetworkBrowserSelectedMenu* _instance;
NetworkBrowserSelectedMenu* NetworkBrowserSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkBrowserSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkBrowserSelectedMenu::~NetworkBrowserSelectedMenu() { delete _instance; }