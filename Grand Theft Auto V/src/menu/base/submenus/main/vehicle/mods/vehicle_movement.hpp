#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsMovementMenu : public Menu::Submenu {
public:
	static VehicleModsMovementMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsMovementMenu()
		: Menu::Submenu() {}

	~VehicleModsMovementMenu();
};

namespace VehicleModsMovementMenuVars {
	struct Vars_ {
		bool m_FlyingVehicle;
		bool m_DisableMaxSpeed;
		bool m_LimitMaxSpeed;

		float m_FlyingVehicleSpeed = 1.f;
		float m_LimitMaxSpeedVal;

		Vehicle m_Container;
	};

	void RunAcrobatic(Vehicle Veh, std::pair<Math::Vector3_<float>, Math::Vector3_<float>> Data);
	extern Vars_ m_Vars;
}