#include "network_host_toolkit_join_block_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/util/notify.hpp"

using namespace NetworkHostToolkitJoinBlockingSelectedMenuVars;

namespace NetworkHostToolkitJoinBlockingSelectedMenuVars {
	Vars_ m_Vars;

	NetworkHostToolkitJoinBlockingMenuVars::ToolkitBlockResponse m_Responses[] = {
		{ "Failed Intended Session", "Failed to join intended session", FAILED_TO_JOIN_INTENDED_SESSION },
		{ "Failed Intended GTA Session", "Failed to join intended GTA Online session", FAILED_TO_JOIN_INTENDED_GTA_ONLINE_SESSION },
		{ "Incompatible Assets", "Failed to join session due to incompatible assets", FAILED_TO_JOIN_DUE_TO_INCOMPATIBLE_ASSETS },
		{ "Incompatible Downloadable Content", "Incompatible downloadable content. All players must have the latest compatibility pack", INCOMPATILBLE_DOWNLOADABLE_CONTENT },
		{ "Currently Full", "The session you're trying to join is currently full", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_CURRENTLY_FULL },
		{ "Currently Full - Queue", "The session you're trying to join is currently full of players", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_CURRENTLY_FULL_OF_PLAYERS_DO_YOU_WANT_TO_JOIN_QUEUE },
		{ "Wrong Version (Title Update)", "Please make sure all players have the latest Title Update", PLEASE_MAKE_SURE_ALL_PLAYERS_HAVE_THE_LATEST_TITLE_UPDATE },
		{ "Voted Out", "You have already been voted out of this game session", YOU_HAVE_BEEN_VOTED_OUT },
		{ "Invites are Disabled", "Invites are currently disabled in the session", INVITES_ARE_CURRENTLY_DISABLED_IN_THIS_SESSION },
		{ "Active Crew Limit", "The session you are trying to join has an active Crew limit and cannot accommodate your Crew", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_HAS_AN_ACTIVE_CREW_LIMIT },
		{ "You are Bad Sport", "The session you are trying to join is for people who are not Bad Sports or cheaters - you are a Bad Sport", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FOR_PEOPLE_WHO_ARE_NOT_BADSPORT_OR_CHEATERS_YOU_ARE_BADSPORT },
		{ "Cheat", "You are classed as a cheat and can only play with other cheats until you are forgiven", YOU_ARE_CLASSED_AS_A_CHEAT_AND_CAN_ONLY_PLAY_WITH_OTHER_CHEATS_UNTIL_YOU_ARE_FORGIVEN },
		{ "Cheaters Only", "The session you are trying to join is for cheaters only", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FOR_CHEATERS_ONLY },
		{ "Bad Sport Only", "The session you are trying to join is for Bad Sports only", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FOR_BADSPORTS_ONLY },
		{ "Session No Longer Exists", "The session you are trying to join no longer exists", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_NO_LONGER_EXISTS },
		{ "Invite Only", "The session you are trying to join is private. You will need to be invited to join this session", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_PRIVATE },
		{ "Friends Only", "The session you are trying to join is friends only", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FRIENDS_ONLY },
		{ "Different Targeting Preference", "The session you are trying to join is using a different targeting preference. You can change your preference in the Settings tab of the Pause Menu in Grand Theft Auto V. Joining a new GTA Online Session", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_USING_A_DIFFERENT_TARGETING_PREFERENCE },
		{ "Different Build Type", "The session you are trying to join is a different build type", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_A_DIFFERENT_BUILD_TYPE },
		{ "Different Content", "The session you are trying to join is not using the same content", THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_NOT_USING_THE_SAME_CONTENT },
		{ "Bad Reputation", "Unable to join this session, your account has a bad reputation", UNABLE_TO_JOIN_THIS_SESSION_YOUR_ACCOUNT_HAS_A_BAD_REPUTATION },
		{ "Unable To Connect", "Unable to connect to session. The session may no longer exist", UNABLE_TO_CONNECT_TO_SESSION },
		{ "Premium Race", "Unable to Join. The session you are trying to join is a Premium Race. Joining and accepting invites is disabled for this mode", UNABLE_TO_JOIN_THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_A_PREMIUM_RACE },
	};
}

void NetworkHostToolkitJoinBlockingSelectedMenu::Init() {
	SetName("", false, false);
	SetParentSubmenu<NetworkHostToolkitJoinBlockingMenu>();

	addString("Block Event");
	addString("Notify Event");
	addString("Response Message");

	for (auto& Response : m_Responses) {
		addString(Response.m_Tooltip);
	}
}

void NetworkHostToolkitJoinBlockingSelectedMenu::Update() {}

/*Called once on submenu open*/
void NetworkHostToolkitJoinBlockingSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	addOption(ToggleOption(getString("Block Event"))
		.addToggle(m_Vars.m_Selected->m_Enabled));

	addOption(ToggleOption(getString("Notify Event"))
		.addToggle(m_Vars.m_Selected->m_Notify));

	addOption(BreakOption(getString("Response Message")));

	int LastIndex = m_Vars.m_Selected->m_Context.m_Selected;
	m_Vars.m_Selected->m_Context.Reset();
	m_Vars.m_Selected->m_Context.m_Selected = 0;

	std::vector<std::shared_ptr<RadioOption>> TempOptions;

	for (auto& Response : m_Responses) {
		TempOptions.push_back(addOption(RadioOption(Response.m_Name)
			.addOnHover([=] { Menu::GetNotify()->Subtitle(getString(Response.m_Tooltip).c_str()); })
			.addRadio(m_Vars.m_Selected->m_Context)));
	}

	if (LastIndex > 0) {
		TempOptions[LastIndex]->UpdateSelected();
	}
}

/*Called always*/
void NetworkHostToolkitJoinBlockingSelectedMenu::FeatureUpdate() {}

NetworkHostToolkitJoinBlockingSelectedMenu* _instance;
NetworkHostToolkitJoinBlockingSelectedMenu* NetworkHostToolkitJoinBlockingSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkHostToolkitJoinBlockingSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkHostToolkitJoinBlockingSelectedMenu::~NetworkHostToolkitJoinBlockingSelectedMenu() { delete _instance; }