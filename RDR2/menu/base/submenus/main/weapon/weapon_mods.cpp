#include "weapon_mods.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../weapon.hpp"
#include "rage/entity/entity.hpp"
#include "menu/base/util/raycast.hpp"
#include "menu/base/util/camera.hpp"
#include "menu/base/util/input.hpp"
#include "explosion.hpp"
#include "aimbot.hpp"
#include "weapon_visuals.hpp"
#include "gravity.hpp"
#include "shooter.hpp"
#include "trigger_bot.hpp"
#include "global/arrays.hpp"
#include "utils/log.hpp"
#include "menu/base/util/timers.hpp"

using namespace weaponModsMenuVars;

namespace weaponModsMenuVars {
	vars_ vars;
	
	void deleteGun() {
		if (native::isPedShooting(menu::getLocalPlayer().ped)) {
			menu::raycast cast = menu::raycastInfront(9999.f);
			if (cast.didHit()) {
				native::setEntityAsMissionEntity(cast.getEntity(), TRUE, TRUE);

				auto entity = cast.getEntity();
				native::setEntityCoords(entity, { 4503.726f, -635.393f, 71.421f });
				native::deleteEntity2(&entity);
			}
		}
	}

	void forceGun() {
		if (native::isPedShooting(menu::getLocalPlayer().ped)) {
			menu::raycast cast = menu::raycastInfront(9999.f);
			if (cast.didHit()) {
				native::setPedToRagdoll(cast.getEntity(), -1, -1, 0, 1, 1, 0);
				native::setEntityVelocity(cast.getEntity(), ((native::getEntityCoords(menu::getLocalPlayer().ped) + (menu::getCamera()->getCameraDirection() * 9999.f)) - native::getEntityCoords(cast.getEntity())) * 4);
			}
		}
	}

	void rapidFire() {
		if (native::isControlPressed(0, INPUT_ATTACK)) {
			native::setPedShootsAtCoord(menu::getLocalPlayer().ped, 0, 0, 0, 0, 0, 0, -1.f);
		}
	}

	void defibrillatorGun() {
		Ped ped;
		if (native::getEntityPlayerIsfreeAimingAt(menu::getLocalPlayer().id, &ped)) {
			if (native::isEntityAPed(ped) && native::isEntityDead(ped)) {
				native::resurrectPed(ped);
				native::reviveInjuredPed(ped);
				native::setEntityHealth(ped, 100, 1);
				native::clearPedTasksImmediately(ped, 1, 0);
			}
		}
	}

	void instantKill() {
		native::setPlayerWeaponDamageMultiplier(menu::getLocalPlayer().id, 100.f);
		native::setPlayerMeleeDamageMultiplier(menu::getLocalPlayer().id, 100.f);
	}

	void explosiveBullets() {
		explosionMenuVars::explosionGun(menu::getLocalPlayer().ped, 1, 1.0f, 1, 0, .0f);
	}

	
	void shrinkGun() {
		Ped ped;
		if (native::isPlayerFreeAiming(menu::getLocalPlayer().id) && native::getEntityPlayerIsfreeAimingAt(menu::getLocalPlayer().id, &ped)) {
			if (native::isEntityAPed(ped)) {
				native::setPedScale(ped, 0.6f);
			}
		}
	}

	void teleportGun() {
		if (native::isPedShooting(menu::getLocalPlayer().ped)) {
			menu::raycast cast = menu::raycastInfront(9999.f);
			if (!cast.getCoords().isZero()) {
				native::setEntityCoords(menu::getLocalPlayer().ped, cast.getCoords());
			}
		}
	}

	void smitePed(Ped ped) {
		auto head = native::getPedBoneCoords(ped, 0x5226, 0, 0, 0);
		native::shootSingleBulletBetweenCoords({ head.x, head.y, head.z + 0.4f }, head, 10000, 1, native::getHashKey("WEAPON_SHOTGUN_SAWEDOFF"), 0, 1, 0, -1.f, 0);
		native::addExplosion(head, EXP_TAG_LIGHTNING_STRIKE, 0.6f, 1, 0, .3f);
	}

