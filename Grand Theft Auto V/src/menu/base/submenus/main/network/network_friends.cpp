#include "network_friends.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "menu/base/util/panels.hpp"
#include "utils/va.hpp"
#include "friends/network_friends_selected.hpp"

using namespace NetworkFriendsMenuVars;

namespace NetworkFriendsMenuVars {
	Vars_ m_Vars;
}

void NetworkFriendsMenu::Init() {
	SetName("Friends");
	SetParentSubmenu<NetworkMenu>();

	/*addOption(SubmenuOption("All Friends")
		.addTranslation()
		.addSubmenu<NetworkFriendsSelectedMenu>()
		.addOnHover([] { Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_FRIEND", false); })
		.addOnClick([] { NetworkFriendsSelectedMenuVars::m_Vars.m_Type = 1; }));

	addOption(BreakOption("Friends")
		.addTranslation());*/

	addString("~m~None");
}

void NetworkFriendsMenu::Update() {}

/*Called once on submenu open*/
void NetworkFriendsMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Native::NetworkGetFriendCount() == 0) {
		addOption(ButtonOption(getString("~m~None")));
	} else {
		for (int i = 0; i < Native::NetworkGetFriendCount(); i++) {
			Rage::Network::Friends* Friend = (Rage::Network::Friends*) & Global::Vars::g_Friends[i];
			addOption(SubmenuOption(Utils::VA::VA("%s%s", (abs(Friend->m_State) == 3) ? "" : "~c~", Friend->m_Name))
				.addSubmenu<NetworkFriendsSelectedMenu>()
				.addOnHover([=] {
				Menu::Panels::PanelCustomFriend Tmp;
				Tmp.m_RockstarID = Friend->m_RockstarID;
				Tmp.m_State = Friend->m_State;
				strcpy(Tmp.m_Name, Friend->m_Name);

				Menu::Panels::GetPanelManager()->SetStructure(Menu::Panels::GetPanelManager()->GetChild(Menu::Panels::GetPanelManager()->GetParent("PANEL_FRIEND"), "PANEL_FRIEND_INFO"), &Tmp, sizeof(Tmp));
				Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_FRIEND", true);
			})
				.addOnClick([=] {
				NetworkFriendsSelectedMenuVars::m_Vars.m_Selected = Friend;
				NetworkFriendsSelectedMenuVars::m_Vars.m_Type = 0;
			}));
		}
	}
}

/*Called always*/
void NetworkFriendsMenu::FeatureUpdate() {}

NetworkFriendsMenu* _instance;
NetworkFriendsMenu* NetworkFriendsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkFriendsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkFriendsMenu::~NetworkFriendsMenu() { delete _instance; }