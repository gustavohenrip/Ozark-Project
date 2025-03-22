#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/network_types.hpp"

class WorldAdvancedUIPuddlesMenu : public Menu::Submenu {
public:
	static WorldAdvancedUIPuddlesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUIPuddlesMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUIPuddlesMenu();
};

namespace WorldAdvancedUIPuddlesMenuVars {
	struct Vars_ {
		Rage::Types::UIPuddle m_Original;
		Rage::Types::UIPuddleRipple m_OriginalRipple;
	};

	extern Vars_ m_Vars;
}