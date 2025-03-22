#pragma once
#include "menu/base/submenu.hpp"

class PedSpawnerMenu : public Menu::Submenu {
public:
	static PedSpawnerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PedSpawnerMenu()
		: Menu::Submenu() {}

	~PedSpawnerMenu();
};

namespace PedSpawnerMenuVars {
	struct Vars_ {
		int m_Popular;
		int m_Story;
		int m_Animals;
		int m_Emergency;
		int m_Role;
		int m_Ambient;
		int m_Misc;
	};

	extern Vars_ m_Vars;
}