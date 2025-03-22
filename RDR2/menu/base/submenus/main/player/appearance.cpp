#include "appearance.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../player.hpp"
#include "model.hpp"
#include "face.hpp"
#include "wardrobe.hpp"
#include "menu/base/util/presence.hpp"

using namespace appearanceMenuVars;

namespace appearanceMenuVars {
	vars_ vars;

	scrollStruct<const char*> gender[] = {
	{ "Male", "mp_male" },
	{ "Female", "mp_female" },
	};

}

void appearanceMenu::init() {
	setName("Appearance");
	setParentSubmenu<playerMenu>();

	addOption(submenuOption("Model")
		.addTranslation()
		.addSubmenu<modelMenu>()
		.addTooltip("Change your current model"));

	addOption(submenuOption("Wardrobe")
		.addTranslation()
		.addSubmenu<wardrobeMenu>()
		.addTooltip("Edit your current model"));

	addOption(submenuOption("Facial Features")
		.addTranslation()
		.addSubmenu<faceMenu>()
		.addTooltip("Change your current models facial features"));

	//addOption(scrollOption<const char*>(SCROLLSELECT, "Gender")
	//	.addScroll(vars.gender, 0, NUMOF(gender) , gender)
	//	.addOnClick([] { modelMenuVars::changeModel(native::getHashKey(gender[vars.gender].result)); })
	//	.addTooltip("Transgender surgery..."));
	//mp_female
	addOption(numberOption<float>(SCROLL, "Player Scale")
		.addTranslation()
		.addNumber(vars.scale, "%.2f", 0.01f).addMin(0.01f).addMax(20.f).setScrollSpeed(10)
		.addOnClick([] { native::setPedScale(menu::getLocalPlayer().ped, vars.scale); })
		.addTooltip("Make big or smol"));

	addOption(buttonOption("Synchronize Scale")
		.addTranslation()
		.addOnClick([] {
			menu::getPresence()->refreshSession([] {
				native::setPedScale(menu::getLocalPlayer().ped, vars.scale);
			});
		})
		.addTooltip("Will refresh your session in order to synchronize your players scale."));
}

/*Called while in submenu*/
void appearanceMenu::update() {}

/*Called once on submenu open*/
void appearanceMenu::updateOnce() {}

/*Called always*/
void appearanceMenu::featureUpdate() {}

appearanceMenu* _instance;
appearanceMenu* appearanceMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new appearanceMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

appearanceMenu::~appearanceMenu() { delete _instance; }