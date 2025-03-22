#pragma once
#include "menu/base/submenu.hpp"

class WorldOceanQuadsEditMenu : public Menu::Submenu {
public:
	static WorldOceanQuadsEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldOceanQuadsEditMenu()
		: Menu::Submenu() {}

	~WorldOceanQuadsEditMenu();
};

namespace WorldOceanQuadsEditMenuVars {
	struct Vars_ {
		Rage::Types::OceanCalmingQuad* m_Calming;
		Rage::Types::OceanWaveQuad* m_Wave;
		Rage::Types::OceanQuad* m_Ocean;

		bool m_Clear;
	};

	extern Vars_ m_Vars;
}