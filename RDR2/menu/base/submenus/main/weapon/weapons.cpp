#include "weapons.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../weapon.hpp"
#include "weapon_list.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "rage/session/session.hpp"
#include "menu/base/util/presence.hpp"
#include "menu/base/util/notify.hpp"

using namespace weaponsMenuVars;

namespace weaponsMenuVars {
	vars_ vars;
	
	void givePedWeapon(Ped ped, Hash weapon, int ammo) {
		native::giveDelayedWeaponToPed(ped, weapon, ammo, 1, 0x2CD419DC);
		native::setCurrentPedWeapon(ped, weapon, 1, 0, 0, 0);
	}

	void addAllWeapons() {
		if (native::networkIsInSession()) {
			if (!menu::getPresence()->refreshSession([] {
				for (std::size_t i = 0; i < global::vars::g_weaponHashes.size(); i++) {
					if (global::vars::g_weaponHashes[i]) {
						givePedWeapon(menu::getLocalPlayer().ped, global::vars::g_weaponHashes[i], 9999);
					}
				}
				native::setCurrentPedWeapon(menu::getLocalPlayer().ped, 0xdb21ac8c, 1, 0, 0, 0);
				})) { menu::getNotify()->right("~e~Hold Your horses"); }
		}
	/*	if (native::networkIsInSession()) {
			static bool hasFinished = true;
			if (hasFinished) {
				auto host = menu::getPlayer(rage::network::getNetworkTask()->getSessionHost());
				uint64_t joinbackRid = 0;
				if (host.id != menu::getLocalPlayer().id) {
					joinbackRid = rage::network::getNetworkTask()->getNetPlayerInfo(rage::network::getNetworkTask()->getSessionHost())->rockstarId2;
				} else {
					joinbackRid = rage::network::getNetworkTask()->getNetPlayerInfo(1)->rockstarId2;
				}
				menu::util::getFiberPool()->run([=] {
					hasFinished = false;
					static char sessionID[0x30];
					memset(sessionID, 0, sizeof(sessionID));
					native::createPrivateSession(17, 1, 1, sessionID);
					menu::util::waitFiber(2000);
					for (std::size_t i = 0; i < global::vars::g_weaponHashes.size(); i++) {
						if (global::vars::g_weaponHashes[i]) {
							givePedWeapon(menu::getLocalPlayer().ped, global::vars::g_weaponHashes[i], 9999);
						}
					}
					native::setCurrentPedWeapon(menu::getLocalPlayer().ped, 0xdb21ac8c, 1, 0, 0, 0);
					menu::getPresence()->joinThem(joinbackRid);
					hasFinished = true;
				});
			} else {
				menu::getNotify()->right("~e~Hold Your horses");
			}
		}*/
	}

	void giveAmmoToPed(Ped ped, Hash ammo) {
		native::giveAmmoOfTypeToPed(ped, ammo, 9999);
	}

