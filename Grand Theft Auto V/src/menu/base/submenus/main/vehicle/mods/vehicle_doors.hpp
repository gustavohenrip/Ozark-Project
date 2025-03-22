#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsDoorMenu : public Menu::Submenu {
public:
	static VehicleModsDoorMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsDoorMenu()
		: Menu::Submenu() {}

	~VehicleModsDoorMenu();
};

namespace VehicleModsDoorMenuVars {
	struct Vars_ {
		int m_OpenDoor;
		int m_CloseDoor;
		int m_DeleteDoor;
	};

	extern Vars_ m_Vars;
}