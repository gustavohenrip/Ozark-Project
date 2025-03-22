#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountMenu : public Menu::Submenu {
public:
	static NetworkAccountMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountMenu()
		: Menu::Submenu() {}

	~NetworkAccountMenu();
};

namespace NetworkAccountMenuVars {
	struct Vars_ {
		int m_SelectedCharacter;
	};

	extern Vars_ m_Vars;
}