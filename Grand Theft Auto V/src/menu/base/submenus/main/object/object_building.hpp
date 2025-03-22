#pragma once
#include "menu/base/submenu.hpp"

class ObjectBuildingMenu : public Menu::Submenu {
public:
	static ObjectBuildingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectBuildingMenu()
		: Menu::Submenu() {}

	~ObjectBuildingMenu();
};

namespace ObjectBuildingMenuVars {
	struct Vars_ {
		int m_Type;
		int m_Count[4] = { 30, 33, 15, 20 };
		float m_VerticalAngle;
	};

	extern Vars_ m_Vars;
}