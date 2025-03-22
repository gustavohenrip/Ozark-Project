#include "player.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "player/visions.hpp"
#include "player/appearance.hpp"
#include "player/particles.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "menu/base/util/timers.hpp"

using namespace playerMenuVars;

namespace playerMenuVars {
	vars_ vars;

	void noRagdoll(bool toggle) {
		native::setPedCanRagdoll(menu::getLocalPlayer().ped, !toggle);
		native::setPedCanRagdollFromPlayerImpact(menu::getLocalPlayer().ped, !toggle);
		native::setPedCanBeKnockedOffVehicle(menu::getLocalPlayer().ped, !toggle);
		native::setPedCanRagdollOnCollision(menu::getLocalPlayer().ped, !toggle);
	}

	void superchargeCore(int core) {
		_i<int>(0x4AF5A4C7B9157D14, menu::getLocalPlayer().ped, core);
		_i<int>(0xF6A7C08DF2E28B28, menu::getLocalPlayer().ped, core);
	}
}

void playerMenu::init() {
	setName("Player");
	setParentSubmenu<mainMenu>();

	// Animations

	addOption(submenuOption("Appearance")
		.addTranslation()
		.addSubmenu<appearanceMenu>());

	addOption(submenuOption("Particles")
		.addTranslation()
		.addSubmenu<particleMenu>());

	addOption(submenuOption("Visions")
		.addTranslation()
		.addSubmenu<visionMenu>());

	addOption(toggleOption("Invincibility")
		.addTranslation()
		.addToggle(vars.invincibility).canBeSaved(name.getOriginal())
		.addOnClick([] { native::setEntityInvincible(menu::getLocalPlayer().ped, vars.invincibility); })
		.addTooltip("You can't die!"));

	addOption(toggleOption("Fast Health Regeneration")
		.addTranslation()
		.addToggle(vars.fasthealthRegen).canBeSaved(name.getOriginal())
		.addTooltip("It'll be hard to die!"));

	addOption(toggleOption("Never Wanted")
		.addTranslation()
		.addToggle(vars.neverWanted).canBeSaved(name.getOriginal())
		.addOnClick([] { native::setWantedLevelMultiplier(1.f); })
		.addTooltip("Authorities won't bother you"));

	addOption(toggleOption("No Bounty")
		.addTranslation()
		.addToggle(vars.noBounty).canBeSaved(name.getOriginal())
		.addTooltip("Bounty hunters won't bother you"));

	addOption(toggleOption("Off the Radar")
		.addTranslation()
		.addToggle(vars.offTheRadar).canBeSaved(name.getOriginal())
		.addOnClick([] {
			//auto flags = global::vars::g_offTheRadarFlag;
			//*(uint16_t*)(flags + 0xC) = vars.offTheRadar;
			*(uint16_t*)(global::vars::g_offTheRadarFlag + 0xC) = 0x100;
			//*(uint16_t*)(flags) = vars.offTheRadar;
			//*(uint16_t*)(flags + 0xC) = vars.offTheRadar;
			//*(uint16_t*)(flags + 0x4) = vars.offTheRadar;
			//*(uint8_t*)(flags - 0xB) = !vars.offTheRadar;
			//*(uint8_t*)(flags - 0xA) = !vars.offTheRadar;
			//*(uint8_t*)(flags - 0xC) = !vars.offTheRadar;
		})
		.addTooltip("Removes your player blip from the map for all clients"));

	addOption(buttonOption("End Pursuit")
		.addTranslation()
		.addOnClick([] { native::endPursuit(); })
		.addTooltip("End current police pursuit"));

	addOption(toggleOption("Invisibility")
		.addTranslation()
		.addToggle(vars.invisibility).canBeSaved(name.getOriginal())
		.addOnClick([] { 
			native::setEntityVisible(menu::getLocalPlayer().ped, !vars.invisibility); 
		//	auto flags = global::vars::g_offTheRadarFlag;
		//	*(uint16_t*)(flags + 0xC) = vars.invisibility;
			*(uint16_t*)(global::vars::g_offTheRadarFlag + 0xC) = vars.invisibility ? 1 : 100;
			//*(uint16_t*)(flags) = vars.invisibility;
			//*(uint16_t*)(flags + 0xC) = vars.invisibility;
			//*(uint16_t*)(flags + 0x4) = vars.invisibility;
			//*(uint8_t*)(flags - 0xB) = !vars.invisibility;
			//*(uint8_t*)(flags - 0xA) = !vars.invisibility;
			//*(uint8_t*)(flags - 0xC) = !vars.invisibility;
		})
		.addTooltip("Become Casper the Ghost and not be seen by anyone"));

	addOption(buttonOption("Supercharge Cores")
		.addTranslation()
		.addOnClick([] { for (std::size_t i = 0; i < 3; i++)superchargeCore((int32_t)i); })
		.addTooltip("Overpower cores"));

	addOption(toggleOption("Supercharged Cores")
		.addTranslation()
		.addToggle(vars.superchargeCores).canBeSaved(name.getOriginal())
		.addTooltip("Continuously overpowers all cores."));

	addOption(toggleOption("Unlimited Stamina")
		.addTranslation()
		.addToggle(vars.unlimitedStamina).canBeSaved(name.getOriginal())
		.addTooltip("You'll never guess what this option does"));
	
	addOption(toggleOption("Infinite Dead-Eye")
		.addTranslation()
		.addToggle(vars.unlimitedDeadEye).canBeSaved(name.getOriginal())
		.addTooltip("Dead-Eye core never drains"));

	addOption(toggleOption("Infinite Eagle-Eye")
		.addTranslation()
		.addToggle(vars.infiniteEagleEye).canBeSaved(name.getOriginal())
		.addOnClick([] { native::setPlayerInfiniteDeadEyeEagleEye(menu::getLocalPlayer().id, vars.infiniteEagleEye); })
		.addTooltip("Eagle-Eye core never drains"));

	addOption(toggleOption("No Ragdoll")
		.addTranslation()
		.addToggle(vars.noRagdoll).canBeSaved(name.getOriginal())
		.addOnClick([] { noRagdoll(!vars.noRagdoll); })
		.addTooltip("No more falling over"));

	addOption(toggleOption("Super Jump")
		.addTranslation()
		.addToggle(vars.superJump).canBeSaved(name.getOriginal())
		.addTooltip("Jump jump jump!"));

	addOption(toggleOption("Super Swim")
		.addTranslation()
		.addToggle(vars.superSwim).canBeSaved(name.getOriginal())
		.addOnClick([] { native::setSwimMultiplierForPlayer(menu::getLocalPlayer().id, vars.superSwim ? 1.49f : 1.f); })
		.addTooltip("Swim swim swim!"));

	addOption(numberOption<int>(TOGGLE, "Super Run")
		.addTranslation()
		.addNumber(vars.superRunSpeed, "%i", 1).addMin(1).addMax(100)
		.addToggle(vars.superRun).canBeSaved(name.getOriginal())
		.addTooltip("Run forest, run!"));

	addOption(buttonOption("Clone")
		.addTranslation()
		.addOnClick([] { native::clonePed(menu::getLocalPlayer().ped, native::getEntityHeading(menu::getLocalPlayer().ped), TRUE, FALSE); })
		.addTooltip("Make another one of you :o"));

	addOption(buttonOption("Clean")
		.addTranslation()
		.addOnClick([] {
			native::clearPedBloodDamage(menu::getLocalPlayer().ped);
			native::clearPedDamageDecalByZone(menu::getLocalPlayer().ped, 10, "ALL");
			native::clearPedDirt(menu::getLocalPlayer().ped);
			native::clearPedWetness(menu::getLocalPlayer().ped);
			})
		.addTooltip("Scrubs your player of filth"));

	addOption(buttonOption("Suicide")
		.addTranslation()
		.addOnClick([] { native::applyDamageToPed(menu::getLocalPlayer().ped, 30000, true, 0, 0); })
		.addTooltip("Kill yourself :("));

	addOption(toggleOption("Forcefield")
		.addTranslation()
		.addToggle(vars.forcefield).canBeSaved(name.getOriginal())
		.addTooltip("Make sure you enable invincibility before you do something like this :/"));

	addOption(toggleOption("Shrink")
		.addTranslation()
		.addToggle(vars.shrink).canBeSaved(name.getOriginal())
		.addOnClick([] { native::setPedScale(menu::getLocalPlayer().ped, vars.shrink ? 0.5f : 1.f); })
		.addTooltip("Smol.."));
}

