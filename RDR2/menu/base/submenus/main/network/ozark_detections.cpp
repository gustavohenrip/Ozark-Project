#include "ozark_detections.hpp"
#include "menu/base/submenu_handler.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/notify.hpp"
#include "../settings.hpp"

using namespace networkOzarkDetectsMenuVars;

namespace networkOzarkDetectsMenuVars {
	vars_ vars;
}

void networkOzarkDetectsMenu::init() {
	setName("Ozark Detections");
	setParentSubmenu<settingsMenu>();

	addString("~e~Error~q~: You cannot enable passive unless you're broadcasting!");

	addOption(toggleOption("Broadcast Detection")
		.addTranslation()
		.addToggle(vars.detection).canBeSaved(name.getOriginal())
		.addTooltip("You will not show up as an ozark user to other users if disabled."));

	addOption(toggleOption("Passive")
		.addTranslation()
		.addToggle(vars.disableOzarkAccess).canBeSaved(name.getOriginal())
		.addTooltip("Ozark users will not be able to do anything but teleport to you if enabled."));
}

/*Called while in submenu*/
void networkOzarkDetectsMenu::update() {}

/*Called once on submenu open*/
void networkOzarkDetectsMenu::updateOnce() {}

/*Called always*/
void networkOzarkDetectsMenu::featureUpdate() {
	static int update = 0;
	menu::util::getTimers()->runTimedFunction(&update, 1000, [this] {
		if (!native::decorIsRegisteredAsType(vars.ozarkDetects.c_str(), 3)) {
			native::decorRegister(vars.ozarkDetects.c_str(), 3);
		} else {
			if (vars.detection) {
				vars.ozarkBit |= OZARK_USER;
#ifdef DEV
				vars.ozarkBit |= OZARK_USER_DEVELOPER;
#endif

				if (vars.disableOzarkAccess) {
					vars.ozarkBit |= OZARK_DONT_HURT_ME;
				} else {
					vars.ozarkBit &= ~OZARK_DONT_HURT_ME;
				}
			} else {
				vars.ozarkBit &= ~OZARK_USER;
				vars.ozarkBit &= ~OZARK_DONT_HURT_ME;

				if (vars.disableOzarkAccess) {
					vars.disableOzarkAccess = false;
					menu::getNotify()->right(getString("~e~Error~q~: You cannot enable passive unless you're broadcasting!"_Protect));
				}
			}

			native::decorSetInt(menu::getLocalPlayer().ped, vars.ozarkDetects.c_str(), vars.ozarkBit);
		}
	});
}

networkOzarkDetectsMenu* _instance;
networkOzarkDetectsMenu* networkOzarkDetectsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkOzarkDetectsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkOzarkDetectsMenu::~networkOzarkDetectsMenu() { delete _instance; }