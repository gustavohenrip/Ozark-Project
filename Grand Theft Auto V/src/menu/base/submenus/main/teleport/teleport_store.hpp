#pragma once
#include "menu/base/submenu.hpp"

class TeleportStoreMenu : public Menu::Submenu {
public:
	static TeleportStoreMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TeleportStoreMenu()
		: Menu::Submenu() {}

	~TeleportStoreMenu();
};

namespace TeleportStoreMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}