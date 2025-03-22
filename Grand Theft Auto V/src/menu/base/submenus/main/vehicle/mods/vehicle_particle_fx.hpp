#pragma once
#include "menu/base/submenu.hpp"

class VehicleModsParticleFXMenu : public Menu::Submenu {
public:
	static VehicleModsParticleFXMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleModsParticleFXMenu()
		: Menu::Submenu() {}

	~VehicleModsParticleFXMenu();
};

namespace VehicleModsParticleFXMenuVars {
	struct Vars_ {
		bool m_Toggle;
		bool m_Bones[10];
		int m_Particle;
		float m_Scale = 0.2f;
	};

	extern Vars_ m_Vars;
}