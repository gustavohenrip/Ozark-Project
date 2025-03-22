#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/generic_types.hpp"

class WorldAdvancedUILiquidMenu : public Menu::Submenu {
public:
	static WorldAdvancedUILiquidMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUILiquidMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUILiquidMenu();
};

namespace WorldAdvancedUILiquidMenuVars {
	enum eLiquid {
		LIQUID_PUDDLE,
		LIQUID_BLOOD,
		LIQUID_OIL,
		LIQUID_PETROL,
		LIQUID_MUD
	};

	struct Vars_ {
		int m_Selected;
		Rage::Types::VfxLiquid m_Backup[5];
	};

	extern Vars_ m_Vars;
}