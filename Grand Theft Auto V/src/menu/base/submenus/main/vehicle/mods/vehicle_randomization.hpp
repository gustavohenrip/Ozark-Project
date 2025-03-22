#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/rainbow.hpp"

class VehicleModsRandomizationMenu : public Menu::Submenu {
public:
	static VehicleModsRandomizationMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsRandomizationMenu()
		: Menu::Submenu() {}

	~VehicleModsRandomizationMenu();
};

namespace VehicleModsRandomizationMenuVars {
	struct Vars_ {
		bool m_RainbowPrimary;
		bool m_RainbowSecondary;
		bool m_RainbowTireSmoke;
		bool m_RandomUpgrades;
		bool m_RandomAcrobatics;

		Menu::Rainbow::Rainbow m_RainbowPrimaryRun = { 0, 255, 255 };
		Color m_RainbowPrimaryColor;

		Menu::Rainbow::Rainbow m_RainbowSecondaryRun = { 0, 255, 255 };
		Color m_RainbowSecondaryColor;

		Menu::Rainbow::Rainbow m_RainbowTireSmokeRun = { 0, 255, 255 };
		Color m_RainbowTireSmokeColor;
	};

	extern Vars_ m_Vars;
}
