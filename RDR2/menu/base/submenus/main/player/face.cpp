#include "face.hpp"
#include "menu/base/submenu_handler.hpp"
#include "appearance.hpp"

using namespace faceMenuVars;

namespace faceMenuVars {
	vars_ vars;

	scrollStruct<int> ids[39] = {
		{ "Head Width", 0x84D6 },
		{ "Eyebrow Height", 0x3303 },
		{ "Eyebrow Width", 0x2FF9 },
		{ "Eyebrow Forward", 0x4AD1 },
		{ "Ear Length", 0xC04F },
		{ "Ear Angle", 0xB6CE },
		{ "Ear Width", 0x2844 },
		{ "Ear Dangle", 0xED30 },
		{ "Cheekbone Height", 0x6A0B },
		{ "Cheekbone Width", 0xABCF },
		{ "Cheekbone Forward", 0x358D },
		{ "Jaw Sharpness", 0x8D0A },
		{ "Jaw Width", 0xEBAE },
		{ "Jaw Forward", 0x1DF6 },
		{ "Jaw Up", 0x3C0F },
		{ "Chin Width", 0xC3B2 },
		{ "Chin Forward", 0xE323 },
		{ "Eyelid Size", 0x8B2B },
		{ "Eye Width", 0x1B6B },
		{ "Eye Forward", 0xEE44 },
		{ "Eye Rotation", 0xD266 },
		{ "Eye Out", 0xA54E },
		{ "Eye Up", 0xDDFB },
		{ "Nose Width", 0x6E7F },
		{ "Nose Length", 0x3471 },
		{ "Nose Down", 0x03F5 },
		{ "Nose Up", 0x34B1 },
		{ "Nose Curvature", 0xF156 },
		{ "Nostril Size", 0x561E },
		{ "Mouth Width", 0xF065 },
		{ "Mouth Forward", 0xAA69 },
		{ "Mouth Angle", 0x7AC3 },
		{ "Mouth Up", 0x410D },
		{ "Upper Lip Height", 0x1A00 },
		{ "Upper Lip Width", 0x91C1 },
		{ "Upper Lip Forward", 0xC375 },
		{ "Lower Lip Down", 0xBB4D },
		{ "Lower Lip Width", 0xB0B0 },
		{ "Lower Lip Forward", 0x5D16 },
	};

	void setFeature(int id, float value) {
		native::setPedFaceFeature(menu::getLocalPlayer().ped, id, value); 
		native::setPedComponentEnabled(menu::getLocalPlayer().ped, 0x158CB7F2, true, true, true);
	}
}

void faceMenu::init() {
	setName("Face");
	setParentSubmenu<appearanceMenu>();

	addOption(scrollOption<int>(SCROLL, "Feature")
		.addTranslation()
		.addScroll(vars.id, 0, NUMOF(ids), ids)
		.addTooltip("Select facial feature index to edit"));

	addOption(numberOption<float>(SCROLL, "Value")
		.addTranslation()
		.addNumber(vars.value, "%.01f", .1f).addMin(-50.f).addMax(50.f).setScrollSpeed(2)
		.addOnUpdate([] (numberOption<float>* option) { vars.value = native::getPedFaceFeature(menu::getLocalPlayer().ped, ids[vars.id].result); })
		.addOnClick([] { setFeature(ids[vars.id].result, vars.value); })
		.addTooltip("Edit the facial feature value"));
}

/*Called while in submenu*/
void faceMenu::update() {}

/*Called once on submenu open*/
void faceMenu::updateOnce() {}

/*Called always*/
void faceMenu::featureUpdate() {}

faceMenu* _instance;
faceMenu* faceMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new faceMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

faceMenu::~faceMenu() { delete _instance; }