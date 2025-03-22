#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/generic_types.hpp"
#include "menu/base/util/rainbow.hpp"

class VehicleModsTyreTracksMenu : public Menu::Submenu {
public:
	static VehicleModsTyreTracksMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsTyreTracksMenu()
		: Menu::Submenu() {}

	~VehicleModsTyreTracksMenu();
};

namespace VehicleModsTyreTracksMenuVars {
	struct Vars_ {
		bool m_Persistent;
		Rage::Types::VfxWheel* m_Cache;
		Color m_Color = Color(255, 0, 0, 255);
		bool m_RainbowToggle;
		bool m_SaveToConfig;
		Menu::Rainbow::Rainbow m_Rainbow;
	};

	extern Vars_ m_Vars;
}