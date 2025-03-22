#include "mount.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle.hpp"

using namespace mountMenuVars;

namespace mountMenuVars {
	vars_ vars;

	void setMountInvincible(BOOL toggle) {
		if (native::isPedOnMount(menu::getLocalPlayer().ped)) {
			native::setEntityInvincible(menu::getLocalPlayer().mount, toggle);
		}
	}

	void noRagdoll(bool toggle) {
		if (native::isPedOnMount(menu::getLocalPlayer().ped)) {
			native::setPedCanRagdoll(menu::getLocalPlayer().mount, !toggle);
			native::setPedCanRagdollFromPlayerImpact(menu::getLocalPlayer().mount, !toggle);
			native::setPedCanBeKnockedOffVehicle(menu::getLocalPlayer().mount, !toggle);
			native::setPedCanRagdollOnCollision(menu::getLocalPlayer().mount, !toggle);
		}
	}
}

void mountMenu::init() {
	setName("Mount");
	setParentSubmenu<vehicleMenu>();

	addOption(buttonOption("Get on Last Mount")
		.addTranslation()
		.addOnClick([] { native::setPedOnMount(menu::getLocalPlayer().ped, native::getLastPedMount(menu::getLocalPlayer().ped), -1, 1); }));

	addOption(toggleOption("Invincibility")
		.addTranslation()
		.addToggle(vars.invincibility).canBeSaved(name.getOriginal())
		.addOnClick([] { setMountInvincible(FALSE); })
		.addTooltip("Your mount can't die!"));

	addOption(toggleOption("Invisibility")
		.addTranslation()
		.addToggle(vars.invisibility).canBeSaved(name.getOriginal())
		.addOnClick([] {if (native::isPedOnMount(menu::getLocalPlayer().ped)) { native::setEntityVisible(menu::getLocalPlayer().mount, !vars.invisibility); } }));

	addOption(toggleOption("Unlimited Stamina")
		.addTranslation()
		.addToggle(vars.unlimitedStamina).canBeSaved(name.getOriginal())
		.addTooltip("Eliminate breathless mount."));

	addOption(toggleOption("Instant Health Regen")
		.addTranslation()
		.addToggle(vars.fastRegenHealth).canBeSaved(name.getOriginal()));

	addOption(toggleOption("No Ragdoll")
		.addTranslation()
		.addToggle(vars.noRagdoll).canBeSaved(name.getOriginal())
		.addOnClick([] { noRagdoll(!vars.noRagdoll); })
		.addTooltip("No more falling over"));

	addOption(numberOption<int>(TOGGLE, "Super Gallop")
		.addTranslation()
		.addNumber(vars.superGallopSpeed, "%i", 1).addMin(1).addMax(100)
		.addToggle(vars.superGallop).canBeSaved(name.getOriginal())
		.addTooltip("Run forest, run!"));

	//addOption(toggleOption("Horn Boost")
	//	.addTranslation()
	//	.addToggle(vars.hornBoost).canBeSaved(name.getOriginal())
	//	.addTooltip("Use horn button to boost horse"));

	addOption(buttonOption("Clone")
		.addTranslation()
		.addOnClick([] { native::clonePed(menu::getLocalPlayer().mount, native::getEntityHeading(menu::getLocalPlayer().ped), TRUE, FALSE); })
		.addTooltip("More than one mount ????"));

	addOption(buttonOption("Clean")
		.addTranslation()
		.addOnClick([] { native::clearPedBloodDamage(menu::getLocalPlayer().mount); })
		.addTooltip("Wash the dirt off your mount"));

	addOption(buttonOption("Euthanize")
		.addTranslation()
		.addOnClick([] { native::applyDamageToPed(menu::getLocalPlayer().mount, 30000, true, 0, 0); })
		.addTooltip("Put her down..."));

	addOption(numberOption<float>(SCROLL, "Mount Scale")
		.addTranslation()
		.addNumber(vars.scale, "%.2f", 0.01f).addMin(0.01f).addMax(20.f).setScrollSpeed(10)
		.addOnClick([] { native::setPedScale(menu::getLocalPlayer().mount, vars.scale); }));
}

/*Called while in submenu*/
void mountMenu::update() {}

/*Called once on submenu open*/
void mountMenu::updateOnce() {}

/*Called always*/
void mountMenu::featureUpdate() {
	if (menu::getLocalPlayer().isOnMount) {
		if (vars.invincibility) {
			setMountInvincible(TRUE);
		}

		if (vars.fastRegenHealth) {
			native::setEntityHealth(menu::getLocalPlayer().mount, 100, 1);
			native::setPedCoreLevel(menu::getLocalPlayer().mount, 0, 100);

		}

		if (vars.unlimitedStamina) {
			if (native::isPedOnMount(menu::getLocalPlayer().ped)) {
				native::restorePedStamina(menu::getLocalPlayer().mount, 100.f);
				native::setPedCoreLevel(menu::getLocalPlayer().mount, 1,100);
			}
		}

		if (vars.hornBoost) {
			float move = native::getControlNormal(0, INPUT_MOVE_UD);
			if (move && move < .0f) {
				if (native::isPedOnMount(menu::getLocalPlayer().ped)) {
					native::applyForceToEntityCenterOfMass(menu::getLocalPlayer().mount, 1, 0.f, 3.f, 0.f, FALSE, TRUE, TRUE, TRUE);
				}
			}
		}

		if (vars.superGallop) {
			if (!menu::getBase()->open) {
				static bool stop = false;
				if (!menu::getLocalPlayer().inVehicle) {
					if (native::isDisabledControlPressed(0, INPUT_FRONTEND_ACCEPT) || menu::util::getInput()->isPressed(true, VK_SHIFT)) {
						if (native::doesEntityExist(menu::getLocalPlayer().mount)) {
							native::applyForceToEntityCenterOfMass(menu::getLocalPlayer().mount, 1, 0.f, (float)vars.superGallopSpeed, 0.f, FALSE, TRUE, TRUE, TRUE);
							stop = true;
						}
					} else if (stop) {
						native::setEntityCoords(menu::getLocalPlayer().mount, native::getEntityCoords(menu::getLocalPlayer().mount));
						native::taskStandStill(menu::getLocalPlayer().mount, 1);
						stop = false;
					}
				}
			}
		}
	}
}

mountMenu* _instance;
mountMenu* mountMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new mountMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

mountMenu::~mountMenu() { delete _instance; }