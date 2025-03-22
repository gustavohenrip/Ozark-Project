#include "network_browser.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_session.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/timers.hpp"
#include "rage/engine.hpp"
#include "utils/caller.hpp"
#include "rage/api/social.hpp"
#include "menu/base/util/panels.hpp"
#include "network_browser_selected.hpp"

using namespace NetworkBrowserMenuVars;

namespace NetworkBrowserMenuVars {
	Vars_ m_Vars;
}

void NetworkBrowserMenu::Init() {
	SetName("Browser");
	SetParentSubmenu<NetworkSessionMenu>();

	addString("Improved version with faster populating coming soon!");
	addString("Sessions that expire will still show in the list!");
	addString("~m~None");

	addOption(ToggleOption("Toggle Population")
		.addTranslation()
		.addToggle(m_Vars.m_Populate)
		.addTooltip("Does cause some stuttering so don't leave it enabled all the time"));

	// TODO: Search

	addOption(ButtonOption("Clear List")
		.addTranslation()
		.addOnClick([] { Global::Vars::g_Sessions.clear(); }));

	addOption(BreakOption("Session Hosts")
		.addTranslation());
}

void NetworkBrowserMenu::Update() {
	static int Timer = 0;

	Menu::Timers::RunTimedFunction(&Timer, 1000, [this] {
		ClearOptionsOffset(3);

		for (std::size_t i = 0; i < Global::Vars::g_Sessions.size(); i++) {
			auto& Info = Global::Vars::g_Sessions[i];
			if (!Info.m_Requested) {
				Info.m_Requested = true;
				Rage::API::GetSocialClub()->GetProfileFromRockstarID(Info.m_RockstarID, [] (bool Successful, Rage::API::RockstarAccount Account) {
					if (Successful) {
						auto Vit = std::find_if(Global::Vars::g_Sessions.begin(), Global::Vars::g_Sessions.end(), [=] (SessionBrowser& Element) { return Element.m_RockstarID == Account.m_RockstarID; });
						if (Vit != Global::Vars::g_Sessions.end()) {
							Vit->m_Name = Account.m_Name;
						}
					}
				});
			}

			bool Modder = ((Info.m_PeerToken >> 32) & 0xFFFFFFFF) < 0x1000 || Info.m_PeerToken < 0x1000 || Info.m_RockstarID < 0x1000;

			char Option[150];
			if (Info.m_Name.empty()) {
				snprintf(Option, 150, "%s%i%s", Modder ? "~r~" : "", (uint32_t)Info.m_RockstarID, Modder ? " - Modder" : "");
			} else {
				snprintf(Option, 150, "%s%s%s", Modder ? "~r~" : "", Info.m_Name.c_str(), Modder ? " - Modder" : "");
			}
			
			addOption(SubmenuOption(Option)
				.addSubmenu<NetworkBrowserSelectedMenu>()
				.addOnHover([=] {
					Menu::Panels::PanelCustomSessionBrowser Browser;
					Browser.m_PeerToken = Info.m_PeerToken;
					Browser.m_RockstarID = Info.m_RockstarID;
					Browser.m_Name[0] = 0;
					if (!Info.m_Name.empty()) strcpy(Browser.m_Name, Info.m_Name.c_str());

					auto Parent = Menu::Panels::GetPanelManager()->GetParent("PANEL_SESSION_BROWSER");
					if (Parent) {
						Menu::Panels::GetPanelManager()->SetStructure(Menu::Panels::GetPanelManager()->GetChild(Parent, "PANEL_SESSION_BROWSER_INFO"), (void*)&Browser, sizeof(Browser));
						Parent->m_Render = true;
					}
				})
				.addOnClick([=] { NetworkBrowserSelectedMenuVars::m_Vars.m_Selected = i; }));
		}

		if (Global::Vars::g_Sessions.size() == 0) {
			addOption(ButtonOption(getString("~m~None")));
		}
	});

	if (Menu::GetBase()->m_CurrentOption < 3) Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_SESSION_BROWSER", false);
}

/*Called once on submenu open*/
void NetworkBrowserMenu::UpdateOnce() {}

/*Called always*/
void NetworkBrowserMenu::FeatureUpdate() {}

NetworkBrowserMenu* _instance;
NetworkBrowserMenu* NetworkBrowserMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkBrowserMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkBrowserMenu::~NetworkBrowserMenu() { delete _instance; }