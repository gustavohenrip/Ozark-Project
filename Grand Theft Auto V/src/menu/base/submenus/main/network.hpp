#pragma once
#include "menu/base/submenu.hpp"

class NetworkMenu : public Menu::Submenu {
public:
	static NetworkMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkMenu()
		: Menu::Submenu() {}

	~NetworkMenu();
};

namespace NetworkMenuVars {
	struct Vars_ {
		int m_Transition;
	};

	extern Vars_ m_Vars;
}