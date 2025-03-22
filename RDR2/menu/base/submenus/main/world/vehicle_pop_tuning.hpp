#pragma once
#include "menu/base/submenu.hpp"

class vehiclePopulationTuningMenu : public menu::submenu {
public:
	static vehiclePopulationTuningMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehiclePopulationTuningMenu()
		: menu::submenu()
	{}

	~vehiclePopulationTuningMenu();
};

namespace vehiclePopulationTuningMenuVars {
	struct vars_ {

	};

	extern vars_ vars;
}