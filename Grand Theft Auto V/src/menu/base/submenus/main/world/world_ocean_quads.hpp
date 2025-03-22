#pragma once
#include "menu/base/submenu.hpp"

class WorldOceanQuadsMenu : public Menu::Submenu {
public:
	static WorldOceanQuadsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldOceanQuadsMenu()
		: Menu::Submenu() {}

	~WorldOceanQuadsMenu();
};

namespace WorldOceanQuadsMenuVars {
	struct Vars_ {
		int m_Type;
		uint64_t m_ClosestQuad;
	};

	void QuadESP();
	extern Vars_ m_Vars;
}