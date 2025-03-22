#pragma once
#include "menu/base/submenu.hpp"

class WorldOceanTuningMenu : public Menu::Submenu {
public:
	static WorldOceanTuningMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldOceanTuningMenu()
		: Menu::Submenu() {}

	~WorldOceanTuningMenu();
};

namespace WorldOceanTuningMenuVars {
	struct Vars_ {
		Rage::Types::WaterTune m_WaterTune;
	};

	extern Vars_ m_Vars;
}