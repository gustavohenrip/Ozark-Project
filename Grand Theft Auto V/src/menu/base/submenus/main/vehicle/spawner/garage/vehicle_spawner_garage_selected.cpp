#include "vehicle_spawner_garage_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_spawner_garage.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/network/players/network_players_spawn.hpp"
#include "../vehicle_spawner_settings.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/submenus/main/vehicle/spawner/vehicle_spawner_manage.hpp"
#include "menu/base/util/global.hpp"

using namespace VehicleSpawnerGarageSelectedMenuVars;

namespace VehicleSpawnerGarageSelectedMenuVars {
	Vars_ m_Vars;

	enum GlobalGarageIndices { // void func_355(bool bParam0, var uParam1)
		Garage_PlateType = 0,
		Garage_PlateText = 1,
		Garage_PaintPrimary = 5,
		Garage_PaintSecondary = 6,
		Garage_PaintPearlescant = 7,
		Garage_PaintRim = 8,
		Garage_ModCount = 9,
		Garage_Spoiler = 10,
		Garage_FrontBumper = 11,
		Garage_RearBumper = 12,
		Garage_Skirts = 13,
		Garage_Exhaust = 14,
		Garage_Frame = 15,
		Garage_Grille = 16,
		Garage_Hood = 17,
		Garage_LeftFender = 18,
		Garage_RightFender = 19,
		Garage_Roof = 20,
		Garage_Engine = 21,
		Garage_Brakes = 22,
		Garage_Transmission = 23,
		Garage_Horn = 24,
		Garage_Suspension = 25,
		Garage_Armor = 26,
		Garage_UNK_17 = 27,
		Garage_Turbo = 28,
		Garage_UNK_19 = 29,
		Garage_TireSmoke = 30,
		Garage_UNK_21 = 31,
		Garage_Xenon = 32,
		Garage_FrontRims = 33,
		Garage_RearRims = 34,

		// Bennys
		Garage_PlateHolder,
		Garage_VanityPlates,
		Garage_TrimDesign,
		Garage_Ornaments,
		Garage_Dashboard,
		Garage_Dial,
		Garage_DoorSpeaker,
		Garage_Seats,
		Garage_SteeringWheel,
		Garage_ShifterLeavers,
		Garage_Plaques,
		Garage_Speakers,
		Garage_Trunk,
		Garage_Hydrulics,
		Garage_EngineBlock,
		Garage_AirFilter,
		Garage_Struts,
		Garage_ArchCovers,
		Garage_Aerials,
		Garage_Trim,
		Garage_Tank,
		Garage_Windows,
		Garage_UNK47,
		Garage_Livery,
		Garage_FrontWheelCustom,
		Garage_RearWheelCustom,

		Garage_SmokeR = 62,
		Garage_SmokeG = 63,
		Garage_SmokeB = 64,

		Garage_WindowTint = 65,
		Garage_Model = 66,
		Garage_ConvertibleRoofState = 68,
		Garage_WheelType = 69,
		Garage_CustomR = 71,
		Garage_CustomG = 72,
		Garage_CustomB = 73,
		Garage_NeonR = 74,
		Garage_NeonG = 75,
		Garage_NeonB = 76,
		Garage_AppearanceBitset = 77,				// Custom Paints
		/**/Garage_AppearanceBitset_SecondaryCustom = 12,
		Garage_GeneralBitset = 102,					// Insurance
		Garage_DefaultRadioStation = 120,
	};

	template<typename T>
	T GetSlotTrait(int Slot, int Trait) {
		return *Menu::Global(1323678).At(Slot, 141).At(Trait).Get<T>();
	}

	template<typename T>
	void SetSlotTrait(int Slot, int Trait, T Value) {
		Menu::Global(1323678).At(Slot, 141).At(Trait).As<T>() = Value;
		Native::SetBit(Menu::Global(1323678).At(Slot, 141).At(Garage_GeneralBitset).Get<int>(), 12);
	}

