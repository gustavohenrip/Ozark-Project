#include "vehicle_editor.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle.hpp"
#include "menu/base/util/global.hpp"
#include "editor/vehicle_editor_lsc.hpp"
#include "menu/base/util/global.hpp"
#include "editor/vehicle_editor_lights.hpp"
#include "editor/vehicle_editor_horn.hpp"
#include "editor/vehicle_editor_wheels.hpp"
#include "editor/vehicle_editor_respray.hpp"
#include "editor/vehicle_editor_bumpers.hpp"
#include "editor/vehicle_editor_fenders.hpp"
#include "editor/vehicle_editor_bennys_interior.hpp"
#include "utils/log.hpp"

using namespace VehicleEditorMenuVars;

namespace VehicleEditorMenuVars
{
	Vars_ m_Vars;

	namespace Scripts
	{
		void func_1666(float fParam0, float fParam1, float fParam2, float fParam3, int iParam4, int iParam5, int iParam6, int iParam7)
		{
			Native::DrawRect((fParam0 + (fParam2 * 0.5f)), (fParam1 + (fParam3 * 0.5f)), fParam2, fParam3, iParam4, iParam5, iParam6, iParam7, 0);
		}

		void DrawVehicleStatRects(float fParam0, float fParam1, float fParam2, float fParam3, int iParam4, int iParam5, int iParam6, int iParam7)
		{
			float fVar0;
			float fVar1;
			float fVar2;
			int iVar3;

			fVar0 = ((125.f - 8.f) * 0.00078125f);
			fVar1 = (125.f * 0.00078125f);
			fVar2 = fParam0;
			iVar3 = 0;
			while (iVar3 < 5)
			{
				if (fParam2 > ((fVar0 / 100.f) * 20.f))
				{
					func_1666(fVar2, fParam1, ((fVar0 / 100.f) * 20.f), fParam3, iParam4, iParam5, iParam6, iParam7);
				}
				else if (fParam2 > 0.f)
				{
					func_1666(fVar2, fParam1, fParam2, fParam3, iParam4, iParam5, iParam6, iParam7);
				}

				fParam2 = (fParam2 - ((fVar0 / 100.f) * 20.f));
				fVar2 = (fVar2 + ((fVar1 / 100.f) * 20.f));
				iVar3++;
			}
		}

		void func_1665(int iParam0, int *uParam1, int *uParam2, int *uParam3)
		{
			switch (iParam0)
			{
			case 0:
				*uParam1 = 8;
				*uParam2 = 8;
				*uParam3 = 8;
				break;

			case 1:
				*uParam1 = 37;
				*uParam2 = 37;
				*uParam3 = 39;
				break;

			case 22:
				*uParam1 = 140;
				*uParam2 = 146;
				*uParam3 = 154;
				break;

			case 23:
				*uParam1 = 91;
				*uParam2 = 93;
				*uParam3 = 94;
				break;

			case 6:
				*uParam1 = 81;
				*uParam2 = 84;
				*uParam3 = 89;
				break;

			case 111:
				*uParam1 = 240;
				*uParam2 = 240;
				*uParam3 = 240;
				break;

			case 28:
				*uParam1 = 150;
				*uParam2 = 8;
				*uParam3 = 0;
				break;

			case 34:
				*uParam1 = 38;
				*uParam2 = 3;
				*uParam3 = 6;
				break;

			case 88:
				*uParam1 = 245;
				*uParam2 = 137;
				*uParam3 = 15;
				break;

			case 45:
				*uParam1 = 74;
				*uParam2 = 22;
				*uParam3 = 7;
				break;

			case 56:
				*uParam1 = 45;
				*uParam2 = 58;
				*uParam3 = 53;
				break;

			case 58:
				*uParam1 = 71;
				*uParam2 = 120;
				*uParam3 = 60;
				break;

			case 54:
				*uParam1 = 77;
				*uParam2 = 98;
				*uParam3 = 104;
				break;

			case 73:
				*uParam1 = 14;
				*uParam2 = 49;
				*uParam3 = 109;
				break;

			case 68:
				*uParam1 = 22;
				*uParam2 = 34;
				*uParam3 = 72;
				break;

			case 140:
				*uParam1 = 0;
				*uParam2 = 174;
				*uParam3 = 239;
				break;

			case 131:
				*uParam1 = 255;
				*uParam2 = 183;
				*uParam3 = 0;
				break;

			case 90:
				*uParam1 = 142;
				*uParam2 = 140;
				*uParam3 = 70;
				break;

			case 97:
				*uParam1 = 156;
				*uParam2 = 141;
				*uParam3 = 113;
				break;

			case 89:
				*uParam1 = 145;
				*uParam2 = 115;
				*uParam3 = 71;
				break;

			case 105:
				*uParam1 = 98;
				*uParam2 = 68;
				*uParam3 = 40;
				break;

			case 100:
				*uParam1 = 124;
				*uParam2 = 27;
				*uParam3 = 68;
				break;

			case 99:
				*uParam1 = 114;
				*uParam2 = 42;
				*uParam3 = 63;
				break;

			case 136:
				*uParam1 = 246;
				*uParam2 = 151;
				*uParam3 = 153;
				break;

			case 49:
				*uParam1 = 32;
				*uParam2 = 32;
				*uParam3 = 44;
				break;

			case 146:
				*uParam1 = 26;
				*uParam2 = 1;
				*uParam3 = 23;
				break;

			default:
				*uParam1 = 255;
				*uParam2 = 255;
				*uParam3 = 255;
				break;
			}
		}

