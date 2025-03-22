#include "directional.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"

using namespace directionalMenuVars;

namespace directionalMenuVars {
	vars_ vars;
	
	void slideToCoord(math::vector3<float> offset) {
		math::vector3<float> coords = native::getOffsetFromEntityInWorldCoords(menu::getLocalPlayer().entity, offset);
		native::setEntityCoordsNoOffset(menu::getLocalPlayer().entity, coords.x, coords.y, coords.z, 0, 0, 0);
	}
}

void directionalMenu::init() {
	setName("Directional");
	setParentSubmenu<teleportMenu>();

	addOption(numberOption<float>(SCROLLSELECT, "Up")
		.addTranslation()
		.addNumber(vars.up, "%.0fm", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { slideToCoord({ 0.f, 0.f, vars.up }); }));

	addOption(numberOption<float>(SCROLLSELECT, "Down")
		.addTranslation()
		.addNumber(vars.down, "%.0fm", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { slideToCoord({ 0.f, 0.f, vars.down * -1.f }); }));

	addOption(numberOption<float>(SCROLLSELECT, "Left")
		.addTranslation()
		.addNumber(vars.left, "%.0fm", 1.f).addMin(0.f).addMax(100.f)
		.addOnClick([] { slideToCoord({ vars.left * -1.f, 0.f, 0.f }); }));

	addOption(numberOption<float>(SCROLLSELECT, "Right")
		.addTranslation()
		.addNumber(vars.right, "%.0fm", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { slideToCoord({ vars.right, 0.f, 0.f }); }));

	addOption(numberOption<float>(SCROLLSELECT, "Forward")
		.addTranslation()
		.addNumber(vars.forward, "%.0fm", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { slideToCoord({ 0.f, vars.forward, 0.f }); }));

	addOption(numberOption<float>(SCROLLSELECT, "Back")
		.addTranslation()
		.addNumber(vars.back, "%.0fm", 1.f).addMin(1.f).addMax(100.f)
		.addOnClick([] { slideToCoord({ 0.f, vars.back * -1.f, 0.f }); }));
}

/*Called while in submenu*/
void directionalMenu::update() {}

/*Called once on submenu open*/
void directionalMenu::updateOnce() {}

/*Called always*/
void directionalMenu::featureUpdate() {}

directionalMenu* _instance;
directionalMenu* directionalMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new directionalMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

directionalMenu::~directionalMenu() { delete _instance; }