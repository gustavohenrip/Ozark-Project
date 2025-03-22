#include "attack.hpp"
#include "menu/base/submenu_handler.hpp"
#include "nasty.hpp"
#include "../all/all_nasty.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace networkAttackMenuVars;

namespace networkAttackMenuVars {
	vars_ vars;

	scrollStruct<math::vector2<uint32_t>> attackers[14] = {
		{ "Alligator", { 0xA0B33A7B, 0 } },
		{ "Black Bear", { 0x2b845466, 0 } },
		{ "Bat", { 0x28308168, 0 } },
		{ "Buck", { 0x8af5c2a8, 0 } },
		{ "Chicken", { 0x8506531d, 0 } },
		{ "Cougar", { 0x56154f7, 0 } },
		{ "Hawk", { 0x80184d63, 0 } },
		{ "Arabian Black", { 0x88d6a59e, 0 } },
		{ "Obese Woman", { 0x18fe1eb6, 0x6d9bb970 } },
		{ "Obese Man", { 0x41907533, 0x6d9bb970 } },
		{ "Two Headed Skeleton", {0x5F3EE4D3, 0} },
		{ "Wise Old Man", {0xde86823e, 0x6d9bb970} },
		{ "Rogue Robot", { 0x3bf7829e, 0x9e4ad291 }},
		{ "Ku Klux Member", { 0x39c84a35, 0x9e4ad291 }},
	};

	void sendKuKluxKlan(menu::playerVars& player) {
		int weaponIndex = 0;
		for (std::size_t i = 0; i < 5; i++) {
			menu::getControlManager()->requestModel(0x39c84a35, [&](DWORD model) {
				Ped ped = menu::util::getSpawnManager()->spawnPed(model, player.coords, 0.f);
				if (ped) {
					if (vars.spawnInvincible) {
						native::setEntityInvincible(ped, TRUE);
					}
					std::vector<float> random;
					for (std::size_t j = 0; j < 2; j++) {
						random.push_back(native::getRandomFloatInRange(-.5f, .5f));
					}
					auto offset = native::getOffsetFromEntityInWorldCoords(player.ped, { random[0], random[1], .0f });
					native::setEntityCoords(ped, offset);
					native::setPedAsGroupMember(ped, 69);
					native::setPedCombatAbility(ped, 100);
					native::taskCombatPed(ped, player.ped, 0, 16);
					native::setPedCanRagdoll(ped, FALSE);
					native::giveDelayedWeaponToPed(ped, 0xa84762ec, 25000, 1, 0x2CD419DC);
					native::setCurrentPedWeapon(ped, 0xa84762ec, 1, 0, 0, 0);

					if (weaponIndex > 3) {
						weaponIndex = 0;
					} else {
						weaponIndex++;
					}
				}
			});
		}
		native::setModelAsNoLongerNeeded(0x39c84a35);
	}

	void sendAttacker(menu::playerVars& player, Hash hash, Hash weapon) {
		menu::getControlManager()->requestModel(hash, [=](DWORD model) {
			Ped ped = menu::util::getSpawnManager()->spawnPed(model, player.coords, 0.f);
			if (ped) {
				if (vars.spawnInvincible) {
					native::setEntityInvincible(ped, TRUE);
				}

				native::setPedAsGroupMember(ped, 69);
				native::setPedCombatAbility(ped, 100);
				native::taskCombatPed(ped, player.ped, 0, 16);
				native::setPedCanRagdoll(ped, FALSE);

				if (weapon) {
					native::setPedCanRagdollFromPlayerImpact(ped, FALSE);
					native::giveDelayedWeaponToPed(ped, weapon, 25000, 1, 0x2CD419DC);
					native::setCurrentPedWeapon(ped, weapon, 1, 0, 0, 0);
				}

				native::setModelAsNoLongerNeeded(model);
			}
		});
	}
}

void networkAttackMenu::init() {
	setName("Attackers");
	setParentSubmenu<networkNastyMenu>();
}

/*Called while in submenu*/
void networkAttackMenu::update() {
	if (!vars.allPlayers) {
		menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
	}
}

/*Called once on submenu open*/
void networkAttackMenu::updateOnce() {
	clearOptionsOffset(0);

	addOption(toggleOption("Spawn Invincible")
		.addTranslation()
		.addToggle(vars.spawnInvincible));

	addOption(breakOption("Attackers"));

	if (!vars.allPlayers) {
		setParentSubmenu<networkNastyMenu>();

		addOption(buttonOption("Ku Klux Klan")
			.addOnClick([] { sendKuKluxKlan(menu::getSelectedPlayer()); }));

		for (std::size_t i = 0; i < NUMOF(attackers); i++) {
			addOption(buttonOption(attackers[i].name)
				.addOnClick([=] { sendAttacker(menu::getSelectedPlayer(), attackers[i].result.x, attackers[i].result.y); }));
		}
	} else {
		setParentSubmenu<networkAllNastyMenu>();

		addOption(buttonOption("Ku Klux Klan")
			.addOnClick([] { menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { sendKuKluxKlan(lobby); }); }));

		for (std::size_t i = 0; i < NUMOF(attackers); i++) {
			addOption(buttonOption(attackers[i].name)
				.addOnClick([=] { menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) { if (lobby.id != menu::getLocalPlayer().id) { sendAttacker(lobby, attackers[i].result.x, attackers[i].result.y); }}); }));
		}
	}
}

/*Called always*/
void networkAttackMenu::featureUpdate() {}

networkAttackMenu* _instance;
networkAttackMenu* networkAttackMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAttackMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAttackMenu::~networkAttackMenu() { delete _instance; }