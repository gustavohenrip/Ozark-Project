#include "network_players.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "menu/base/util/timers.hpp"
#include "rage/engine.hpp"
#include "players/network_players_selected.hpp"
#include "rage/types/global_types.hpp"
#include "menu/base/util/global.hpp"
#include "utils/caller.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/util/panels.hpp"
#include <algorithm>

using namespace NetworkPlayersMenuVars;

namespace NetworkPlayersMenuVars {
	Vars_ m_Vars;

	void RenderSprite(Menu::PlayerVars& Player, int Pos) {
		auto RenderBaseIcon = [] (const char* Name, int Pos) {
			Menu::GetRenderer()->DrawSprite("mpinventory", Name,
				{ Global::UIVars::g_MenuX + 0.004f + 0.007f, Global::UIVars::g_MenuY + (Pos * 0.0315f) + 0.005f + 0.012f },
				{ 0.017f * 0.8f, 0.027f * 0.8f }, 0.f, Global::UIVars::g_MainTextColor);
		};

		bool NetGameIsValid = ((uint64_t)Player.m_NetGamePlayer & 0xFFFFFFFF) != 0;
		if (NetGameIsValid && (Player.m_NetGamePlayer->m_IsRockstarDeveloper || Player.m_NetGamePlayer->m_IsRockstarQA)) {
			return RenderBaseIcon("custom_mission", Pos);
		}

		Blip BlipHandle = Menu::Global(2416085 + Player.m_ID + 1).As<Blip>();
		if (BlipHandle == 0) BlipHandle = Native::GetBlipFromEntity(Player.m_Ped);

		if (BlipHandle) {
			int BlipID = Native::GetBlipSprite(BlipHandle);

			if (BlipID > 0 && BlipID != 164) {
				char BlipName[20];
				sprintf(BlipName, "blip_%i", BlipID);

				static const uint64_t InvalidSprite = Rage::Engine::GetSpriteTexture("yes", "yes");
				uint64_t OzarkBlipSprite = Rage::Engine::GetSpriteTexture("OzarkBlips", BlipName);

				if (OzarkBlipSprite && OzarkBlipSprite != InvalidSprite) {
					struct Special {
						Math::Vector3_<float> m_Coords;
						int m_Sprite;
					};

					std::unordered_map<int, Special> SpecialBlips;
					float Rotation = 0.f;

					auto BlipList = Global::Vars::g_BlipList;
					for (int i = 0; i < 1500; i++) {
						Rage::Types::Blip* Blip = BlipList->m_Blips[i];
						if (Blip) {
							if (Blip->m_Handle == BlipHandle) {
								if (BlipID == 422) {
									Blip->m_Rotation += 5.f;
									if (Blip->m_Rotation == 360.f) Blip->m_Rotation = 0.f;
								}

								Rotation = Blip->m_Rotation;
							}

							if (Blip->m_Sprite == 72
								|| Blip->m_Sprite == 71
								|| Blip->m_Sprite == 73
								|| Blip->m_Sprite == 75
								|| Blip->m_Sprite == 110) {
								SpecialBlips[Blip->m_Handle] = { Blip->m_Coords, Blip->m_Sprite };
							}
						}
					}

					uint32_t BlipColorHex = (uint32_t)Native::GetBlipColour(BlipHandle);
					Color BlipColor = Color(BlipColorHex);
					if (BlipColorHex < 500) {
						BlipColor = Menu::Helpers::GetColorFromBlip(BlipColorHex);
					}

					if (!Native::IsPlayerControlOn(Player.m_ID)) {
						for (auto& SpecialBlip : SpecialBlips) {
							if (Math::Vector3_<float>::ToPadded(SpecialBlip.second.m_Coords).GetMathmaticalDistance(Native::GetBlipCoords(BlipHandle)) < 18.f) {
								sprintf(BlipName, "blip_%i", SpecialBlip.second.m_Sprite);
								BlipColor = { 255, 255, 255 };
								Rotation = 0.f;
								break;
							}
						}
					}

					Menu::GetRenderer()->DrawSprite("OzarkBlips", BlipName,
						{ Global::UIVars::g_MenuX + 0.004f + 0.007f, Global::UIVars::g_MenuY + (Pos * 0.0315f) + 0.005f + 0.012f },
						{ 0.017f * 0.8f, 0.027f * 0.8f }, Rotation, BlipColor);

					return;
				}
			}
		}

		if (!Player.m_InVehicle) {
			Menu::GetRenderer()->DrawSprite("OzarkBlips", "blip_1",
				{ Global::UIVars::g_MenuX + 0.004f + 0.007f, Global::UIVars::g_MenuY + (Pos * 0.0315f) + 0.005f + 0.012f },
				{ 0.017f * 0.8f, 0.027f * 0.8f }, 0.f, { 255, 255, 255, 255 });

			return;
		}

		uint32_t VehicleModel = Native::GetEntityModel(Player.m_Vehicle);
		if (Native::IsThisModelABicycle(VehicleModel)) {
			return RenderBaseIcon(Menu::GetBase()->IsOptionSelected(Pos) ? "mp_specitem_bike_black" : "mp_specitem_bike", Pos);
		}

		if (Native::IsThisModelABike(VehicleModel)) {
			return RenderBaseIcon(Menu::GetBase()->IsOptionSelected(Pos) ? "mp_specitem_bike_black" : "mp_specitem_bike", Pos);
		}

		if (Native::IsThisModelABoat(VehicleModel)) {
			return RenderBaseIcon(Menu::GetBase()->IsOptionSelected(Pos) ? "mp_specitem_boat_black" : "mp_specitem_boat", Pos);
		}

		if (Native::IsThisModelAHeli(VehicleModel)) {
			return RenderBaseIcon(Menu::GetBase()->IsOptionSelected(Pos) ? "mp_specitem_heli_black" : "mp_specitem_heli", Pos);
		}

		if (Native::IsThisModelAPlane(VehicleModel)) {
			return RenderBaseIcon(Menu::GetBase()->IsOptionSelected(Pos) ? "mp_specitem_plane_black" : "mp_specitem_plane", Pos);
		}

		return RenderBaseIcon(Menu::GetBase()->IsOptionSelected(Pos) ? "mp_specitem_car_black" : "mp_specitem_car", Pos);
	}

