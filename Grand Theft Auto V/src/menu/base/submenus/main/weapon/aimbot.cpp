#include "aimbot.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/camera.hpp"

using namespace AimbotMenuVars;

namespace AimbotMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> AimEntity[] = {
		{ TranslationString("Players Only", true), 0, },
		{ TranslationString("Peds Only", true), 1, },
		{ TranslationString("Everyone", true), 2 },
	};

	ScrollStruct<int> AimFilter[] = {
		{ TranslationString("Visible", true), 0, },
		{ TranslationString("Logical", true), 1 },
	};

	ScrollStruct<int> AimType[] = {
		{ TranslationString("Snap", true), 0, },
		{ TranslationString("Silent", true), 1 },
	};
}

void AimbotMenu::Init() {
	SetName("Aimbot");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ToggleOption("Toggle Aimbot")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Toggle).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Aiming Required")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AimingRequired).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Aim Entity")
		.addTranslation()
		.addScroll(m_Vars.m_AimEntity, 0, NUMOF(AimEntity), AimEntity).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Aim Filter")
		.addTranslation()
		.addScroll(m_Vars.m_AimFilter, 0, NUMOF(AimFilter), AimFilter).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Aim Type")
		.addTranslation()
		.addScroll(m_Vars.m_AimType, 0, NUMOF(AimType), AimType).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Aim Tag")
		.addTranslation()
		.addScroll(m_Vars.m_AimTag, 0, NUMOF(Global::Arrays::g_PedBones), Global::Arrays::g_PedBones).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<float>(SCROLL, "Aim Distance")
		.addTranslation()
		.addScroll(m_Vars.m_AimDistance, 0, NUMOF(Global::Arrays::g_Distances), Global::Arrays::g_Distances).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(TOGGLE, "Limit to FOV")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_FOV)
		.addNumber(m_Vars.m_FOVRange, "%.2f", 0.01f).addMin(0.f).addMax(5.f).canLoop().setScrollSpeed(10).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Draw FOV Radius")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_FOVDraw).canBeSaved(m_ParentNameStack)
		.addTooltip("Draws the FOV radius on-screen")
		.addRequirement([] { return m_Vars.m_FOV; }));

	addOption(ScrollOption<int>(TOGGLE, "Auto Fire (With Delay)")
		.addTranslation()
		.addToggle(m_Vars.m_AutoFire)
		.addScroll(m_Vars.m_AutoFireDelay, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers).canBeSaved(m_ParentNameStack));
}

void AimbotMenu::Update() {}

/*Called once on submenu open*/
void AimbotMenu::UpdateOnce() {}

