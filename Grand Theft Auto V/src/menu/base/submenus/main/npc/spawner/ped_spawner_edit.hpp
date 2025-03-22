#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class PedSpawnerEditMenu : public Menu::Submenu {
public:
	static PedSpawnerEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PedSpawnerEditMenu()
		: Menu::Submenu() {}

	~PedSpawnerEditMenu();
};

namespace PedSpawnerEditMenuVars {
	struct Vars_ {
		bool m_EditingAll;
		int m_Teleport;
		std::size_t m_SelectedPed;
		ESPMenuVars::ESP m_ESP;
	};

	void Run(int Index, bool EditingAll, bool EditingLocalPool, int* Var, Entity Ent);
	extern Vars_ m_Vars;
}