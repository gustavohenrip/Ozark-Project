#include "network_friendly_drops.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players_friendly.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace NetworkFriendlyDropsMenuVars;

namespace NetworkFriendlyDropsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<uint32_t> DropType[] = {
		{ TranslationString("Money", true), 0xCE6FDD6B },
		{ TranslationString("RP", true), 0x2C014CA6 },
	};

	ScrollStruct<uint32_t> DropModels[] = {
		{ TranslationString("Money Bag", true), 0x113FD533 },
		{ TranslationString("Alien Egg", true), 0x6B795EBC },
		{ TranslationString("Weed", true), 0x1AFA6A0A },
		{ TranslationString("Beach Fire", true), 0xC079B265 },
		{ TranslationString("Xmas Tree", true), 0xE3BA450 },
		{ TranslationString("Bear", true), 0x772114C9 },
		{ TranslationString("Gold Bar", true), 0xF046EA37 },
		{ TranslationString("Guitar", true), 0xA4E7E0A7 },
		{ TranslationString("Dildo", true), 0xE6CB661E },
		{ TranslationString("Basketball", true), 0x6DB9599A },
		{ TranslationString("Bong", true), 0xF0A61994 },
		{ TranslationString("C4", true), 0xB4861EB7 },
		{ TranslationString("Ammo Box", true), 0x6DE6824F },
		{ TranslationString("Toilet", true), 0x6F9939C7 },
		{ TranslationString("Wheelchair", true), 0x4B3D240F }
	};

	ScrollStruct<int> DropStyles[] = {
		{ TranslationString("Traditional", true), 0 },
		{ TranslationString("Rain", true), 0 },
		{ TranslationString("Shower", true), 0 },
		{ TranslationString("Bait", true), 0 }
	};

	ScrollStruct<uint32_t> DropPickups[] = {
		{ TranslationString("Armor", true), 0x4BFB42D1 },
		{ TranslationString("Health Pack", true), 0x8F707C18 },
		{ TranslationString("Repair Kit", true), 0x098D79EF }
	};

	ScrollStruct<uint32_t> DropWeapons[] = {
		{ TranslationString("RPG", true), 0x4D36C349 },
		{ TranslationString("Minigun", true), 0x2F36B434 },
		{ TranslationString("Parachute", true), 0x6773257D },
		{ TranslationString("Fire Extinguisher", true), 0xCB13D282 },
		{ TranslationString("Flare", true), 0xE013E01C },
		{ TranslationString("Flare Gun", true), 0xBD4DE242 },
		{ TranslationString("Crowbar", true), 0x872DC888 },
		{ TranslationString("Firework", true), 0x22B15640 },
	};

	void SpawnDrop(Menu::PlayerVars Player, uint32_t Model) {
		Native::CreateAmbientPickup(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0, 1, 0, false, true);
	}
}

void NetworkFriendlyDropsMenu::Init() {
	SetName("Drops");
	SetParentSubmenu<NetworkPlayersFriendlyMenu>();

	addString("Drop Type");
	addString("Drop Pickup");
	addString("Drop Weapon");
	addString("Settings");
	addString("Money Model");
	addString("Style");
	addString("Delay");
	addString("Stealth");
	addString("The pickup will only be synced to this player");
}

void NetworkFriendlyDropsMenu::Update() {}

