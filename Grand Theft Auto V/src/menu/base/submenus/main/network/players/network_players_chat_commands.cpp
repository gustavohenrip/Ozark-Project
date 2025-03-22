#include "network_players_chat_commands.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "network_players_friendly.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/vehicle/vehicle_mods.hpp"
#include "menu/base/submenus/main/world/world_weather.hpp"
#include "menu/base/submenus/main/world/world_time.hpp"
#include "menu/base/submenus/main/network/players/network_players_friendly.hpp"
#include "menu/base/submenus/main/vehicle/vehicle_spawner.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/network/network_session.hpp"
#include "utils/fiber_pool.hpp"

using namespace NetworkPlayersChatCommandsMenuVars;

TranslationString T_ProcessedCommand("Processed command", true, true);
TranslationString T_From("from", true, true);

namespace NetworkPlayersChatCommandsMenuVars {
	Vars_ m_Vars;

	struct CommandContext {
		const char* m_Command;
		const char* m_Tooltip;
		int m_Element;
		std::function<void(Menu::PlayerVars&, std::vector<std::string>)> m_Callback;
	};

	auto DefaultCallback = [](Menu::PlayerVars&, std::vector<std::string>) {};

	ScrollStruct<CommandContext> Commands[] = {
		{ TranslationString("Give Weapons", true), { "weapons", "Gives them all weapons", 0, DefaultCallback } },
		{ TranslationString("Toggle Money", true), { "money", "Toggles money drop", 1, DefaultCallback } },
		{ TranslationString("Fix Vehicle", true), { "vfix", "Fixes their vehicle", 2, DefaultCallback } },
		{ TranslationString("Upgrade Vehicle", true), { "vupgrade", "Upgrades their vehicle", 3, DefaultCallback } },
		{ TranslationString("Set Weather", true), { "weather", "Sets the weather (check the sites list)", 4, DefaultCallback } },
		{ TranslationString("Set Time", true), { "time", "Sets the time (check the sites list)", 5, DefaultCallback } },
		{ TranslationString("Spawn Entity", true), { "spawn", "Spawns an entity (check the sites list)", 6, DefaultCallback } },
		{ TranslationString("Spawn Bodyguard", true), { "guard", "Spawns a bodyguard", 7, DefaultCallback } },
		{ TranslationString("Off the Radar", true), { "otr", "Gives them off the radar", 8, DefaultCallback } },
		{ TranslationString("Remove Wanted Level", true), { "nocops", "Removes their wanted level", 9, DefaultCallback } }
	};

	void CallCommand(const char* CommandName, Rage::Network::NetGamePlayer* Player, std::vector<std::string> Tokens) {
		Menu::PlayerVars& PlayerVar = Menu::GetPlayer(Player->m_ID);

		if (PlayerVar.m_ChatCommands) {
			for (ScrollStruct<CommandContext> C : Commands) {
				if (!strcmp(C.m_Result.m_Command, CommandName)) {
					if (PlayerVar.m_ChatCommand[C.m_Result.m_Element] || (m_Vars.m_ChatCommands && m_Vars.m_ChatCommand[C.m_Result.m_Element])) {
						Utils::GetFiberPool()->Push([=] {
							C.m_Result.m_Callback(Menu::GetPlayer(Player->m_ID), Tokens);
						});

						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s \"%s\" %s %s", TRANSLATE(T_ProcessedCommand), CommandName, TRANSLATE(T_From), PlayerVar.m_Name));
					}

					break;
				}
			}
		}
	}
}