	scrollStruct<const char*> throwables[] = {
		"Throwing Knives", "WEAPON_THROWN_THROWING_KNIVES",
		"Tomohawk", "WEAPON_THROWN_TOMAHAWK",
		"Arrows", "WEAPON_BOW",
		"Molotov", "WEAPON_THROWN_MOLOTOV",
		"Bolas", "WEAPON_THROWN_BOLAS",
		//"Dynamite", "WEAPON_THROWN_DYNAMITE"

	};

	void throwableGun(const char* throwable) {
		if (native::isControlPressed(0, INPUT_ATTACK)) {
			static int timer = 0;
			menu::util::getTimers()->runTimedFunction(&timer, 250, [=] {
				menu::raycast cast = menu::raycastInfront(9999.f);
				if (!cast.getCoords().isZero()) {
					auto start = native::getOffsetFromEntityInWorldCoords(native::getCurrentPedWeaponEntityIndex(menu::getLocalPlayer().ped), { 1.0f, .0f, .0f });
					native::shootSingleBulletBetweenCoords(start, cast.getCoords(), 10000, 1, native::getHashKey(throwable), menu::getLocalPlayer().id, 1, 0, -1.f, 1);
				}
			});
		}
	}

	void smiteGun() {
		Ped ped;
		if (native::getEntityPlayerIsfreeAimingAt(menu::getLocalPlayer().id, &ped)) {
			if (native::isEntityAPed(ped) && !native::isEntityDead(ped)) {
				smitePed(ped);
			}
		}
	}

	void hashGun() {
		static int timer = 0;
		menu::util::getTimers()->runTimedFunction(&timer, 250, [] {
			Entity entity;
			if (native::getEntityPlayerIsfreeAimingAt(menu::getLocalPlayer().id, &entity)) {
				Hash model = native::getEntityModel(entity);
				custom(consoleForeground::MAGENTA, "Hash Gun", "Aimed entity hash: %X (%u)", model, model);
			}
		});
	}

	void neverReload() {
		int maxAmmo = 0;
		if (native::getMaxAmmo(menu::getLocalPlayer().ped, &maxAmmo, menu::getLocalPlayer().weaponHash)) {
			native::setPedAmmo(menu::getLocalPlayer().ped, menu::getLocalPlayer().weaponHash, maxAmmo);

			maxAmmo = native::getMaxAmmoInClip(menu::getLocalPlayer().ped, menu::getLocalPlayer().weaponHash, TRUE);
			if (maxAmmo > 0) {
				native::setAmmoInClip(menu::getLocalPlayer().ped, menu::getLocalPlayer().weaponHash, maxAmmo);
			}
		}
	}

	void lightningGun() {
		if (native::isPedShooting(menu::getLocalPlayer().ped)) {
			menu::raycast cast = menu::raycastInfront(9999.f);
			if (!cast.getCoords().isZero()) {
				native::startLightningStrikeAtCoords(cast.getCoords());
			}
		}
	}

	void cleanCurrentWeapon(bool sound = false) {
		Entity weapon = native::getCurrentPedWeaponEntityIndex(menu::getLocalPlayer().ped);
		if (weapon) {
			_i<int>(0xE22060121602493B, weapon, .0f, 1);
			_i<int>(0x812CE61DEBCAB948, weapon, .0f, 1);
			_i<int>(0xA9EF4AD10BDDDB57, weapon, .0f, 1);
			_i<int>(0xA7A57E89E965D839, weapon, 0);
			if (sound) {
				native::playSoundFrontend("GUN_OIL", "PICKUP_SOUNDSET", 0, 0);
			}
		}
	}


	void setInfiniteAmmo(BOOL toggle) {
		if (!global::vars::g_weaponHashes.size()) {
			global::vars::g_weaponHashes = rage::engine::getHashListFromHashTable((*(WORD*)global::vars::g_weaponHashListCount) - 1, *(uint64_t*)global::vars::g_weaponHashList);
		}
		for (std::size_t i = 0; i < global::vars::g_weaponHashes.size(); i++) {
			if (global::vars::g_weaponHashes[i]) {
				native::setPedInfiniteAmmo(menu::getLocalPlayer().ped, toggle, global::vars::g_weaponHashes[i]);
			}
		}
	}
}