	namespace Scripts {
		int func_1086(int iParam0, int iParam1) {
			switch (iParam0) {
				case joaat("faction2"):
				case joaat("buccaneer2"):
				case joaat("chino2"):
				case joaat("moonbeam2"):
				case joaat("primo2"):
				case joaat("voodoo"):
					return 1;
					break;

				case joaat("sabregt2"):
					if (!Menu::Global(262145).At(14227).As<int>()) {
						return 0;
					} else {
						return 1;
					}
					break;

				case joaat("tornado5"):
					if (!Menu::Global(262145).At(14228).As<int>()) {
						return 0;
					} else {
						return 1;
					}
					break;

				case joaat("virgo2"):
					if (!Menu::Global(262145).At(14226).As<int>()) {
						return 0;
					} else {
						return 1;
					}
					break;

				case joaat("minivan2"):
					if (!Menu::Global(262145).At(14229).As<int>()) {
						return 0;
					} else {
						return 1;
					}
					break;

				case joaat("slamvan3"):
					if (!Menu::Global(262145).At(14231).As<int>()) {
						return 0;
					} else {
						return 1;
					}
					break;

				case joaat("faction3"):
					if (!Menu::Global(262145).At(14230).As<int>()) {
						return 0;
					} else {
						return 1;
					}
					break;

				case joaat("sultanrs"):
				case joaat("banshee2"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("comet3"):
					if (Menu::Global(262145).At(18677).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("diablous2"):
					if (Menu::Global(262145).At(18679).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("fcr2"):
					if (Menu::Global(262145).At(18683).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("elegy"):
					if (Menu::Global(262145).At(18680).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("nero2"):
					if (Menu::Global(262145).At(18687).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("italigtb2"):
					if (Menu::Global(262145).At(18685).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("specter2"):
					if (Menu::Global(262145).At(18690).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("technical3"):
					if (Menu::Global(262145).At(20635).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("insurgent3"):
					if (Menu::Global(262145).At(20636).As<int>()) {
						if ((iParam1 & 1) != 0) {
							return 0;
						}
						return 1;
					}
					return 0;
					break;

				case joaat("slamvan4"):
				case joaat("slamvan5"):
				case joaat("slamvan6"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("issi4"):
				case joaat("issi5"):
				case joaat("issi6"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("impaler2"):
				case joaat("impaler3"):
				case joaat("impaler4"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("deathbike"):
				case joaat("deathbike2"):
				case joaat("deathbike3"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("monster3"):
				case joaat("monster4"):
				case joaat("monster5"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("dominator4"):
				case joaat("dominator5"):
				case joaat("dominator6"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("bruiser"):
				case joaat("bruiser2"):
				case joaat("bruiser3"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("youga3"):
				case joaat("gauntlet5"):
				case joaat("yosemite3"):
					if ((iParam1 & 1) != 0) {
						return 0;
					}
					return 1;
					break;

				case joaat("manana2"):
				case joaat("peyote3"):
				case joaat("glendale2"):
					return 1;
					break;
			}
			return 0;
		}

		int func_1085(int iParam0, int iParam1) {
			int iVar0;
			int iVar1;
			float fVar2;
			int iVar3;

			if (Native::DoesEntityExist(iParam0) && Native::IsVehicleDriveable(iParam0, 0)) {
				switch (Native::GetEntityModel(iParam0)) {
					case joaat("tornado5"):
						switch (iParam1) {
							case 0:
								return 0;
								break;

							case 1:
								return 1;
								break;

							case 2:
								return 2;
								break;

							case 3:
								return 3;
								break;

							case 4:
								return 4;
								break;

							case 5:
								return 4;
								break;
						}
						break;

					case joaat("faction3"):
						return 3;
						break;
				}
				iVar0 = Native::GetNumVehicleMods(iParam0, 38);
				iVar1 = Native::GetNumVehicleMods(iParam0, 24);
				fVar2 = (float(iParam1 + 1) / float(iVar0));
				iVar3 = (floor((float(iVar1) * fVar2)) - 1);
				if (iVar3 < 0) {
					iVar3 = 0;
				}
				if (iVar3 >= iVar0) {
					iVar3 = (iVar0 - 1);
				}
				return iVar3;
			}
			return 0;
		}

		bool ScriptApplyMods(Vehicle* iParam0, int Slot) {
			int iVar0;

			if (!Native::IsVehicleDriveable(*iParam0, 0)) {
				return 0;
			}

			if (Native::GetNumModKits(*iParam0) == 0) {
				return 0;
			}

			Native::SetVehicleModKit(*iParam0, 0);

			iVar0 = 0;
			while (iVar0 < GetSlotTrait<int>(Slot, 9)) {
				if ((((iVar0 == 17 || iVar0 == 18) || iVar0 == 19) || iVar0 == 20) || iVar0 == 21) {
					Native::ToggleVehicleMod(*iParam0, iVar0, GetSlotTrait<int>(Slot, 10 + iVar0) > 0);
				} else if (iVar0 == 22) {
					if (GetSlotTrait<int>(Slot, 10 + iVar0) > 0) {
						Native::ToggleVehicleMod(*iParam0, iVar0, true);
						if (GetSlotTrait<int>(Slot, 10 + iVar0) == 1) {
							Native::SetVehicleXenonLightsColour(*iParam0, 255);
						} else {
							Native::SetVehicleXenonLightsColour(*iParam0, (GetSlotTrait<int>(Slot, 10 + iVar0) - 2));
						}
					} else {
						Native::ToggleVehicleMod(*iParam0, iVar0, false);
					}
				} else if (Native::GetVehicleMod(*iParam0, iVar0) != (GetSlotTrait<int>(Slot, 10 + iVar0) - 1)) {
					Native::RemoveVehicleMod(*iParam0, iVar0);
					if (GetSlotTrait<int>(Slot, 10 + iVar0) > 0) {
						if (iVar0 == 23) {
							Native::SetVehicleMod(*iParam0, iVar0, (GetSlotTrait<int>(Slot, 10 + iVar0) - 1), GetSlotTrait<int>(Slot, Garage_FrontWheelCustom) > 0);
						} else if (iVar0 == 24) {
							Native::SetVehicleMod(*iParam0, iVar0, (GetSlotTrait<int>(Slot, 10 + iVar0) - 1), GetSlotTrait<int>(Slot, Garage_RearWheelCustom) > 0);
						} else {
							Native::SetVehicleMod(*iParam0, iVar0, (GetSlotTrait<int>(Slot, 10 + iVar0) - 1), false);
						}
					}
				}

				iVar0++;
			}

			if (func_1086(Native::GetEntityModel(*iParam0), 1) && Native::GetVehicleMod(*iParam0, 24) != func_1085(*iParam0, (GetSlotTrait<int>(Slot, 39) - 1))) {
				Native::SetVehicleMod(*iParam0, 24, func_1085(*iParam0, (GetSlotTrait<int>(Slot, 39) - 1)), false);
			}

			Native::SetVehicleStrong(*iParam0, true);
			Native::SetVehicleHasStrongAxles(*iParam0, true);

			return 1;
		}
	}

	void ApplyMods(Vehicle Veh, int Slot) {
		Native::SetVehicleNumberPlateTextIndex(Veh, GetSlotTrait<int>(Slot, Garage_PlateType));
		Native::SetVehicleNumberPlateText(Veh, (const char*)Menu::Global(1323678).At(Slot, 141).At(Garage_PlateText).Get<uint64_t>());

		Native::SetVehicleWheelType(Veh, GetSlotTrait<int>(Slot, Garage_WheelType));

		Scripts::ScriptApplyMods(&Veh, Slot);
		Native::SetVehicleWindowTint(Veh, GetSlotTrait<int>(Slot, Garage_WindowTint));

		int ConvertibleRoofState = GetSlotTrait<int>(Slot, Garage_ConvertibleRoofState);
		if ((ConvertibleRoofState == 0 || ConvertibleRoofState == 3) || ConvertibleRoofState == 5) {
			Native::RaiseConvertibleRoof(Veh, true);
		} else {
			Native::LowerConvertibleRoof(Veh, true);
		}

		Native::_SetVehicleNeonLightEnabled(Veh, 0, Native::IsBitSet(GetSlotTrait<int>(Slot, Garage_AppearanceBitset), 30));
		Native::_SetVehicleNeonLightEnabled(Veh, 1, Native::IsBitSet(GetSlotTrait<int>(Slot, Garage_AppearanceBitset), 31));
		Native::_SetVehicleNeonLightEnabled(Veh, 2, Native::IsBitSet(GetSlotTrait<int>(Slot, Garage_AppearanceBitset), 28));
		Native::_SetVehicleNeonLightEnabled(Veh, 3, Native::IsBitSet(GetSlotTrait<int>(Slot, Garage_AppearanceBitset), 29));

		int Pearlescant = GetSlotTrait<int>(Slot, Garage_PaintPearlescant);
		if (Pearlescant < 0) { Pearlescant = 0; }

		int Rim = GetSlotTrait<int>(Slot, Garage_PaintRim);
		if (Rim < 0) { Rim = 0; }

		Native::SetVehicleExtraColours(Veh, Pearlescant, Rim);

		if (GetSlotTrait<int>(Slot, Garage_PaintPrimary) != -1 && GetSlotTrait<int>(Slot, Garage_PaintSecondary) != -1) {
			Native::SetVehicleColours(Veh, GetSlotTrait<int>(Slot, Garage_PaintPrimary), GetSlotTrait<int>(Slot, Garage_PaintSecondary));
		}

		Color SmokeColor;
		SmokeColor.R = GetSlotTrait<int>(Slot, Garage_SmokeR);
		SmokeColor.G = GetSlotTrait<int>(Slot, Garage_SmokeG);
		SmokeColor.B = GetSlotTrait<int>(Slot, Garage_SmokeB);

		Native::SetVehicleTyreSmokeColor(Veh, SmokeColor.R, SmokeColor.G, SmokeColor.B);

		Color NeonColor;
		NeonColor.R = GetSlotTrait<int>(Slot, Garage_NeonR);
		NeonColor.G = GetSlotTrait<int>(Slot, Garage_NeonG);
		NeonColor.B = GetSlotTrait<int>(Slot, Garage_NeonB);

		Native::_SetVehicleNeonLightsColour(Veh, NeonColor.R, NeonColor.G, NeonColor.B);

		Color CustomColor;
		CustomColor.R = GetSlotTrait<int>(Slot, Garage_CustomR);
		CustomColor.G = GetSlotTrait<int>(Slot, Garage_CustomG);
		CustomColor.B = GetSlotTrait<int>(Slot, Garage_CustomB);

		if (Native::IsBitSet(GetSlotTrait<int>(Slot, Garage_AppearanceBitset), 13)) {
			Native::SetVehicleCustomPrimaryColour(Veh, CustomColor.R, CustomColor.G, CustomColor.B);
		}

		if (Native::IsBitSet(GetSlotTrait<int>(Slot, Garage_AppearanceBitset), 12)) {
			Native::SetVehicleCustomSecondaryColour(Veh, CustomColor.R, CustomColor.G, CustomColor.B);
		}
	}

	void SpawnVehicle(int Slot) {
		Menu::GetControlManager()->RequestModel(GetSlotTrait<uint32_t>(Slot, Garage_Model), [=] (uint32_t Hash) {
			Menu::PlayerVars Target;

			if (!Menu::GetSubmenuHandler()->GetPreviousSubmenu()) {
				Target = Menu::GetLocalPlayer();
			} else {
				if (Menu::GetSubmenuHandler()->GetPreviousSubmenu()->m_Parent == NetworkPlayersSpawnMenu::GetInstance()) {
					Target = Menu::GetSelectedPlayer();
				} else {
					Target = Menu::GetLocalPlayer();
				}
			}

			bool Network = Target.m_ID != Menu::GetLocalPlayer().m_ID;

			bool SpawnVectored = false;
			float OldSpeed = 0.f;

			if (VehicleSpawnerSettingsMenuVars::m_Vars.m_DeleteCurrent && !Network) {
				if (Target.m_InVehicle) {
					if (Native::GetPedInVehicleSeat(Target.m_Vehicle, -1, 0) == Target.m_Ped) {
						// It's my vehicle
						OldSpeed = Native::GetEntitySpeed(Target.m_Vehicle);
						Native::SetEntityAsMissionEntity(Target.m_Vehicle, true, true);
						Native::DeleteVehicle(&Target.m_Vehicle);
					} else {
						SpawnVectored = true;
					}
				}
			}
			
			Entity Vehicle = 0;
			if (!Network && VehicleSpawnerSettingsMenuVars::m_Vars.m_TeleportInside && !SpawnVectored) {
				Vehicle = Native::CreateVehicle(Hash, Target.m_Coords.m_X, Target.m_Coords.m_Y, Target.m_Coords.m_Z, Target.m_Heading, true, true, 0);
			} else {
				auto Coords = Target.m_Coords + (Native::GetEntityForwardVector(Target.m_Entity) * 5.f);
				Vehicle = Native::CreateVehicle(Hash, Coords.m_X, Coords.m_Y, Coords.m_Z, Target.m_Heading, true, true, 0);
			}

			if (Native::DoesEntityExist(Vehicle)) {
				auto Coords = Native::GetEntityCoords(Vehicle, true);
				Native::SetVehicleOnGroundProperly(Vehicle, 1);

				Native::SetVehicleDirtLevel(Vehicle, 0);
				Native::SetVehRadioStation(Vehicle, "OFF");

				if (VehicleSpawnerSettingsMenuVars::m_Vars.m_AirVehiclesInAir) {
					if (Native::IsThisModelAPlane(Hash) || Native::IsThisModelAHeli(Hash)) {
						Math::Vector3<float> Coords = Native::GetEntityCoords(Vehicle, true);
						Native::SetEntityCoords(Vehicle, Coords.m_X, Coords.m_Y, Coords.m_Z + VehicleSpawnerSettingsMenuVars::m_Vars.m_SpawnHeight, 0, 0, 0, 0);
						Native::SetVehicleEngineOn(Vehicle, true, true, false);
						Native::SetHeliBladesFullSpeed(Vehicle);
						Native::SetVehicleForwardSpeed(Vehicle, 100);
					}
				}

				if (VehicleSpawnerSettingsMenuVars::m_Vars.m_Fade) {
					Native::NetworkFadeInEntity(Vehicle, true, false);
				}

				Native::DecorSetInt(Vehicle, "MPBitset", (1 << 10));
				Native::SetVehicleIsStolen(Vehicle, false);

				ApplyMods(Vehicle, Slot);

				if (VehicleSpawnerSettingsMenuVars::m_Vars.m_TeleportInside && !Network) {
					Native::SetPedIntoVehicle(Target.m_Ped, Vehicle, -1);
					Native::SetVehicleEngineOn(Vehicle, true, true, false);
					Native::SetVehicleForwardSpeed(Vehicle, OldSpeed);
				}

				if (VehicleSpawnerSettingsMenuVars::m_Vars.m_Godmode) {
					Native::SetEntityInvincible(Vehicle, true);
					Native::SetEntityProofs(Vehicle, true, true, true, true, true, true, true, true);
					Native::SetVehicleCanBeVisiblyDamaged(Vehicle, false);
					Native::SetVehicleEngineOn(Vehicle, true, true, true);
				}

				if (VehicleSpawnerSettingsMenuVars::m_Vars.m_ParticleFX) {
					Native::RequestNamedPtfxAsset("proj_indep_firework_v2");
					Native::_UseParticleFxAssetNextCall("proj_indep_firework_v2");
					Native::_StartParticleFxNonLoopedAtCoord2("scr_firework_indep_repeat_burst_rwb", Coords.m_X, Coords.m_Y, Coords.m_Z, 0, 0, 0, 1.2f, true, true, true);
				}

				if (VehicleSpawnerSettingsMenuVars::m_Vars.m_Notification) {
					Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s!", Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Hash)), VehicleSpawnerGarageSelectedMenu::GetInstance()->getString("spawned").c_str()));
				}

				VehicleSpawnerManageMenuVars::m_Vars.m_SpawnedVehicles.push_back({ Vehicle, Hash, Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Hash)), Native::GetGameTimer() });
			}
		});
	}

	namespace Scripts {
		int func_24(int iParam0) {
			int iVar0;
			int iVar1;

			iVar0 = iParam0;
			if (iVar0 == -1) {
				iVar1 = Menu::Global(1312763).As<int>();
				if (iVar1 > -1) {
					Menu::Global(2551772).As<int>() = 0;
					iVar0 = iVar1;
				} else {
					iVar0 = 0;
					Menu::Global(2551772).As<int>() = 1;
				}
			}
			return iVar0;
		}

		void func_41(int iParam0, int iParam1, int iParam2, int iParam3, bool bParam4) {
			int iVar0;

			if (bParam4) {
			}
			iVar0 = Menu::Global(2552060).At(iParam0, 3).At(func_24(iParam2), 1).As<int>();
			if (iVar0 != 0) {
				Native::StatSetInt(iVar0, iParam1, iParam3);
			}
		}

		int func_285(int iParam0) {
			if (iParam0 != -1) {
				return Native::IsBitSet(Menu::Global(2426097).At(iParam0, 443).At(199).As<int>(), 9);
			}
			return 0;
		}

		int func_609(int iParam0) {
			iParam0 = (iParam0 - 1000);
			if (iParam0 >= 0 && iParam0 <= 4) {
				return iParam0;
			}
			return -1;
		}

		int func_608(int iParam0) {
			switch (iParam0) {
				case 0:
					return 157;

				case 2:
					return 224;

				case 1:
					return 227;

				case 3:
					return 279;
			}
			return -1;
		}

		int func_607(int iParam0) {
			if (iParam0 != -1) {
				return Native::IsBitSet(Menu::Global(1590682).At(iParam0, 883).At(274).At(351).As<int>(), 2);
			}
			return 0;
		}

		bool func_606(int iParam0, bool bParam1) {
			if (Menu::Global(1590518).As<int>() != -1) {
				if (!func_607(Menu::Global(1590518).As<int>())) {
					return 0;
				}
				if (bParam1) {
					if (Menu::GetLocalPlayer().m_ID != Menu::Global(1590518).As<int>()) {
						if (Native::IsBitSet(Menu::Global(2426097).At(Menu::Global(1590518).As<int>(), 443).At(199).As<int>(), 24) || func_285(Menu::Global(1590518).As<int>())) {
							return 1;
						}
					}
				}
			}
			return Native::IsBitSet(Menu::Global(2426097).At(iParam0, 443).At(199).As<int>(), 24);
		}

		int func_605(int iParam0, bool bParam1, bool bParam2) {
			if (bParam2) {
				return func_606(Menu::GetLocalPlayer().m_ID, 0);
			}
			if (bParam1) {
				if (func_606(Menu::GetLocalPlayer().m_ID, 0)) {
					return 0;
				}
				switch (iParam0) {
					case 103:
					case 106:
					case 109:
					case 112:
					case 104:
					case 107:
					case 110:
					case 113:
					case 105:
					case 108:
					case 111:
					case 114:
						return 1;
						break;
				}
			}
			switch (iParam0) {
				case 103:
				case 106:
				case 109:
				case 112:
				case 104:
				case 107:
				case 110:
				case 113:
				case 105:
				case 108:
				case 111:
				case 114:
					return 1;
					break;
			}
			return 0;
		}

		int func_604(int iParam0) {
			switch (iParam0) {
				case 87:
				case 88:
				case 89:
				case 90:
					return 1;
					break;
			}
			return 0;
		}

		int func_603(int iParam0, int iParam1) {
			if (iParam1 == -1) {
				switch (iParam0) {
					case 91:
					case 92:
					case 93:
					case 94:
					case 95:
					case 96:
					case 97:
					case 98:
					case 99:
					case 100:
					case 101:
					case 102:
						return 1;
						break;
				}
			} else if (iParam1 == 91) {
				switch (iParam0) {
					case 91:
					case 92:
					case 93:
					case 94:
					case 95:
					case 96:
						return 1;
						break;
				}
			} else if (iParam1 == 97) {
				switch (iParam0) {
					case 97:
					case 98:
					case 99:
					case 100:
					case 101:
					case 102:
						return 1;
						break;
				}
			}
			return 0;
		}

		int func_602(int iParam0, int iParam1, bool bParam2) {
			if (iParam0 == -1) {
				if (iParam1 >= 1) {
					if (func_605(iParam1, 0, 0)) {
						return 20;
					} else if (func_604(iParam1)) {
						return 0;
					} else if (func_603(iParam1, -1)) {
						return 10;
					} else if (iParam1 == 115) {
						return 8;
					} else if (iParam1 == 116) {
						return 20;
					} else if (iParam1 == 117) {
						return 7;
					} else if (iParam1 == 118) {
						return 1;
					} else if ((iParam1 == 119 || iParam1 == 120) || iParam1 == 121) {
						return 10;
					} else if (iParam1 == 122) {
						return 9;
					} else if (iParam1 == 123 || iParam1 == 124) {
						return 10;
					} else if (iParam1 == 125) {
						return 10;
					} else if (iParam1 == 126) {
						return 10;
					} else if (iParam1 <= 126 && iParam1 > 0) {
						if (Menu::Global(1049924).At(iParam1, 1951).At(33).As<int>() == 2) {
							if (bParam2) {
								return 3;
							} else {
								return 2;
							}
						} else if (Menu::Global(1049924).At(iParam1, 1951).At(33).As<int>() == 6) {
							if (bParam2) {
								return 8;
							} else {
								return 6;
							}
						} else if (Menu::Global(1049924).At(iParam1, 1951).At(33).As<int>() == 10) {
							if (bParam2) {
								return 13;
							} else {
								return 10;
							}
						}
					}
				}
			}
			switch (iParam0) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 7:
				case 23:
				case 24:
					return 13;
					break;

				case 5:
					return 0;
					break;

				case 6:
					return 10;
					break;

				case 8:
				case 9:
				case 10:
					return 20;
					break;

				case 11:
					return 8;
					break;

				case 12:
					return 20;
					break;

				case 13:
					return 7;
					break;

				case 14:
					return 1;
					break;

				case 15:
				case 16:
				case 17:
					return 10;
					break;

				case 18:
				case 19:
				case 20:
					return 10;
					break;

				case 21:
					return 10;
					break;

				case 22:
					return 10;
					break;
			}
			return 0;
		}

		int func_601(int iParam0) {
			int iVar0;

			switch (iParam0) {
				case 8:
					return 108;
					break;

				case 9:
					return 128;
					break;

				case 10:
					return 148;
					break;

				case 11:
					return 156;
					break;

				case 6:
					return 75;
					break;

				case 7:
					return 88;
					break;

				case 5:
					return -1;
					break;

				case 12:
					return 179;
					break;

				case 13:
					return 186;
					break;

				case 14:
					return 192;
					break;

				case 15:
					return 202;
					break;

				case 16:
					return 212;
					break;

				case 17:
					return 222;
					break;

				case 18:
					return 236;
					break;

				case 19:
					return 246;
					break;

				case 20:
					return 256;
					break;

				case 21:
					return 268;
					break;

				case 22:
					return 278;
					break;

				case 23:
					return 294;
					break;

				case 24:
					return 307;
					break;
			}
			if (iParam0 >= 1000) {
				iVar0 = func_609(iParam0);
				return func_608(iVar0);
			}
			return (func_602(iParam0, -1, 1) * iParam0 + 1);
		}

		int func_600(int iParam0) {
			if (iParam0 < 10) {
				return (7263 + iParam0);
			} else if (iParam0 > 12 && iParam0 < 23) {
				return (7263 + (iParam0 - 3));
			} else if (iParam0 > 25 && iParam0 < 36) {
				return (7263 + (iParam0 - 6));
			} else if (iParam0 > 38 && iParam0 < 49) {
				return (7263 + (iParam0 - 9));
			} else if (iParam0 > 51 && iParam0 < 62) {
				return ((7263 + iParam0) - 12);
			} else if (iParam0 > 64 && iParam0 < 85) {
				return ((9330 + iParam0) - 65);
			} else if (iParam0 >= 281 && iParam0 < 291) {
				return ((30154 + iParam0) - 281);
			} else if (iParam0 >= 294 && iParam0 < 304) {
				return (30154 + ((iParam0 - 281) - 3));
			} else if (iParam0 < func_601(11)) {
				return ((15154 + iParam0) - 88);
			} else if (iParam0 <= 157) {
				return ((15154 + iParam0) - 88);
			} else if (iParam0 == 158) {
				return 15362;
			} else if (iParam0 < 184) {
				return ((18061 + iParam0) - 159);
			} else if (iParam0 < 194) {
				return ((18963 + iParam0) - 184);
			} else if (iParam0 < 231) {
				return ((21979 + iParam0) - 194);
			} else if (iParam0 < 261) {
				return ((24565 + iParam0) - 231);
			} else if (iParam0 < 271) {
				return ((26329 + iParam0) - 261);
			} else if (iParam0 < 281) {
				return ((28049 + iParam0) - 271);
			} else if (iParam0 < 281) {
				return ((28049 + iParam0) - 271);
			}
			return 0;
		}

		int func_599(int iParam0, int iParam1) {
			switch (iParam0) {
				case 1:
					switch (iParam1) {
						case 0:
							return 1629;

						case 1:
							return 1636;

						case 2:
							return 1643;

						case 3:
							return 1650;

						case 4:
							return 1657;

						case 5:
							return 1664;

						case 6:
							return 1671;

						case 7:
							return 1678;

						case 8:
							return 1685;

						case 9:
							return 1692;

						case 10:
							return 1699;

						case 11:
							return 1705;

						case 12:
							return 1711;

						case 13:
							return 1717;

						case 14:
							return 1796;

						case 15:
							return 1803;

						case 16:
							return 1810;

						case 17:
							return 1817;

						case 18:
							return 1824;

						case 19:
							return 1831;

						case 20:
							return 1838;

						case 21:
							return 1845;

						case 22:
							return 1852;

						case 23:
							return 1859;

						case 24:
							return 1865;

						case 25:
							return 1871;

						case 26:
							return 2166;

						case 27:
							return 2173;

						case 28:
							return 2180;

						case 29:
							return 2187;

						case 30:
							return 2194;

						case 31:
							return 2201;

						case 32:
							return 2208;

						case 33:
							return 2215;

						case 34:
							return 2222;

						case 35:
							return 2229;

						case 36:
							return 2236;

						case 37:
							return 2242;

						case 38:
							return 2248;

						case 39:
							return 2829;

						case 40:
							return 2836;

						case 41:
							return 2843;

						case 42:
							return 2850;

						case 43:
							return 2857;

						case 44:
							return 2864;

						case 45:
							return 2871;

						case 46:
							return 2878;

						case 47:
							return 2885;

						case 48:
							return 2892;

						case 49:
							return 2899;

						case 50:
							return 2905;

						case 51:
							return 2911;

						case 52:
							return 2953;

						case 53:
							return 2960;

						case 54:
							return 2967;

						case 55:
							return 2974;

						case 56:
							return 2981;

						case 57:
							return 2988;

						case 58:
							return 2995;

						case 59:
							return 3002;

						case 60:
							return 3009;

						case 61:
							return 3016;

						case 62:
							return 3023;

						case 63:
							return 3029;

						case 64:
							return 3035;

						case 65:
							return 3236;

						case 66:
							return 3244;

						case 67:
							return 3252;

						case 68:
							return 3260;

						case 69:
							return 3268;

						case 70:
							return 3276;

						case 71:
							return 3284;

						case 72:
							return 3292;

						case 73:
							return 3300;

						case 74:
							return 3308;

						case 75:
							return 3316;

						case 76:
							return 3324;

						case 77:
							return 3332;

						case 78:
							return 3340;

						case 79:
							return 3348;

						case 80:
							return 3356;

						case 81:
							return 3364;

						case 82:
							return 3372;

						case 83:
							return 3380;

						case 84:
							return 3388;

						case 85:
							return 3396;

						case 86:
							return 3403;

						case 87:
							return 3410;
					}
					switch (iParam1) {
						case 88:
							return 4032;

						case 89:
							return 4040;

						case 90:
							return 4048;

						case 91:
							return 4056;

						case 92:
							return 4064;

						case 93:
							return 4072;

						case 94:
							return 4080;

						case 95:
							return 4088;

						case 96:
							return 4096;

						case 97:
							return 4104;

						case 98:
							return 4112;

						case 99:
							return 4120;

						case 100:
							return 4128;

						case 101:
							return 4136;

						case 102:
							return 4144;

						case 103:
							return 4152;

						case 104:
							return 4160;

						case 105:
							return 4168;

						case 106:
							return 4176;

						case 107:
							return 4184;

						case 108:
							return 4192;

						case 109:
							return 4200;

						case 110:
							return 4208;

						case 111:
							return 4216;

						case 112:
							return 4224;

						case 113:
							return 4232;

						case 114:
							return 4240;

						case 115:
							return 4248;

						case 116:
							return 4256;

						case 117:
							return 4264;

						case 118:
							return 4272;

						case 119:
							return 4280;

						case 120:
							return 4288;

						case 121:
							return 4296;

						case 122:
							return 4304;

						case 123:
							return 4312;

						case 124:
							return 4320;

						case 125:
							return 4328;

						case 126:
							return 4336;

						case 127:
							return 4344;

						case 128:
							return 4352;

						case 129:
							return 4360;

						case 130:
							return 4368;

						case 131:
							return 4376;

						case 132:
							return 4384;

						case 133:
							return 4392;

						case 134:
							return 4400;

						case 135:
							return 4408;

						case 136:
							return 4416;

						case 137:
							return 4424;

						case 138:
							return 4432;

						case 139:
							return 4440;

						case 140:
							return 4448;

						case 141:
							return 4456;

						case 142:
							return 4464;

						case 143:
							return 4472;

						case 144:
							return 4480;

						case 145:
							return 4488;

						case 146:
							return 4496;

						case 147:
							return 4504;

						case 148:
							return 4512;

						case 149:
							return 4520;

						case 150:
							return 4528;

						case 151:
							return 4536;

						case 152:
							return 4544;

						case 153:
							return 4552;

						case 154:
							return 4560;

						case 155:
							return 4568;

						case 156:
							return 4576;

						case 157:
							return 4584;

						case 158:
							return 5477;
					}
					switch (iParam1) {
						case 159:
							return 5926;

						case 160:
							return 5933;

						case 161:
							return 5940;

						case 162:
							return 5947;

						case 163:
							return 5954;

						case 164:
							return 5961;

						case 165:
							return 5968;

						case 166:
							return 5975;

						case 167:
							return 5982;

						case 168:
							return 5989;

						case 169:
							return 5996;

						case 170:
							return 6003;

						case 171:
							return 6010;

						case 172:
							return 6017;

						case 173:
							return 6024;

						case 174:
							return 6031;

						case 175:
							return 6038;

						case 176:
							return 6045;

						case 177:
							return 6052;

						case 178:
							return 6059;

						case 179:
							return 6066;

						case 180:
							return 6073;

						case 181:
							return 6080;

						case 182:
							return 6087;

						case 183:
							return 6094;

						
					}
					switch (iParam1) {
						case 184:
							return 6171;

						case 185:
							return 6178;

						case 186:
							return 6185;

						case 187:
							return 6192;

						case 188:
							return 6199;

						case 189:
							return 6206;

						case 190:
							return 6213;

						case 191:
							return 6220;

						case 192:
							return 6227;

						case 193:
							return 6234;

						
					}
					switch (iParam1) {
						case 194:
							return 6568;

						case 195:
							return 6575;

						case 196:
							return 6582;

						case 197:
							return 6589;

						case 198:
							return 6596;

						case 199:
							return 6603;

						case 200:
							return 6610;

						case 201:
							return 6617;

						case 202:
							return 6624;

						case 203:
							return 6631;

						case 204:
							return 6638;

						case 205:
							return 6645;

						case 206:
							return 6652;

						case 207:
							return 6659;

						case 208:
							return 6666;

						case 209:
							return 6673;

						case 210:
							return 6680;

						case 211:
							return 6687;

						case 212:
							return 6694;

						case 213:
							return 6701;

						case 214:
							return 6708;

						case 215:
							return 6715;

						case 216:
							return 6722;

						case 217:
							return 6729;

						case 218:
							return 6736;

						case 219:
							return 6743;

						case 220:
							return 6750;

						case 221:
							return 6757;

						case 222:
							return 6764;

						case 223:
							return 6771;

						case 224:
							return 6778;

						case 225:
							return 6785;

						case 226:
							return 6792;

						case 227:
							return 6799;

						case 228:
							return 6806;

						case 229:
							return 6813;

						case 230:
							return 6820;

						
					}
					switch (iParam1) {
						case 231:
							return 7290;

						case 232:
							return 7297;

						case 233:
							return 7304;

						case 234:
							return 7311;

						case 235:
							return 7318;

						case 236:
							return 7325;

						case 237:
							return 7332;

						case 238:
							return 7339;

						case 239:
							return 7346;

						case 240:
							return 7353;

						case 241:
							return 7360;

						case 242:
							return 7367;

						case 243:
							return 7374;

						case 244:
							return 7381;

						case 245:
							return 7388;

						case 246:
							return 7395;

						case 247:
							return 7402;

						case 248:
							return 7409;

						case 249:
							return 7416;

						case 250:
							return 7423;

						case 251:
							return 7430;

						case 252:
							return 7437;

						case 253:
							return 7444;

						case 254:
							return 7451;

						case 255:
							return 7458;

						case 256:
							return 7465;

						case 257:
							return 7472;

						case 258:
							return 7479;

						case 259:
							return 7486;

						case 260:
							return 7493;

						
					}
					switch (iParam1) {
						case 261:
							return 8013;

						case 262:
							return 8020;

						case 263:
							return 8027;

						case 264:
							return 8034;

						case 265:
							return 8041;

						case 266:
							return 8048;

						case 267:
							return 8055;

						case 268:
							return 8062;

						case 269:
							return 8069;

						case 270:
							return 8076;

						case 271:
							return 8537;

						case 272:
							return 8544;

						case 273:
							return 8551;

						case 274:
							return 8558;

						case 275:
							return 8565;

						case 276:
							return 8572;

						case 277:
							return 8579;

						case 278:
							return 8586;

						case 279:
							return 8593;

						case 280:
							return 8600;

						case 281:
							return 8982;

						case 282:
							return 8989;

						case 283:
							return 8996;

						case 284:
							return 9003;

						case 285:
							return 9010;

						case 286:
							return 9017;

						case 287:
							return 9024;

						case 288:
							return 9031;

						case 289:
							return 9038;

						case 290:
							return 9045;

						case 291:
							return 9052;

						case 292:
							return 9058;

						case 293:
							return 9064;

						case 294:
							return 9070;

						case 295:
							return 9077;

						case 296:
							return 9084;

						case 297:
							return 9091;

						case 298:
							return 9098;

						case 299:
							return 9105;

						case 300:
							return 9112;

						case 301:
							return 9119;

						case 302:
							return 9126;

						case 303:
							return 9133;

						case 304:
							return 9140;

						case 305:
							return 9146;

						case 306:
							return 9152;

						
					}
					break;

				case 2:
					switch (iParam1) {
						case 0:
							return 1630;

						case 1:
							return 1637;

						case 2:
							return 1644;

						case 3:
							return 1651;

						case 4:
							return 1658;

						case 5:
							return 1665;

						case 6:
							return 1672;

						case 7:
							return 1679;

						case 8:
							return 1686;

						case 9:
							return 1693;

						case 10:
							return 1700;

						case 11:
							return 1706;

						case 12:
							return 1712;

						case 13:
							return 1718;

						case 14:
							return 1797;

						case 15:
							return 1804;

						case 16:
							return 1811;

						case 17:
							return 1818;

						case 18:
							return 1825;

						case 19:
							return 1832;

						case 20:
							return 1839;

						case 21:
							return 1846;

						case 22:
							return 1853;

						case 23:
							return 1860;

						case 24:
							return 1866;

						case 25:
							return 1872;

						case 26:
							return 2167;

						case 27:
							return 2174;

						case 28:
							return 2181;

						case 29:
							return 2188;

						case 30:
							return 2195;

						case 31:
							return 2202;

						case 32:
							return 2209;

						case 33:
							return 2216;

						case 34:
							return 2223;

						case 35:
							return 2230;

						case 36:
							return 2237;

						case 37:
							return 2243;

						case 38:
							return 2249;

						case 39:
							return 2830;

						case 40:
							return 2837;

						case 41:
							return 2844;

						case 42:
							return 2851;

						case 43:
							return 2858;

						case 44:
							return 2865;

						case 45:
							return 2872;

						case 46:
							return 2879;

						case 47:
							return 2886;

						case 48:
							return 2893;

						case 49:
							return 2900;

						case 50:
							return 2906;

						case 51:
							return 2912;

						case 52:
							return 2954;

						case 53:
							return 2961;

						case 54:
							return 2968;

						case 55:
							return 2975;

						case 56:
							return 2982;

						case 57:
							return 2989;

						case 58:
							return 2996;

						case 59:
							return 3003;

						case 60:
							return 3010;

						case 61:
							return 3017;

						case 62:
							return 3024;

						case 63:
							return 3030;

						case 64:
							return 3036;

						case 65:
							return 3237;

						case 66:
							return 3245;

						case 67:
							return 3253;

						case 68:
							return 3261;

						case 69:
							return 3269;

						case 70:
							return 3277;

						case 71:
							return 3285;

						case 72:
							return 3293;

						case 73:
							return 3301;

						case 74:
							return 3309;

						case 75:
							return 3317;

						case 76:
							return 3325;

						case 77:
							return 3333;

						case 78:
							return 3341;

						case 79:
							return 3349;

						case 80:
							return 3357;

						case 81:
							return 3365;

						case 82:
							return 3373;

						case 83:
							return 3381;

						case 84:
							return 3389;

						case 85:
							return 3397;

						case 86:
							return 3404;

						case 87:
							return 3411;

						
					}
					switch (iParam1) {
						case 88:
							return 4033;

						case 89:
							return 4041;

						case 90:
							return 4049;

						case 91:
							return 4057;

						case 92:
							return 4065;

						case 93:
							return 4073;

						case 94:
							return 4081;

						case 95:
							return 4089;

						case 96:
							return 4097;

						case 97:
							return 4105;

						case 98:
							return 4113;

						case 99:
							return 4121;

						case 100:
							return 4129;

						case 101:
							return 4137;

						case 102:
							return 4145;

						case 103:
							return 4153;

						case 104:
							return 4161;

						case 105:
							return 4169;

						case 106:
							return 4177;

						case 107:
							return 4185;

						case 108:
							return 4193;

						case 109:
							return 4201;

						case 110:
							return 4209;

						case 111:
							return 4217;

						case 112:
							return 4225;

						case 113:
							return 4233;

						case 114:
							return 4241;

						case 115:
							return 4249;

						case 116:
							return 4257;

						case 117:
							return 4265;

						case 118:
							return 4273;

						case 119:
							return 4281;

						case 120:
							return 4289;

						case 121:
							return 4297;

						case 122:
							return 4305;

						case 123:
							return 4313;

						case 124:
							return 4321;

						case 125:
							return 4329;

						case 126:
							return 4337;

						case 127:
							return 4345;

						case 128:
							return 4353;

						case 129:
							return 4361;

						case 130:
							return 4369;

						case 131:
							return 4377;

						case 132:
							return 4385;

						case 133:
							return 4393;

						case 134:
							return 4401;

						case 135:
							return 4409;

						case 136:
							return 4417;

						case 137:
							return 4425;

						case 138:
							return 4433;

						case 139:
							return 4441;

						case 140:
							return 4449;

						case 141:
							return 4457;

						case 142:
							return 4465;

						case 143:
							return 4473;

						case 144:
							return 4481;

						case 145:
							return 4489;

						case 146:
							return 4497;

						case 147:
							return 4505;

						case 148:
							return 4513;

						case 149:
							return 4521;

						case 150:
							return 4529;

						case 151:
							return 4537;

						case 152:
							return 4545;

						case 153:
							return 4553;

						case 154:
							return 4561;

						case 155:
							return 4569;

						case 156:
							return 4577;

						case 157:
							return 4585;

						case 158:
							return 5478;

						
					}
					switch (iParam1) {
						case 159:
							return 5927;

						case 160:
							return 5934;

						case 161:
							return 5941;

						case 162:
							return 5948;

						case 163:
							return 5955;

						case 164:
							return 5962;

						case 165:
							return 5969;

						case 166:
							return 5976;

						case 167:
							return 5983;

						case 168:
							return 5990;

						case 169:
							return 5997;

						case 170:
							return 6004;

						case 171:
							return 6011;

						case 172:
							return 6018;

						case 173:
							return 6025;

						case 174:
							return 6032;

						case 175:
							return 6039;

						case 176:
							return 6046;

						case 177:
							return 6053;

						case 178:
							return 6060;

						case 179:
							return 6067;

						case 180:
							return 6074;

						case 181:
							return 6081;

						case 182:
							return 6088;

						case 183:
							return 6095;

						
					}
					switch (iParam1) {
						case 184:
							return 6172;

						case 185:
							return 6179;

						case 186:
							return 6186;

						case 187:
							return 6193;

						case 188:
							return 6200;

						case 189:
							return 6207;

						case 190:
							return 6214;

						case 191:
							return 6221;

						case 192:
							return 6228;

						case 193:
							return 6235;

						
					}
					switch (iParam1) {
						case 194:
							return 6569;

						case 195:
							return 6576;

						case 196:
							return 6583;

						case 197:
							return 6590;

						case 198:
							return 6597;

						case 199:
							return 6604;

						case 200:
							return 6611;

						case 201:
							return 6618;

						case 202:
							return 6625;

						case 203:
							return 6632;

						case 204:
							return 6639;

						case 205:
							return 6646;

						case 206:
							return 6653;

						case 207:
							return 6660;

						case 208:
							return 6667;

						case 209:
							return 6674;

						case 210:
							return 6681;

						case 211:
							return 6688;

						case 212:
							return 6695;

						case 213:
							return 6702;

						case 214:
							return 6709;

						case 215:
							return 6716;

						case 216:
							return 6723;

						case 217:
							return 6730;

						case 218:
							return 6737;

						case 219:
							return 6744;

						case 220:
							return 6751;

						case 221:
							return 6758;

						case 222:
							return 6765;

						case 223:
							return 6772;

						case 224:
							return 6779;

						case 225:
							return 6786;

						case 226:
							return 6793;

						case 227:
							return 6800;

						case 228:
							return 6807;

						case 229:
							return 6814;

						case 230:
							return 6821;

						
					}
					switch (iParam1) {
						case 231:
							return 7291;

						case 232:
							return 7298;

						case 233:
							return 7305;

						case 234:
							return 7312;

						case 235:
							return 7319;

						case 236:
							return 7326;

						case 237:
							return 7333;

						case 238:
							return 7340;

						case 239:
							return 7347;

						case 240:
							return 7354;

						case 241:
							return 7361;

						case 242:
							return 7368;

						case 243:
							return 7375;

						case 244:
							return 7382;

						case 245:
							return 7389;

						case 246:
							return 7396;

						case 247:
							return 7403;

						case 248:
							return 7410;

						case 249:
							return 7417;

						case 250:
							return 7424;

						case 251:
							return 7431;

						case 252:
							return 7438;

						case 253:
							return 7445;

						case 254:
							return 7452;

						case 255:
							return 7459;

						case 256:
							return 7466;

						case 257:
							return 7473;

						case 258:
							return 7480;

						case 259:
							return 7487;

						case 260:
							return 7494;

						
					}
					switch (iParam1) {
						case 261:
							return 8014;

						case 262:
							return 8021;

						case 263:
							return 8028;

						case 264:
							return 8035;

						case 265:
							return 8042;

						case 266:
							return 8049;

						case 267:
							return 8056;

						case 268:
							return 8063;

						case 269:
							return 8070;

						case 270:
							return 8077;

						case 271:
							return 8538;

						case 272:
							return 8545;

						case 273:
							return 8552;

						case 274:
							return 8559;

						case 275:
							return 8566;

						case 276:
							return 8573;

						case 277:
							return 8580;

						case 278:
							return 8587;

						case 279:
							return 8594;

						case 280:
							return 8601;

						case 281:
							return 8983;

						case 282:
							return 8990;

						case 283:
							return 8997;

						case 284:
							return 9004;

						case 285:
							return 9011;

						case 286:
							return 9018;

						case 287:
							return 9025;

						case 288:
							return 9032;

						case 289:
							return 9039;

						case 290:
							return 9046;

						case 291:
							return 9053;

						case 292:
							return 9059;

						case 293:
							return 9065;

						case 294:
							return 9071;

						case 295:
							return 9078;

						case 296:
							return 9085;

						case 297:
							return 9092;

						case 298:
							return 9099;

						case 299:
							return 9106;

						case 300:
							return 9113;

						case 301:
							return 9120;

						case 302:
							return 9127;

						case 303:
							return 9134;

						case 304:
							return 9141;

						case 305:
							return 9147;

						case 306:
							return 9153;

						
					}
					break;

				case 3:
					switch (iParam1) {
						case 0:
							return 1631;

						case 1:
							return 1638;

						case 2:
							return 1645;

						case 3:
							return 1652;

						case 4:
							return 1659;

						case 5:
							return 1666;

						case 6:
							return 1673;

						case 7:
							return 1680;

						case 8:
							return 1687;

						case 9:
							return 1694;

						case 10:
							return 1701;

						case 11:
							return 1707;

						case 12:
							return 1713;

						case 13:
							return 1719;

						case 14:
							return 1798;

						case 15:
							return 1805;

						case 16:
							return 1812;

						case 17:
							return 1819;

						case 18:
							return 1826;

						case 19:
							return 1833;

						case 20:
							return 1840;

						case 21:
							return 1847;

						case 22:
							return 1854;

						case 23:
							return 1861;

						case 24:
							return 1867;

						case 25:
							return 1873;

						case 26:
							return 2168;

						case 27:
							return 2175;

						case 28:
							return 2182;

						case 29:
							return 2189;

						case 30:
							return 2196;

						case 31:
							return 2203;

						case 32:
							return 2210;

						case 33:
							return 2217;

						case 34:
							return 2224;

						case 35:
							return 2231;

						case 36:
							return 2238;

						case 37:
							return 2244;

						case 38:
							return 2250;

						case 39:
							return 2831;

						case 40:
							return 2838;

						case 41:
							return 2845;

						case 42:
							return 2852;

						case 43:
							return 2859;

						case 44:
							return 2866;

						case 45:
							return 2873;

						case 46:
							return 2880;

						case 47:
							return 2887;

						case 48:
							return 2894;

						case 49:
							return 2901;

						case 50:
							return 2907;

						case 51:
							return 2913;

						case 52:
							return 2955;

						case 53:
							return 2962;

						case 54:
							return 2969;

						case 55:
							return 2976;

						case 56:
							return 2983;

						case 57:
							return 2990;

						case 58:
							return 2997;

						case 59:
							return 3004;

						case 60:
							return 3011;

						case 61:
							return 3018;

						case 62:
							return 3025;

						case 63:
							return 3031;

						case 64:
							return 3037;

						case 65:
							return 3238;

						case 66:
							return 3246;

						case 67:
							return 3254;

						case 68:
							return 3262;

						case 69:
							return 3270;

						case 70:
							return 3278;

						case 71:
							return 3286;

						case 72:
							return 3294;

						case 73:
							return 3302;

						case 74:
							return 3310;

						case 75:
							return 3318;

						case 76:
							return 3326;

						case 77:
							return 3334;

						case 78:
							return 3342;

						case 79:
							return 3350;

						case 80:
							return 3358;

						case 81:
							return 3366;

						case 82:
							return 3374;

						case 83:
							return 3382;

						case 84:
							return 3390;

						case 85:
							return 3398;

						case 86:
							return 3405;

						case 87:
							return 3412;

						
					}
					switch (iParam1) {
						case 88:
							return 4034;

						case 89:
							return 4042;

						case 90:
							return 4050;

						case 91:
							return 4058;

						case 92:
							return 4066;

						case 93:
							return 4074;

						case 94:
							return 4082;

						case 95:
							return 4090;

						case 96:
							return 4098;

						case 97:
							return 4106;

						case 98:
							return 4114;

						case 99:
							return 4122;

						case 100:
							return 4130;

						case 101:
							return 4138;

						case 102:
							return 4146;

						case 103:
							return 4154;

						case 104:
							return 4162;

						case 105:
							return 4170;

						case 106:
							return 4178;

						case 107:
							return 4186;

						case 108:
							return 4194;

						case 109:
							return 4202;

						case 110:
							return 4210;

						case 111:
							return 4218;

						case 112:
							return 4226;

						case 113:
							return 4234;

						case 114:
							return 4242;

						case 115:
							return 4250;

						case 116:
							return 4258;

						case 117:
							return 4266;

						case 118:
							return 4274;

						case 119:
							return 4282;

						case 120:
							return 4290;

						case 121:
							return 4298;

						case 122:
							return 4306;

						case 123:
							return 4314;

						case 124:
							return 4322;

						case 125:
							return 4330;

						case 126:
							return 4338;

						case 127:
							return 4346;

						case 128:
							return 4354;

						case 129:
							return 4362;

						case 130:
							return 4370;

						case 131:
							return 4378;

						case 132:
							return 4386;

						case 133:
							return 4394;

						case 134:
							return 4402;

						case 135:
							return 4410;

						case 136:
							return 4418;

						case 137:
							return 4426;

						case 138:
							return 4434;

						case 139:
							return 4442;

						case 140:
							return 4450;

						case 141:
							return 4458;

						case 142:
							return 4466;

						case 143:
							return 4474;

						case 144:
							return 4482;

						case 145:
							return 4490;

						case 146:
							return 4498;

						case 147:
							return 4506;

						case 148:
							return 4514;

						case 149:
							return 4522;

						case 150:
							return 4530;

						case 151:
							return 4538;

						case 152:
							return 4546;

						case 153:
							return 4554;

						case 154:
							return 4562;

						case 155:
							return 4570;

						case 156:
							return 4578;

						case 157:
							return 4586;

						case 158:
							return 5479;

						
					}
					switch (iParam1) {
						case 159:
							return 5928;

						case 160:
							return 5935;

						case 161:
							return 5942;

						case 162:
							return 5949;

						case 163:
							return 5956;

						case 164:
							return 5963;

						case 165:
							return 5970;

						case 166:
							return 5977;

						case 167:
							return 5984;

						case 168:
							return 5991;

						case 169:
							return 5998;

						case 170:
							return 6005;

						case 171:
							return 6012;

						case 172:
							return 6019;

						case 173:
							return 6026;

						case 174:
							return 6033;

						case 175:
							return 6040;

						case 176:
							return 6047;

						case 177:
							return 6054;

						case 178:
							return 6061;

						case 179:
							return 6068;

						case 180:
							return 6075;

						case 181:
							return 6082;

						case 182:
							return 6089;

						case 183:
							return 6096;

						
					}
					switch (iParam1) {
						case 184:
							return 6173;

						case 185:
							return 6180;

						case 186:
							return 6187;

						case 187:
							return 6194;

						case 188:
							return 6201;

						case 189:
							return 6208;

						case 190:
							return 6215;

						case 191:
							return 6222;

						case 192:
							return 6229;

						case 193:
							return 6236;

						
					}
					switch (iParam1) {
						case 194:
							return 6570;

						case 195:
							return 6577;

						case 196:
							return 6584;

						case 197:
							return 6591;

						case 198:
							return 6598;

						case 199:
							return 6605;

						case 200:
							return 6612;

						case 201:
							return 6619;

						case 202:
							return 6626;

						case 203:
							return 6633;

						case 204:
							return 6640;

						case 205:
							return 6647;

						case 206:
							return 6654;

						case 207:
							return 6661;

						case 208:
							return 6668;

						case 209:
							return 6675;

						case 210:
							return 6682;

						case 211:
							return 6689;

						case 212:
							return 6696;

						case 213:
							return 6703;

						case 214:
							return 6710;

						case 215:
							return 6717;

						case 216:
							return 6724;

						case 217:
							return 6731;

						case 218:
							return 6738;

						case 219:
							return 6745;

						case 220:
							return 6752;

						case 221:
							return 6759;

						case 222:
							return 6766;

						case 223:
							return 6773;

						case 224:
							return 6780;

						case 225:
							return 6787;

						case 226:
							return 6794;

						case 227:
							return 6801;

						case 228:
							return 6808;

						case 229:
							return 6815;

						case 230:
							return 6822;

						
					}
					switch (iParam1) {
						case 231:
							return 7292;

						case 232:
							return 7299;

						case 233:
							return 7306;

						case 234:
							return 7313;

						case 235:
							return 7320;

						case 236:
							return 7327;

						case 237:
							return 7334;

						case 238:
							return 7341;

						case 239:
							return 7348;

						case 240:
							return 7355;

						case 241:
							return 7362;

						case 242:
							return 7369;

						case 243:
							return 7376;

						case 244:
							return 7383;

						case 245:
							return 7390;

						case 246:
							return 7397;

						case 247:
							return 7404;

						case 248:
							return 7411;

						case 249:
							return 7418;

						case 250:
							return 7425;

						case 251:
							return 7432;

						case 252:
							return 7439;

						case 253:
							return 7446;

						case 254:
							return 7453;

						case 255:
							return 7460;

						case 256:
							return 7467;

						case 257:
							return 7474;

						case 258:
							return 7481;

						case 259:
							return 7488;

						case 260:
							return 7495;

						
					}
					switch (iParam1) {
						case 261:
							return 8015;

						case 262:
							return 8022;

						case 263:
							return 8029;

						case 264:
							return 8036;

						case 265:
							return 8043;

						case 266:
							return 8050;

						case 267:
							return 8057;

						case 268:
							return 8064;

						case 269:
							return 8071;

						case 270:
							return 8078;

						case 271:
							return 8539;

						case 272:
							return 8546;

						case 273:
							return 8553;

						case 274:
							return 8560;

						case 275:
							return 8567;

						case 276:
							return 8574;

						case 277:
							return 8581;

						case 278:
							return 8588;

						case 279:
							return 8595;

						case 280:
							return 8602;

						case 281:
							return 8984;

						case 282:
							return 8991;

						case 283:
							return 8998;

						case 284:
							return 9005;

						case 285:
							return 9012;

						case 286:
							return 9019;

						case 287:
							return 9026;

						case 288:
							return 9033;

						case 289:
							return 9040;

						case 290:
							return 9047;

						case 291:
							return 9054;

						case 292:
							return 9060;

						case 293:
							return 9066;

						case 294:
							return 9072;

						case 295:
							return 9079;

						case 296:
							return 9086;

						case 297:
							return 9093;

						case 298:
							return 9100;

						case 299:
							return 9107;

						case 300:
							return 9114;

						case 301:
							return 9121;

						case 302:
							return 9128;

						case 303:
							return 9135;

						case 304:
							return 9142;

						case 305:
							return 9148;

						case 306:
							return 9154;

						
					}
					break;

				case 4:
					switch (iParam1) {
						case 0:
							return 1632;

						case 1:
							return 1639;

						case 2:
							return 1646;

						case 3:
							return 1653;

						case 4:
							return 1660;

						case 5:
							return 1667;

						case 6:
							return 1674;

						case 7:
							return 1681;

						case 8:
							return 1688;

						case 9:
							return 1695;

						case 10:
							return 1702;

						case 11:
							return 1708;

						case 12:
							return 1714;

						case 13:
							return 1720;

						case 14:
							return 1799;

						case 15:
							return 1806;

						case 16:
							return 1813;

						case 17:
							return 1820;

						case 18:
							return 1827;

						case 19:
							return 1834;

						case 20:
							return 1841;

						case 21:
							return 1848;

						case 22:
							return 1855;

						case 23:
							return 1862;

						case 24:
							return 1868;

						case 25:
							return 1874;

						case 26:
							return 2169;

						case 27:
							return 2176;

						case 28:
							return 2183;

						case 29:
							return 2190;

						case 30:
							return 2197;

						case 31:
							return 2204;

						case 32:
							return 2211;

						case 33:
							return 2218;

						case 34:
							return 2225;

						case 35:
							return 2232;

						case 36:
							return 2239;

						case 37:
							return 2245;

						case 38:
							return 2251;

						case 39:
							return 2832;

						case 40:
							return 2839;

						case 41:
							return 2846;

						case 42:
							return 2853;

						case 43:
							return 2860;

						case 44:
							return 2867;

						case 45:
							return 2874;

						case 46:
							return 2881;

						case 47:
							return 2888;

						case 48:
							return 2895;

						case 49:
							return 2902;

						case 50:
							return 2908;

						case 51:
							return 2914;

						case 52:
							return 2956;

						case 53:
							return 2963;

						case 54:
							return 2970;

						case 55:
							return 2977;

						case 56:
							return 2984;

						case 57:
							return 2991;

						case 58:
							return 2998;

						case 59:
							return 3005;

						case 60:
							return 3012;

						case 61:
							return 3019;

						case 62:
							return 3026;

						case 63:
							return 3032;

						case 64:
							return 3038;

						case 65:
							return 3239;

						case 66:
							return 3247;

						case 67:
							return 3255;

						case 68:
							return 3263;

						case 69:
							return 3271;

						case 70:
							return 3279;

						case 71:
							return 3287;

						case 72:
							return 3295;

						case 73:
							return 3303;

						case 74:
							return 3311;

						case 75:
							return 3319;

						case 76:
							return 3327;

						case 77:
							return 3335;

						case 78:
							return 3343;

						case 79:
							return 3351;

						case 80:
							return 3359;

						case 81:
							return 3367;

						case 82:
							return 3375;

						case 83:
							return 3383;

						case 84:
							return 3391;

						case 85:
							return 3399;

						case 86:
							return 3406;

						case 87:
							return 3413;

						
					}
					switch (iParam1) {
						case 88:
							return 4035;

						case 89:
							return 4043;

						case 90:
							return 4051;

						case 91:
							return 4059;

						case 92:
							return 4067;

						case 93:
							return 4075;

						case 94:
							return 4083;

						case 95:
							return 4091;

						case 96:
							return 4099;

						case 97:
							return 4107;

						case 98:
							return 4115;

						case 99:
							return 4123;

						case 100:
							return 4131;

						case 101:
							return 4139;

						case 102:
							return 4147;

						case 103:
							return 4155;

						case 104:
							return 4163;

						case 105:
							return 4171;

						case 106:
							return 4179;

						case 107:
							return 4187;

						case 108:
							return 4195;

						case 109:
							return 4203;

						case 110:
							return 4211;

						case 111:
							return 4219;

						case 112:
							return 4227;

						case 113:
							return 4235;

						case 114:
							return 4243;

						case 115:
							return 4251;

						case 116:
							return 4259;

						case 117:
							return 4267;

						case 118:
							return 4275;

						case 119:
							return 4283;

						case 120:
							return 4291;

						case 121:
							return 4299;

						case 122:
							return 4307;

						case 123:
							return 4315;

						case 124:
							return 4323;

						case 125:
							return 4331;

						case 126:
							return 4339;

						case 127:
							return 4347;

						case 128:
							return 4355;

						case 129:
							return 4363;

						case 130:
							return 4371;

						case 131:
							return 4379;

						case 132:
							return 4387;

						case 133:
							return 4395;

						case 134:
							return 4403;

						case 135:
							return 4411;

						case 136:
							return 4419;

						case 137:
							return 4427;

						case 138:
							return 4435;

						case 139:
							return 4443;

						case 140:
							return 4451;

						case 141:
							return 4459;

						case 142:
							return 4467;

						case 143:
							return 4475;

						case 144:
							return 4483;

						case 145:
							return 4491;

						case 146:
							return 4499;

						case 147:
							return 4507;

						case 148:
							return 4515;

						case 149:
							return 4523;

						case 150:
							return 4531;

						case 151:
							return 4539;

						case 152:
							return 4547;

						case 153:
							return 4555;

						case 154:
							return 4563;

						case 155:
							return 4571;

						case 156:
							return 4579;

						case 157:
							return 4587;

						case 158:
							return 5480;

						
					}
					switch (iParam1) {
						case 159:
							return 5929;

						case 160:
							return 5936;

						case 161:
							return 5943;

						case 162:
							return 5950;

						case 163:
							return 5957;

						case 164:
							return 5964;

						case 165:
							return 5971;

						case 166:
							return 5978;

						case 167:
							return 5985;

						case 168:
							return 5992;

						case 169:
							return 5999;

						case 170:
							return 6006;

						case 171:
							return 6013;

						case 172:
							return 6020;

						case 173:
							return 6027;

						case 174:
							return 6034;

						case 175:
							return 6041;

						case 176:
							return 6048;

						case 177:
							return 6055;

						case 178:
							return 6062;

						case 179:
							return 6069;

						case 180:
							return 6076;

						case 181:
							return 6083;

						case 182:
							return 6090;

						case 183:
							return 6097;

						
					}
					switch (iParam1) {
						case 184:
							return 6174;

						case 185:
							return 6181;

						case 186:
							return 6188;

						case 187:
							return 6195;

						case 188:
							return 6202;

						case 189:
							return 6209;

						case 190:
							return 6216;

						case 191:
							return 6223;

						case 192:
							return 6230;

						case 193:
							return 6237;

						
					}
					switch (iParam1) {
						case 194:
							return 6571;

						case 195:
							return 6578;

						case 196:
							return 6585;

						case 197:
							return 6592;

						case 198:
							return 6599;

						case 199:
							return 6606;

						case 200:
							return 6613;

						case 201:
							return 6620;

						case 202:
							return 6627;

						case 203:
							return 6634;

						case 204:
							return 6641;

						case 205:
							return 6648;

						case 206:
							return 6655;

						case 207:
							return 6662;

						case 208:
							return 6669;

						case 209:
							return 6676;

						case 210:
							return 6683;

						case 211:
							return 6690;

						case 212:
							return 6697;

						case 213:
							return 6704;

						case 214:
							return 6711;

						case 215:
							return 6718;

						case 216:
							return 6725;

						case 217:
							return 6732;

						case 218:
							return 6739;

						case 219:
							return 6746;

						case 220:
							return 6753;

						case 221:
							return 6760;

						case 222:
							return 6767;

						case 223:
							return 6774;

						case 224:
							return 6781;

						case 225:
							return 6788;

						case 226:
							return 6795;

						case 227:
							return 6802;

						case 228:
							return 6809;

						case 229:
							return 6816;

						case 230:
							return 6823;

						
					}
					switch (iParam1) {
						case 231:
							return 7293;

						case 232:
							return 7300;

						case 233:
							return 7307;

						case 234:
							return 7314;

						case 235:
							return 7321;

						case 236:
							return 7328;

						case 237:
							return 7335;

						case 238:
							return 7342;

						case 239:
							return 7349;

						case 240:
							return 7356;

						case 241:
							return 7363;

						case 242:
							return 7370;

						case 243:
							return 7377;

						case 244:
							return 7384;

						case 245:
							return 7391;

						case 246:
							return 7398;

						case 247:
							return 7405;

						case 248:
							return 7412;

						case 249:
							return 7419;

						case 250:
							return 7426;

						case 251:
							return 7433;

						case 252:
							return 7440;

						case 253:
							return 7447;

						case 254:
							return 7454;

						case 255:
							return 7461;

						case 256:
							return 7468;

						case 257:
							return 7475;

						case 258:
							return 7482;

						case 259:
							return 7489;

						case 260:
							return 7496;

						
					}
					switch (iParam1) {
						case 261:
							return 8016;

						case 262:
							return 8023;

						case 263:
							return 8030;

						case 264:
							return 8037;

						case 265:
							return 8044;

						case 266:
							return 8051;

						case 267:
							return 8058;

						case 268:
							return 8065;

						case 269:
							return 8072;

						case 270:
							return 8079;

						case 271:
							return 8540;

						case 272:
							return 8547;

						case 273:
							return 8554;

						case 274:
							return 8561;

						case 275:
							return 8568;

						case 276:
							return 8575;

						case 277:
							return 8582;

						case 278:
							return 8589;

						case 279:
							return 8596;

						case 280:
							return 8603;

						case 281:
							return 8985;

						case 282:
							return 8992;

						case 283:
							return 8999;

						case 284:
							return 9006;

						case 285:
							return 9013;

						case 286:
							return 9020;

						case 287:
							return 9027;

						case 288:
							return 9034;

						case 289:
							return 9041;

						case 290:
							return 9048;

						case 291:
							return 9055;

						case 292:
							return 9061;

						case 293:
							return 9067;

						case 294:
							return 9073;

						case 295:
							return 9080;

						case 296:
							return 9087;

						case 297:
							return 9094;

						case 298:
							return 9101;

						case 299:
							return 9108;

						case 300:
							return 9115;

						case 301:
							return 9122;

						case 302:
							return 9129;

						case 303:
							return 9136;

						case 304:
							return 9143;

						case 305:
							return 9149;

						case 306:
							return 9155;

						
					}
					break;

				case 5:
					switch (iParam1) {
						case 0:
							return 1633;

						case 1:
							return 1640;

						case 2:
							return 1647;

						case 3:
							return 1654;

						case 4:
							return 1661;

						case 5:
							return 1668;

						case 6:
							return 1675;

						case 7:
							return 1682;

						case 8:
							return 1689;

						case 9:
							return 1696;

						case 10:
							return 1703;

						case 11:
							return 1709;

						case 12:
							return 1715;

						case 13:
							return 1721;

						case 14:
							return 1800;

						case 15:
							return 1807;

						case 16:
							return 1814;

						case 17:
							return 1821;

						case 18:
							return 1828;

						case 19:
							return 1835;

						case 20:
							return 1842;

						case 21:
							return 1849;

						case 22:
							return 1856;

						case 23:
							return 1863;

						case 24:
							return 1869;

						case 25:
							return 1875;

						case 26:
							return 2170;

						case 27:
							return 2177;

						case 28:
							return 2184;

						case 29:
							return 2191;

						case 30:
							return 2198;

						case 31:
							return 2205;

						case 32:
							return 2212;

						case 33:
							return 2219;

						case 34:
							return 2226;

						case 35:
							return 2233;

						case 36:
							return 2240;

						case 37:
							return 2246;

						case 38:
							return 2252;

						case 39:
							return 2833;

						case 40:
							return 2840;

						case 41:
							return 2847;

						case 42:
							return 2854;

						case 43:
							return 2861;

						case 44:
							return 2868;

						case 45:
							return 2875;

						case 46:
							return 2882;

						case 47:
							return 2889;

						case 48:
							return 2896;

						case 49:
							return 2903;

						case 50:
							return 2909;

						case 51:
							return 2915;

						case 52:
							return 2957;

						case 53:
							return 2964;

						case 54:
							return 2971;

						case 55:
							return 2978;

						case 56:
							return 2985;

						case 57:
							return 2992;

						case 58:
							return 2999;

						case 59:
							return 3006;

						case 60:
							return 3013;

						case 61:
							return 3020;

						case 62:
							return 3027;

						case 63:
							return 3033;

						case 64:
							return 3039;

						case 65:
							return 3240;

						case 66:
							return 3248;

						case 67:
							return 3256;

						case 68:
							return 3264;

						case 69:
							return 3272;

						case 70:
							return 3280;

						case 71:
							return 3288;

						case 72:
							return 3296;

						case 73:
							return 3304;

						case 74:
							return 3312;

						case 75:
							return 3320;

						case 76:
							return 3328;

						case 77:
							return 3336;

						case 78:
							return 3344;

						case 79:
							return 3352;

						case 80:
							return 3360;

						case 81:
							return 3368;

						case 82:
							return 3376;

						case 83:
							return 3384;

						case 84:
							return 3392;

						case 85:
							return 3400;

						case 86:
							return 3407;

						case 87:
							return 3414;

						
					}
					switch (iParam1) {
						case 88:
							return 4036;

						case 89:
							return 4044;

						case 90:
							return 4052;

						case 91:
							return 4060;

						case 92:
							return 4068;

						case 93:
							return 4076;

						case 94:
							return 4084;

						case 95:
							return 4092;

						case 96:
							return 4100;

						case 97:
							return 4108;

						case 98:
							return 4116;

						case 99:
							return 4124;

						case 100:
							return 4132;

						case 101:
							return 4140;

						case 102:
							return 4148;

						case 103:
							return 4156;

						case 104:
							return 4164;

						case 105:
							return 4172;

						case 106:
							return 4180;

						case 107:
							return 4188;

						case 108:
							return 4196;

						case 109:
							return 4204;

						case 110:
							return 4212;

						case 111:
							return 4220;

						case 112:
							return 4228;

						case 113:
							return 4236;

						case 114:
							return 4244;

						case 115:
							return 4252;

						case 116:
							return 4260;

						case 117:
							return 4268;

						case 118:
							return 4276;

						case 119:
							return 4284;

						case 120:
							return 4292;

						case 121:
							return 4300;

						case 122:
							return 4308;

						case 123:
							return 4316;

						case 124:
							return 4324;

						case 125:
							return 4332;

						case 126:
							return 4340;

						case 127:
							return 4348;

						case 128:
							return 4356;

						case 129:
							return 4364;

						case 130:
							return 4372;

						case 131:
							return 4380;

						case 132:
							return 4388;

						case 133:
							return 4396;

						case 134:
							return 4404;

						case 135:
							return 4412;

						case 136:
							return 4420;

						case 137:
							return 4428;

						case 138:
							return 4436;

						case 139:
							return 4444;

						case 140:
							return 4452;

						case 141:
							return 4460;

						case 142:
							return 4468;

						case 143:
							return 4476;

						case 144:
							return 4484;

						case 145:
							return 4492;

						case 146:
							return 4500;

						case 147:
							return 4508;

						case 148:
							return 4516;

						case 149:
							return 4524;

						case 150:
							return 4532;

						case 151:
							return 4540;

						case 152:
							return 4548;

						case 153:
							return 4556;

						case 154:
							return 4564;

						case 155:
							return 4572;

						case 156:
							return 4580;

						case 157:
							return 4588;

						case 158:
							return 5481;

						
					}
					switch (iParam1) {
						case 159:
							return 5930;

						case 160:
							return 5937;

						case 161:
							return 5944;

						case 162:
							return 5951;

						case 163:
							return 5958;

						case 164:
							return 5965;

						case 165:
							return 5972;

						case 166:
							return 5979;

						case 167:
							return 5986;

						case 168:
							return 5993;

						case 169:
							return 6000;

						case 170:
							return 6007;

						case 171:
							return 6014;

						case 172:
							return 6021;

						case 173:
							return 6028;

						case 174:
							return 6035;

						case 175:
							return 6042;

						case 176:
							return 6049;

						case 177:
							return 6056;

						case 178:
							return 6063;

						case 179:
							return 6070;

						case 180:
							return 6077;

						case 181:
							return 6084;

						case 182:
							return 6091;

						case 183:
							return 6098;

						
					}
					switch (iParam1) {
						case 184:
							return 6175;

						case 185:
							return 6182;

						case 186:
							return 6189;

						case 187:
							return 6196;

						case 188:
							return 6203;

						case 189:
							return 6210;

						case 190:
							return 6217;

						case 191:
							return 6224;

						case 192:
							return 6231;

						case 193:
							return 6238;

						
					}
					switch (iParam1) {
						case 194:
							return 6572;

						case 195:
							return 6579;

						case 196:
							return 6586;

						case 197:
							return 6593;

						case 198:
							return 6600;

						case 199:
							return 6607;

						case 200:
							return 6614;

						case 201:
							return 6621;

						case 202:
							return 6628;

						case 203:
							return 6635;

						case 204:
							return 6642;

						case 205:
							return 6649;

						case 206:
							return 6656;

						case 207:
							return 6663;

						case 208:
							return 6670;

						case 209:
							return 6677;

						case 210:
							return 6684;

						case 211:
							return 6691;

						case 212:
							return 6698;

						case 213:
							return 6705;

						case 214:
							return 6712;

						case 215:
							return 6719;

						case 216:
							return 6726;

						case 217:
							return 6733;

						case 218:
							return 6740;

						case 219:
							return 6747;

						case 220:
							return 6754;

						case 221:
							return 6761;

						case 222:
							return 6768;

						case 223:
							return 6775;

						case 224:
							return 6782;

						case 225:
							return 6789;

						case 226:
							return 6796;

						case 227:
							return 6803;

						case 228:
							return 6810;

						case 229:
							return 6817;

						case 230:
							return 6824;

						
					}
					switch (iParam1) {
						case 231:
							return 7294;

						case 232:
							return 7301;

						case 233:
							return 7308;

						case 234:
							return 7315;

						case 235:
							return 7322;

						case 236:
							return 7329;

						case 237:
							return 7336;

						case 238:
							return 7343;

						case 239:
							return 7350;

						case 240:
							return 7357;

						case 241:
							return 7364;

						case 242:
							return 7371;

						case 243:
							return 7378;

						case 244:
							return 7385;

						case 245:
							return 7392;

						case 246:
							return 7399;

						case 247:
							return 7406;

						case 248:
							return 7413;

						case 249:
							return 7420;

						case 250:
							return 7427;

						case 251:
							return 7434;

						case 252:
							return 7441;

						case 253:
							return 7448;

						case 254:
							return 7455;

						case 255:
							return 7462;

						case 256:
							return 7469;

						case 257:
							return 7476;

						case 258:
							return 7483;

						case 259:
							return 7490;

						case 260:
							return 7497;

						
					}
					switch (iParam1) {
						case 261:
							return 8017;

						case 262:
							return 8024;

						case 263:
							return 8031;

						case 264:
							return 8038;

						case 265:
							return 8045;

						case 266:
							return 8052;

						case 267:
							return 8059;

						case 268:
							return 8066;

						case 269:
							return 8073;

						case 270:
							return 8080;

						case 271:
							return 8541;

						case 272:
							return 8548;

						case 273:
							return 8555;

						case 274:
							return 8562;

						case 275:
							return 8569;

						case 276:
							return 8576;

						case 277:
							return 8583;

						case 278:
							return 8590;

						case 279:
							return 8597;

						case 280:
							return 8604;

						case 281:
							return 8986;

						case 282:
							return 8993;

						case 283:
							return 9000;

						case 284:
							return 9007;

						case 285:
							return 9014;

						case 286:
							return 9021;

						case 287:
							return 9028;

						case 288:
							return 9035;

						case 289:
							return 9042;

						case 290:
							return 9049;

						case 291:
							return 9056;

						case 292:
							return 9062;

						case 293:
							return 9068;

						case 294:
							return 9074;

						case 295:
							return 9081;

						case 296:
							return 9088;

						case 297:
							return 9095;

						case 298:
							return 9102;

						case 299:
							return 9109;

						case 300:
							return 9116;

						case 301:
							return 9123;

						case 302:
							return 9130;

						case 303:
							return 9137;

						case 304:
							return 9144;

						case 305:
							return 9150;

						case 306:
							return 9156;

						
					}
					break;

				case 6:
					switch (iParam1) {
						case 0:
							return 1635;

						case 1:
							return 1642;

						case 2:
							return 1649;

						case 3:
							return 1656;

						case 4:
							return 1663;

						case 5:
							return 1670;

						case 6:
							return 1677;

						case 7:
							return 1684;

						case 8:
							return 1691;

						case 9:
							return 1698;

						case 13:
							return 1723;

						case 14:
							return 1802;

						case 15:
							return 1809;

						case 16:
							return 1816;

						case 17:
							return 1823;

						case 18:
							return 1830;

						case 19:
							return 1837;

						case 20:
							return 1844;

						case 21:
							return 1851;

						case 22:
							return 1858;

						case 26:
							return 2172;

						case 27:
							return 2179;

						case 28:
							return 2186;

						case 29:
							return 2193;

						case 30:
							return 2200;

						case 31:
							return 2207;

						case 32:
							return 2214;

						case 33:
							return 2221;

						case 34:
							return 2228;

						case 35:
							return 2235;

						case 39:
							return 2835;

						case 40:
							return 2842;

						case 41:
							return 2849;

						case 42:
							return 2856;

						case 43:
							return 2863;

						case 44:
							return 2870;

						case 45:
							return 2877;

						case 46:
							return 2884;

						case 47:
							return 2891;

						case 48:
							return 2898;

						case 52:
							return 2959;

						case 53:
							return 2966;

						case 54:
							return 2973;

						case 55:
							return 2980;

						case 56:
							return 2987;

						case 57:
							return 2994;

						case 58:
							return 3001;

						case 59:
							return 3008;

						case 60:
							return 3015;

						case 61:
							return 3022;

						case 65:
							return 3243;

						case 66:
							return 3251;

						case 67:
							return 3259;

						case 68:
							return 3267;

						case 69:
							return 3275;

						case 70:
							return 3283;

						case 71:
							return 3291;

						case 72:
							return 3299;

						case 73:
							return 3307;

						case 74:
							return 3315;

						case 75:
							return 3323;

						case 76:
							return 3331;

						case 77:
							return 3339;

						case 78:
							return 3347;

						case 79:
							return 3355;

						case 80:
							return 3363;

						case 81:
							return 3371;

						case 82:
							return 3379;

						case 83:
							return 3387;

						case 84:
							return 3395;

						
					}
					switch (iParam1) {
						case 88:
							return 4039;

						case 89:
							return 4047;

						case 90:
							return 4055;

						case 91:
							return 4063;

						case 92:
							return 4071;

						case 93:
							return 4079;

						case 94:
							return 4087;

						case 95:
							return 4095;

						case 96:
							return 4103;

						case 97:
							return 4111;

						case 98:
							return 4119;

						case 99:
							return 4127;

						case 100:
							return 4135;

						case 101:
							return 4143;

						case 102:
							return 4151;

						case 103:
							return 4159;

						case 104:
							return 4167;

						case 105:
							return 4175;

						case 106:
							return 4183;

						case 107:
							return 4191;

						case 108:
							return 4199;

						case 109:
							return 4207;

						case 110:
							return 4215;

						case 111:
							return 4223;

						case 112:
							return 4231;

						case 113:
							return 4239;

						case 114:
							return 4247;

						case 115:
							return 4255;

						case 116:
							return 4263;

						case 117:
							return 4271;

						case 118:
							return 4279;

						case 119:
							return 4287;

						case 120:
							return 4295;

						case 121:
							return 4303;

						case 122:
							return 4311;

						case 123:
							return 4319;

						case 124:
							return 4327;

						case 125:
							return 4335;

						case 126:
							return 4343;

						case 127:
							return 4351;

						case 128:
							return 4359;

						case 129:
							return 4367;

						case 130:
							return 4375;

						case 131:
							return 4383;

						case 132:
							return 4391;

						case 133:
							return 4399;

						case 134:
							return 4407;

						case 135:
							return 4415;

						case 136:
							return 4423;

						case 137:
							return 4431;

						case 138:
							return 4439;

						case 139:
							return 4447;

						case 140:
							return 4455;

						case 141:
							return 4463;

						case 142:
							return 4471;

						case 143:
							return 4479;

						case 144:
							return 4487;

						case 145:
							return 4495;

						case 146:
							return 4503;

						case 147:
							return 4511;

						case 148:
							return 4519;

						case 149:
							return 4527;

						case 150:
							return 4535;

						case 151:
							return 4543;

						case 152:
							return 4551;

						case 153:
							return 4559;

						case 154:
							return 4567;

						case 155:
							return 4575;

						case 156:
							return 4583;

						case 157:
							return 4591;

						case 158:
							return 5484;

						
					}
					switch (iParam1) {
						case 159:
							return 5932;

						case 160:
							return 5939;

						case 161:
							return 5946;

						case 162:
							return 5953;

						case 163:
							return 5960;

						case 164:
							return 5967;

						case 165:
							return 5974;

						case 166:
							return 5981;

						case 167:
							return 5988;

						case 168:
							return 5995;

						case 169:
							return 6002;

						case 170:
							return 6009;

						case 171:
							return 6016;

						case 172:
							return 6023;

						case 173:
							return 6030;

						case 174:
							return 6037;

						case 175:
							return 6044;

						case 176:
							return 6051;

						case 177:
							return 6058;

						case 178:
							return 6065;

						case 179:
							return 6072;

						case 180:
							return 6079;

						case 181:
							return 6086;

						case 182:
							return 6093;

						case 183:
							return 6100;

						
					}
					switch (iParam1) {
						case 184:
							return 6177;

						case 185:
							return 6184;

						case 186:
							return 6191;

						case 187:
							return 6198;

						case 188:
							return 6205;

						case 189:
							return 6212;

						case 190:
							return 6219;

						case 191:
							return 6226;

						case 192:
							return 6233;

						case 193:
							return 6240;

						
					}
					switch (iParam1) {
						case 194:
							return 6574;

						case 195:
							return 6581;

						case 196:
							return 6588;

						case 197:
							return 6595;

						case 198:
							return 6602;

						case 199:
							return 6609;

						case 200:
							return 6616;

						case 201:
							return 6623;

						case 202:
							return 6630;

						case 203:
							return 6637;

						case 204:
							return 6644;

						case 205:
							return 6651;

						case 206:
							return 6658;

						case 207:
							return 6665;

						case 208:
							return 6672;

						case 209:
							return 6679;

						case 210:
							return 6686;

						case 211:
							return 6693;

						case 212:
							return 6700;

						case 213:
							return 6707;

						case 214:
							return 6714;

						case 215:
							return 6721;

						case 216:
							return 6728;

						case 217:
							return 6735;

						case 218:
							return 6742;

						case 219:
							return 6749;

						case 220:
							return 6756;

						case 221:
							return 6763;

						case 222:
							return 6770;

						case 223:
							return 6777;

						case 224:
							return 6784;

						case 225:
							return 6791;

						case 226:
							return 6798;

						case 227:
							return 6805;

						case 228:
							return 6812;

						case 229:
							return 6819;

						case 230:
							return 6826;

						
					}
					switch (iParam1) {
						case 231:
							return 7296;

						case 232:
							return 7303;

						case 233:
							return 7310;

						case 234:
							return 7317;

						case 235:
							return 7324;

						case 236:
							return 7331;

						case 237:
							return 7338;

						case 238:
							return 7345;

						case 239:
							return 7352;

						case 240:
							return 7359;

						case 241:
							return 7366;

						case 242:
							return 7373;

						case 243:
							return 7380;

						case 244:
							return 7387;

						case 245:
							return 7394;

						case 246:
							return 7401;

						case 247:
							return 7408;

						case 248:
							return 7415;

						case 249:
							return 7422;

						case 250:
							return 7429;

						case 251:
							return 7436;

						case 252:
							return 7443;

						case 253:
							return 7450;

						case 254:
							return 7457;

						case 255:
							return 7464;

						case 256:
							return 7471;

						case 257:
							return 7478;

						case 258:
							return 7485;

						case 259:
							return 7492;

						case 260:
							return 7499;

						
					}
					switch (iParam1) {
						case 261:
							return 8019;

						case 262:
							return 8026;

						case 263:
							return 8033;

						case 264:
							return 8040;

						case 265:
							return 8047;

						case 266:
							return 8054;

						case 267:
							return 8061;

						case 268:
							return 8068;

						case 269:
							return 8075;

						case 270:
							return 8082;

						case 271:
							return 8543;

						case 272:
							return 8550;

						case 273:
							return 8557;

						case 274:
							return 8564;

						case 275:
							return 8571;

						case 276:
							return 8578;

						case 277:
							return 8585;

						case 278:
							return 8592;

						case 279:
							return 8599;

						case 280:
							return 8606;

						case 281:
							return 8988;

						case 282:
							return 8995;

						case 283:
							return 9002;

						case 284:
							return 9009;

						case 285:
							return 9016;

						case 286:
							return 9023;

						case 287:
							return 9030;

						case 288:
							return 9037;

						case 289:
							return 9044;

						case 290:
							return 9051;

						case 294:
							return 9076;

						case 295:
							return 9083;

						case 296:
							return 9090;

						case 297:
							return 9097;

						case 298:
							return 9104;

						case 299:
							return 9111;

						case 300:
							return 9118;

						case 301:
							return 9125;

						case 302:
							return 9132;

						case 303:
							return 9139;

						
					}
					break;

				case 7:
					switch (iParam1) {
						case 0:
							return 1634;

						case 1:
							return 1641;

						case 2:
							return 1648;

						case 3:
							return 1655;

						case 4:
							return 1662;

						case 5:
							return 1669;

						case 6:
							return 1676;

						case 7:
							return 1683;

						case 8:
							return 1690;

						case 9:
							return 1697;

						case 10:
							return 1704;

						case 11:
							return 1710;

						case 12:
							return 1716;

						case 13:
							return 1722;

						case 14:
							return 1801;

						case 15:
							return 1808;

						case 16:
							return 1815;

						case 17:
							return 1822;

						case 18:
							return 1829;

						case 19:
							return 1836;

						case 20:
							return 1843;

						case 21:
							return 1850;

						case 22:
							return 1857;

						case 23:
							return 1864;

						case 24:
							return 1870;

						case 25:
							return 1876;

						case 26:
							return 2171;

						case 27:
							return 2178;

						case 28:
							return 2185;

						case 29:
							return 2192;

						case 30:
							return 2199;

						case 31:
							return 2206;

						case 32:
							return 2213;

						case 33:
							return 2220;

						case 34:
							return 2227;

						case 35:
							return 2234;

						case 36:
							return 2241;

						case 37:
							return 2247;

						case 38:
							return 2253;

						case 39:
							return 2834;

						case 40:
							return 2841;

						case 41:
							return 2848;

						case 42:
							return 2855;

						case 43:
							return 2862;

						case 44:
							return 2869;

						case 45:
							return 2876;

						case 46:
							return 2883;

						case 47:
							return 2890;

						case 48:
							return 2897;

						case 49:
							return 2904;

						case 50:
							return 2910;

						case 51:
							return 2916;

						case 52:
							return 2958;

						case 53:
							return 2965;

						case 54:
							return 2972;

						case 55:
							return 2979;

						case 56:
							return 2986;

						case 57:
							return 2993;

						case 58:
							return 3000;

						case 59:
							return 3007;

						case 60:
							return 3014;

						case 61:
							return 3021;

						case 62:
							return 3028;

						case 63:
							return 3034;

						case 64:
							return 3040;

						case 65:
							return 3241;

						case 66:
							return 3249;

						case 67:
							return 3257;

						case 68:
							return 3265;

						case 69:
							return 3273;

						case 70:
							return 3281;

						case 71:
							return 3289;

						case 72:
							return 3297;

						case 73:
							return 3305;

						case 74:
							return 3313;

						case 75:
							return 3321;

						case 76:
							return 3329;

						case 77:
							return 3338;

						case 78:
							return 3346;

						case 79:
							return 3353;

						case 80:
							return 3361;

						case 81:
							return 3369;

						case 82:
							return 3377;

						case 83:
							return 3385;

						case 84:
							return 3393;

						case 85:
							return 3401;

						case 86:
							return 3408;

						case 87:
							return 3415;
					}
					switch (iParam1) {
						case 88:
							return 4037;

						case 89:
							return 4045;

						case 90:
							return 4053;

						case 91:
							return 4061;

						case 92:
							return 4069;

						case 93:
							return 4077;

						case 94:
							return 4085;

						case 95:
							return 4093;

						case 96:
							return 4101;

						case 97:
							return 4109;

						case 98:
							return 4117;

						case 99:
							return 4125;

						case 100:
							return 4134;

						case 101:
							return 4142;

						case 102:
							return 4149;

						case 103:
							return 4157;

						case 104:
							return 4165;

						case 105:
							return 4173;

						case 106:
							return 4181;

						case 107:
							return 4189;

						case 108:
							return 4197;

						case 109:
							return 4205;

						case 110:
							return 4213;

						case 111:
							return 4221;

						case 112:
							return 4229;

						case 113:
							return 4237;

						case 114:
							return 4245;

						case 115:
							return 4253;

						case 116:
							return 4261;

						case 117:
							return 4269;

						case 118:
							return 4277;

						case 119:
							return 4285;

						case 120:
							return 4294;

						case 121:
							return 4302;

						case 122:
							return 4309;

						case 123:
							return 4317;

						case 124:
							return 4325;

						case 125:
							return 4333;

						case 126:
							return 4341;

						case 127:
							return 4349;

						case 128:
							return 4357;

						case 129:
							return 4365;

						case 130:
							return 4373;

						case 131:
							return 4381;

						case 132:
							return 4389;

						case 133:
							return 4397;

						case 134:
							return 4405;

						case 135:
							return 4413;

						case 136:
							return 4421;

						case 137:
							return 4429;

						case 138:
							return 4437;

						case 139:
							return 4445;

						case 140:
							return 4454;

						case 141:
							return 4462;

						case 142:
							return 4469;

						case 143:
							return 4477;

						case 144:
							return 4485;

						case 145:
							return 4493;

						case 146:
							return 4501;

						case 147:
							return 4509;

						case 148:
							return 4517;

						case 149:
							return 4525;

						case 150:
							return 4533;

						case 151:
							return 4541;

						case 152:
							return 4549;

						case 153:
							return 4557;

						case 154:
							return 4565;

						case 155:
							return 4573;

						case 156:
							return 4581;

						case 157:
							return 4589;

						case 158:
							return 5482;

						
					}
					switch (iParam1) {
						case 159:
							return 5931;

						case 160:
							return 5938;

						case 161:
							return 5945;

						case 162:
							return 5952;

						case 163:
							return 5959;

						case 164:
							return 5966;

						case 165:
							return 5973;

						case 166:
							return 5980;

						case 167:
							return 5987;

						case 168:
							return 5994;

						case 169:
							return 6001;

						case 170:
							return 6008;

						case 171:
							return 6015;

						case 172:
							return 6022;

						case 173:
							return 6029;

						case 174:
							return 6036;

						case 175:
							return 6043;

						case 176:
							return 6050;

						case 177:
							return 6057;

						case 178:
							return 6064;

						case 179:
							return 6071;

						case 180:
							return 6078;

						case 181:
							return 6085;

						case 182:
							return 6092;

						case 183:
							return 6099;
					}
					switch (iParam1) {
						case 184:
							return 6176;

						case 185:
							return 6183;

						case 186:
							return 6190;

						case 187:
							return 6197;

						case 188:
							return 6204;

						case 189:
							return 6211;

						case 190:
							return 6218;

						case 191:
							return 6225;

						case 192:
							return 6232;

						case 193:
							return 6239;

						
					}
					switch (iParam1) {
						case 194:
							return 6573;

						case 195:
							return 6580;

						case 196:
							return 6587;

						case 197:
							return 6594;

						case 198:
							return 6601;

						case 199:
							return 6608;

						case 200:
							return 6615;

						case 201:
							return 6622;

						case 202:
							return 6629;

						case 203:
							return 6636;

						case 204:
							return 6643;

						case 205:
							return 6650;

						case 206:
							return 6657;

						case 207:
							return 6664;

						case 208:
							return 6671;

						case 209:
							return 6678;

						case 210:
							return 6685;

						case 211:
							return 6692;

						case 212:
							return 6699;

						case 213:
							return 6706;

						case 214:
							return 6713;

						case 215:
							return 6720;

						case 216:
							return 6727;

						case 217:
							return 6734;

						case 218:
							return 6741;

						case 219:
							return 6748;

						case 220:
							return 6755;

						case 221:
							return 6762;

						case 222:
							return 6769;

						case 223:
							return 6776;

						case 224:
							return 6783;

						case 225:
							return 6790;

						case 226:
							return 6797;

						case 227:
							return 6804;

						case 228:
							return 6811;

						case 229:
							return 6818;

						case 230:
							return 6825;

						
					}
					switch (iParam1) {
						case 231:
							return 7295;

						case 232:
							return 7302;

						case 233:
							return 7309;

						case 234:
							return 7316;

						case 235:
							return 7323;

						case 236:
							return 7330;

						case 237:
							return 7337;

						case 238:
							return 7344;

						case 239:
							return 7351;

						case 240:
							return 7358;

						case 241:
							return 7365;

						case 242:
							return 7372;

						case 243:
							return 7379;

						case 244:
							return 7386;

						case 245:
							return 7393;

						case 246:
							return 7400;

						case 247:
							return 7407;

						case 248:
							return 7414;

						case 249:
							return 7421;

						case 250:
							return 7428;

						case 251:
							return 7435;

						case 252:
							return 7442;

						case 253:
							return 7449;

						case 254:
							return 7456;

						case 255:
							return 7463;

						case 256:
							return 7470;

						case 257:
							return 7477;

						case 258:
							return 7484;

						case 259:
							return 7491;

						case 260:
							return 7498;

						
					}
					switch (iParam1) {
						case 261:
							return 8018;

						case 262:
							return 8025;

						case 263:
							return 8032;

						case 264:
							return 8039;

						case 265:
							return 8046;

						case 266:
							return 8053;

						case 267:
							return 8060;

						case 268:
							return 8067;

						case 269:
							return 8074;

						case 270:
							return 8081;

						case 271:
							return 8542;

						case 272:
							return 8549;

						case 273:
							return 8556;

						case 274:
							return 8563;

						case 275:
							return 8570;

						case 276:
							return 8577;

						case 277:
							return 8584;

						case 278:
							return 8591;

						case 279:
							return 8598;

						case 280:
							return 8605;

						case 281:
							return 8987;

						case 282:
							return 8994;

						case 283:
							return 9001;

						case 284:
							return 9008;

						case 285:
							return 9015;

						case 286:
							return 9022;

						case 287:
							return 9029;

						case 288:
							return 9036;

						case 289:
							return 9043;

						case 290:
							return 9050;

						case 291:
							return 9057;

						case 292:
							return 9063;

						case 293:
							return 9069;

						case 294:
							return 9075;

						case 295:
							return 9082;

						case 296:
							return 9089;

						case 297:
							return 9096;

						case 298:
							return 9103;

						case 299:
							return 9110;

						case 300:
							return 9117;

						case 301:
							return 9124;

						case 302:
							return 9131;

						case 303:
							return 9138;

						case 304:
							return 9145;

						case 305:
							return 9151;

						case 306:
							return 9157;

						
					}
					break;
			}
			return 1629;
		}

		int func_615(int iParam0, int iParam1) {
			int iVar0;

			if ((iParam0 == 3880 || iParam0 == 3881) || iParam0 == 3882) {
				if (iParam1 < 10) {
					iVar0 = (iParam0 + iParam1 * 3);
				} else if (iParam1 < 23) {
					if (iParam0 == 3880) {
						iVar0 = (3910 + (iParam1 - 13) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (3911 + (iParam1 - 13) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (3912 + (iParam1 - 13) * 3);
					}
				} else if (iParam1 < 36) {
					if (iParam0 == 3880) {
						iVar0 = (3946 + (iParam1 - 26) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (3947 + (iParam1 - 26) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (3948 + (iParam1 - 26) * 3);
					}
				} else if (iParam1 < 49) {
					if (iParam0 == 3880) {
						iVar0 = (5855 + (iParam1 - 39) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (5856 + (iParam1 - 39) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (5857 + (iParam1 - 39) * 3);
					}
				} else if (iParam1 < 62) {
					if (iParam0 == 3880) {
						iVar0 = (7149 + (iParam1 - 52) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (7150 + (iParam1 - 52) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (7151 + (iParam1 - 52) * 3);
					}
				} else if (iParam1 < 85) {
					if (iParam0 == 3880) {
						iVar0 = (9107 + (iParam1 - 65) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (9108 + (iParam1 - 65) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (9109 + (iParam1 - 65) * 3);
					}
				} else if (iParam1 < func_601(11)) {
					if (iParam0 == 3880) {
						iVar0 = (14384 + (iParam1 - 88) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (14385 + (iParam1 - 88) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (14386 + (iParam1 - 88) * 3);
					}
				} else if (iParam1 <= 157) {
					if (iParam0 == 3880) {
						iVar0 = (14384 + (iParam1 - 88) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (14385 + (iParam1 - 88) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (14386 + (iParam1 - 88) * 3);
					}
				} else if (iParam1 == 158) {
					if (iParam0 == 3880) {
						iVar0 = 15351;
					} else if (iParam0 == 3881) {
						iVar0 = 15352;
					} else if (iParam0 == 3882) {
						iVar0 = 15353;
					}
				} else if (iParam1 < 184) {
					if (iParam0 == 3880) {
						iVar0 = (17786 + (iParam1 - 159) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (17787 + (iParam1 - 159) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (17788 + (iParam1 - 159) * 3);
					}
				} else if (iParam1 < 194) {
					if (iParam0 == 3880) {
						iVar0 = (18853 + (iParam1 - 184) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (18854 + (iParam1 - 184) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (18855 + (iParam1 - 184) * 3);
					}
				} else if (iParam1 < 231) {
					if (iParam0 == 3880) {
						iVar0 = (21572 + (iParam1 - 194) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (21573 + (iParam1 - 194) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (21574 + (iParam1 - 194) * 3);
					}
				} else if (iParam1 < 261) {
					if (iParam0 == 3880) {
						iVar0 = (24265 + (iParam1 - 231) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (24266 + (iParam1 - 231) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (24267 + (iParam1 - 231) * 3);
					}
				} else if (iParam1 < 271) {
					if (iParam0 == 3880) {
						iVar0 = (26229 + (iParam1 - 261) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (26230 + (iParam1 - 261) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (26231 + (iParam1 - 261) * 3);
					}
				} else if (iParam1 < 281) {
					if (iParam0 == 3880) {
						iVar0 = (27949 + (iParam1 - 271) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (27950 + (iParam1 - 271) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (27951 + (iParam1 - 271) * 3);
					}
				} else if (iParam1 < 291) {
					if (iParam0 == 3880) {
						iVar0 = (29474 + (iParam1 - 281) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (29475 + (iParam1 - 281) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (29476 + (iParam1 - 281) * 3);
					}
				} else if (iParam1 < 304) {
					if (iParam0 == 3880) {
						iVar0 = (29504 + (iParam1 - 294) * 3);
					} else if (iParam0 == 3881) {
						iVar0 = (29505 + (iParam1 - 294) * 3);
					} else if (iParam0 == 3882) {
						iVar0 = (29506 + (iParam1 - 294) * 3);
					}
				}
			} else if (iParam1 < 10) {
				iVar0 = (iParam0 + (iParam1 * (1456 - 1411)));
			} else if (iParam1 >= 10 && iParam1 <= 12) {
				iVar0 = ((iParam1 - 10) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 1861;
				} else if (iParam0 == 1412) {
					iVar0 += 1862;
				} else if (iParam0 == 1441) {
					iVar0 += 1863;
				} else if (iParam0 == 1442) {
					iVar0 += 1864;
				} else if (iParam0 == 1443) {
					iVar0 += 1865;
				} else if (iParam0 == 1445) {
					iVar0 += 1866;
				} else if (iParam0 == 1447) {
					iVar0 += 1867;
				} else if (iParam0 == 1448) {
					iVar0 += 1868;
				} else if (iParam0 == 1449) {
					iVar0 += 1869;
				} else if (iParam0 == 1450) {
					iVar0 += 1870;
				} else if (iParam0 == 1451) {
					iVar0 += 1871;
				} else if (iParam0 == 1452) {
					iVar0 += 1872;
				} else if (iParam0 == 1453) {
					iVar0 += 1873;
				} else if (iParam0 == 1454) {
					iVar0 += 1874;
				} else if (iParam0 == 1455) {
					iVar0 += 1875;
				}
			} else if (iParam1 < 23) {
				iVar0 = (1906 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 13) * (1456 - 1411)));
			} else if (iParam1 >= 23 && iParam1 <= 25) {
				iVar0 = ((iParam1 - 23) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 2356;
				} else if (iParam0 == 1412) {
					iVar0 += 2357;
				} else if (iParam0 == 1441) {
					iVar0 += 2358;
				} else if (iParam0 == 1442) {
					iVar0 += 2359;
				} else if (iParam0 == 1443) {
					iVar0 += 2360;
				} else if (iParam0 == 1445) {
					iVar0 += 2361;
				} else if (iParam0 == 1447) {
					iVar0 += 2362;
				} else if (iParam0 == 1448) {
					iVar0 += 2363;
				} else if (iParam0 == 1449) {
					iVar0 += 2364;
				} else if (iParam0 == 1450) {
					iVar0 += 2365;
				} else if (iParam0 == 1451) {
					iVar0 += 2366;
				} else if (iParam0 == 1452) {
					iVar0 += 2367;
				} else if (iParam0 == 1453) {
					iVar0 += 2368;
				} else if (iParam0 == 1454) {
					iVar0 += 2369;
				} else if (iParam0 == 1455) {
					iVar0 += 2370;
				}
			} else if (iParam1 < 36) {
				iVar0 = (2405 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 26) * (1456 - 1411)));
			} else if (iParam1 >= 36 && iParam1 <= 38) {
				iVar0 = ((iParam1 - 36) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 2855;
				} else if (iParam0 == 1412) {
					iVar0 += 2856;
				} else if (iParam0 == 1441) {
					iVar0 += 2857;
				} else if (iParam0 == 1442) {
					iVar0 += 2858;
				} else if (iParam0 == 1443) {
					iVar0 += 2859;
				} else if (iParam0 == 1445) {
					iVar0 += 2860;
				} else if (iParam0 == 1447) {
					iVar0 += 2861;
				} else if (iParam0 == 1448) {
					iVar0 += 2862;
				} else if (iParam0 == 1449) {
					iVar0 += 2863;
				} else if (iParam0 == 1450) {
					iVar0 += 2864;
				} else if (iParam0 == 1451) {
					iVar0 += 2865;
				} else if (iParam0 == 1452) {
					iVar0 += 2866;
				} else if (iParam0 == 1453) {
					iVar0 += 2867;
				} else if (iParam0 == 1454) {
					iVar0 += 2868;
				} else if (iParam0 == 1455) {
					iVar0 += 2869;
				}
			} else if (iParam1 < 49) {
				iVar0 = (5360 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 39) * (1456 - 1411)));
			} else if (iParam1 >= 49 && iParam1 <= 51) {
				iVar0 = ((iParam1 - 49) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 5810;
				} else if (iParam0 == 1412) {
					iVar0 += 5811;
				} else if (iParam0 == 1441) {
					iVar0 += 5812;
				} else if (iParam0 == 1442) {
					iVar0 += 5813;
				} else if (iParam0 == 1443) {
					iVar0 += 5814;
				} else if (iParam0 == 1445) {
					iVar0 += 5815;
				} else if (iParam0 == 1447) {
					iVar0 += 5816;
				} else if (iParam0 == 1448) {
					iVar0 += 5817;
				} else if (iParam0 == 1449) {
					iVar0 += 5818;
				} else if (iParam0 == 1450) {
					iVar0 += 5819;
				} else if (iParam0 == 1451) {
					iVar0 += 5820;
				} else if (iParam0 == 1452) {
					iVar0 += 5821;
				} else if (iParam0 == 1453) {
					iVar0 += 5822;
				} else if (iParam0 == 1454) {
					iVar0 += 5823;
				} else if (iParam0 == 1455) {
					iVar0 += 5824;
				}
			} else if (iParam1 < 62) {
				iVar0 = (6654 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 52) * (1456 - 1411)));
			} else if (iParam1 >= 62 && iParam1 <= 64) {
				iVar0 = ((iParam1 - 62) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 7104;
				} else if (iParam0 == 1412) {
					iVar0 += 7105;
				} else if (iParam0 == 1441) {
					iVar0 += 7106;
				} else if (iParam0 == 1442) {
					iVar0 += 7107;
				} else if (iParam0 == 1443) {
					iVar0 += 7108;
				} else if (iParam0 == 1445) {
					iVar0 += 7109;
				} else if (iParam0 == 1447) {
					iVar0 += 7110;
				} else if (iParam0 == 1448) {
					iVar0 += 7111;
				} else if (iParam0 == 1449) {
					iVar0 += 7112;
				} else if (iParam0 == 1450) {
					iVar0 += 7113;
				} else if (iParam0 == 1451) {
					iVar0 += 7114;
				} else if (iParam0 == 1452) {
					iVar0 += 7115;
				} else if (iParam0 == 1453) {
					iVar0 += 7116;
				} else if (iParam0 == 1454) {
					iVar0 += 7117;
				} else if (iParam0 == 1455) {
					iVar0 += 7118;
				}
			} else if (iParam1 < 85) {
				iVar0 = (8162 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 65) * (1456 - 1411)));
			} else if (iParam1 >= 85 && iParam1 <= 87) {
				iVar0 = ((iParam1 - 85) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 9062;
				} else if (iParam0 == 1412) {
					iVar0 += 9063;
				} else if (iParam0 == 1441) {
					iVar0 += 9064;
				} else if (iParam0 == 1442) {
					iVar0 += 9065;
				} else if (iParam0 == 1443) {
					iVar0 += 9066;
				} else if (iParam0 == 1445) {
					iVar0 += 9067;
				} else if (iParam0 == 1447) {
					iVar0 += 9068;
				} else if (iParam0 == 1448) {
					iVar0 += 9069;
				} else if (iParam0 == 1449) {
					iVar0 += 9070;
				} else if (iParam0 == 1450) {
					iVar0 += 9071;
				} else if (iParam0 == 1451) {
					iVar0 += 9072;
				} else if (iParam0 == 1452) {
					iVar0 += 9073;
				} else if (iParam0 == 1453) {
					iVar0 += 9074;
				} else if (iParam0 == 1454) {
					iVar0 += 9075;
				} else if (iParam0 == 1455) {
					iVar0 += 9076;
				}
			} else if (iParam1 < func_601(11)) {
				iVar0 = (11234 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - func_601(7)) * (1456 - 1411)));
			} else if (iParam1 <= 157) {
				iVar0 = (11234 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - func_601(7)) * (1456 - 1411)));
			} else if (iParam1 == 158) {
				iVar0 = (15306 - 1411);
				iVar0 = (iVar0 + iParam0);
			} else if (iParam1 < 184) {
				iVar0 = (16661 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 159) * (1456 - 1411)));
			} else if (iParam1 < 194) {
				iVar0 = (18403 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 184) * (1456 - 1411)));
			} else if (iParam1 < 231) {
				iVar0 = (19907 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 194) * (1456 - 1411)));
			} else if (iParam1 < 261) {
				iVar0 = (22915 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 231) * (1456 - 1411)));
			} else if (iParam1 < 271) {
				iVar0 = (25779 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 261) * (1456 - 1411)));
			} else if (iParam1 < 281) {
				iVar0 = (27499 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 271) * (1456 - 1411)));
			} else if (iParam1 < 291) {
				iVar0 = (28484 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 281) * (1456 - 1411)));
			} else if (iParam1 >= 291 && iParam1 <= 293) {
				iVar0 = ((iParam1 - 291) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 28934;
				} else if (iParam0 == 1412) {
					iVar0 += 28935;
				} else if (iParam0 == 1441) {
					iVar0 += 28936;
				} else if (iParam0 == 1442) {
					iVar0 += 28937;
				} else if (iParam0 == 1443) {
					iVar0 += 28938;
				} else if (iParam0 == 1445) {
					iVar0 += 28939;
				} else if (iParam0 == 1447) {
					iVar0 += 28940;
				} else if (iParam0 == 1448) {
					iVar0 += 28941;
				} else if (iParam0 == 1449) {
					iVar0 += 28942;
				} else if (iParam0 == 1450) {
					iVar0 += 28943;
				} else if (iParam0 == 1451) {
					iVar0 += 28944;
				} else if (iParam0 == 1452) {
					iVar0 += 28945;
				} else if (iParam0 == 1453) {
					iVar0 += 28946;
				} else if (iParam0 == 1454) {
					iVar0 += 28947;
				} else if (iParam0 == 1455) {
					iVar0 += 28948;
				}
			} else if (iParam1 < 304) {
				iVar0 = (28979 - 1411);
				iVar0 = (iVar0 + iParam0);
				iVar0 = (iVar0 + ((iParam1 - 294) * (1456 - 1411)));
			} else if (iParam1 >= 304 && iParam1 <= 306) {
				iVar0 = ((iParam1 - 304) * (1876 - 1861));
				if (iParam0 == 1411) {
					iVar0 += 29429;
				} else if (iParam0 == 1412) {
					iVar0 += 29430;
				} else if (iParam0 == 1441) {
					iVar0 += 29431;
				} else if (iParam0 == 1442) {
					iVar0 += 29432;
				} else if (iParam0 == 1443) {
					iVar0 += 29433;
				} else if (iParam0 == 1445) {
					iVar0 += 29434;
				} else if (iParam0 == 1447) {
					iVar0 += 29435;
				} else if (iParam0 == 1448) {
					iVar0 += 29436;
				} else if (iParam0 == 1449) {
					iVar0 += 29437;
				} else if (iParam0 == 1450) {
					iVar0 += 29438;
				} else if (iParam0 == 1451) {
					iVar0 += 29439;
				} else if (iParam0 == 1452) {
					iVar0 += 29440;
				} else if (iParam0 == 1453) {
					iVar0 += 29441;
				} else if (iParam0 == 1454) {
					iVar0 += 29442;
				} else if (iParam0 == 1455) {
					iVar0 += 29443;
				}
			}
			return iVar0;
		}

		int func_610(int iParam0) {
			if (iParam0 < 10) {
				return (5975 + iParam0);
			} else if (iParam0 > 12 && iParam0 < 23) {
				return (5975 + (iParam0 - 3));
			} else if (iParam0 > 25 && iParam0 < 36) {
				return (5975 + (iParam0 - 6));
			} else if (iParam0 > 38 && iParam0 < 49) {
				return (5975 + (iParam0 - 9));
			} else if (iParam0 > 51 && iParam0 < 62) {
				return ((7242 + iParam0) - 52);
			} else if (iParam0 > 64 && iParam0 < 85) {
				return ((9290 + iParam0) - 65);
			} else if (iParam0 >= 281 && iParam0 < 291) {
				return ((30114 + iParam0) - 281);
			} else if (iParam0 >= 294 && iParam0 < 304) {
				return (30114 + ((iParam0 - 281) - 3));
			} else if (iParam0 < func_601(11)) {
				return ((15014 + iParam0) - 88);
			} else if (iParam0 <= 157) {
				return ((15014 + iParam0) - 88);
			} else if (iParam0 == 158) {
				return 15360;
			} else if (iParam0 < 184) {
				return ((18011 + iParam0) - 159);
			} else if (iParam0 < 194) {
				return ((18943 + iParam0) - 184);
			} else if (iParam0 < 231) {
				return ((21905 + iParam0) - 194);
			} else if (iParam0 < 261) {
				return ((24505 + iParam0) - 231);
			} else if (iParam0 < 271) {
				return ((26309 + iParam0) - 261);
			} else if (iParam0 < 281) {
				return ((28029 + iParam0) - 271);
			}
			return 0;
		}

		int func_611(int iParam0) {
			if (iParam0 < 10) {
				return (5935 + iParam0);
			} else if (iParam0 > 12 && iParam0 < 23) {
				return (5935 + (iParam0 - 3));
			} else if (iParam0 > 25 && iParam0 < 36) {
				return (5935 + (iParam0 - 6));
			} else if (iParam0 > 38 && iParam0 < 49) {
				return (5935 + (iParam0 - 9));
			} else if (iParam0 > 51 && iParam0 < 62) {
				return ((7232 + iParam0) - 52);
			} else if (iParam0 > 64 && iParam0 < 85) {
				return ((9270 + iParam0) - 65);
			} else if (iParam0 >= 281 && iParam0 < 291) {
				return ((30094 + iParam0) - 281);
			} else if (iParam0 >= 294 && iParam0 < 304) {
				return (30094 + ((iParam0 - 281) - 3));
			} else if (iParam0 < func_601(11)) {
				return ((14944 + iParam0) - 88);
			} else if (iParam0 <= 157) {
				return ((14944 + iParam0) - 88);
			} else if (iParam0 == 158) {
				return 15359;
			} else if (iParam0 < 184) {
				return ((17986 + iParam0) - 159);
			} else if (iParam0 < 194) {
				return ((18933 + iParam0) - 184);
			} else if (iParam0 < 231) {
				return ((21868 + iParam0) - 194);
			} else if (iParam0 < 261) {
				return ((24475 + iParam0) - 231);
			} else if (iParam0 < 271) {
				return ((26299 + iParam0) - 261);
			} else if (iParam0 < 281) {
				return ((28019 + iParam0) - 271);
			}
			return 0;
		}

		int func_612(int iParam0) {
			int iVar0;

			iVar0 = (4424 - 4400);
			if (iParam0 < 10) {
				return (4400 + (iParam0 * iVar0));
			} else if (iParam0 > 12 && iParam0 < 23) {
				return (4640 + ((iParam0 - 13) * iVar0));
			} else if (iParam0 > 25 && iParam0 < 36) {
				return (4880 + ((iParam0 - 26) * iVar0));
			} else if (iParam0 > 38 && iParam0 < 49) {
				return (5120 + ((iParam0 - 39) * iVar0));
			} else if (iParam0 > 51 && iParam0 < 62) {
				return (6414 + ((iParam0 - 52) * iVar0));
			} else if (iParam0 > 64 && iParam0 < 85) {
				return (7682 + ((iParam0 - 65) * iVar0));
			} else if (iParam0 >= 281 && iParam0 < 291) {
				return (29534 + ((iParam0 - 281) * iVar0));
			} else if (iParam0 >= 294 && iParam0 < 304) {
				return (29774 + ((iParam0 - 294) * iVar0));
			} else if (iParam0 < func_601(11)) {
				return (9554 + ((iParam0 - 88) * iVar0));
			} else if (iParam0 <= 157) {
				return (9554 + ((iParam0 - 88) * iVar0));
			} else if (iParam0 == 158) {
				return 15282;
			} else if (iParam0 < 184) {
				return (16061 + ((iParam0 - 159) * iVar0));
			} else if (iParam0 < 194) {
				return (18163 + ((iParam0 - 184) * iVar0));
			} else if (iParam0 < 231) {
				return (19019 + ((iParam0 - 194) * iVar0));
			} else if (iParam0 < 261) {
				return (22195 + ((iParam0 - 231) * iVar0));
			} else if (iParam0 < 271) {
				return (25539 + ((iParam0 - 261) * iVar0));
			} else if (iParam0 < 281) {
				return (27259 + ((iParam0 - 271) * iVar0));
			}
			return 0;
		}

		int func_613(int iParam0) // checks if it's a bike on the rack
		{
			switch (iParam0) {
				case 10:
				case 11:
				case 12:
				case 23:
				case 24:
				case 25:
				case 36:
				case 37:
				case 38:
				case 49:
				case 50:
				case 51:
				case 62:
				case 63:
				case 64:
				case 85:
				case 86:
				case 87:
				case 291:
				case 292:
				case 293:
				case 304:
				case 305:
				case 306:
					return 1;
					break;
			}
			return 0;
		}

		BOOL func_614(int iParam0, int iParam1, int iParam2, int iParam3) {

			int iVar0;
			int iVar1;

			if (iParam2 == -1) {
				iParam2 = Menu::Global(1312763).As<int>();
			}
			if (iParam1 < 0) {
				iParam1 = 255;
			}
			iVar0 = 0;
			iVar1 = 0;

			// Native::_GetNgstatIntHash = Native::_GetNgstatIntHash
			if (iParam0 >= 384 && iParam0 < 457) {
				iVar0 = Native::_GetPstatIntHash((iParam0 - 384), 0, 1, iParam2);
				iVar1 = ((iParam0 - 384) - Native::_0x94F12ABF9C79E339((iParam0 - 384)) * 8) * 8;
			} else if (iParam0 >= 457 && iParam0 < 513) {
				iVar0 = Native::_GetPstatIntHash((iParam0 - 457), 1, 1, iParam2);
				iVar1 = ((iParam0 - 457) - Native::_0x94F12ABF9C79E339((iParam0 - 457)) * 8) * 8;
			} else if (iParam0 >= 1281 && iParam0 < 1305) {
				iVar0 = Native::_GetPstatIntHash((iParam0 - 1281), 0, 0, 0);
				iVar1 = ((iParam0 - 1281) - Native::_0x94F12ABF9C79E339((iParam0 - 1281)) * 8) * 8;
			} else if (iParam0 >= 1305 && iParam0 < 1361) {
				iVar0 = Native::_GetPstatIntHash((iParam0 - 1305), 1, 0, 0);
				iVar1 = ((iParam0 - 1305) - Native::_0x94F12ABF9C79E339((iParam0 - 1305)) * 8) * 8;
			} else if (iParam0 >= 1393 && iParam0 < 2919) {
				iVar0 = Native::_GetTupstatIntHash((iParam0 - 1393), 0, 1, iParam2);
				iVar1 = ((iParam0 - 1393) - Native::_0x94F12ABF9C79E339((iParam0 - 1393)) * 8) * 8;
			} else if (iParam0 >= 1361 && iParam0 < 1393) {
				iVar0 = Native::_GetTupstatIntHash((iParam0 - 1361), 0, 0, 0);
				iVar1 = ((iParam0 - 1361) - Native::_0x94F12ABF9C79E339((iParam0 - 1361)) * 8) * 8;
			} else if (iParam0 >= 3879 && iParam0 < 4143) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 3879), 0, 1, iParam2, "_NGPSTAT_INT");
				iVar1 = ((iParam0 - 3879) - Native::_0x94F12ABF9C79E339((iParam0 - 3879)) * 8) * 8;
			} else if (iParam0 >= 4143 && iParam0 < 4207) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 4143), 0, 0, 0, "_NGPSTAT_INT");
				iVar1 = ((iParam0 - 4143) - Native::_0x94F12ABF9C79E339((iParam0 - 4143)) * 8) * 8;
			} else if (iParam0 >= 4399 && iParam0 < 6028) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 4399), 0, 1, iParam2, "_LRPSTAT_INT");
				iVar1 = ((iParam0 - 4399) - Native::_0x94F12ABF9C79E339((iParam0 - 4399)) * 8) * 8;
			} else if (iParam0 >= 6413 && iParam0 < 7262) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 6413), 0, 1, iParam2, "_APAPSTAT_INT");
				iVar1 = ((iParam0 - 6413) - Native::_0x94F12ABF9C79E339((iParam0 - 6413)) * 8) * 8;
			} else if (iParam0 >= 7262 && iParam0 < 7313) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 7262), 0, 1, iParam2, "_LR2PSTAT_INT");
				iVar1 = ((iParam0 - 7262) - Native::_0x94F12ABF9C79E339((iParam0 - 7262)) * 8) * 8;
			} else if (iParam0 >= 7681 && iParam0 < 9361) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 7681), 0, 1, iParam2, "_BIKEPSTAT_INT");
				iVar1 = ((iParam0 - 7681) - Native::_0x94F12ABF9C79E339((iParam0 - 7681)) * 8) * 8;
			} else if (iParam0 >= 9553 && iParam0 < 15265) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 9553), 0, 1, iParam2, "_IMPEXPPSTAT_INT");
				iVar1 = ((iParam0 - 9553) - Native::_0x94F12ABF9C79E339((iParam0 - 9553)) * 8) * 8;
			} else if (iParam0 >= 15265 && iParam0 < 15369) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 15265), 0, 1, iParam2, "_GUNRPSTAT_INT");
				iVar1 = ((iParam0 - 15265) - Native::_0x94F12ABF9C79E339((iParam0 - 15265)) * 8) * 8;
			} else if (iParam0 >= 16010 && iParam0 < 18098) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 16010), 0, 1, iParam2, "_DLCSMUGCHARPSTAT_INT");
				iVar1 = ((iParam0 - 16010) - Native::_0x94F12ABF9C79E339((iParam0 - 16010)) * 8) * 8;
			} else if (iParam0 >= 18162 && iParam0 < 19018) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 18162), 0, 1, iParam2, "_GANGOPSPSTAT_INT");
				iVar1 = ((iParam0 - 18162) - Native::_0x94F12ABF9C79E339((iParam0 - 18162)) * 8) * 8;
			} else if (iParam0 >= 19018 && iParam0 < 22066) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 19018), 0, 1, iParam2, "_BUSINESSBATPSTAT_INT");
				iVar1 = ((iParam0 - 19018) - Native::_0x94F12ABF9C79E339((iParam0 - 19018)) * 8) * 8;
			} else if (iParam0 >= 22194 && iParam0 < 24962) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 22194), 0, 1, iParam2, "_ARENAWARSPSTAT_INT");
				iVar1 = ((iParam0 - 22194) - Native::_0x94F12ABF9C79E339((iParam0 - 22194)) * 8) * 8;
			} else if (iParam0 >= 25538 && iParam0 < 26810) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 25538), 0, 1, iParam2, "_CASINOPSTAT_INT");
				iVar1 = ((iParam0 - 25538) - Native::_0x94F12ABF9C79E339((iParam0 - 25538)) * 8) * 8;
			} else if (iParam0 >= 27258 && iParam0 < 28098) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 27258), 0, 1, iParam2, "_CASINOHSTPSTAT_INT");
				iVar1 = ((iParam0 - 27258) - Native::_0x94F12ABF9C79E339((iParam0 - 27258)) * 8) * 8;
			} else if (iParam0 >= 28483 && iParam0 < 30227) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 28483), 0, 1, iParam2, "_SU20PSTAT_INT");
				iVar1 = ((iParam0 - 28483) - Native::_0x94F12ABF9C79E339((iParam0 - 28483)) * 8) * 8;
			} else if (iParam0 >= 30483 && iParam0 < 30515) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 30483), 0, 1, iParam2, "_HISLANDPSTAT_INT");
				iVar1 = ((iParam0 - 30483) - Native::_0x94F12ABF9C79E339((iParam0 - 30483)) * 8) * 8;
			} else if (iParam0 >= 7641 && iParam0 < 7681) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 7641), 0, 1, iParam2, "_NGDLCPSTAT_INT");
				iVar1 = ((iParam0 - 7641) - Native::_0x94F12ABF9C79E339((iParam0 - 7641)) * 8) * 8;
			} else if (iParam0 >= 7313 && iParam0 < 7321) {
				iVar0 = Native::_GetNgstatIntHash((iParam0 - 7313), 0, 0, 0, "_NGDLCPSTAT_INT");
				iVar1 = ((iParam0 - 7313) - Native::_0x94F12ABF9C79E339((iParam0 - 7313)) * 8) * 8;
			}

			return Native::StatSetMaskedInt(iVar0, iParam1, iVar1, 8, iParam3);;
		}

		void func_598(int iParam0, char* uParam1, int iParam2) {
			int iVar0;

			iVar0 = Menu::Global(2552060).At(iParam0, 3).At(func_24(iParam2), 1).As<int>();
			if (iVar0 != 0) {
				Native::StatSetLicensePlate(iVar0, uParam1);
			}
		}

		int func_597(int VehicleIndex, bool bParam2, int iParam3, bool bParam4, bool bParam5) {

			int iVar0;
			int iVar1;
			int iVar2;

			if (bParam2) {
				Menu::Global(2097152).At(0, 10931).At(6175).At(1274).As<int>() = Native::_GetPosixTime();
			}
			iVar1 = func_615(1411, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 137), iParam3, 1);
			iVar1 = func_615(1412, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 138), iParam3, 1);
			if (!func_613(VehicleIndex)) {
				iVar1 = func_615(1413, VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 0), iParam3, 1);
				iVar0 = 0;
				while (iVar0 < 49) {
					if (iVar0 < 25) {
						iVar1 = (func_615(1414, VehicleIndex) + iVar0);
						func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 9 + iVar0 + 1), iParam3, 1);
					} else {
						iVar1 = (func_612(VehicleIndex) + (iVar0 - 25));
						func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 9 + iVar0 + 1), iParam3, 1);
					}
					iVar0++;
				}
				iVar0 = 0;
				iVar0 = 0;
				while (iVar0 < 2) {
					iVar1 = (func_615(1439, VehicleIndex) + iVar0);
					func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 59 + iVar0 + 1), iParam3, 1);
					iVar0++;
				}
			}
			iVar1 = func_615(1441, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 62), iParam3, 1);
			iVar1 = func_615(1442, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 63), iParam3, 1);
			iVar1 = func_615(1443, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 64), iParam3, 1);
			if (!func_613(VehicleIndex)) {
				iVar1 = func_615(1444, VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 65), iParam3, 1);
			}
			iVar1 = func_615(1445, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 67), iParam3, 1);
			if (!func_613(VehicleIndex)) {
				iVar1 = func_615(1446, VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 68), iParam3, 1);
			}
			iVar1 = func_615(1447, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 69), iParam3, 1);
			iVar1 = func_615(1448, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 70), -1, 1);
			iVar1 = func_615(1449, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 71), iParam3, 1);
			iVar1 = func_615(1450, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 72), iParam3, 1);
			iVar1 = func_615(1451, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 73), iParam3, 1);
			iVar1 = func_615(1452, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 5), iParam3, 1);
			iVar1 = func_615(1453, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 6), iParam3, 1);
			iVar1 = func_615(1454, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 7), iParam3, 1);
			iVar1 = func_615(1455, VehicleIndex);
			func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 8), iParam3, 1);
			if (!func_613(VehicleIndex)) {
				iVar1 = func_615(3880, VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 74), iParam3, 1);
				iVar1 = func_615(3881, VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 75), iParam3, 1);
				iVar1 = func_615(3882, VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 76), iParam3, 1);
				iVar1 = func_611(VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 97), iParam3, 1);
				iVar1 = func_610(VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 99), iParam3, 1);
				iVar1 = func_600(VehicleIndex);
				func_614(iVar1, GetSlotTrait<int>(VehicleIndex, 98), iParam3, 1);
			}
			iVar2 = Menu::Global(2097152).At(0, 10931).At(6175).At(1274).As<int>();
			if (bParam5) {
				iVar2 = Native::_GetPosixTime();
			}
			func_41(1628, iVar2, iParam3, 1, 0);
			func_41(func_599(1, VehicleIndex), GetSlotTrait<int>(VehicleIndex, 102), iParam3, 1, 0);
			func_41(func_599(2, VehicleIndex), GetSlotTrait<int>(VehicleIndex, 103), iParam3, 1, 0);
			func_41(func_599(3, VehicleIndex), GetSlotTrait<int>(VehicleIndex, 104), iParam3, 1, 0);
			func_41(func_599(4, VehicleIndex), GetSlotTrait<int>(VehicleIndex, 66), iParam3, 1, 0);
			func_41(func_599(5, VehicleIndex), GetSlotTrait<int>(VehicleIndex, 77), iParam3, 1, 0);
			func_41(func_599(7, VehicleIndex), GetSlotTrait<int>(VehicleIndex, 139), iParam3, 1, 0);
			if (!func_613(VehicleIndex)) {
				if (!Native::IsStringNullOrEmpty((const char*)Menu::Global(1323678).At(VehicleIndex, 141).At(Garage_PlateText).Get<uint64_t>())) {
					func_598(func_599(6, VehicleIndex), (char*)Menu::Global(1323678).At(VehicleIndex, 141).At(Garage_PlateText).Get<uint64_t>(), iParam3); // set license plate
				}
			}
			return 1;
		}

	}

	void Test(int Slot) {
		SetSlotTrait(Slot, Garage_PaintPrimary, 120);
		Scripts::func_597(Slot, false, -1, false, false);
		Native::StatSave(0, false, 3);
	}
}

void VehicleSpawnerGarageSelectedMenu::Init() {
	SetName("Garage Vehicles");
	SetParentSubmenu<VehicleSpawnerGarageMenu>();

	addString("spawned");
}

void VehicleSpawnerGarageSelectedMenu::Update() {}

/*Called once on submenu open*/
void VehicleSpawnerGarageSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	for (int Slot : VehicleSpawnerGarageMenuVars::m_Vars.m_ValidSlots[m_Vars.m_Slot]) {
		addOption(ButtonOption(Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(GetSlotTrait<uint32_t>(Slot, Garage_Model))))
			.addOnClick([=] { SpawnVehicle(Slot); }));
	}
}

/*Called always*/
void VehicleSpawnerGarageSelectedMenu::FeatureUpdate() {}

VehicleSpawnerGarageSelectedMenu* _instance;
VehicleSpawnerGarageSelectedMenu* VehicleSpawnerGarageSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerGarageSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerGarageSelectedMenu::~VehicleSpawnerGarageSelectedMenu() { delete _instance; }