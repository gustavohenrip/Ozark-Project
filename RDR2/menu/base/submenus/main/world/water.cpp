#include "water.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"

using namespace waterMenuVars;

namespace waterMenuVars {
	vars_ vars;
}

void waterMenu::init() {
	setName("Water");
	setParentSubmenu<worldMenu>();

	addString("Water options aren't available :(");
	addString("Ripple Speed");
	addString("Fog Pierce Intensity");
	addString("Water Cycle Depth");
	addString("Water Cycle Fade");
	addString("Water Lightning Depth");
	addString("Water Lightning Fade");
	addString("Deep Water Mod Depth");
	addString("Deep Water Mod Fade");
	addString("God Rays Lerp Start");
	addString("God Rays Lerp End");
	addString("Disturb Foam Scale");
	addString("Fog Min X");
	addString("Fog Min Y");
	addString("Fog Max X");
	addString("Fog Max Y");
	addString("Rain Fade Min");
	addString("Rain Fade Max");
	addString("Ocean Foam Depth Fade Range");
	addString("Water Clip (Underwater)");
	addString("Water Clip View Distance");
	addString("Water Clip Fog Brightness");
	addString("Water Clip Camera Caustic Strength");
	addString("Water Clip Caustic Strength");
}

/*Called while in submenu*/
void waterMenu::update() {}

/*Called once on submenu open*/
void waterMenu::updateOnce() {
	if (global::vars::wtune) {
		addOption(numberOption<float>(SCROLL, getString("Ripple Speed"))
			.addNumber(global::vars::wtune->rippleSpeed, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Fog Pierce Intensity"))
			.addNumber(global::vars::wtune->fogPierceIntensity, "%.2f", .05f).addMin(0.f).addMax(1000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Water Cycle Depth"))
			.addNumber(global::vars::wtune->waterCycleDepth, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Water Cycle Fade"))
			.addNumber(global::vars::wtune->waterCycleFade, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Water Lightning Depth"))
			.addNumber(global::vars::wtune->waterLightningDepth, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Water Lightning Fade"))
			.addNumber(global::vars::wtune->waterLightningFade, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Deep Water Mod Depth"))
			.addNumber(global::vars::wtune->deepWaterModDepth, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Deep Water Mod Fade"))
			.addNumber(global::vars::wtune->deepWaterModFade, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("God Rays Lerp Start"))
			.addNumber(global::vars::wtune->godRaysLerpStart, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("God Rays Lerp End"))
			.addNumber(global::vars::wtune->godRaysLerpEnd, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Disturb Foam Scale"))
			.addNumber(global::vars::wtune->disturbFoamScale, "%.1f", 0.1f).addMin(0.f).addMax(1000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Fog Min X"))
			.addNumber(global::vars::wtune->fogMin[0], "%.0f", 1.f).addMin(-100000.f).addMax(100000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Fog Min Y"))
			.addNumber(global::vars::wtune->fogMin[1], "%.0f", 1.f).addMin(-100000.f).addMax(100000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Fog Max X"))
			.addNumber(global::vars::wtune->fogMax[0], "%.0f", 1.f).addMin(-100000.f).addMax(100000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Fog Max Y"))
			.addNumber(global::vars::wtune->fogMax[1], "%.0f", 1.f).addMin(-100000.f).addMax(100000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Rain Fade Min"))
			.addNumber(global::vars::wtune->rainFadeMin, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Rain Fade Max"))
			.addNumber(global::vars::wtune->rainFadeMax, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Ocean Foam Depth Fade Range"))
			.addNumber(global::vars::wtune->oceanFoamDepthFadeRange, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(breakOption(getString("Water Clip (Underwater)")));

		addOption(numberOption<float>(SCROLL, getString("Water Clip View Distance"))
			.addNumber(global::vars::wtune->waterClipViewDistance, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Water Clip Fog Brightness"))
			.addNumber(global::vars::wtune->waterClipFogBrightness, "%.1f", 0.1f).addMin(0.f).addMax(1000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Water Clip Camera Caustic Strength"))
			.addNumber(global::vars::wtune->waterClipCameraCausticStrength, "%.1f", 0.1f).addMin(0.f).addMax(1000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Water Clip Caustic Strength"))
			.addNumber(global::vars::wtune->waterClipCausticStrength, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));
	} else {
		addOption(buttonOption(getString("Water options aren't available :(")));
	}
}

/*Called always*/
void waterMenu::featureUpdate() {}

waterMenu* _instance;
waterMenu* waterMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new waterMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

waterMenu::~waterMenu() { delete _instance; }