		void func_1664(bool bParam0, bool bParam1, bool bParam2, bool bParam3, int iParam4, bool bParam5, bool bParam6)
		{
			Native::SetTextScale(0.f, 0.35f);
			Native::SetTextJustification(1);

			if (bParam5)
			{
				Native::SetTextScale(0.f, 0.425f);
				Native::SetTextFont(4);
			}
			else if (bParam6)
			{
				Native::SetTextScale(0.f, 0.425f);
				Native::SetTextFont(6);
			}
			else
			{
				Native::SetTextFont(0);
			}

			Native::SetTextWrap(0.f, 1.f);
			Native::SetTextCentre(0);
			Native::SetTextDropshadow(0, 0, 0, 0, 0);
			Native::SetTextEdge(0, 0, 0, 0, 0);
			Native::SetTextColour(255, 255, 255, 255);
		}

		int func_243(int iParam0)
		{
			if (iParam0 != -1)
			{
				return Native::IsBitSet(Menu::Global(2426097).At(iParam0, 443).At(314).As<int>(), 6);
			}
			return 0;
		}

		int func_1027(int iParam0)
		{
			if (iParam0 != -1)
			{
				return Native::IsBitSet(Menu::Global(2426097).At(iParam0, 443).At(314).At(2).As<int>(), 6);
			}
			return 0;
		}

		int func_1013(int iParam0)
		{
			if (iParam0 != -1)
			{
				return Native::IsBitSet(Menu::Global(2426097).At(iParam0, 443).At(314).At(3).As<int>(), 4);
			}
			return 0;
		}

		int IsModelBicycle(int iParam0)
		{
			if (iParam0 == joaat("BMX"))
			{
				return 1;
			}
			if (iParam0 == joaat("CRUISER"))
			{
				return 1;
			}
			if (iParam0 == joaat("SCORCHER"))
			{
				return 1;
			}
			if ((iParam0 == joaat("TRIBIKE") || iParam0 == joaat("TRIBIKE2")) || iParam0 == joaat("TRIBIKE3"))
			{
				return 1;
			}
			if (iParam0 == joaat("FIXTER"))
			{
				return 1;
			}
			return 0;
		}

		int func_2480(int iParam0)
		{
			switch (iParam0)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 17:
			case 18:
			case 19:
			case 20:
			case 22:
				return 0;
				break;

			case 14:
				return 3;
				break;

			case 15:
				return 1;
				break;

			case 16:
				return 2;
				break;
			}
			return -1;
		}

		int func_19(int iParam0, int iParam1)
		{
			switch (iParam0)
			{
			case 0:
				switch (iParam1)
				{
				case 0:
					return 0;

				case 1:
					return 1;

				case 2:
					return 2;

				case 3:
					return 3;

				case 4:
					return 4;

				case 5:
					return 5;

				case 6:
					return 6;

				case 7:
					return 7;

				case 8:
					return 8;

				case 9:
					return 9;

				case 10:
					return 10;

				case 11:
					return 11;

				case 12:
					return 12;

				case 13:
					return 13;

				case 14:
					return 17;

				case 15:
					return 18;

				case 16:
					return 19;

				case 17:
					return 20;

				case 18:
					return 22;
				}
				break;

			case 3:
				switch (iParam1)
				{
				case 0:
					return 14;
				}
				break;

			case 1:
				switch (iParam1)
				{
				case 0:
					return 15;
				}
				break;

			case 2:
				switch (iParam1)
				{
				case 0:
					return 16;
				}
				break;
			}
			return -1;
		}

		float func_18(int iParam0)
		{
			if (iParam0 == 7)
			{
				return 51.77096f;
			}
			return Native::GetVehicleClassEstimatedSpeed(iParam0);
		}

		bool test1 = false;

		void PopulateGlobals(int *uParam0)
		{
			int iVar0;
			int iVar1;
			float fVar2;

			if (!test1)
			{
				iVar1 = func_19(uParam0[0], uParam0[1]);
				if (iVar1 == -1)
				{
					uParam0[0]++;
					uParam0[1] = 0;
					if (uParam0[0] >= 4)
					{
						test1 = 1;
					}
					return;
				}
				else
				{
					iVar0 = iVar1;

					if ((iVar0 == 15 || iVar0 == 16) || iVar0 == 14)
					{
						fVar2 = Native::GetVehicleClassMaxAgility(iVar0);
					}
					else
					{
						fVar2 = Native::GetVehicleClassMaxTraction(iVar0);
					}

					if (m_Vars.m_Values[uParam0[0]][0] < func_18(iVar0))
						m_Vars.m_Values[uParam0[0]][0] = func_18(iVar0);
					if (m_Vars.m_Values[uParam0[0]][1] < Native::GetVehicleClassMaxAcceleration(iVar0))
						m_Vars.m_Values[uParam0[0]][1] = Native::GetVehicleClassMaxAcceleration(iVar0);
					if (m_Vars.m_Values[uParam0[0]][2] < Native::GetVehicleClassMaxBraking(iVar0))
						m_Vars.m_Values[uParam0[0]][2] = Native::GetVehicleClassMaxBraking(iVar0);
					if (m_Vars.m_Values[uParam0[0]][3] < fVar2)
						m_Vars.m_Values[uParam0[0]][3] = fVar2;

					uParam0[1]++;
				}
			}
		}

