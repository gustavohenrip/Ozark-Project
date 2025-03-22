#include "weapon_mods.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../weapon.hpp"
#include "aimbot.hpp"
#include "triggerbot.hpp"
#include "modifiers.hpp"
#include "explosions/explosion_editor.hpp"
#include "explosions/explosion_gun.hpp"
#include "vehicle_gun.hpp"
#include "particle_gun.hpp"
#include "gravity/gravity_gun.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/raycast.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/submenus/main/player/movement.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "utils/log.hpp"
#include "menu/base/util/stats.hpp"
#include "weapon_disables.hpp"

using namespace WeaponModsMenuVars;

namespace WeaponModsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> TargetingMode[] = {
		{ TranslationString("Traditional", true), 0, },
		{ TranslationString("Free Aim", true), 1, },
		{ TranslationString("Assisted Aim", true), 2, },
		{ TranslationString("Assisted Aim - Full", true), 3 },
		{ TranslationString("Assisted Aim - Partial", true), 4 }
	};
}

void WeaponModsMenu::Init() {
	SetName("Weapon Mods");
	SetParentSubmenu<WeaponMenu>();

	addOption(SubmenuOption("Aimbot")
		.addTranslation().addHotkey()
		.addSubmenu<AimbotMenu>());

	addOption(SubmenuOption("Triggerbot")
		.addTranslation().addHotkey()
		.addSubmenu<TriggerbotMenu>());

	addOption(SubmenuOption("Modifiers")
		.addTranslation().addHotkey()
		.addSubmenu<WeaponModifierMenu>());

	addOption(SubmenuOption("Explosion Editor")
		.addTranslation().addHotkey()
		.addSubmenu<ExplosionEditorMenu>());

	addOption(SubmenuOption("Explosion Gun")
		.addTranslation().addHotkey()
		.addSubmenu<ExplosionGunMenu>());

	addOption(SubmenuOption("Vehicle Gun")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleGunMenu>());

	addOption(SubmenuOption("Particle Gun")
		.addTranslation().addHotkey()
		.addSubmenu<ParticleGunMenu>());

	addOption(SubmenuOption("Gravity Gun")
		.addTranslation().addHotkey()
		.addSubmenu<GravityGunMenu>());

	addOption(SubmenuOption("Disables")
		.addTranslation().addHotkey()
		.addSubmenu<WeaponDisablesMenu>());

	addOption(ToggleOption("Infinite Ammo")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_InfiniteAmmo)
		.addOnClick([] { if (!m_Vars.m_InfiniteAmmo) Native::SetPedInfiniteAmmoClip(Menu::GetLocalPlayer().m_Ped, false); }).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Instant Kill")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_InstantKill).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Rapid Fire")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RapidFire).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Laser Sight")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_LaserSight).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Explosive Bullets")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ExplosiveAmmo).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Incendiary Bullets")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_IncendiaryAmmo).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Delete Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DeleteGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Force Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ForceGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Teleport Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_TeleportGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Airstrike Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_AirstrikeGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Defibrillator Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DefibrillatorGun)
		.addOnClick([] { if (m_Vars.m_DefibrillatorGun) Native::GiveWeaponToPed(Menu::GetLocalPlayer().m_Ped, 0x3656c8c1, -1, true, true); }).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Money Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_MoneyGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Clown Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ClownGun)
		.addOnClick([] {
			if (m_Vars.m_ClownGun) {
				Native::RequestNamedPtfxAsset("scr_rcbarry2");
				Native::_UseParticleFxAssetNextCall("scr_rcbarry2");
				Native::StartParticleFxLoopedAtCoord("scr_clown_death", Menu::GetLocalPlayer().m_Coords.m_X, Menu::GetLocalPlayer().m_Coords.m_Y, Menu::GetLocalPlayer().m_Coords.m_Z, 0, 0, 0, 0, true, true, true, true);
			} else {
				Native::EnableClownBloodVfx(false);
			}
		}).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Shrink Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ShrinkGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Dead Eye Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DeadEyeGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Rope Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RopeGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Vehicle Stealer Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_VehicleStealerGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Valkyrie Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ValkyrieGun).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Hash Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_HashGun).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Targeting Mode")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_TargetingMode, 0, NUMOF(TargetingMode), TargetingMode)
		.addOnClick([] { Native::SetPlayerTargetingMode(m_Vars.m_TargetingMode); }).canBeSaved(m_ParentNameStack));
}

