#pragma once
#include "menu/base/submenu.hpp"

class TriggerbotMenu : public Menu::Submenu {
public:
	static TriggerbotMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TriggerbotMenu()
		: Menu::Submenu() {}

	~TriggerbotMenu();
};

namespace TriggerbotMenuVars {
	struct Vars_ {
		bool m_Enable;
		int m_BulletBuffer = 100;
		int m_AimTag;
		int m_AimEntity;
	};

	extern Vars_ m_Vars;
}