	void dropAmmo(Ped ped) {
		static std::vector<uint32_t> ammo = {
			 native::getHashKey("pickup_weapon_pistol_semiauto_mp") ,
			 native::getHashKey("pickup_weapon_pistol_volcanic_mp") ,
			 native::getHashKey("pickup_weapon_pistol_mauser_mp") ,
			 native::getHashKey("pickup_weapon_revolver_schofield_mp") ,
			 native::getHashKey("pickup_weapon_revolver_doubleaction_mp") ,
			 0x573E66CA ,
			 0x5F22B3DB ,
			 native::getHashKey("pickup_weapon_revolver_cattleman_mp") ,
			 0x7529BF7D ,
			 native::getHashKey("pickup_weapon_repeater_carbine_mp") ,
			 native::getHashKey("pickup_weapon_repeater_henry_mp") ,
			 native::getHashKey("pickup_weapon_repeater_winchester_mp") ,
			 native::getHashKey("pickup_weapon_rifle_springfield_mp") ,
			 native::getHashKey("pickup_weapon_rifle_varmint_mp") ,
			 native::getHashKey("pickup_weapon_rifle_boltaction_mp") ,
			 native::getHashKey("pickup_weapon_sniperrifle_carcano_mp") ,
			 native::getHashKey("pickup_weapon_sniperrifle_rollingblock_mp") ,
			 native::getHashKey("pickup_weapon_shotgun_pump_mp") ,
			 native::getHashKey("pickup_weapon_shotgun_semiauto_mp") ,
			 native::getHashKey("pickup_weapon_shotgun_repeating_mp") ,
			 native::getHashKey("pickup_weapon_shotgun_sawedoff_mp") ,
			 native::getHashKey("pickup_weapon_shotgun_doublebarrel_mp") ,
			 native::getHashKey("pickup_weapon_bow_mp") ,
			 native::getHashKey("pickup_weapon_melee_knife_mp") ,
			 native::getHashKey("pickup_weapon_melee_machete_mp") ,
			 native::getHashKey("pickup_weapon_melee_torch_mp") ,
			 native::getHashKey("pickup_weapon_thrown_dynamite_mp") ,
			 native::getHashKey("PICKUP_WEAPON_THROWN_MOLOTOV_MP") ,
			 native::getHashKey("pickup_weapon_thrown_throwing_knives_mp") ,
			 native::getHashKey("pickup_weapon_thrown_tomahawk_mp") ,
			 native::getHashKey("PICKUP_WEAPON_THROWN_BOLAS"),
			 native::getHashKey("PICKUP_WEAPON_REVOLVER_NAVY_MP"),
			  native::getHashKey("PICKUP_WEAPON_REVOLVER_NAVY_CROSSOVER"),
			 //PICKUP_WEAPON_REVOLVER_NAVY_CROSSOVER
			 //PICKUP_WEAPON_THROWN_BOLAS_MP
			//native::getHashKey("PICKUP_AMMO_ARROW"),
			//native::getHashKey("PICKUP_AMMO_ARROW_CONFUSION"),
			//native::getHashKey("PICKUP_AMMO_ARROW_DISORIENT"),
			//native::getHashKey("PICKUP_AMMO_ARROW_DRAIN"),
			//native::getHashKey("PICKUP_AMMO_ARROW_WOUND"),
			//native::getHashKey("PICKUP_AMMO_PISTOL"),
			//native::getHashKey("PICKUP_AMMO_REPEATER"),
			//native::getHashKey("PICKUP_AMMO_REVOLVER"),
			//native::getHashKey("PICKUP_AMMO_RIFLE"),
			//native::getHashKey("PICKUP_AMMO_SHOTGUN"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_CONFUSION"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_DISORIENT"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_DRAIN"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_DYNAMITE"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_FIRE"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_IMPROVED"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_POISON"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_SMALL_GAME"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_TRAIL"),
			//native::getHashKey("PICKUP_AMMO_SINGLE_ARROW_WOUND"),
			//native::getHashKey("PICKUP_AMMO_THROWING_KNIVES_CONFUSION"),
			//native::getHashKey("PICKUP_AMMO_THROWING_KNIVES_DISORIENT"),
			//native::getHashKey("PICKUP_AMMO_THROWING_KNIVES_DRAIN"),
			//native::getHashKey("PICKUP_AMMO_THROWING_KNIVES_WOUND"),
			//native::getHashKey("PICKUP_WEAPON_AMMO_ARROW_DYNAMITE_BUNDLE"),
			//native::getHashKey("PICKUP_WEAPON_BOW"),
			//native::getHashKey("PICKUP_WEAPON_BOW_MP"),
			//native::getHashKey("PICKUP_WEAPON_FISHINGROD"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_ANCIENT_HATCHET"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_BROKEN_SWORD"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_BROKEN_SWORD_MP"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_CLEAVER"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_CLEAVER_MP"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET_DOUBLE_BIT"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET_DOUBLE_BIT_RUSTED"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET_HEWING"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET_HUNTER"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET_HUNTER_RUSTED"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET_MP"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_HATCHET_VIKING"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_KNIFE_MP"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_LANTERN_ELECTRIC"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_LASSO_MP"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_MACHETE_MP"),
			//native::getHashKey("PICKUP_WEAPON_MELEE_TORCH_MP"),
			//native::getHashKey("PICKUP_WEAPON_MOONSHINEJUG_MP"),
			//native::getHashKey("PICKUP_WEAPON_PISTOL_MAUSER_MP"),
			//native::getHashKey("PICKUP_WEAPON_PISTOL_SEMIAUTO_MP"),
			//native::getHashKey("PICKUP_WEAPON_PISTOL_VOLCANIC_MP"),
			//native::getHashKey("PICKUP_WEAPON_REPEATER_CARBINE_MP"),
			//native::getHashKey("PICKUP_WEAPON_REPEATER_HENRY_MP"),
			//native::getHashKey("PICKUP_WEAPON_REPEATER_WINCHESTER_MP"),
			//native::getHashKey("PICKUP_WEAPON_REVOLVER_CATTLEMAN_MP"),
			//native::getHashKey("PICKUP_WEAPON_REVOLVER_DOUBLEACTION_MP"),
			//native::getHashKey("PICKUP_WEAPON_REVOLVER_NAVY"),
			//native::getHashKey("PICKUP_WEAPON_REVOLVER_SCHOFIELD_MP"),
			//native::getHashKey("PICKUP_WEAPON_RIFLE_BOLTACTION_MP"),
			//native::getHashKey("PICKUP_WEAPON_RIFLE_SPRINGFIELD_MP"),
			//native::getHashKey("PICKUP_WEAPON_RIFLE_VARMINT_MP"),
			//native::getHashKey("PICKUP_WEAPON_SHOTGUN_DOUBLEBARREL_MP"),
			//native::getHashKey("PICKUP_WEAPON_SHOTGUN_PUMP_MP"),
			//native::getHashKey("PICKUP_WEAPON_SHOTGUN_REPEATING_MP"),
			//native::getHashKey("PICKUP_WEAPON_SHOTGUN_SAWEDOFF_MP"),
			//native::getHashKey("PICKUP_WEAPON_SHOTGUN_SEMIAUTO_MP"),
			//native::getHashKey("PICKUP_WEAPON_SINGLE_ARROW"),
			//native::getHashKey("PICKUP_WEAPON_SINGLE_ARROW"),
			//native::getHashKey("PICKUP_WEAPON_SINGLE_ARROW_FIRE"),
			//native::getHashKey("PICKUP_WEAPON_SNIPERRIFLE_CARCANO_MP"),
			//native::getHashKey("PICKUP_WEAPON_SNIPERRIFLE_ROLLINGBLOCK_MP"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_DYNAMITE_MP"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_MOLOTOV_MP"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_POISONBOTTLE"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_THROWING_KNIVES_MP"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_TOMAHAWK"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_TOMAHAWK_ANCIENT"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_TOMAHAWK_HOMING"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_TOMAHAWK_IMPROVED"),
			//native::getHashKey("PICKUP_WEAPON_THROWN_TOMAHAWK_MP"),
				// WEAPON_THROWN_BOLAS
		};
		int bit = 0;
		native::setBit(&bit, 16);
		native::setBit(&bit, 8);
		for (std::size_t i = 0; i < ammo.size(); i++) {
			
			menu::getControlManager()->requestModel(_i<Hash>(0xF70825EB340E7D15, _i<int>(0x08F96CA6C551AD51,ammo[i])), [=](DWORD model) {
				auto c = native::getEntityCoords(ped);
				auto handle = native::createAmbientPickup(ammo[i], { c.x,c.y,c.z + 1.f }, bit, 60, model, TRUE, FALSE, 0, .0f);
				menu::util::waitFiber(10);
				if (native::doesEntityExist(handle)) {
					native::setNetworkIdExistsOnAllMachines(native::networkGetNetworkIdFromEntity(handle), TRUE);
					native::networkRegisterEntityAsNetworked(handle);
				}
			});
		}
	}



}

