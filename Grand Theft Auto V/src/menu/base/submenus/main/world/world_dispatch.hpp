#pragma once
#include "menu/base/submenu.hpp"

class WorldDispatchMenu : public Menu::Submenu {
public:
	static WorldDispatchMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldDispatchMenu()
		: Menu::Submenu() {}

	~WorldDispatchMenu();
};

namespace WorldDispatchMenuVars {
	struct Vars_ {
		bool m_DisablePoliceAutomobile;
		bool m_DisablePoliceHelicopter;
		bool m_DisablePoliceRiders;
		bool m_DisablePoliceVehicleRequest;
		bool m_DisablePoliceRoadBlock;
		bool m_DisablePoliceBoat;
		bool m_DisableSwatAutomobile;
		bool m_DisableSwatHelicopter;
		bool m_DisableFireDepartment;
		bool m_DisableAmbulanceDepartment;
		bool m_DisableGangs;
		bool m_DisableArmyVehicle;
		bool m_DisableBikers;
	};

	extern Vars_ m_Vars;
}