/*Called once on submenu open*/
void NetworkFriendlyDropsMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		addOption(ScrollOption<uint32_t>(TOGGLE, getString("Drop Type"))
			.addToggle(Menu::GetSelectedPlayer().m_DropToggle)
			.addScroll(Menu::GetSelectedPlayer().m_DropType, 0, NUMOF(DropType), DropType));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Drop Pickup"))
			.addScroll(Menu::GetSelectedPlayer().m_DropPickup, 0, NUMOF(DropPickups), DropPickups)
			.addOnClick([] { SpawnDrop(Menu::GetSelectedPlayer(), DropPickups[Menu::GetSelectedPlayer().m_DropPickup].m_Result); }));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Drop Weapon"))
			.addScroll(Menu::GetSelectedPlayer().m_DropWeapon, 0, NUMOF(DropWeapons), DropWeapons)
			.addOnClick([] { SpawnDrop(Menu::GetSelectedPlayer(), DropWeapons[Menu::GetSelectedPlayer().m_DropWeapon].m_Result); }));

		addOption(BreakOption(getString("Settings")));

		addOption(ToggleOption(getString("Stealth"))
			.addToggle(Menu::GetSelectedPlayer().m_DropStealth)
			.addTooltip(getString("The pickup will only be synced to this player")));

		addOption(ScrollOption<uint32_t>(SCROLL, getString("Money Model"))
			.addScroll(Menu::GetSelectedPlayer().m_DropMoneyModel, 0, NUMOF(DropModels), DropModels)
			.addRequirement([] {return Menu::GetSelectedPlayer().m_DropType == 0; }));

		addOption(ScrollOption<int>(SCROLL, getString("Style"))
			.addScroll(Menu::GetSelectedPlayer().m_DropMoneyStyle, 0, NUMOF(DropStyles), DropStyles));

		addOption(ScrollOption<int>(SCROLL, getString("Delay"))
			.addScroll(Menu::GetSelectedPlayer().m_DropMoneyDelay, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers));
	} else {
		addOption(ScrollOption<uint32_t>(TOGGLE, getString("Drop Type"))
			.addToggle(m_Vars.m_DropToggle)
			.addScroll(m_Vars.m_DropType, 0, NUMOF(DropType), DropType));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Drop Pickup"))
			.addScroll(m_Vars.m_DropPickup, 0, NUMOF(DropPickups), DropPickups)
			.addOnClick([] { SpawnDrop(Menu::GetSelectedPlayer(), DropPickups[m_Vars.m_DropPickup].m_Result); }));

		addOption(ScrollOption<uint32_t>(SCROLLSELECT, getString("Drop Weapon"))
			.addScroll(m_Vars.m_DropWeapon, 0, NUMOF(DropWeapons), DropWeapons)
			.addOnClick([] { SpawnDrop(Menu::GetSelectedPlayer(), DropWeapons[m_Vars.m_DropWeapon].m_Result); }));

		addOption(BreakOption(getString("Settings")));

		addOption(ScrollOption<uint32_t>(SCROLL, getString("Money Model"))
			.addScroll(m_Vars.m_DropMoneyModel, 0, NUMOF(DropModels), DropModels)
			.addRequirement([] {return m_Vars.m_DropType == 0; }));

		addOption(ScrollOption<int>(SCROLL, getString("Style"))
			.addScroll(m_Vars.m_DropMoneyStyle, 0, NUMOF(DropStyles), DropStyles));

		addOption(ScrollOption<int>(SCROLL, getString("Delay"))
			.addScroll(m_Vars.m_DropMoneyDelay, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers));
	}
}

/*Called always*/
void NetworkFriendlyDropsMenu::FeatureUpdate() {
	Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
		if (Player.m_DropToggle) {
			Math::Vector3<float> Coords;
			switch (Player.m_DropMoneyStyle) {
				case 0: Coords = Player.m_Coords; break;
				case 1: Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, Native::GetRandomFloatInRange(-3.f, 3.f), Native::GetRandomFloatInRange(-3.f, 3.f), Native::GetRandomFloatInRange(-3.f, 3.f)); break;
				case 2: Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, 0.f, 0.f, 2.f); break;
				case 3: Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, 0.f, 2.f, 0.f); break;
			}

			Menu::Timers::RunTimedFunction(&Player.m_DropMoneyTimer, Global::Arrays::g_Timers[Player.m_DropMoneyDelay].m_Result, [=] {
				auto Model = Player.m_DropType == 0 ? DropModels[Player.m_DropMoneyModel].m_Result : joaat("vw_prop_vw_colle_alien");
				Menu::GetControlManager()->RequestModel(Model, [=](uint32_t M) {
					Native::CreateAmbientPickup(DropType[Player.m_DropType].m_Result, Coords.m_X, Coords.m_Y, Coords.m_Z, 0, Player.m_DropType == 0 ? 2500 : 10, M, false, true);
				});
			});
		}
	}, true);

	if (m_Vars.m_DropToggle) {
		Menu::Timers::RunTimedFunction(&m_Vars.m_DropMoneyTimer, Global::Arrays::g_Timers[m_Vars.m_DropMoneyDelay].m_Result, [=] {
			Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
				Math::Vector3<float> Coords;
				switch (m_Vars.m_DropMoneyStyle) {
					case 0: Coords = Player.m_Coords; break;
					case 1: Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, Native::GetRandomFloatInRange(-3.f, 3.f), Native::GetRandomFloatInRange(-3.f, 3.f), Native::GetRandomFloatInRange(-3.f, 3.f)); break;
					case 2: Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, 0.f, 0.f, 2.f); break;
					case 3: Coords = Native::GetOffsetFromEntityInWorldCoords(Player.m_Ped, 0.f, 2.f, 0.f); break;
				}

				auto Model = m_Vars.m_DropType == 0 ? DropModels[m_Vars.m_DropMoneyModel].m_Result : joaat("vw_prop_vw_colle_alien");
				Menu::GetControlManager()->RequestModel(Model, [=](uint32_t M) {
					Native::CreateAmbientPickup(DropType[m_Vars.m_DropType].m_Result, Coords.m_X, Coords.m_Y, Coords.m_Z, 0, m_Vars.m_DropType == 0 ? 2500 : 10, M, false, true);
				});
			});
		});
	}
}

NetworkFriendlyDropsMenu* _instance;
NetworkFriendlyDropsMenu* NetworkFriendlyDropsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkFriendlyDropsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkFriendlyDropsMenu::~NetworkFriendlyDropsMenu() { delete _instance; }