/*Called while in submenu*/
void playerMenu::update() {}

/*Called once on submenu open*/
void playerMenu::updateOnce() {}

/*Called always*/
void playerMenu::featureUpdate() {

	if (vars.invincibility) {
		native::setEntityInvincible(menu::getLocalPlayer().ped, true);
	}

	if (vars.fasthealthRegen) {
		if (!native::isPlayerDead(menu::getLocalPlayer().id)) {
			int health = native::getEntityHealth(menu::getLocalPlayer().ped);
			if (health < native::getEntityMaxHealth(menu::getLocalPlayer().ped, FALSE)) {
				native::setEntityHealth(menu::getLocalPlayer().ped, health + 4, 1);
				native::setPedCoreLevel(menu::getLocalPlayer().ped, 0, health + 4);// 0 health core 1 stamina core
			}
		}
	}

	if (vars.superchargeCores) {
		for (std::size_t i = 0; i < 3; i++) {
			superchargeCore((int32_t)i);
		}
	}

	if (vars.neverWanted) {
		native::setWantedLevelMultiplier(.0f);
	}

	if (vars.noBounty) {
		native::setBountyForPlayer(menu::getLocalPlayer().id, 0);
	}

	if (vars.invisibility) {
		native::setEntityVisible(menu::getLocalPlayer().ped, false);
	}

	if (vars.unlimitedStamina) {
		native::restorePlayerStamina(menu::getLocalPlayer().id, 100.f);
		native::setPedCoreLevel(menu::getLocalPlayer().ped, 1, 100);
	}

	if (vars.unlimitedDeadEye) {
		native::restoreSpecialAbility(menu::getLocalPlayer().id, 0, 0);
		native::setPedCoreLevel(menu::getLocalPlayer().ped, 2, 100);
	}

	if (vars.infiniteEagleEye) {
		native::setPlayerInfiniteDeadEyeEagleEye(menu::getLocalPlayer().id, TRUE);
	}

	if (vars.noRagdoll) {
		noRagdoll(true);
	}

	if (vars.superJump) {
		native::setSuperJumpThisFrame(menu::getLocalPlayer().id);
	}

	if (vars.superRun) {
		if (!menu::getBase()->open) {
			static bool stop = false;
			if (!menu::getLocalPlayer().inVehicle) {
				/*if (native::isDisabledControlPressed(0, INPUT_FRONTEND_ACCEPT) || menu::util::getInput()->isPressed(true, VK_SHIFT)) {
					native::applyForceToEntityCenterOfMass(menu::getLocalPlayer().ped, 1, 0.f, (float)vars.superRunSpeed, 0.f, FALSE, TRUE, TRUE, TRUE);
					stop = true;
				} else if (stop) {
					native::setEntityCoords(menu::getLocalPlayer().ped, menu::getLocalPlayer().coords);
					native::taskStandStill(menu::getLocalPlayer().ped, 1);
					stop = false;
				}*/

				if (native::isDisabledControlPressed(0, INPUT_FRONTEND_ACCEPT) || menu::util::getInput()->isPressed(true, VK_SHIFT)) {
					native::applyForceToEntityCenterOfMass(menu::getLocalPlayer().ped, 1, 0.f, (float)vars.superRunSpeed, 0.f, FALSE, TRUE, TRUE, TRUE);
				}
			}
		}
	}

	if (vars.superSwim) {
		native::setSwimMultiplierForPlayer(menu::getLocalPlayer().id, 1.49f);
	}

	if (vars.forcefield) {
		native::addExplosion(menu::getLocalPlayer().coords, 29, 100.f, FALSE, TRUE, 0.f);
	}

	if (vars.offTheRadar) {
		static int otfTimer = 0;
		menu::util::getTimers()->runTimedFunction(&otfTimer, 10000, [] {
			//auto flags = global::vars::g_offTheRadarFlag;
			*(uint16_t*)(global::vars::g_offTheRadarFlag + 0xC) = 1;
			//*(uint16_t*)(flags) = vars.offTheRadar;
			//*(uint16_t*)(flags + 0xC) = vars.offTheRadar;
			//*(uint16_t*)(flags + 0x4) = vars.offTheRadar;
			//*(uint8_t*)(flags - 0xB) = !vars.offTheRadar;
			//*(uint8_t*)(flags - 0xA) = !vars.offTheRadar;
			//*(uint8_t*)(flags - 0xC) = !vars.offTheRadar;
		});
	}
}

playerMenu* _instance;
playerMenu* playerMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new playerMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

playerMenu::~playerMenu() { delete _instance; }