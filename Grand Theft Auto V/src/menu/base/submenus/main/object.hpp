#pragma once
#include "menu/base/submenu.hpp"

class ObjectMenu : public Menu::Submenu {
public:
	static ObjectMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectMenu()
		: Menu::Submenu() {}

	~ObjectMenu();
};

namespace ObjectMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}