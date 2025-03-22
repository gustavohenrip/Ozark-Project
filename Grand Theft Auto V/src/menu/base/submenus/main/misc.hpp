#pragma once
#include "menu/base/submenu.hpp"

class MiscMenu : public Menu::Submenu {
public:
	static MiscMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscMenu()
		: Menu::Submenu() {}

	~MiscMenu();
};

namespace MiscMenuVars {
	struct Vars_ {
		bool m_ShowJoinEvents = true;
		bool m_RockstarDeveloperPerks;

		int m_MetricSystem;
	};

	extern Vars_ m_Vars;
}