void WeaponModsMenu::Update() {}

/*Called once on submenu open*/
void WeaponModsMenu::UpdateOnce() {
	m_Vars.m_TargetingMode = Native::_0xDFFA5BE8381C3314();
}

/*Called always*/
void WeaponModsMenu::FeatureUpdate() {
	if (m_Vars.m_HashGun) {
		static int Timer = 0;
		Menu::Timers::RunTimedFunction(&Timer, 250, [] {
			if (Menu::GetLocalPlayer().m_IsAiming) {
				auto Cast = Menu::RaycastInfront(999.f);
				if (Cast.HitSomething() && Cast.HitEntity()) {
					uint32_t Model = Native::GetEntityModel(Cast.GetHitEntity());
					LOG_CUSTOM(ConsoleForeground::MAGENTA, "Hash Gun", "Hash: %X (%u)", Model, Model);
				}
			}
		});
	}

	if (m_Vars.m_InfiniteAmmo) {
		Native::SetPedInfiniteAmmoClip(Menu::GetLocalPlayer().m_Ped, true);
	}

	if (m_Vars.m_InstantKill) {
		Native::SetPlayerMeleeWeaponDamageModifier(Menu::GetLocalPlayer().m_ID, 99999.f, 1);
		Native::SetPlayerWeaponDamageModifier(Menu::GetLocalPlayer().m_ID, 99999.f);
	}

	if (m_Vars.m_RapidFire && !MovementMenuVars::m_Vars.m_NoClipBind) {
		if (!Menu::GetLocalPlayer().m_InVehicle) {
			if (Native::IsControlPressed(0, ControlAttack) && Menu::GetLocalPlayer().m_HasWeaponInHand) {
				Math::Vector3<float> StartPoint = Native::GetWorldPositionOfEntityBone(Menu::GetLocalPlayer().m_Ped, Native::GetPedBoneIndex(Menu::GetLocalPlayer().m_Ped, 0x6F06));
				Math::Vector3<float> UpdatedCoords = { 0.0f, 200.0f, (Native::Tan(Native::GetGameplayCamRelativePitch()) * 200.0f) };
				Math::Vector3<float> EndPoint = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Ped, UpdatedCoords.m_X, UpdatedCoords.m_Y, UpdatedCoords.m_Z);
				
				auto Start = Native::GetGameplayCamCoord() + (Menu::Camera::GetCameraDirection());
				auto End = Native::GetGameplayCamCoord() + (Menu::Camera::GetCameraDirection() * 200.f);
				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(Start.m_X, Start.m_Y, Start.m_Z, End.m_X, End.m_Y, End.m_Z, Native::GetWeaponDamageType(Menu::GetLocalPlayer().m_WeaponHash), true, Menu::GetLocalPlayer().m_WeaponHash, Menu::GetLocalPlayer().m_Ped, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);

				// Native::SetPedShootsAtCoord(Menu::GetLocalPlayer().m_Ped, EndPoint.m_X, EndPoint.m_Y, EndPoint.m_Z, true);
			}
		}
	}

	if (m_Vars.m_LaserSight) {
		if (!Menu::GetLocalPlayer().m_InVehicle) {
			if (Menu::GetLocalPlayer().m_IsAiming) {
				Math::Vector3<float> StartPoint = Native::GetWorldPositionOfEntityBone(Menu::GetLocalPlayer().m_Ped, Native::GetPedBoneIndex(Menu::GetLocalPlayer().m_Ped, 0x6F06));
				Math::Vector3<float> UpdatedCoords = { 0.0f, 200.0f, (Native::Tan(Native::GetGameplayCamRelativePitch()) * 200.0f) };
				Math::Vector3<float> EndPoint = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Ped, UpdatedCoords.m_X, UpdatedCoords.m_Y, UpdatedCoords.m_Z);
				Native::DrawLine(StartPoint.m_X, StartPoint.m_Y, StartPoint.m_Z, EndPoint.m_X, EndPoint.m_Y, EndPoint.m_Z, 255, 0, 0, 255);
			}
		}
	}

	if (m_Vars.m_ExplosiveAmmo) {
		auto NetGamePlayer = Menu::GetLocalPlayer().m_NetGamePlayer;
		if (NetGamePlayer) {
			if (NetGamePlayer->m_PlayerInfo) {
				NetGamePlayer->m_PlayerInfo->m_FrameFlags |= 0x800;
			}
		}
	}

	if (m_Vars.m_IncendiaryAmmo) {
		auto NetGamePlayer = Menu::GetLocalPlayer().m_NetGamePlayer;
		if (NetGamePlayer) {
			if (NetGamePlayer->m_PlayerInfo) {
				NetGamePlayer->m_PlayerInfo->m_FrameFlags |= 0x1000;
			}
		}
	}

	if (m_Vars.m_DeleteGun) {
		Entity Entity;
		if (Menu::GetLocalPlayer().m_IsShooting) {
			if (Native::GetEntityPlayerIsFreeAimingAt(Menu::GetLocalPlayer().m_ID, &Entity)) {
				if (Native::IsEntityAPed(Entity)) {
					Entity = Native::IsPedInAnyVehicle(Entity, true) ? Native::GetVehiclePedIsIn(Entity, false) : Entity;
				}
				
				if (Native::DoesEntityExist(Entity)) {
					Menu::GetControlManager()->RequestControl(Entity, [] (::Entity Ent) {
						Native::SetEntityAsMissionEntity(Ent, true, true);
						Native::DeleteEntity(&Ent);
					});
				}
			}
		}
	}

	if (m_Vars.m_ForceGun) {
		if (Menu::GetLocalPlayer().m_IsShooting) {
			Menu::Raycast Cast = Menu::RaycastInfront(99999.0f);
			if (Cast.HitEntity()) {
				Menu::GetControlManager()->RequestControl(Cast.GetHitEntity(), [] (Entity Ent) {
					auto Velocity = ((Menu::GetLocalPlayer().m_Coords + (Menu::Camera::GetCameraDirection() * 9999.f)) - Native::GetEntityCoords(Ent, false)) * 4.f;
					Native::SetEntityVelocity(Ent, Velocity.m_X, Velocity.m_Y, Velocity.m_Z);
				}, false);
			}
		}
	}

	if (m_Vars.m_TeleportGun) {
		if (Menu::GetLocalPlayer().m_IsShooting) {
			Menu::Raycast Cast = Menu::RaycastInfront(99999.0f);
			if (!Cast.GetHitCoords().IsZero() && Cast.GetHitCoords().GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords) > 3.5f) {
				Native::SetEntityCoords(Menu::GetLocalPlayer().m_Entity, Cast.GetHitCoords().m_X, Cast.GetHitCoords().m_Y, Cast.GetHitCoords().m_Z, true, true, true, false);
			}
		}
	}

	if (m_Vars.m_AirstrikeGun) {
		if (Menu::GetLocalPlayer().m_IsShooting) {
			Menu::Raycast Cast = Menu::RaycastInfront(99999.0f);
			if (!Cast.GetHitCoords().IsZero() && Cast.GetHitCoords().GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords) > 3.5f) {
				Menu::GetControlManager()->SimpleRequestWeaponAsset(0x47757124);
				Menu::GetControlManager()->SimpleRequestWeaponAsset(0xF8A3939F);

				auto HitCoords = Cast.GetHitCoords();
				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(HitCoords.m_X, HitCoords.m_Y, HitCoords.m_Z, HitCoords.m_X, HitCoords.m_Y, HitCoords.m_Z, Native::GetWeaponDamageType(0x47757124), true, 0x47757124, Menu::GetLocalPlayer().m_Ped, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
				Native::ShootSingleBulletBetweenCoordsIgnoreEntity(HitCoords.m_X, HitCoords.m_Y, HitCoords.m_Z + 60.0f, HitCoords.m_X, HitCoords.m_Y, HitCoords.m_Z, Native::GetWeaponDamageType(0xF8A3939F), true, 0xF8A3939F, Menu::GetLocalPlayer().m_Ped, true, false, 500.0f, Menu::GetLocalPlayer().m_Entity, 0);
			}
		}
	}

	if (m_Vars.m_DefibrillatorGun) {
		if (Menu::GetLocalPlayer().m_WeaponHash == 0x3656c8c1) {
			Entity Aimed;
			if (Native::GetEntityPlayerIsFreeAimingAt(Menu::GetLocalPlayer().m_ID, &Aimed)) {
				if (Native::IsEntityAPed(Aimed) && Native::IsEntityDead(Aimed, 0)) {
					Native::ResurrectPed(Aimed);
					Native::ReviveInjuredPed(Aimed);
					Native::SetEntityHealth(Aimed, 100);
					Native::ClearPedTasksImmediately(Aimed);
				}
			}
		}
	}

	if (m_Vars.m_MoneyGun) {
		if (Menu::GetLocalPlayer().m_IsShooting) {
			Menu::Raycast Cast = Menu::RaycastInfront(99999.0f);
			if (!Cast.GetHitCoords().IsZero()) {
				auto HitCoords = Cast.GetHitCoords();

				Menu::GetControlManager()->SimpleRequestModel(0x113FD533);
				Native::CreateAmbientPickup(0x1E9A99F8, HitCoords.m_X, HitCoords.m_Y, HitCoords.m_Z, 1, 2500, 0x113FD533, 1, 1);
			}
		}
	}

	if (m_Vars.m_ClownGun) {
		Native::EnableClownBloodVfx(true);
	}

	if (m_Vars.m_ShrinkGun) {
		Entity Aimed;
		if (Native::GetEntityPlayerIsFreeAimingAt(Menu::GetLocalPlayer().m_ID, &Aimed)) {
			if (Native::IsEntityAPed(Aimed)) {
				Native::SetPedConfigFlag(Aimed, 223, true);
			}
		}
	}

	if (m_Vars.m_DeadEyeGun) {
		if (Menu::GetLocalPlayer().m_IsAiming) {
			if (Menu::GetLocalPlayer().m_IsShooting) {
				Native::SetTimeScale(0.2f);
				Native::SetTimecycleModifier("DEATH");
			} else {
				Native::ClearTimecycleModifier();
				Native::SetTimeScale(0.35f);
			}

			Native::_StartScreenEffect("ExplosionJosh3", -1, true);
		} else {
			Native::SetTimeScale(1.0f);
			Native::ClearTimecycleModifier();
			Native::_StopScreenEffect("ExplosionJosh3");
		}
	}

	if (m_Vars.m_RopeGun) {
		Entity Ent;
		auto MyCoords = Menu::GetLocalPlayer().m_Coords;

		if (Menu::GetLocalPlayer().m_IsShooting) {
			if (!Native::DoesEntityExist(m_Vars.m_RopeGunEntities[0]) && !Native::DoesEntityExist(m_Vars.m_RopeGunEntities[1])) {
				if (Native::GetEntityPlayerIsFreeAimingAt(Menu::GetLocalPlayer().m_ID, &Ent)) {
					m_Vars.m_RopeGunEntities[0] = Native::IsPedInAnyVehicle(Ent, true) ? Ent = Native::GetVehiclePedIsIn(Ent, false) : Ent;

					auto EntityCoords = Native::GetEntityCoords(m_Vars.m_RopeGunEntities[0], true);
					float RopeLength = Native::GetDistanceBetweenCoords(MyCoords.m_X, MyCoords.m_Y, MyCoords.m_Z, EntityCoords.m_X, EntityCoords.m_Y, EntityCoords.m_Z, true);
					m_Vars.m_RopeGunObject = Native::AddRope(MyCoords.m_X, MyCoords.m_Y, MyCoords.m_Z, 0.f, 0.f, 0.f, RopeLength, 4, RopeLength, 0.5f, 0.5f, false, false, true, 1.f, false, 0);
					Native::RopeLoadTextures();
					Native::ActivatePhysics(m_Vars.m_RopeGunObject);
				}
			} else {
				if (Native::DoesEntityExist(m_Vars.m_RopeGunEntities[0]) && !Native::DoesEntityExist(m_Vars.m_RopeGunEntities[1])) {
					if (Native::GetEntityPlayerIsFreeAimingAt(Menu::GetLocalPlayer().m_ID, &Ent)) {
						m_Vars.m_RopeGunEntities[1] = Native::IsPedInAnyVehicle(Ent, true) ? Ent = Native::GetVehiclePedIsIn(Ent, false) : Ent;

						auto EntityCoords = Native::GetEntityCoords(m_Vars.m_RopeGunEntities[0], true);
						Menu::Raycast Cast = Menu::RaycastInfront(100.f);
						if (!Cast.GetHitCoords().IsZero()) {
							Native::AttachEntitiesToRope(m_Vars.m_RopeGunObject, m_Vars.m_RopeGunEntities[0], m_Vars.m_RopeGunEntities[1], EntityCoords.m_X, EntityCoords.m_Y, EntityCoords.m_Z, Cast.GetHitCoords().m_X, Cast.GetHitCoords().m_Y, Cast.GetHitCoords().m_Z, Native::GetDistanceBetweenCoords(EntityCoords.m_X, EntityCoords.m_Y, EntityCoords.m_Z, Cast.GetHitCoords().m_X, Cast.GetHitCoords().m_Y, Cast.GetHitCoords().m_Z, true), 0, 0, 0, 0);
							Native::PinRopeVertex(m_Vars.m_RopeGunObject, Native::GetRopeVertexCount(m_Vars.m_RopeGunObject) - 1, Cast.GetHitCoords().m_X, Cast.GetHitCoords().m_Y, Cast.GetHitCoords().m_Z);
							m_Vars.m_RopeGunEntities[0] = 0;
							m_Vars.m_RopeGunEntities[1] = 0;
						}
					}
				}
			}
		}

		if (Native::DoesEntityExist(m_Vars.m_RopeGunEntities[0]) && !Native::DoesEntityExist(m_Vars.m_RopeGunEntities[1])) {
			auto RightHand = Native::GetPedBoneCoords(Menu::GetLocalPlayer().m_Ped, 6286, 0, 0, 0);
			auto EntityCoords = Native::GetEntityCoords(m_Vars.m_RopeGunEntities[0], true);
			Native::PinRopeVertex(m_Vars.m_RopeGunObject, 0, RightHand.m_X, RightHand.m_Y, RightHand.m_Z);
			Native::PinRopeVertex(m_Vars.m_RopeGunObject, Native::GetRopeVertexCount(m_Vars.m_RopeGunObject) - 1, EntityCoords.m_X, EntityCoords.m_Y, EntityCoords.m_Z);
		}
	}

	if (m_Vars.m_VehicleStealerGun) {
		if (Menu::GetLocalPlayer().m_InVehicle) {
			if (Native::IsThisModelAHeli(Menu::GetLocalPlayer().m_Vehicle)) {
				if (Native::GetPedInVehicleSeat(Menu::GetLocalPlayer().m_Vehicle, -1, 0) == Menu::GetLocalPlayer().m_Ped) {
					return;
				}
			}
		}

		Entity Ent;
		if (Native::GetEntityPlayerIsFreeAimingAt(Menu::GetLocalPlayer().m_ID, &Ent)) {
			if (Native::IsEntityAVehicle(Ent) || (Native::IsEntityAPed(Ent) && Native::IsPedInAnyVehicle(Ent, true))) {
				if (Menu::GetLocalPlayer().m_IsShooting) {
					bool WasPlayer = false;

					if (Native::IsEntityAPed(Ent)) {
						Ped P = Native::GetPedInVehicleSeat(Native::GetVehiclePedIsIn(Ent, false), -1, 1);
						if (Native::DoesEntityExist(P)) {
							Ent = Native::GetVehiclePedIsIn(Ent, false);

							if (Native::IsPedAPlayer(P)) {
								// Do the nasty
								WasPlayer = true;
								Menu::GetControlManager()->RequestControl(Ent, [] (Entity) {}, true);
							}
						}
					}

					if (!WasPlayer) {
						Menu::GetControlManager()->SimpleRequestControl(Ent);
						Native::ClearPedTasksImmediately(Native::GetPedInVehicleSeat(Ent, -1, 1));
						Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, Ent, -1);
						Native::SetVehicleEngineOn(Ent, true, true, false);
					} else {
						Utils::GetFiberPool()->Push([=] {
							Utils::WaitFiber(500);
							Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, Ent, -1);
							Native::SetVehicleEngineOn(Ent, true, true, false);
						});
					}
				}
			}
		}
	}

	if (m_Vars.m_ValkyrieGun) {
		uint32_t RocketHash = joaat("prop_ld_bomb_01_open");
		if (!Native::HasModelLoaded(RocketHash)) { // temp
			Native::RequestModel(RocketHash);
		}

		if (Menu::GetLocalPlayer().m_IsShooting && !m_Vars.m_ValkryieRunning) {
			Math::Vector3<float> SpawnPosition = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Ped, 0.f, 2.f, 0.f);
			m_Vars.m_ValkyrieObject = Native::CreateObject(RocketHash, SpawnPosition.m_X, SpawnPosition.m_Y, SpawnPosition.m_Z, true, true, false);
			if (Native::DoesEntityExist(m_Vars.m_ValkyrieObject)) {
				Math::Vector3<float> Rotation = Menu::GetLocalPlayer().m_Rotation;
				Native::SetEntityRotation(m_Vars.m_ValkyrieObject, Rotation.m_X, Rotation.m_Y, Rotation.m_Z, 2, true);

				m_Vars.m_ValkyrieCam = Native::CreateCamWithParams("DEFAULT_SCRIPTED_CAMERA", SpawnPosition.m_X, SpawnPosition.m_Y, SpawnPosition.m_Z, Rotation.m_X, Rotation.m_Y, Rotation.m_Z, 60.f, true, 2);
				Native::AttachCamToEntity(m_Vars.m_ValkyrieCam, m_Vars.m_ValkyrieObject, 0.f, 1.f, 0.f, true);
				Native::SetCamActive(m_Vars.m_ValkyrieCam, true);
				Native::RenderScriptCams(true, false, 1000, true, false, 0);

				m_Vars.m_ValkyrieTimer = Native::GetGameTimer() + 10000;
				m_Vars.m_ValkryieRunning = true;
			}
		}

		if (m_Vars.m_ValkryieRunning) {
			float Height = (m_Vars.m_ValkyrieTimer - Native::GetGameTimer()) / 20000.0f;
			Color Col;

			if (Height > 0.4f) {
				Col = Color::Green();
			} else if (Height <= 0.4f && Height > 0.3f) {
				Col = Color(255, 255, 0);
			} else if (Height <= 0.3f && Height > 0.2f) {
				Col = Color(255, 119, 0);
			} else if (Height <= 0.2f) {
				Col = Color::Red();
			}

			Menu::GetRenderer()->DrawRect({ 0.18f, 0.3f }, { 0.01f, 0.5f }, Color(255, 255, 255, 150));
			Menu::GetRenderer()->DrawRect({ 0.18f, 0.3f + (0.5f - Height) }, { 0.01f, Height }, Col);

			Math::Vector3<float> GameplayRotation = Native::GetGameplayCamRot(2);
			Native::SetCamRot(m_Vars.m_ValkyrieCam, GameplayRotation.m_X, GameplayRotation.m_Y, GameplayRotation.m_Z, 2);

			Math::Vector3<float> Coords = Native::GetOffsetFromEntityInWorldCoords(m_Vars.m_ValkyrieObject, 0.f, 0.4f, 0.f);
			Native::SetEntityCoords(m_Vars.m_ValkyrieObject, Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);

			Native::SetEntityRotation(m_Vars.m_ValkyrieObject, GameplayRotation.m_X, GameplayRotation.m_Y, GameplayRotation.m_Z, 2, true);

			Menu::Raycast Cast = Menu::RaycastInfront(25.f, Native::GetCamCoord(m_Vars.m_ValkyrieCam));
			if (Native::GetGameTimer() >= m_Vars.m_ValkyrieTimer || (!Cast.GetHitCoords().IsZero() && Cast.GetHitCoords().GetMathmaticalDistance(Native::GetCamCoord(m_Vars.m_ValkyrieCam)) < 3.f)) {
				m_Vars.m_ValkryieRunning = false;

				Math::Vector3<float> CamCoords = Native::GetCamCoord(m_Vars.m_ValkyrieCam);
				Native::AddOwnedExplosion(Menu::GetLocalPlayer().m_Ped, CamCoords.m_X, CamCoords.m_Y, CamCoords.m_Z, 27, 2.f, true, false, 0.5f);

				Native::SetEntityAsMissionEntity(m_Vars.m_ValkyrieObject, false, true);
				Native::DeleteEntity(&m_Vars.m_ValkyrieObject);
				Native::RenderScriptCams(false, false, 1000, true, false, 0);
				Native::DestroyCam(m_Vars.m_ValkyrieCam, true);
			}
		}
	}
}

WeaponModsMenu* _instance;
WeaponModsMenu* WeaponModsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WeaponModsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WeaponModsMenu::~WeaponModsMenu() { delete _instance; }