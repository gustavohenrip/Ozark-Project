#pragma once
#include "menu/base/submenu.hpp"

class ScenarioMenu : public Menu::Submenu {
public:
	static ScenarioMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ScenarioMenu()
		: Menu::Submenu() {}

	~ScenarioMenu();
};

namespace ScenarioMenuVars {
	struct Vars_ {
		int m_Ambient;
		int m_Activities;
		int m_Animals;
		int m_Fitness;
		int m_Industrial;
	};

	extern Vars_ m_Vars;
}