	std::string CalculateFlags(Menu::PlayerVars& Player) {
		std::string Return = "";

		bool NetGameIsValid = ((uint64_t)Player.m_NetGamePlayer & 0xFFFFFFFF) != 0;

#ifdef DEV
		if (Player.m_IsImpulseUser) {
			Return += "[IMP]";
		}
#endif

		if (Player.m_UsingGhost) {
			Return += "[GHOST]";
		}

		if (Player.m_IsOzarkUser) {
			Return += "~HUD_COLOUR_BLUEDARK~[Ozark]";
		}
		
		if (Player.m_Spectate) {
			Return += "~HUD_COLOUR_GREYLIGHT~[SPEC]";
		}

		if (Player.m_IsFriend) {
			Return += "~HUD_COLOUR_BLUE~[F]";
		}

		if (NetGameIsValid && Player.m_NetGamePlayer == Rage::Engine::GetHostNetGamePlayer()) {
			Return += "~HUD_COLOUR_BLUELIGHT~[H]";
		}

		if (Player.m_IsModder) {
			Return += "~g~[M]";
		}

		if (Player.m_IsGodmode) {
			Return += "~y~[G]";
		}

		if (Player.m_IsScriptHost) {
			Return += "~o~[SC-H]";
		}

		if (Player.m_ID > 31) {
			Return += "~r~[SCTV]";
		}

		if (Player.m_ID == Menu::GetLocalPlayer().m_ID) {
			Return += "~HUD_COLOUR_PINK~[ME]";
		}

		if (NetGameIsValid && Player.m_NetGamePlayer->m_IsRockstarDeveloper) {
			Return += "~c~[R*]";
		}

		if (NetGameIsValid && Player.m_NetGamePlayer->m_IsRockstarQA) {
			Return += "~c~[R*QA]";
		}

		return Return;
	}

	std::string CalculateName(Menu::PlayerVars& Player) {
		std::string Return = Player.m_Name;

		if (Player.m_NetGamePlayer && strlen(Player.m_NetGamePlayer->GetCrewName()) > 0) {
			std::string Name = Player.m_NetGamePlayer->GetCrewTag();
			std::transform(Name.begin(), Name.end(), Name.begin(), ::toupper);
			Return += " ~HUD_COLOUR_BRONZE~[" + Name + "]";
		}

		return Return;
	}
}

void NetworkPlayersMenu::Init() {
	SetName("Players");
	SetParentSubmenu<NetworkMenu>();

	NetworkPlayersSelectedMenu::GetInstance(); // Init
}

void NetworkPlayersMenu::Update() {
	static int Timer = 0;
	Menu::Timers::RunTimedFunction(&Timer, 250, [this] {
		ClearOptionsOffset(0);

		Menu::GetPlayerManager()->Foreach([this] (Menu::PlayerVars& Player) {
			if (!Player.IsValidName()) return;
			addOption(SubmenuOption(CalculateName(Player))
				.addSubmenu<NetworkPlayersSelectedMenu>()
				.addOnUpdate([&] (SubmenuOption* Option, int Pos) {
					Option->addTextOffset(0.016f);
					RenderSprite(Player, Pos);
				})
				.addOnHover([=] { m_Vars.m_SelectedPlayer = Player.m_ID; })
				.addOnClick([=] { if (Player.m_UsingGhost) Menu::GetSubmenuHandler()->SetSubmenu(NetworkPlayersMenu::GetInstance());  })
				.addSideText(CalculateFlags(Player)));
		}, true, true);
	});

	if (Menu::GetBase()->m_CurrentOption >= m_Options.size()) {
		Menu::GetBase()->m_CurrentOption = (int)m_Options.size() - 1;
	}
}

/*Called once on submenu open*/
void NetworkPlayersMenu::UpdateOnce() {
	Menu::Panels::GetPanelManager()->TogglePanelRender("PANEL_PLAYER_INFO", true);
}

/*Called always*/
void NetworkPlayersMenu::FeatureUpdate() {
	Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
		if (Player.m_Spectate) {
			if (!Menu::GetLocalPlayer().m_Alive) {
				Native::NetworkSetInSpectatorMode(false, Player.m_Ped);
			}
		}
	});
}

NetworkPlayersMenu* _instance;
NetworkPlayersMenu* NetworkPlayersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersMenu::~NetworkPlayersMenu() { delete _instance; }