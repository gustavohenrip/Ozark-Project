#include "network_host_toolkit_join_block.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_host_toolkit.hpp"
#include "network_host_toolkit_join_block_selected.hpp"

using namespace NetworkHostToolkitJoinBlockingMenuVars;

namespace NetworkHostToolkitJoinBlockingMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> WhoCanJoin[] = {
		{ TranslationString("Anybody", true), 0 },
		{ TranslationString("Nobody", true), 1 },
		{ TranslationString("Friends Only", true), 2 },
	};

	const char* Strings[] = {
		"Spoofed Next Session Host",
		"Spoofed RID",
		"Spoofed Name",
		"Spoofed IP"
	};
}

void NetworkHostToolkitJoinBlockingMenu::Init() {
	SetName("Join Blocking");
	SetParentSubmenu<NetworkHostToolkitMenu>();

	int LoadIndex = 0;
	for (auto Str : Strings) {
		m_Vars.m_Responses[LoadIndex].m_Enabled = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Toggled", false, { Str });
		m_Vars.m_Responses[LoadIndex].m_Notify = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Notify", true, { Str });
		m_Vars.m_Responses[LoadIndex].m_Context.m_Selected = Utils::GetConfig()->ReadInt(m_ParentNameStack, "Selected", 1, { Str });
		LoadIndex++;
	}

	addOption(SubmenuOption("Spoofed Next Session Host")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitJoinBlockingSelectedMenu>()
		.addOnClick([this] {
			NetworkHostToolkitJoinBlockingSelectedMenu::GetInstance()->SetName(getString("Spoofed Next Session Host"));
			NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Responses[TOOLKIT_NEXT_SESSION_HOST];
		})
		.addTooltip("Blocks most force host spoofers from joining the session. You have to be host!"));

	addOption(SubmenuOption("Spoofed RID")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitJoinBlockingSelectedMenu>()
		.addOnClick([this] {
			NetworkHostToolkitJoinBlockingSelectedMenu::GetInstance()->SetName(getString("Spoofed RID"));
			NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Responses[TOOLKIT_RID];
		})
		.addTooltip("Blocks most RID spoofers from joining the session. You have to be host!"));

	addOption(SubmenuOption("Spoofed Name")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitJoinBlockingSelectedMenu>()
		.addOnClick([this] {
			NetworkHostToolkitJoinBlockingSelectedMenu::GetInstance()->SetName(getString("Spoofed Name"));
			NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Responses[TOOLKIT_NAME];
		})
		.addTooltip("Blocks obvious name spoofers from joining the session. You have to be host!"));

	addOption(SubmenuOption("Spoofed IP")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkHostToolkitJoinBlockingSelectedMenu>()
		.addOnClick([this] {
			NetworkHostToolkitJoinBlockingSelectedMenu::GetInstance()->SetName(getString("Spoofed IP"));
			NetworkHostToolkitJoinBlockingSelectedMenuVars::m_Vars.m_Selected = &m_Vars.m_Responses[TOOLKIT_IP];
		})
		.addTooltip("Blocks IP spoofers from joining the session. You have to be host!"));

	addOption(ScrollOption<int>(SCROLLSELECT, "Who Is't May Joineth")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_WhoMayJoin, 0, NUMOF(WhoCanJoin), WhoCanJoin).canBeSaved(m_ParentNameStack)
		.addTooltip("Who can join your session?"));
}

void NetworkHostToolkitJoinBlockingMenu::Update() {}

/*Called once on submenu open*/
void NetworkHostToolkitJoinBlockingMenu::UpdateOnce() {
	int LoadIndex = 0;
	for (auto Str : Strings) {
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Toggled", m_Vars.m_Responses[LoadIndex].m_Enabled, { Str });
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Notify", m_Vars.m_Responses[LoadIndex].m_Notify, { Str });
		Utils::GetConfig()->WriteInt(m_ParentNameStack, "Selected", m_Vars.m_Responses[LoadIndex].m_Context.m_Selected, { Str });
		LoadIndex++;
	}
}

/*Called always*/
void NetworkHostToolkitJoinBlockingMenu::FeatureUpdate() {}

NetworkHostToolkitJoinBlockingMenu* _instance;
NetworkHostToolkitJoinBlockingMenu* NetworkHostToolkitJoinBlockingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkHostToolkitJoinBlockingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkHostToolkitJoinBlockingMenu::~NetworkHostToolkitJoinBlockingMenu() { delete _instance; }