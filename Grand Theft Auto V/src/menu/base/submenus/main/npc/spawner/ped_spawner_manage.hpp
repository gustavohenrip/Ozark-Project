#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class PedSpawnerManageMenu : public Menu::Submenu {
public:
	static PedSpawnerManageMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PedSpawnerManageMenu()
		: Menu::Submenu() {}

	~PedSpawnerManageMenu();
};

namespace PedSpawnerManageMenuVars {
	struct SpawnedPedContext {
		Ped m_Handle;
		uint32_t m_ModelHash = 0;
		std::string m_Name;
		int m_CreationTime = 0;
		ESPMenuVars::ESP m_ESP;
	};

	struct Vars_ {
		std::vector<SpawnedPedContext> m_SpawnedPeds;
	};

	extern Vars_ m_Vars;
}