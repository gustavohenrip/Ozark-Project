#include "vehicle_pop_tuning.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"

using namespace vehiclePopulationTuningMenuVars;

namespace vehiclePopulationTuningMenuVars {
	vars_ vars;
}

void vehiclePopulationTuningMenu::init() {
	setName("Vehicle Population Tuning");
	setParentSubmenu<worldMenu>();

	addString("Tuning options aren't available :(");
	addString("Multiplayer Random Vehicle Density Multiplier");
	addString("Multiplayer Parked Vehicle Density Multiplier");
	addString("Max Vehicle Spacing Above Furthest");
	addString("Divisor for Vehicle Spacing Past Furthest");
	addString("Min Random Density Variance Multiplier");
	addString("Max Random Density Variance Multiplier");
	addString("Reject All Vehicles Moving Away");
	addString("Also Reject If Stationary");
	addString("Multiplayer Disable Vehicles");
}

/*Called while in submenu*/
void vehiclePopulationTuningMenu::update() {}

/*Called once on submenu open*/
void vehiclePopulationTuningMenu::updateOnce() {
	clearOptionsOffset(0);

	if (global::vars::vehiclePopTuning) {
		addOption(numberOption<float>(SCROLL, getString("Multiplayer Random Vehicle Density Multiplier"))
			.addNumber(global::vars::vehiclePopTuning->multiplayerRandomVehicleDensityMultiplier, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Multiplayer Parked Vehicle Density Multiplier"))
			.addNumber(global::vars::vehiclePopTuning->multiplayerParkedVehicleDensityMultiplier, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Max Vehicle Spacing Above Furthest"))
			.addNumber(global::vars::vehiclePopTuning->maxVehicleSpacingAboveFurthestDefaultValue, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Divisor for Vehicle Spacing Past Furthest"))
			.addNumber(global::vars::vehiclePopTuning->divisorForVehicleSpacingPastFurthestDefaultValue, "%.0f", 1.f).addMin(0.f).addMax(1000.f).setScrollSpeed(10));

		addOption(numberOption<float>(SCROLL, getString("Min Random Density Variance Multiplier"))
			.addNumber(global::vars::vehiclePopTuning->minRandomDensityVarianceMultiplier, "%.1f", 0.1f).addMin(0.f).addMax(1000.f).setScrollSpeed(5));

		addOption(numberOption<float>(SCROLL, getString("Max Random Density Variance Multiplier"))
			.addNumber(global::vars::vehiclePopTuning->maxRandomDensityVarianceMultiplier, "%.1f", 0.1f).addMin(0.f).addMax(1000.f).setScrollSpeed(5));

		addOption(toggleOption(getString("Reject All Vehicles Moving Away"))
			.addToggle(global::vars::vehiclePopTuning->rejectAllVehiclesMovingAway));

		addOption(toggleOption(getString("Also Reject If Stationary"))
			.addToggle(global::vars::vehiclePopTuning->alsoRejectIfStationary));

		addOption(toggleOption(getString("Multiplayer Disable Vehicles"))
			.addToggle(global::vars::vehiclePopTuning->disableVehiclesInMP));
	} else {
		addOption(buttonOption(getString("Tuning options aren't available :(")));
	}
}

/*Called always*/
void vehiclePopulationTuningMenu::featureUpdate() {}

vehiclePopulationTuningMenu* _instance;
vehiclePopulationTuningMenu* vehiclePopulationTuningMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehiclePopulationTuningMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehiclePopulationTuningMenu::~vehiclePopulationTuningMenu() { delete _instance; }