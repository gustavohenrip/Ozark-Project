#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountLevelMenu : public Menu::Submenu {
public:
	static NetworkAccountLevelMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountLevelMenu()
		: Menu::Submenu() {}

	~NetworkAccountLevelMenu();
};

namespace NetworkAccountLevelMenuVars {
	struct Vars_ {
		int m_Type;
	};

	extern Vars_ m_Vars;
}