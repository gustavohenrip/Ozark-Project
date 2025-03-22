#include "time.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/player_manager.hpp"

using namespace timeMenuVars;

namespace timeMenuVars {
	vars_ vars;

	void setTime(math::vector3<int> time, bool networked) {
		if (native::networkIsInSession()) {
			native::networkClockTimeOverride(time.x, time.y, time.z, 0, 1);
			if (networked) {
				rage::engine::triggerClockEvent(2, 0);
			}
		} else {
			native::setClockTime(time.x, time.y, time.z);
		}
	}
}

void timeMenu::init() {
	setName("Time");
	setParentSubmenu<worldMenu>();

	addOption(toggleOption("Network Time")
		.addTranslation()
		.addToggle(vars.networkTime));

	addOption(numberOption<int>(SCROLL, "Time (Hour)")
		.addTranslation()
	.addNumber(vars.hours, "%i", 1).addMin(0).addMax(23).canLoop()
		.addOnClick([] { setTime({ vars.hours, 0, 0 }, vars.networkTime); }));

	addOption(numberOption<float>(SCROLL, "Time Scale")
		.addTranslation()
		.addNumber(vars.scale, "%.01f", 0.1f).addMin(0.f).addMax(1.1f).canLoop()
		.addOnClick([] { native::setTimeScale(vars.scale); })
		.addRequirement([] { return !vars.networkTime; }));

	addOption(toggleOption("Sync with Network")
		.addTranslation()
		.addToggle(vars.syncWithNetworkTime));

	addOption(toggleOption("Sync with System")
		.addTranslation()
		.addToggle(vars.syncWithSystemTime));

	addOption(toggleOption("Always Daytime")
		.addTranslation()
		.addToggle(vars.alwaysDay));

	addOption(toggleOption("Always Night")
		.addTranslation()
		.addToggle(vars.alwaysNight));
    
	addOption(breakOption("Presets")
		.addTranslation());

	addOption(buttonOption("Morning")
		.addTranslation()
		.addOnClick([] { setTime({ 7, 0, 0 }, vars.networkTime); }));

	addOption(buttonOption("Afternoon")
		.addTranslation()
		.addOnClick([] { setTime({ 12, 0, 0 }, vars.networkTime); }));

	addOption(buttonOption("Evening")
		.addTranslation()
		.addOnClick([] { setTime({ 18, 0, 0 }, vars.networkTime); }));

	addOption(buttonOption("Night")
		.addTranslation()
		.addOnClick([] { setTime({ 21, 0, 0 }, vars.networkTime); }));
}

/*Called while in submenu*/
void timeMenu::update() {}

/*Called once on submenu open*/
void timeMenu::updateOnce() {}

/*Called always*/
void timeMenu::featureUpdate() {
	if (vars.alwaysDay) {
		setTime({ 13, 0, 0 }, vars.networkTime);
	}

	if (vars.alwaysNight) {
		setTime({ 21, 0, 0 }, vars.networkTime);
	}

	if (vars.syncWithNetworkTime) {
		math::vector3<int> time;
		native::networkGetClockTimeOverride(&time.x, &time.y, &time.z);
		native::networkClockTimeOverride(time.x, time.y, time.z, 0, 1);
	}

	if (vars.syncWithSystemTime) {
		int day, month, year, hour, min, sec;
		native::getLocaltime(&year, &month, &day, &hour, &min, &sec);
		setTime({ hour, min, sec }, vars.networkTime);
	}
}

timeMenu* _instance;
timeMenu* timeMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new timeMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

timeMenu::~timeMenu() { delete _instance; }