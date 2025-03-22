#pragma once
#include "menu/base/submenu.hpp"

class WorldAdvancedUIMenu : public Menu::Submenu {
public:
	static WorldAdvancedUIMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUIMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUIMenu();
};

namespace WorldAdvancedUIMenuVars {
	

	struct Vars_ {

	};

	extern Vars_ m_Vars;
}