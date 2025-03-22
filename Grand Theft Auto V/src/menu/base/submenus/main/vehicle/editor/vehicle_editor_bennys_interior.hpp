#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorBennysInteriorMenu : public Menu::Submenu {
public:
	static VehicleEditorBennysInteriorMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorBennysInteriorMenu()
		: Menu::Submenu() {}

	~VehicleEditorBennysInteriorMenu();
};

namespace VehicleEditorBennysInteriorMenuVars {
	struct Vars_ {
		bool m_UpdateCache;
	};

	extern Vars_ m_Vars;
}