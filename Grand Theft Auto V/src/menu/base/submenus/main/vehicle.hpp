#pragma once
#include "menu/base/submenu.hpp"

class VehicleMenu : public Menu::Submenu {
public:
	static VehicleMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleMenu()
		: Menu::Submenu() {}

	~VehicleMenu();
};

namespace VehicleMenuVars {
	struct Vars_ {};

	bool CanRunLosSantosCustoms();
	bool CanRunBennys();
	extern Vars_ m_Vars;
}