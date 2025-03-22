#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class ObjectSpawnerEditMenu : public Menu::Submenu {
public:
	static ObjectSpawnerEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectSpawnerEditMenu()
		: Menu::Submenu() {}

	~ObjectSpawnerEditMenu();
};

namespace ObjectSpawnerEditMenuVars {
	struct Vars_ {
		bool m_EditingAll;
		std::size_t m_SelectedObject;
		int m_Teleport;

		ESPMenuVars::ESP m_ESP;
	};

	void Run(int Index, bool EditingAll, bool EditingLocalPool, int* Var, Entity Ent);
	extern Vars_ m_Vars;
}