		void GetVehicleStats(int iParam0, VehicleStats *uParam1)
		{
			float fVar1;
			int iVar2;
			float fVar3;

			if (func_243(Menu::GetLocalPlayer().m_ID) && Native::DoesEntityExist(Menu::Global(2540384).At(304).At(0).As<Entity>()))
			{
				if (iParam0 == Menu::Global(2540384).At(304).At(1).As<Entity>() || iParam0 == Menu::Global(101708).As<int>())
				{
					return;
				}
			}

			if (func_1027(Menu::GetLocalPlayer().m_ID) && Native::DoesEntityExist(Menu::Global(2540384).At(308).As<int>()))
			{
				if (iParam0 == Menu::Global(2540384).At(308).As<int>())
				{
					return;
				}
			}

			if (func_1013(Menu::GetLocalPlayer().m_ID) && Native::DoesEntityExist(Menu::Global(2540384).At(316).As<int>()))
			{
				if (iParam0 == Menu::Global(2540384).At(316).As<int>())
				{
					return;
				}
			}

			if (!Native::IsEntityDead(iParam0, 0))
			{
				if (!uParam1->m_Success)
				{
					fVar1 = 1.f;
					iVar2 = Native::GetEntityModel(iParam0);

					if (IsModelBicycle(iVar2))
					{
						fVar1 = 0.5f;
					}
					else
					{
						fVar1 = 1.f;
					}

					uParam1->m_EstimatedMaxSpeed = Native::GetVehicleEstimatedMaxSpeed(iParam0);
					uParam1->m_Braking = (Native::GetVehicleMaxBraking(iParam0) * fVar1);
					uParam1->m_Acceleration = (Native::GetVehicleAcceleration(iParam0) * fVar1);

					if (iVar2 == joaat("VOLTIC"))
					{
						uParam1->m_Acceleration = (Native::GetVehicleAcceleration(iParam0) * 2.f);
					}

					if (iVar2 == joaat("tezeract"))
					{
						uParam1->m_Acceleration = (Native::GetVehicleAcceleration(iParam0) * 2.6753f);
					}

					if (iVar2 == joaat("jester3"))
					{
						uParam1->m_EstimatedMaxSpeed = (Native::GetVehicleEstimatedMaxSpeed(iParam0) * 0.9890084f);
					}

					if (iVar2 == joaat("freecrawler"))
					{
						uParam1->m_EstimatedMaxSpeed = (Native::GetVehicleEstimatedMaxSpeed(iParam0) * 0.9788762f);
					}

					if (iVar2 == joaat("swinger"))
					{
						uParam1->m_EstimatedMaxSpeed = (Native::GetVehicleEstimatedMaxSpeed(iParam0) * 0.9650553f);
					}

					if (iVar2 == joaat("menacer"))
					{
						uParam1->m_EstimatedMaxSpeed = (Native::GetVehicleEstimatedMaxSpeed(iParam0) * 0.9730466f);
					}

					if (iVar2 == joaat("speedo4"))
					{
						uParam1->m_EstimatedMaxSpeed = (Native::GetVehicleEstimatedMaxSpeed(iParam0) * 0.9426523f);
					}

					if (Native::IsThisModelAHeli(iVar2) || Native::IsThisModelAPlane(iVar2))
					{
						fVar3 = (Native::_0xC6AD107DDC9054CC(iVar2) * fVar1);
					}
					else if (Native::IsThisModelABoat(iVar2))
					{
						fVar3 = (Native::_0x5AA3F878A178C4FC(iVar2) * fVar1);
					}
					else
					{
						fVar3 = (Native::GetVehicleMaxTraction(iParam0) * fVar1);
					}

					uParam1->m_Traction = fVar3;
					if (iVar2 == joaat("T20"))
					{
						uParam1->m_Acceleration = (uParam1->m_Acceleration - 0.05f);
					}
					else if (iVar2 == joaat("VINDICATOR"))
					{
						uParam1->m_Acceleration = (uParam1->m_Acceleration - 0.02f);
					}

					while (!test1)
					{
						PopulateGlobals(m_Vars.m_GlobalStats);
					}

					int iVar4 = func_2480(Native::GetVehicleClass(iParam0));
					if (iVar4 == -1)
					{
						uParam1->m_Success = false;
						uParam1->m_EstimatedMaxSpeed = 0.f;
						uParam1->m_Acceleration = 0.f;
						uParam1->m_Braking = 0.f;
						uParam1->m_Traction = 0.f;
						return;
					}

					uParam1->m_EstimatedMaxSpeed = ((uParam1->m_EstimatedMaxSpeed / m_Vars.m_Values[iVar4][0]) * 100.f);
					if (uParam1->m_EstimatedMaxSpeed > 100.f)
					{
						uParam1->m_EstimatedMaxSpeed = 100.f;
					}

					uParam1->m_Acceleration = ((uParam1->m_Acceleration / m_Vars.m_Values[iVar4][1]) * 100.f);
					if (uParam1->m_Acceleration > 100.f)
					{
						uParam1->m_Acceleration = 100.f;
					}

					uParam1->m_Braking = ((uParam1->m_Braking / m_Vars.m_Values[iVar4][2]) * 100.f);
					if (uParam1->m_Braking > 100.f)
					{
						uParam1->m_Braking = 100.f;
					}

					uParam1->m_Traction = ((uParam1->m_Traction / m_Vars.m_Values[iVar4][3]) * 100.f);
					if (uParam1->m_Traction > 100.f)
					{
						uParam1->m_Traction = 100.f;
					}

					uParam1->m_Success = true;
				}
			}
		}

