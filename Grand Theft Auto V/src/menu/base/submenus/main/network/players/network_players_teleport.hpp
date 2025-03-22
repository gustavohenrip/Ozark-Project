#pragma once
#include "menu/base/submenu.hpp"

class NetworkPlayersTeleportMenu : public Menu::Submenu {
public:
	static NetworkPlayersTeleportMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersTeleportMenu()
		: Menu::Submenu() {}

	~NetworkPlayersTeleportMenu();
};

namespace NetworkPlayersTeleportMenuVars {
	struct Vars_ {
		
	};

	extern Vars_ m_Vars;
}