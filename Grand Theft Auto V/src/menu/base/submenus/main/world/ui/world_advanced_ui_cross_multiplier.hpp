#pragma once
#include "menu/base/submenu.hpp"

class WorldAdvancedUICrossMultiplierMenu : public Menu::Submenu {
public:
	static WorldAdvancedUICrossMultiplierMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUICrossMultiplierMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUICrossMultiplierMenu();
};

namespace WorldAdvancedUICrossMultiplierMenuVars {
	struct Vars_ {
		float m_RimLight;
		float m_GlobalEnvironmentReflection;
		float m_Gamma;
		float m_MidBlur;
		float m_FarBlur;
		float m_SkyMultiplier;
		float m_Desaturation;
	};

	extern Vars_ m_Vars;
}