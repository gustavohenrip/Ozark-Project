#include "nice.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/submenus/main/network/players.hpp"
#include "menu/base/submenus/main/weapon/explosion.hpp"
#include "menu/base/submenus/main/weapon/weapon_mods.hpp"
#include "menu/base/submenus/main/network/account/treasure.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "player_treasure.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/weapon/weapons.hpp"

using namespace networkNiceMenuVars;

namespace networkNiceMenuVars {
	vars_ vars;

	scrollStruct<int> aimedList[3] = {
		{ "Dismember", 0 },
	    { "Smite", 1 },
		{ "Explode", 2}
	};

	void explosiveAmmo(menu::playerVars& player, int type) {
		math::vector3<float> coords;
		if (native::isPedShooting(player.ped) && native::getPedLastWeaponImpactCoord(player.ped, &coords)) {
			native::addOwnedExplosion(player.ped, coords, type, .5f, 1, 0, .1f);
		}
	}

	void antiAim(menu::playerVars& player, int type) {
		Entity entity;
		for (Player i = 0; i < 32; i++) {
			if (native::networkIsPlayerConnected(i) && native::getEntityPlayerIsfreeAimingAt(i, &entity) && entity == player.ped ) {
				if (type == 0) {
					auto head = native::getPedBoneCoords(native::getPlayerPed(i), 0x5226, 0, 0, 0);
					native::shootSingleBulletBetweenCoords({ head.x, head.y, head.z + 0.4f }, head, 10000, 1, native::getHashKey("WEAPON_SHOTGUN_SAWEDOFF"), 0, 1, 0, -1.f, 0);
				} else if (type == 1) {
					weaponModsMenuVars::smitePed(native::getPlayerPed(i));
				} else if (type == 2) {
					auto coords = native::getEntityCoords(native::getPlayerPed(i));
					native::addOwnedExplosion(player.ped, coords, 0, 2.f, 1, 0, .1f);
				}
			}
		}
	}
}


void idkwhatimdoing(uint32_t model) {
	menu::getControlManager()->requestModel(model, [=](int model) {
		int bit = 8913159;
		bit |= 8;
		int unk1 = 1;
		int unk2 = 0;
		//native::setBit(&bit, 16);
		//native::setBit(&bit, 8);
		auto c = menu::getSelectedPlayer().coords;
		//auto handle = native::createAmbientPickup(native::getHashKey("pickup_custom_script"), { c.x, c.y,c.z + 1.5f }, bit, 5, model, TRUE, FALSE, 0, .0f);
		auto coords = native::getOffsetFromEntityInWorldCoords(menu::getSelectedPlayer().ped, { .0f, 2.f, .0f });
		auto handle = native::createPickupRotate(native::getHashKey("pickup_custom_script"), coords, {},bit, unk1,2,FALSE,model,unk2,.0f,0);

		menu::util::waitFiber(10);
		if (native::doesEntityExist(handle)) {
			native::setNetworkIdExistsOnAllMachines(native::networkGetNetworkIdFromEntity(handle), TRUE);
			native::networkRegisterEntityAsNetworked(handle);
		}
	});
}

void networkNiceMenu::init() {
	setName("Nice");
	setParentSubmenu<networkPlayersMenu>();

	addOption(submenuOption("Treasure")
		.addOnClick([] { networkPlayerTreasureMenuVars::vars.allPlayers = false; })
		.addSubmenu<networkPlayerTreasureMenu>());

	addOption(scrollOption<int>(TOGGLE, "Explosive Ammo")
		.addScroll(menu::getSelectedPlayer().explosiveAmmoType, 0, NUMOF(explosionMenuVars::explosions), explosionMenuVars::explosions)
		.addToggle(menu::getSelectedPlayer().explosiveAmmo)
		.addOnUpdate([](scrollOption<int>* option) { option->addScroll(menu::getSelectedPlayer().explosiveAmmoType, 0, NUMOF(explosionMenuVars::explosions), explosionMenuVars::explosions)
			.addToggle(menu::getSelectedPlayer().explosiveAmmo); }));

	addOption(scrollOption<int>(TOGGLE, "Anti Aim")
		.addScroll(menu::getSelectedPlayer().antiAimType, 0, NUMOF(aimedList), aimedList)
		.addToggle(menu::getSelectedPlayer().antiAim)
		.addOnUpdate([](scrollOption<int>* option) { option->addScroll(menu::getSelectedPlayer().antiAimType, 0, NUMOF(aimedList), aimedList)
			.addToggle(menu::getSelectedPlayer().antiAim); }));

	addOption(buttonOption("Drop Ammo")
		.addOnClick([] { weaponsMenuVars::dropAmmo(menu::getSelectedPlayer().ped); }));

	addOption(buttonOption("Drop Health")
		.addOnClick([] {
			auto pickupHash = 0x8F707C18;
			menu::getControlManager()->requestModel(0xf4271318, [=](int model) {
				int bit = 0;
				native::setBit(&bit, 16);
				native::setBit(&bit, 8);
				auto c = menu::getSelectedPlayer().coords;
				auto handle = native::createAmbientPickup(pickupHash, { c.x, c.y,c.z + 1.5f }, bit, 1,  model, TRUE, FALSE, 0, .0f);

				menu::util::waitFiber(10);
				if (native::doesEntityExist(handle)) {
					native::setNetworkIdExistsOnAllMachines(native::networkGetNetworkIdFromEntity(handle), TRUE);
					native::networkRegisterEntityAsNetworked(handle);
				}
			});
		}));

}

/*Called while in submenu*/
void networkNiceMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkNiceMenu::updateOnce() {}

/*Called always*/
void networkNiceMenu::featureUpdate() {
	menu::getPlayerManager()->foreach([=](menu::playerVars& player) {
		if (player.explosiveAmmo) {
			explosiveAmmo(player, player.explosiveAmmoType);
		}

		if (player.antiAim) {
			antiAim(player, player.antiAimType);
		}
	});

}

networkNiceMenu* _instance;
networkNiceMenu* networkNiceMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkNiceMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkNiceMenu::~networkNiceMenu() { delete _instance; }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               