void weaponsMenu::init() {
	setName("Weapons");
	setParentSubmenu<weaponMenu>();

	addOption(submenuOption("Weapon List")
		.addSubmenu<weaponsListMenu>());

	addOption(scrollOption<Hash>(SCROLLSELECT, "Immobile Weapons")
		.addTranslation()
		.addScroll(vars.immobileWeapons, 0, NUMOF(global::arrays::g_vehicleModelTurrets), global::arrays::g_vehicleModelTurrets)
		.addOnClick([] {
			menu::getControlManager()->requestModel(global::arrays::g_vehicleModelTurrets[vars.immobileWeapons].result, [] (DWORD hash) {
				menu::util::getSpawnManager()->spawnVehicle(hash, menu::getLocalPlayer().coords + (native::getEntityForwardVector(menu::getLocalPlayer().ped) * 5.f), native::getEntityHeading(menu::getLocalPlayer().ped));
			});
		}));

	addOption(buttonOption("Max Weapon Upgrades (Current Weapon)")
		.addTranslation()
		.addOnClick([] {
			for (std::size_t i = 0; i < global::vars::g_weaponComponentHashes.size(); i++) {
				native::giveWeaponComponentToEntity(menu::getLocalPlayer().ped, global::vars::g_weaponComponentHashes[i], menu::getLocalPlayer().weaponHash, TRUE);
			}
		}));

	addOption(buttonOption("Max Weapon Upgrades (All Weapons)")
		.addTranslation()
		.addOnClick([] {
			for (int wep = 0; wep < NUMOF(global::arrays::g_weaponList); wep++) {
				for (std::size_t i = 0; i < global::vars::g_weaponComponentHashes.size(); i++) {
					native::giveWeaponComponentToEntity(menu::getLocalPlayer().ped, global::vars::g_weaponComponentHashes[i], global::arrays::g_weaponList[wep].result, TRUE);
				}
			}
		}));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Give Ammo")
		.addTranslation()
		.addScroll(vars.ammoVar, 0, NUMOF(global::arrays::g_ammoList), global::arrays::g_ammoList)
		.addOnClick([] { giveAmmoToPed(menu::getLocalPlayer().ped, global::arrays::g_ammoList[vars.ammoVar].result); }));

	addOption(buttonOption("Give All Ammo (Unlocked Weapons)")
		.addTranslation()
		.addOnClick([] {
			for (std::size_t i = 0; i < global::vars::g_weaponHashes.size(); i++) {
				if (global::vars::g_weaponHashes[i]) {
					giveAmmoToPed(menu::getLocalPlayer().ped, global::vars::g_weaponHashes[i]);
				}
			}
		}));

	addOption(buttonOption("Drop All Ammo")
		.addTranslation()
		.addOnClick([] { dropAmmo(menu::getLocalPlayer().ped); }));

	addOption(buttonOption("Give All Weapons (unlocked)")
		.addTranslation()
		.addOnClick([] {
			for (std::size_t i = 0; i < global::vars::g_weaponHashes.size(); i++) {
				if (global::vars::g_weaponHashes[i]) {
					givePedWeapon(menu::getLocalPlayer().ped, global::vars::g_weaponHashes[i], 9999);
				}
			}

			native::setCurrentPedWeapon(menu::getLocalPlayer().ped, 0xdb21ac8c, 1, 0, 0, 0);
		}));

	addOption(buttonOption("Give All Weapons (freemode)")
		.addTranslation()
		.addOnClick([] { addAllWeapons(); })
		.addTooltip("This is a small process\n~e~NOTE~w~: Only use this in freemode sessions"));
}

/*Called while in submenu*/
void weaponsMenu::update() {}

/*Called once on submenu open*/
void weaponsMenu::updateOnce() {}

/*Called always*/
void weaponsMenu::featureUpdate() {}

weaponsMenu* _instance;
weaponsMenu* weaponsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new weaponsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

weaponsMenu::~weaponsMenu() { delete _instance; }