void NetworkPlayersChatCommandsMenu::Init() {
	SetName("Chat Commands");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	// Give Weapons
	Commands[0].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		NetworkPlayersFriendlyMenuVars::GiveAllWeapons(Player);
	};

	// Toggle Money
	Commands[1].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		Player.m_DropToggle ^= true;
		Player.m_DropPickup = 0;
	};

	// Fix Vehicle
	Commands[2].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		if (Player.m_InVehicle) {
			Menu::GetControlManager()->RequestControl(Player.m_Vehicle, [](Entity Vehicle) {
				Native::SetVehicleFixed(Vehicle);
			});
		}
	};

	// Upgrade Vehicle
	Commands[3].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		if (Player.m_InVehicle) {
			Menu::GetControlManager()->RequestControl(Player.m_Vehicle, [](Entity Vehicle) {
				VehicleModsMenuVars::MaxUpgrades(Vehicle);
			});
		}
	};

	// Set Weather
	Commands[4].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		if (Tokens.size() == 2) {
			std::string WeatherType = Tokens.at(1);
			if (!WeatherType.empty()) {
				bool Sync = WorldWeatherMenuVars::m_Vars.m_Sync;

				switch (Native::GetHashKey(WeatherType.c_str())) {
				case 0x614a1f91: // sunny
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[0].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0x97aa0a79: // extrasunny
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[1].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xed00a767: // cloudy
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[2].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xc45c3225: // smoggy
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[3].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xae737644: // foggy
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[4].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xf3fcdf0a: // stormy
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[5].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xbb898d2d: // overcast
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[6].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xc88faa97: // lightsnow
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[7].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xefb6eff6: // snow
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[8].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0x27ea2814: // blizzard
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[9].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xaac9c895: // xmas
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[10].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;

				case 0xc91a3202: // halloween
					WorldWeatherMenuVars::m_Vars.m_Sync = true;
					WorldWeatherMenuVars::SetWeather(WorldWeatherMenuVars::Weathers[11].m_Result);
					WorldWeatherMenuVars::m_Vars.m_Sync = Sync;
					break;
				}
			}
		}
	};

	// Set Time
	Commands[5].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		if (Tokens.size() == 2) {
			std::string TimeType = Tokens.at(1);
			if (!TimeType.empty()) {
				switch (Native::GetHashKey(TimeType.c_str())) {
				case 0x765dba8c: // Day
					Native::NetworkOverrideClockTime(13, Native::GetClockMinutes(), Native::GetClockSeconds());
					WorldTimeMenuVars::SyncTime();
					break;

				case 0x5478de12: // Night
					Native::NetworkOverrideClockTime(22, Native::GetClockMinutes(), Native::GetClockSeconds());
					WorldTimeMenuVars::SyncTime();
					break;
				}
			}
		}
	};

	// Spawn Entity
	Commands[6].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		if (Tokens.size() == 3) {
			auto SpawnVehicle = [=](uint32_t Hash) {
				Menu::GetControlManager()->RequestModel(Hash, [=](uint32_t Model) {
					Vehicle Veh = Native::CreateVehicle(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Player.m_Heading, true, true, 0);
					if (Native::DoesEntityExist(Veh)) {
						Native::DecorSetInt(Veh, "MPBitset", (1 << 10));
						Native::SetVehicleIsStolen(Veh, false);
					}
				});
			};

			auto SpawnObject = [=](uint32_t Hash) {
				Menu::GetControlManager()->RequestModel(Hash, [=](uint32_t Model) {
					Native::CreateObject(Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, true, true, false);
				});
			};

			auto SpawnPed = [=](uint32_t Hash) {
				switch (Hash) {
					case 0x3F039CBA:
					case 0x856CFB02:
					case 0x2D7030F3:
					case 0x0703F106:
					case 0x431d501c:
					case 0x1CFC0E72:
					case 0x1E15B3EC:
					case 0x1CB5C0C3:
						return;
				}

				Menu::GetControlManager()->RequestModel(Hash, [=](uint32_t Model) {
					Native::CreatePed(13, Model, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Player.m_Heading, true, false);
				});
			};

			std::string StringName = Tokens.at(2);
			switch (Native::GetHashKey(Tokens.at(1).c_str())) {
			case 0xdd245b9c: // vehicle
				if (StringName[0] == '0' && (StringName[1] == 'x' || StringName[1] == 'X')) {
					// hash
					SpawnVehicle((uint32_t)_strtoui64(StringName.c_str(), 0, 0));
				}
				else {
					if (std::find_if(StringName.begin(), StringName.end(), [](unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
						// numeric
						SpawnVehicle((uint32_t)atoi(StringName.c_str()));
					}
					else {
						// name
						SpawnVehicle(Native::GetHashKey(StringName.c_str()));
					}
				}
				break;

			case 0x34d90761: // ped
				if (StringName[0] == '0' && (StringName[1] == 'x' || StringName[1] == 'X')) {
					// hash
					SpawnPed((uint32_t)_strtoui64(StringName.c_str(), 0, 0));
				}
				else {
					if (std::find_if(StringName.begin(), StringName.end(), [](unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
						// numeric
						SpawnPed((uint32_t)atoi(StringName.c_str()));
					}
					else {
						// name
						SpawnPed(Native::GetHashKey(StringName.c_str()));
					}
				}
				break;

			case 0x39958261: // object
				if (StringName[0] == '0' && (StringName[1] == 'x' || StringName[1] == 'X')) {
					// hash
					SpawnObject((uint32_t)_strtoui64(StringName.c_str(), 0, 0));
				}
				else {
					if (std::find_if(StringName.begin(), StringName.end(), [](unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
						// numeric
						SpawnObject((uint32_t)atoi(StringName.c_str()));
					}
					else {
						// name
						SpawnObject(Native::GetHashKey(StringName.c_str()));
					}
				}
				break;
			}
		}
	};

	// Spawn Bodyguard
	Commands[7].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		NetworkPlayersFriendlyMenuVars::SpawnBodyguards(Player, 1);
	};

	// Off the Radar
	Commands[8].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		NetworkPlayersFriendlyMenuVars::GiveOffTheRadar(Player);
	};

	// Remove Wanted Level
	Commands[9].m_Result.m_Callback = [](Menu::PlayerVars& Player, std::vector<std::string> Tokens) {
		NetworkPlayersFriendlyMenuVars::RemoveWantedLevel(Player);
	};

	addString("Toggle Chat Commands");
	addString("Send Command List");
	addString("Commands");
}

void NetworkPlayersChatCommandsMenu::Update() {}

/*Called once on submenu open*/
void NetworkPlayersChatCommandsMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersSelectedMenu>();

		addOption(ToggleOption(getString("Toggle Chat Commands"))
			.addToggle(Menu::GetSelectedPlayer().m_ChatCommands)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_ChatCommands); }));

		addOption(ButtonOption(getString("Send Command List"))
			.addOnClick([] { Menu::Helpers::SendTextMessage(Menu::GetSelectedPlayer().m_ID, Menu::GetLocalPlayer().m_ID, "Goto ozark.gg/chat to view commands given to you!"); }));

		addOption(BreakOption(getString("Commands")));

		for (auto& C : Commands) {
			addOption(ToggleOption(C.m_Name.Get())
				.addToggle(Menu::GetSelectedPlayer().m_ChatCommand[C.m_Result.m_Element])
				.addOnUpdate([=](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_ChatCommand[C.m_Result.m_Element]); })
				.addTooltip(std::string("?") + C.m_Result.m_Command + std::string(" - ") + C.m_Result.m_Tooltip));
		}
	} else {
		SetParentSubmenu<NetworkSessionMenu>();

		addOption(ToggleOption(getString("Toggle Chat Commands"))
			.addToggle(m_Vars.m_ChatCommands));

		addOption(ButtonOption(getString("Send Command List"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
					Menu::Helpers::SendTextMessage(Player.m_ID, Menu::GetLocalPlayer().m_ID, "Goto ozark.gg/chat to view commands given to you!");
				});
			}));

		addOption(BreakOption(getString("Commands")));

		for (auto& C : Commands) {
			addOption(ToggleOption(C.m_Name.Get())
				.addToggle(m_Vars.m_ChatCommand[C.m_Result.m_Element])
				.addTooltip(std::string("?") + C.m_Result.m_Command + std::string(" - ") + C.m_Result.m_Tooltip));
		}
	}
}
/*Called always*/
void NetworkPlayersChatCommandsMenu::FeatureUpdate() {}

NetworkPlayersChatCommandsMenu* _instance;
NetworkPlayersChatCommandsMenu* NetworkPlayersChatCommandsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersChatCommandsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersChatCommandsMenu::~NetworkPlayersChatCommandsMenu() { delete _instance; }