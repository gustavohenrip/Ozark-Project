#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/base_types.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class VehicleSpawnerEditMenu : public Menu::Submenu {
public:
	static VehicleSpawnerEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerEditMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerEditMenu();
};

namespace VehicleSpawnerEditMenuVars {
	struct Vars_ {
		bool m_EditingAll = false;
		std::size_t m_SelectedVehicle;

		int m_Driver;
		int m_Boost;
		int m_Acrobatics;
		int m_Gravity;
		int m_Teleport;

		ESPMenuVars::ESP m_ESP;
	};

	extern Vars_ m_Vars;
	void Run(int Index, bool EditingAllManagerEntities = m_Vars.m_EditingAll, bool EditingLocalPool = false, int* Var = nullptr, Entity Ent = 0);
}