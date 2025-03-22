#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsCollisionMenu : public Menu::Submenu {
public:
	static VehicleModsCollisionMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsCollisionMenu()
		: Menu::Submenu() {}

	~VehicleModsCollisionMenu();
};

namespace VehicleModsCollisionMenuVars {
	struct Vars_ {
		bool m_DisableFullCollision;
		bool m_DisableVehicleCollision;
	};

	extern Vars_ m_Vars;
}