/*Called always*/
void AimbotMenu::FeatureUpdate() {
	if (m_Vars.m_Toggle) {
		if (m_Vars.m_FOV && m_Vars.m_FOVDraw) {
			Menu::GetRenderer()->DrawSprite("Ozark", "circle_rev2", { 0.5f, 0.5f }, { 0.1f * m_Vars.m_FOVRange, 0.18f * m_Vars.m_FOVRange }, 0.f, { 255, 255, 255, 255 });
		}

		if (m_Vars.m_AimingRequired) {
			if (!Menu::GetLocalPlayer().m_IsAiming) {
				return;
			}
		}

		float ClosestDistance = 9999.f;
		Ped ClosestEntity = 0;

		switch (m_Vars.m_AimEntity) {
			case 2:
			case 0: {
				// Players only
				Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
					if (Native::IsEntityDead(Player.m_Ped, 0)) return; // Alive check
					if (Player.m_ID == Menu::GetLocalPlayer().m_ID) return;
					if (m_Vars.m_AimFilter == 0 && !Native::HasEntityClearLosToEntity(Menu::GetLocalPlayer().m_Ped, Player.m_Ped, 1)) return; // Visible check
					if (Native::GetEntityHeightAboveGround(Player.m_Ped) < 0.f) return;

					float Distance = Menu::GetLocalPlayer().m_Coords.GetMathmaticalDistance(Player.m_Coords);
					if (Distance > Global::Arrays::g_Distances[m_Vars.m_AimDistance].m_Result) return;

					if (m_Vars.m_FOV) {
						Math::Vector2<float> ScreenPosition;
						if (Native::GetScreenCoordFromWorldCoord(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, &ScreenPosition.m_X, &ScreenPosition.m_Y)) {
							float LeftSide = 0.5f - ((0.1f * m_Vars.m_FOVRange) / 2.f);
							float RightSide = 0.5f + ((0.1f * m_Vars.m_FOVRange) / 2.f);
							float Top = 0.5f - ((0.18f * m_Vars.m_FOVRange) / 2.f);
							float Bottom = 0.5f + ((0.18f * m_Vars.m_FOVRange) / 2.f);

							if (!((ScreenPosition.m_X >= LeftSide && ScreenPosition.m_X <= RightSide) && (ScreenPosition.m_Y >= Top && ScreenPosition.m_Y <= Bottom))) {
								return;
							}
						} else return;
					}

					if (Distance < ClosestDistance) {
						ClosestDistance = Distance;
						ClosestEntity = Player.m_Ped;
					}
				}, true);

				if (m_Vars.m_AimEntity != 2) break;
			}

			case 1: {
				// Peds only
				Menu::Pools::GetPedPool()->Run([&] (::Ped Ped) {
					if (Native::IsPedAPlayer(Ped)) return;
					if (Native::IsEntityDead(Ped, 0)) return; // Alive check
					if (m_Vars.m_AimFilter == 0 && !Native::HasEntityClearLosToEntity(Menu::GetLocalPlayer().m_Ped, Ped, 1)) return; // Visible check
					if (Native::GetEntityHeightAboveGround(Ped) < 0.f) return;

					auto Coords = Native::GetEntityCoords(Ped, false);
					float Distance = Menu::GetLocalPlayer().m_Coords.GetMathmaticalDistance(Coords);
					if (Distance > Global::Arrays::g_Distances[m_Vars.m_AimDistance].m_Result) return;

					if (m_Vars.m_FOV) {
						Math::Vector2<float> ScreenPosition;
						if (Native::GetScreenCoordFromWorldCoord(Coords.m_X, Coords.m_Y, Coords.m_Z, &ScreenPosition.m_X, &ScreenPosition.m_Y)) {
							float LeftSide = 0.5f - ((0.1f * m_Vars.m_FOVRange) / 2.f);
							float RightSide = 0.5f + ((0.1f * m_Vars.m_FOVRange) / 2.f);
							float Top = 0.5f - ((0.18f * m_Vars.m_FOVRange) / 2.f);
							float Bottom = 0.5f + ((0.18f * m_Vars.m_FOVRange) / 2.f);

							if (!((ScreenPosition.m_X >= LeftSide && ScreenPosition.m_X <= RightSide) && (ScreenPosition.m_Y >= Top && ScreenPosition.m_Y <= Bottom))) {
								return;
							}
						} else return;
					}

					if (Distance < ClosestDistance) {
						ClosestDistance = Distance;
						ClosestEntity = Ped;
					}
				});

				if (m_Vars.m_AimEntity != 2) break;
			}
		}

		if (ClosestEntity) {
			auto BoneCoords = Native::GetWorldPositionOfEntityBone(ClosestEntity, Native::GetPedBoneIndex(ClosestEntity, Global::Arrays::g_PedBones[m_Vars.m_AimTag].m_Result));

			if (m_Vars.m_AimType == 0) {
				auto Camera = Menu::Camera::GetPlayerAngles();
				if (IsValidPtr(Camera)) {
					Math::Vector3_<float> Crosshair = Camera->m_CrosshairLocation;
					Math::Vector3<float> Converted(Crosshair.m_X, Crosshair.m_Y, Crosshair.m_Z);
					float Distance = BoneCoords.GetMathmaticalDistance(Converted);
					Camera->m_ViewAngles = { (BoneCoords.m_X - Crosshair.m_X) / Distance, (BoneCoords.m_Y - Crosshair.m_Y) / Distance, (BoneCoords.m_Z - Crosshair.m_Z) / Distance };
					
					if (m_Vars.m_AutoFire) {
						Menu::Timers::RunTimedFunction(&m_Vars.m_AutoFireDelayTimer, Global::Arrays::g_Timers[m_Vars.m_AutoFireDelay].m_Result, [=] {
							Native::SetPedShootsAtCoord(Menu::GetLocalPlayer().m_Ped, BoneCoords.m_X, BoneCoords.m_Y, BoneCoords.m_Z, true);
						});
					}
				}
			} else {
				if (m_Vars.m_AutoFire) {
					Menu::Timers::RunTimedFunction(&m_Vars.m_AutoFireDelayTimer, Global::Arrays::g_Timers[m_Vars.m_AutoFireDelay].m_Result, [=] {
						Native::SetPedShootsAtCoord(Menu::GetLocalPlayer().m_Ped, BoneCoords.m_X, BoneCoords.m_Y, BoneCoords.m_Z, true);
					});
				}

				if (Menu::GetLocalPlayer().m_IsShooting) {
					Native::SetPedShootsAtCoord(Menu::GetLocalPlayer().m_Ped, BoneCoords.m_X, BoneCoords.m_Y, BoneCoords.m_Z, true);
				}
			}
		}
	}
}

AimbotMenu* _instance;
AimbotMenu* AimbotMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AimbotMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AimbotMenu::~AimbotMenu() { delete _instance; }