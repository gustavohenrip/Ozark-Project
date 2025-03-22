#pragma once
#include "menu/base/submenu.hpp"

class VehicleSpawnerSelectMenu : public Menu::Submenu {
public:
	static VehicleSpawnerSelectMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerSelectMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerSelectMenu();
};

namespace VehicleSpawnerSelectMenuVars {
	struct Vars_ {
		int m_Selected;
		std::unordered_map<uint32_t, std::string> m_Textures;
	};

	extern Vars_ m_Vars;
}