		void DrawVehicleStats(Vehicle Veh)
		{
			float Y = 0.f;

			int Count = 0;
			auto Options = Menu::GetSubmenuHandler()->GetCurrentSubmenu()->m_Options;

			for (std::size_t i = 0; i < Options.size(); i++)
			{
				if (Options.at(i)->IsVisible())
				{
					Count++;
				}
			}

			if (Count > Menu::GetBase()->m_MaxOptions)
			{
				Y = Global::UIVars::g_MenuY + (Menu::GetBase()->m_MaxOptions * 0.0315f) + 0.0315f;
			}
			else
			{
				Y = Global::UIVars::g_MenuY + (Count * 0.0315f);
			}

			Y += 0.002f;

			Menu::GetRenderer()->DrawRect({Global::UIVars::g_MenuX, Y}, {0.23f, 0.15f}, {0, 0, 0, 200});

			float fVar42 = (125.f - 8.f);

			int iVar43 = 0;
			int iVar44 = 0;
			int iVar45 = 0;
			int iVar46 = 0;

			float fVar49 = 0.0115f;
			Menu::GetRenderer()->DrawText("Top Speed", {Global::UIVars::g_MenuX + 0.004f, (Y + fVar49)}, 0.315f, Global::UIVars::g_OptionFont, {255, 255, 255, 255});
			fVar49 = (fVar49 + 0.034722f);

			Menu::GetRenderer()->DrawText("Acceleration", {Global::UIVars::g_MenuX + 0.004f, (Y + fVar49)}, 0.315f, Global::UIVars::g_OptionFont, {255, 255, 255, 255});
			fVar49 = (fVar49 + 0.034722f);

			Menu::GetRenderer()->DrawText("Braking", {Global::UIVars::g_MenuX + 0.004f, (Y + fVar49)}, 0.315f, Global::UIVars::g_OptionFont, {255, 255, 255, 255});
			fVar49 = (fVar49 + 0.034722f);

			Menu::GetRenderer()->DrawText("Traction", {Global::UIVars::g_MenuX + 0.004f, (Y + fVar49)}, 0.315f, Global::UIVars::g_OptionFont, {255, 255, 255, 255});

			Native::GetHudColour(1, &iVar43, &iVar44, &iVar45, &iVar46);
			iVar46 = 76;

			float fVar51 = 0.017f;
			DrawVehicleStatRects(Global::UIVars::g_MenuX + 0.11f, (Y + fVar51), (0.00078125f * fVar42), (0.00138888f * 6.f), iVar43, iVar44, iVar45, iVar46); // faint bars - w=0.09140625

			fVar51 = (fVar51 + 0.034722f);
			DrawVehicleStatRects(Global::UIVars::g_MenuX + 0.11f, (Y + fVar51), (0.00078125f * fVar42), (0.00138888f * 6.f), iVar43, iVar44, iVar45, iVar46); // faint bars

			fVar51 = (fVar51 + 0.034722f);
			DrawVehicleStatRects(Global::UIVars::g_MenuX + 0.11f, (Y + fVar51), (0.00078125f * fVar42), (0.00138888f * 6.f), iVar43, iVar44, iVar45, iVar46); // faint bars

			fVar51 = (fVar51 + 0.034722f);
			DrawVehicleStatRects(Global::UIVars::g_MenuX + 0.11f, (Y + fVar51), (0.00078125f * fVar42), (0.00138888f * 6.f), iVar43, iVar44, iVar45, iVar46); // faint bars

			fVar51 = 0.017f;

			Scripts::test1 = 0;
			m_Vars.m_NewStats.m_Success = false;
			GetVehicleStats(Veh, &m_Vars.m_NewStats);

			int iVar58 = 0;
			float fVar52 = 0.f;
			float fVar53 = 0.f;

			float CachedData[] = {m_Vars.m_CachedStats.m_EstimatedMaxSpeed, m_Vars.m_CachedStats.m_Acceleration, m_Vars.m_CachedStats.m_Braking, m_Vars.m_CachedStats.m_Traction};
			float NewData[] = {m_Vars.m_NewStats.m_EstimatedMaxSpeed, m_Vars.m_NewStats.m_Acceleration, m_Vars.m_NewStats.m_Braking, m_Vars.m_NewStats.m_Traction};

			while (iVar58 < 4)
			{
				fVar52 = (0.00078125f * ((fVar42 / 100.f) * (NewData[iVar58] - CachedData[iVar58])));
				fVar53 = (0.00078125f * ((fVar42 / 100.f) * CachedData[iVar58]));

				// handles showing what the upgrade will do for u
				if (fVar52 < 0.f)
				{
					Native::GetHudColour(6, &iVar43, &iVar44, &iVar45, &iVar46);
					DrawVehicleStatRects(Global::UIVars::g_MenuX + 0.11f, (Y + fVar51), fVar53, (0.00138888f * 6.f), iVar43, iVar44, iVar45, iVar46); // less -> red
					fVar53 = (fVar53 + fVar52);
				}
				else
				{
					Native::GetHudColour(9, &iVar43, &iVar44, &iVar45, &iVar46);
					DrawVehicleStatRects(Global::UIVars::g_MenuX + 0.11f, (Y + fVar51), (fVar53 + fVar52), (0.00138888f * 6.f), iVar43, iVar44, iVar45, iVar46); // more -> blue
				}

				Native::GetHudColour(1, &iVar43, &iVar44, &iVar45, &iVar46);
				DrawVehicleStatRects(Global::UIVars::g_MenuX + 0.11f, (Y + fVar51), fVar53, (0.00138888f * 6.f), iVar43, iVar44, iVar45, 255); // old value
				fVar51 = (fVar51 + 0.034722f);
				iVar58++;
			}
		}
	};

	void UpdateCaches()
	{
		m_Vars.m_CachedStats.m_Success = false;
		Scripts::GetVehicleStats(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_CachedStats);
	}

	void CacheVehicleStats()
	{
		if (!Menu::GetLocalPlayer().m_InVehicle)
			return;

		Scripts::test1 = 0;
		m_Vars.m_GlobalStats[0] = m_Vars.m_GlobalStats[1] = 0;
		m_Vars.m_CachedStats.m_Success = false;
		Scripts::GetVehicleStats(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_CachedStats);

		Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
		m_Vars.m_CachedStats.m_Armor = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR);
		m_Vars.m_CachedStats.m_Brakes = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES);
		m_Vars.m_CachedStats.m_FrontBumper = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER);
		m_Vars.m_CachedStats.m_RearBumper = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER);
		m_Vars.m_CachedStats.m_Engine = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE);
		m_Vars.m_CachedStats.m_Exhaust = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_EXHAUST);
		m_Vars.m_CachedStats.m_Hood = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HOOD);
		m_Vars.m_CachedStats.m_Horn = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS);
		m_Vars.m_CachedStats.m_Skirts = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT);
		m_Vars.m_CachedStats.m_Suspension = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION);
		m_Vars.m_CachedStats.m_Transmission = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRANSMISSION);
		m_Vars.m_CachedStats.m_Spoiler = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPOILER);
		m_Vars.m_CachedStats.m_Grille = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_GRILLE);
		m_Vars.m_CachedStats.m_LeftFender = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER);
		m_Vars.m_CachedStats.m_RightFender = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER);
		m_Vars.m_CachedStats.m_Roof = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ROOF);
		m_Vars.m_CachedStats.m_Chassis = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_CHASSIS);
		m_Vars.m_CachedStats.m_WindowTint = Native::GetVehicleWindowTint(Menu::GetLocalPlayer().m_Vehicle);
		m_Vars.m_CachedStats.m_Turbo = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_TURBO);
		m_Vars.m_CachedStats.m_Lights = Native::IsToggleModOn(Menu::GetLocalPlayer().m_Vehicle, MOD_XENONLIGHTS);
		m_Vars.m_CachedStats.m_XenonColor = Native::GetVehicleXenonLightsColour(Menu::GetLocalPlayer().m_Vehicle);
		m_Vars.m_CachedStats.m_PlateIndex = Native::GetVehicleNumberPlateTextIndex(Menu::GetLocalPlayer().m_Vehicle);

		// Bennys
		m_Vars.m_CachedStats.m_BennysPlateholder = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLATEHOLDER);
		m_Vars.m_CachedStats.m_BennysVanityPlate = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_VANITY_PLATES);
		m_Vars.m_CachedStats.m_BennysTrimDesign = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM_DESIGN);
		m_Vars.m_CachedStats.m_BennysOrnaments = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ORNAMENTS);
		m_Vars.m_CachedStats.m_BennysInteriorDesign = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DASHBOARD);
		m_Vars.m_CachedStats.m_BennysDials = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DIAL);
		m_Vars.m_CachedStats.m_BennysDoors = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_DOOR_SPEAKER);
		m_Vars.m_CachedStats.m_BennysSeats = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SEATS);
		m_Vars.m_CachedStats.m_BennysSteeringWheel = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STEERINGWHEEL);
		m_Vars.m_CachedStats.m_BennysShifter = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SHIFTER_LEAVERS);
		m_Vars.m_CachedStats.m_BennysPlaques = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLAQUES);
		m_Vars.m_CachedStats.m_BennysTrunk = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRUNK);
		m_Vars.m_CachedStats.m_BennysAudioInstall = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPEAKERS);
		m_Vars.m_CachedStats.m_BennysHydraulics = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HYDRULICS);
		m_Vars.m_CachedStats.m_BennysEngineBlock = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE_BLOCK);
		m_Vars.m_CachedStats.m_BennysAirFilters = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AIR_FILTER);
		m_Vars.m_CachedStats.m_BennysStuntBrace = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STRUTS);
		m_Vars.m_CachedStats.m_BennysArchCovers = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARCH_COVER);
		m_Vars.m_CachedStats.m_BennysAerials = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AERIALS);
		m_Vars.m_CachedStats.m_BennysRearStripeColor = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM);
		m_Vars.m_CachedStats.m_BennysTank = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TANK);
		m_Vars.m_CachedStats.m_BennysWindowMod = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_WINDOWS);
		m_Vars.m_CachedStats.m_BennysLivery = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_LIVERY);
		m_Vars.m_CachedStats.m_BennysSpecialFrontRims = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPECIAL_FRONT_RIMS);
		m_Vars.m_CachedStats.m_BennysSpecialRearRims = Native::GetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPECIAL_REAR_RIMS);

		for (int i = 0; i < 4; i++)
		{
			m_Vars.m_CachedStats.m_Neon[i] = Native::_IsVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, i);
		}

		Native::_GetVehicleNeonLightsColour(Menu::GetLocalPlayer().m_Vehicle, &m_Vars.m_CachedStats.m_NeonColor.R, &m_Vars.m_CachedStats.m_NeonColor.G, &m_Vars.m_CachedStats.m_NeonColor.B);
	}

	bool CanRunBennysInterior()
	{
		if (!Menu::GetLocalPlayer().m_InVehicle)
			return false;

		Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_DASHBOARD) > 0 || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_DIAL) > 0 || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_DOOR_SPEAKER) > 0 || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SEATS) > 0 || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_STEERINGWHEEL) > 0 || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ORNAMENTS) > 0 || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SHIFTER_LEAVERS) > 0 || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SPEAKERS) > 0)
		{
			return true;
		}

		return false;
	}

	std::string GetVehicleModName(eVehicleModTypes Type, std::string Default)
	{
		if (Native::DoesTextLabelExist(Native::GetModSlotName(Menu::GetLocalPlayer().m_Vehicle, Type)))
		{
			if (strcmp(Native::_GetLabelText(Native::GetModSlotName(Menu::GetLocalPlayer().m_Vehicle, Type)), "NULL"))
			{
				return Native::_GetLabelText(Native::GetModSlotName(Menu::GetLocalPlayer().m_Vehicle, Type));
			}
		}

		return Default;
	}
}