void weaponModsMenu::init() {
	setName("Weapon Mods");
	setParentSubmenu<weaponMenu>();

	addOption(submenuOption("Aimbot")
		.addTranslation()
		.addSubmenu<aimbotMenu>()
		.addTooltip("Snap-aim/Silent-aim"));

	addOption(submenuOption("Triggerbot")
		.addTranslation()
		.addSubmenu<weaponTriggerBotMenu>());

	addOption(submenuOption("Explosion Gun")
		.addTranslation()
		.addSubmenu<explosionMenu>());

	addOption(submenuOption("Visuals")
		.addTranslation()
		.addSubmenu<visualMenu>());

	addOption(submenuOption("Gravity Gun")
		.addTranslation()
		.addSubmenu<gravityMenu>());

	addOption(submenuOption("Entity Shooter")
		.addTranslation()
		.addSubmenu<shooterMenu>());

	addOption(buttonOption("Clean Weapon")
		.addTranslation()
		.addOnClick([] { cleanCurrentWeapon(true); })
		.addTooltip("Oils your current weapon"));

	addOption(toggleOption("Infinite Clip")
		.addToggle(vars.infiniteAmmo).canBeSaved(name.getOriginal())
		.addOnClick([] { if (menu::getLocalPlayer().weaponHash) native::setPedInfiniteAmmoClip(menu::getLocalPlayer().ped, FALSE); })
		.addTooltip("neva reload hehe"));

	addOption(toggleOption("Infinite Ammo")
		.addTranslation()
		.addToggle(vars.infiniteAmmo2).canBeSaved(name.getOriginal())
		.addOnClick([] { if (!vars.infiniteAmmo2)setInfiniteAmmo(FALSE); })
		.addTooltip("reload but neva run out"));

	addOption(toggleOption("Never Reload")
		.addTranslation()
		.addToggle(vars.neverReload)
		.addTooltip("Bullets will auto load into your weapon"));

	addOption(toggleOption("Anti Degrade")
		.addTranslation()
		.addToggle(vars.neverDegrade).canBeSaved(name.getOriginal())
		.addTooltip("Currently selected weapon will never need to be oiled"));

	addOption(toggleOption("Delete Gun")
		.addTranslation()
		.addToggle(vars.deleteGun).canBeSaved(name.getOriginal())
		.addTooltip("Shoot and delete"));

	addOption(toggleOption("Force Gun")
		.addTranslation()
		.addToggle(vars.forceGun).canBeSaved(name.getOriginal())
		.addTooltip("Shoot and bye-bye"));

	addOption(toggleOption("Rapid Fire")
		.addTranslation()
		.addToggle(vars.rapidFire).canBeSaved(name.getOriginal())
		.addTooltip("Full auto fun hehe"));

	addOption(toggleOption("Smite Gun")
		.addTranslation()
		.addToggle(vars.smiteGun).canBeSaved(name.getOriginal())
		.addTooltip("Take the top off"));

	addOption(toggleOption("Zeus Gun")
		.addTranslation()
		.addToggle(vars.lightningStrikeGun).canBeSaved(name.getOriginal())
		.addTooltip("Lightning bullets"));

	addOption(scrollOption<const char*>(TOGGLE, "Throwable Gun")
		.addTranslation()
		.addScroll(vars.currentThrowable, 0, NUMOF(throwables), throwables)
		.addToggle(vars.throwableGun)
		.addTooltip("Turn your bullets into throwables"));

	addOption(toggleOption("Defibrillator Gun")
		.addTranslation()
		.addToggle(vars.defibrillatorGun).canBeSaved(name.getOriginal())
		.addTooltip("Make em rise from the dead ;)"));

	addOption(toggleOption("Insta-Kill")
		.addTranslation()
		.addToggle(vars.instantKill).canBeSaved(name.getOriginal())
		.addTooltip("Never waste more than a bullet"));

	addOption(toggleOption("Shrink Gun")
		.addTranslation()
		.addToggle(vars.shrinkGun).canBeSaved(name.getOriginal())
		.addTooltip("They'll still be 4 foot kings"));

	addOption(toggleOption("Explosive Ammo")
		.addTranslation()
		.addToggle(vars.explosiveAmmo).canBeSaved(name.getOriginal())
		.addTooltip("Bullets go boom"));

	addOption(toggleOption("Teleport Gun")
		.addTranslation()
		.addToggle(vars.teleportGun).canBeSaved(name.getOriginal())
		.addTooltip("Teleporty to last bullet impact location"));

	addOption(toggleOption("Aimed Entity Hash Gun")
		.addTranslation()
		.addToggle(vars.hashGun).canBeSaved(name.getOriginal())
		.addTooltip("Entity debug when shot at (prints in the log)"));

	addOption(breakOption("Modifiers")
		.addTranslation());

	addOption(numberOption<float>(TOGGLE, "Weapon Damage Modifier")
		.addTranslation()
		.addNumber(vars.weaponDamageVal, "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addToggle(vars.weaponDamage).canBeSaved(name.getOriginal())
		.addOnClick([] { if(menu::getLocalPlayer().weaponHash) native::setPlayerWeaponDamageMultiplier(menu::getLocalPlayer().id, 0.1f); })
		.addTooltip("Set weapons damage modifier"));

	addOption(numberOption<float>(TOGGLE, "Melee Damage Modifier")
		.addTranslation()
		.addNumber(vars.meleeDamageVal, "%.0f", 1.f).addMin(1.f).addMax(100.f)
		.addToggle(vars.meleeDamage).canBeSaved(name.getOriginal())
		.addOnClick([] { if (menu::getLocalPlayer().weaponHash) native::setPlayerMeleeDamageMultiplier(menu::getLocalPlayer().id, 0.1f); })
		.addTooltip("Set melee damage modifier"));
}

/*Called while in submenu*/
void weaponModsMenu::update() {}

/*Called once on submenu open*/
void weaponModsMenu::updateOnce() {}

/*Called always*/
void weaponModsMenu::featureUpdate() {
	if (menu::getLocalPlayer().weaponHash) {

		if (vars.throwableGun) {
			throwableGun(throwables[vars.currentThrowable].result);
		}

		if (vars.weaponDamage) {
			native::setPlayerWeaponDamageMultiplier(menu::getLocalPlayer().id, vars.weaponDamageVal);
		}

		if (vars.meleeDamage) {
			native::setPlayerMeleeDamageMultiplier(menu::getLocalPlayer().id, vars.meleeDamageVal);
		}

		if (vars.hashGun) {
			hashGun();
		}

		if (vars.deleteGun) {
			deleteGun();
		}

		if (vars.forceGun) {
			forceGun();
		}

		if (vars.rapidFire) {
			rapidFire();
		}

		if (vars.defibrillatorGun) {
			defibrillatorGun();
		}

		if (vars.shrinkGun) {
			shrinkGun();
		}

		if (vars.explosiveAmmo) {
			explosiveBullets();
		}

		if (vars.instantKill) {
			instantKill();
		}

		if (vars.teleportGun) {
			teleportGun();
		}

		if (vars.smiteGun) {
			smiteGun();
		}

		if (vars.infiniteAmmo) {
			native::setPedInfiniteAmmoClip(menu::getLocalPlayer().ped, TRUE);
		}

		if (vars.infiniteAmmo2) {
			//setInfiniteAmmo(FALSE);
			static int32_t infiniteAmmoTimer = 0;
			menu::util::getTimers()->runTimedFunction(&infiniteAmmoTimer, 1000, [] {
				setInfiniteAmmo(TRUE);
			});
			//native::setPedInfiniteAmmo(menu::getLocalPlayer().ped, TRUE, menu::getLocalPlayer().weaponHash);
		}

		if (vars.neverReload) {
			neverReload();
		}

		if (vars.lightningStrikeGun) {
			lightningGun();
		}

		if (vars.neverDegrade) {
			native::setWeaponDegrationMultiplier(menu::getLocalPlayer().id, .0f);
			static int32_t cleanTimer = 0;
			menu::util::getTimers()->runTimedFunction(&cleanTimer, 5000, [] {
				cleanCurrentWeapon();
			});
		}
	}
}

weaponModsMenu* _instance;
weaponModsMenu* weaponModsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new weaponModsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

weaponModsMenu::~weaponModsMenu() { delete _instance; }