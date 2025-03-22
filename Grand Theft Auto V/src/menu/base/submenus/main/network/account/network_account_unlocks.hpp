#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountUnlocksMenu : public Menu::Submenu {
public:
	static NetworkAccountUnlocksMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountUnlocksMenu()
		: Menu::Submenu() {}

	~NetworkAccountUnlocksMenu();
};

namespace NetworkAccountUnlocksMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}