void VehicleEditorMenu::Init()
{
	SetName("Los Santos Customs");
	SetParentSubmenu<VehicleMenu>();

	addString("Armor");
	addString("Brakes");
	addString("Bumpers");
	addString("Chassis");
	addString("Engine");
	addString("Exhaust");
	addString("Fenders");
	addString("Grille");
	addString("Hood");
	addString("Horn");
	addString("Lights");
	addString("Plate");
	addString("Respray");
	addString("Roof");
	addString("Skirts");
	addString("Spoiler");
	addString("Suspension");
	addString("Transmission");
	addString("Turbo");
	addString("Wheels");
	addString("Windows");

	addString("Interior");
	addString("Aerials");
	addString("Air Filters");
	addString("Arch Covers");
	addString("Engine Block");
	addString("Hydraulics");
	addString("Livery");
	addString("Plaques");
	addString("Plate Holder");
	addString("Rear Stripe");
	addString("Stunt Brace");
	addString("Tank");
	addString("Trim Design");
	addString("Trunk");
	addString("Vanity Plate");
	addString("Window Mod");
}

void VehicleEditorMenu::Update()
{
	if (!Menu::GetLocalPlayer().m_InVehicle)
	{
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	VehicleEditorMenuVars::Scripts::DrawVehicleStats(Menu::GetLocalPlayer().m_Vehicle);
}

/*Called once on submenu open*/
void VehicleEditorMenu::UpdateOnce()
{
	ClearOptionsOffset(0);

	if (m_Vars.m_UpdateCache)
	{
		m_Vars.m_UpdateCache = false;

		if (Menu::GetLocalPlayer().m_InVehicle)
		{
			Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR, m_Vars.m_CachedStats.m_Armor, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES, m_Vars.m_CachedStats.m_Brakes, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER, m_Vars.m_CachedStats.m_FrontBumper, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER, m_Vars.m_CachedStats.m_RearBumper, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE, m_Vars.m_CachedStats.m_Engine, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_EXHAUST, m_Vars.m_CachedStats.m_Exhaust, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HOOD, m_Vars.m_CachedStats.m_Hood, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS, m_Vars.m_CachedStats.m_Horn, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT, m_Vars.m_CachedStats.m_Skirts, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION, m_Vars.m_CachedStats.m_Suspension, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRANSMISSION, m_Vars.m_CachedStats.m_Transmission, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_SPOILER, m_Vars.m_CachedStats.m_Spoiler, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_GRILLE, m_Vars.m_CachedStats.m_Grille, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER, m_Vars.m_CachedStats.m_LeftFender, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER, m_Vars.m_CachedStats.m_RightFender, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ROOF, m_Vars.m_CachedStats.m_Roof, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_CHASSIS, m_Vars.m_CachedStats.m_Chassis, false);
			Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TURBO, (bool)m_Vars.m_CachedStats.m_Turbo);
			Native::ToggleVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_XENONLIGHTS, (bool)m_Vars.m_CachedStats.m_Lights);
			Native::SetVehicleXenonLightsColour(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_CachedStats.m_XenonColor);
			Native::SetVehicleWindowTint(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_CachedStats.m_WindowTint);
			Native::SetVehicleNumberPlateTextIndex(Menu::GetLocalPlayer().m_Vehicle, m_Vars.m_CachedStats.m_PlateIndex);

			for (int i = 0; i < 4; i++)
			{
				Native::_SetVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, i, m_Vars.m_CachedStats.m_Neon[i]);
			}

			// Bennys
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLATEHOLDER, m_Vars.m_CachedStats.m_BennysPlateholder, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_VANITY_PLATES, m_Vars.m_CachedStats.m_BennysVanityPlate, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM_DESIGN, m_Vars.m_CachedStats.m_BennysTrimDesign, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_PLAQUES, m_Vars.m_CachedStats.m_BennysPlaques, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRUNK, m_Vars.m_CachedStats.m_BennysTrunk, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_HYDRULICS, m_Vars.m_CachedStats.m_BennysHydraulics, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE_BLOCK, m_Vars.m_CachedStats.m_BennysEngineBlock, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AIR_FILTER, m_Vars.m_CachedStats.m_BennysAirFilters, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_STRUTS, m_Vars.m_CachedStats.m_BennysStuntBrace, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_ARCH_COVER, m_Vars.m_CachedStats.m_BennysArchCovers, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_AERIALS, m_Vars.m_CachedStats.m_BennysAerials, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM, m_Vars.m_CachedStats.m_BennysRearStripeColor, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_TANK, m_Vars.m_CachedStats.m_BennysTank, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_WINDOWS, m_Vars.m_CachedStats.m_BennysWindowMod, false);
			Native::SetVehicleMod(Menu::GetLocalPlayer().m_Vehicle, MOD_LIVERY, m_Vars.m_CachedStats.m_BennysLivery, false);
		}
	}

	CacheVehicleStats();

	Native::SetVehicleModKit(Menu::GetLocalPlayer().m_Vehicle, 0);

	if (m_Vars.m_LSC)
	{
		SetName("Los Santos Customs");

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ARMOR))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_ARMOR, getString("Armor")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_ARMOR;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Armor;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_BRAKES))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_BRAKES, getString("Brakes")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_BRAKES;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Brakes;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTBUMPER) || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_REARBUMPER))
		{
			addOption(SubmenuOption(getString("Bumpers"))
						  .addSubmenu<VehicleEditorBumpersMenu>()
						  .addOnClick(CacheVehicleStats));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_CHASSIS))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_CHASSIS, getString("Chassis")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_CHASSIS;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Chassis;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_ENGINE, getString("Engine")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_ENGINE;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Engine;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_EXHAUST))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_EXHAUST, getString("Exhaust")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_EXHAUST;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Exhaust;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FENDER) || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_RIGHTFENDER))
		{
			addOption(SubmenuOption(getString("Fenders"))
						  .addSubmenu<VehicleEditorFendersMenu>()
						  .addOnClick(CacheVehicleStats));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_GRILLE))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_GRILLE, getString("Grille")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_GRILLE;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Grille;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_HOOD))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_HOOD, getString("Hood")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_HOOD;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Hood;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_HORNS))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_HORNS, getString("Horn")))
						  .addSubmenu<VehicleEditorHornMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
						  }));
		}

		addOption(SubmenuOption(getString("Lights"))
					  .addSubmenu<VehicleEditorLightsMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_XENONLIGHTS;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Lights;
					  }));

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_LIVERY))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_LIVERY, getString("Livery")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_LIVERY;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysLivery;
						  }));
		}

		addOption(SubmenuOption(getString("Plate"))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] { CacheVehicleStats(); VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_PLATES; }));

		addOption(SubmenuOption(getString("Respray"))
					  .addSubmenu<VehicleEditorResprayMenu>()
					  .addOnClick(CacheVehicleStats));

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ROOF))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_ROOF, getString("Roof")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_ROOF;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Roof;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SIDESKIRT))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_SIDESKIRT, getString("Skirts")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_SIDESKIRT;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Skirts;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SPOILER))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_SPOILER, getString("Spoiler")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] {
							  CacheVehicleStats();
							  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_SPOILER;
							  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Spoiler;
						  }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_SUSPENSION))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_SUSPENSION, getString("Suspension")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] { CacheVehicleStats(); VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_SUSPENSION; }));
		}

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_TRANSMISSION))
		{
			addOption(SubmenuOption(GetVehicleModName(MOD_TRANSMISSION, getString("Transmission")))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] { CacheVehicleStats(); VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_TRANSMISSION; }));
		}

		addOption(SubmenuOption(getString("Turbo"))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_TURBO;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_Turbo;
					  }));

		if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS) || Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_BACKWHEELS))
		{
			addOption(SubmenuOption(getString("Wheels"))
						  .addSubmenu<VehicleEditorWheelsMenu>()
						  .addOnClick(CacheVehicleStats));

			if (m_Vars.m_WheelSport.empty())
			{
				int Cached = Native::GetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 0);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelSport.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelSport.resize(m_Vars.m_WheelSport.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 1);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelMuscle.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelMuscle.resize(m_Vars.m_WheelMuscle.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 2);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelLowrider.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelLowrider.resize(m_Vars.m_WheelLowrider.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 3);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelSUV.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelSUV.resize(m_Vars.m_WheelSUV.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 4);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelOffroad.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelOffroad.resize(m_Vars.m_WheelOffroad.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 5);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelTuner.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelTuner.resize(m_Vars.m_WheelTuner.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 6);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelBike.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelBike.resize(m_Vars.m_WheelBike.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, 7);
				for (int i = 0; i < Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS); i++)
				{
					m_Vars.m_WheelHighEnd.push_back({Native::GetModTextLabel(Menu::GetLocalPlayer().m_Vehicle, MOD_FRONTWHEELS, i), i});
				}
				m_Vars.m_WheelHighEnd.resize(m_Vars.m_WheelHighEnd.size() / 2);

				Native::SetVehicleWheelType(Menu::GetLocalPlayer().m_Vehicle, Cached);

				LOG_INFO("Cached LSC wheels!");
			}
		}

		if (Native::IsThisModelACar(Native::GetEntityModel(Menu::GetLocalPlayer().m_Vehicle)))
		{
			addOption(SubmenuOption(getString("Windows"))
						  .addSubmenu<VehicleEditorLSCMenu>()
						  .addOnClick([] { CacheVehicleStats(); VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_WINDOWS_CUSTOM; }));
		}

		return;
	}

	// Bennys
	SetName("Benny's");

	addOption(SubmenuOption(getString("Interior"))
				  .addSubmenu<VehicleEditorBennysInteriorMenu>()
				  .addRequirement(CanRunBennysInterior));

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_AERIALS))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_AERIALS, getString("Aerials")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_AERIALS;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysAerials;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_AIR_FILTER))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_AIR_FILTER, getString("Air Filters")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_AIR_FILTER;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysAirFilters;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ARCH_COVER))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_ARCH_COVER, getString("Arch Covers")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_ARCH_COVER;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysArchCovers;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_ENGINE_BLOCK))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_ENGINE_BLOCK, getString("Engine Block")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_ENGINE_BLOCK;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysEngineBlock;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_HYDRULICS))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_HYDRULICS, getString("Hydraulics")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_HYDRULICS;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysHydraulics;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_LIVERY))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_LIVERY, getString("Livery")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_LIVERY;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysLivery;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_PLAQUES))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_PLAQUES, getString("Plaques")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_PLAQUES;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysPlaques;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_PLATEHOLDER))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_PLATEHOLDER, getString("Plate Holder")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_PLATEHOLDER;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysPlateholder;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_TRIM, getString("Rear Stripe")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_TRIM;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysRearStripeColor;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_STRUTS))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_STRUTS, getString("Stunt Brace")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_STRUTS;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysStuntBrace;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_TANK))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_TANK, getString("Tank")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_TANK;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysTank;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_TRIM_DESIGN))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_TRIM_DESIGN, getString("Trim Design")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_TRIM_DESIGN;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysTrimDesign;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_TRUNK))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_TRUNK, getString("Trunk")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_TRUNK;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysTrunk;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_VANITY_PLATES))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_VANITY_PLATES, getString("Vanity Plate")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_VANITY_PLATES;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysVanityPlate;
					  }));
	}

	if (Native::GetNumVehicleMods(Menu::GetLocalPlayer().m_Vehicle, MOD_WINDOWS))
	{
		addOption(SubmenuOption(GetVehicleModName(MOD_WINDOWS, getString("Window Mod")))
					  .addSubmenu<VehicleEditorLSCMenu>()
					  .addOnClick([] {
						  CacheVehicleStats();
						  VehicleEditorLSCMenuVars::m_Vars.m_ModType = MOD_WINDOWS;
						  VehicleEditorLSCMenuVars::m_Vars.m_Value = &m_Vars.m_CachedStats.m_BennysWindowMod;
					  }));
	}
}

/*Called always*/
void VehicleEditorMenu::FeatureUpdate() {}

VehicleEditorMenu *_instance;
VehicleEditorMenu *VehicleEditorMenu::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new VehicleEditorMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorMenu::~VehicleEditorMenu() { delete _instance; }