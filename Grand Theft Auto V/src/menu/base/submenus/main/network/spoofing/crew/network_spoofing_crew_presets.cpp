#include "network_spoofing_crew_presets.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing_crew.hpp"
#include "network_spoofing_crew_name.hpp"
#include "network_spoofing_crew_tag.hpp"
#include "network_spoofing_crew_motto.hpp"
#include "network_spoofing_crew_rank.hpp"
#include "network_spoofing_crew_icon.hpp"

using namespace NetworkSpoofingCrewPresetsMenuVars;

namespace NetworkSpoofingCrewPresetsMenuVars {
	Vars_ m_Vars;

	std::string CrewNamePresets[] = {
		"Barry McKochner", "Ben Dover", "Chris P Bacon", "Micheal Hunt",
		"Hugh Jarse", "Hugh Mungus", "Heywood Jablome", "Mike Oxblack",
		"Mike Oxlonger", "Jack Kanoff", "Anass Rhammar", "Drew Peacock",
		"Mike Hunt", "Nick Gurh", "Jenna Tolls", "Gabe Itch",
		"Jenny Talia", "Rae Piste", "Hue G. Rection", "Moe Lester"
	};

	std::string CrewTagPresets[] = {
		"RSTR", "OZRK", "HIKE", "IGN",
		"CUNT", "SKID", "FUCK", "COCK",
		"POG", "NIGR", "TXIC", "MODS"
	};

	std::string CrewMottoPresets[] = {
		"Rockstar Developers", "White Lives Matter", "Yes", "Pog",
		"Your Cheat Sucks", "Ozark #1"
	};

	std::string CrewRankPresets[] = {
		"Leader", "Member", "Commissioner", "Lieutenant",
		"Muscle", "Thug", "Hustler", "Soldier",
		"Trigger", "Enforcer", "Facilitator", "Public Enemy",
		"Shot Caller", "Street Boss", "Kingpin"
	};

	std::pair<std::string, int> CrewIconPresets[] = {
		{ "Rockstar", 17506743 },
		{ "Kappa", 58800 },
		{ "Doge", 8983550 },
		{ "Troll Face", 1232493 },
		{ "Anonymous", 19365671 },
		{ "Spoderman", 5511527 },
		{ "Skull", 7259919 },
		{ "UMAD", 18989208 },
		{ "Anime Tits", 7523962 },
		{ "Redtube", 6207630 },
		{ "Transformers", 29269474 },
		{ "Joker", 185524 },
		{ "Deadpool", 1328145 },
		{ "Rolling Blunt", 18822398 },
		{ "Blunt Man", 18470608 },
		{ "Micky", 34186928 },
		{ "iFruit", 40030447 },
		{ "Monkey", 15232130 }, // Nigger
		{ "Razer", 23320514 },
		{ "Batman", 16821415 },
		{ "Weed", 36439940 },
		{ "Pika", 24676577 },
		{ "Clown", 512255 },
		{ "Jesus", 8375850 }
	};

	void Callback(std::string Str) {
		*m_Vars.m_Editing = Str;
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
	}
}

void NetworkSpoofingCrewPresetsMenu::Init() {
	SetName("Crew Presets");
	SetParentSubmenu<NetworkSpoofingCrewMenu>();

	for (auto& Name : CrewNamePresets) addString(Name);
	for (auto& Name : CrewTagPresets) addString(Name);
	for (auto& Name : CrewMottoPresets) addString(Name);
	for (auto& Name : CrewRankPresets) addString(Name);
	for (auto& Name : CrewIconPresets) addString(Name.first);
}

void NetworkSpoofingCrewPresetsMenu::Update() {}

/*Called once on submenu open*/
void NetworkSpoofingCrewPresetsMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	// Crew Name presets
	if (m_Parent == NetworkSpoofingCrewNameMenu::GetInstance()) {
		for (auto& Name : CrewNamePresets) {
			addOption(ButtonOption(getString(Name))
				.addOnClick([=] { Callback(Name); }));
		}

		return;
	}

	// Crew Tag presets
	if (m_Parent == NetworkSpoofingCrewTagMenu::GetInstance()) {
		for (auto& Name : CrewTagPresets) {
			addOption(ButtonOption(getString(Name))
				.addOnClick([=] { Callback(Name); }));
		}

		return;
	}

	// Crew Motto presets
	if (m_Parent == NetworkSpoofingCrewMottoMenu::GetInstance()) {
		for (auto& Name : CrewMottoPresets) {
			addOption(ButtonOption(getString(Name))
				.addOnClick([=] { Callback(Name); }));
		}

		return;
	}

	// Crew Rank presets
	if (m_Parent == NetworkSpoofingCrewRankMenu::GetInstance()) {
		for (auto& Name : CrewRankPresets) {
			addOption(ButtonOption(getString(Name))
				.addOnClick([=] { Callback(Name); }));
		}

		return;
	}

	// Crew Icon presets
	if (m_Parent == NetworkSpoofingCrewIconMenu::GetInstance()) {
		for (auto& Name : CrewIconPresets) {
			addOption(ButtonOption(getString(Name.first))
				.addOnClick([=] { Callback(std::to_string(Name.second)); }));
		}

		return;
	}
}

/*Called always*/
void NetworkSpoofingCrewPresetsMenu::FeatureUpdate() {}

NetworkSpoofingCrewPresetsMenu* _instance;
NetworkSpoofingCrewPresetsMenu* NetworkSpoofingCrewPresetsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingCrewPresetsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingCrewPresetsMenu::~NetworkSpoofingCrewPresetsMenu() { delete _instance; }