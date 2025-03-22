#pragma once
#include "menu/base/submenu.hpp"

class TeleportStoreSelectedMenu : public Menu::Submenu {
public:
	static TeleportStoreSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TeleportStoreSelectedMenu()
		: Menu::Submenu() {}

	~TeleportStoreSelectedMenu();
};

namespace TeleportStoreSelectedMenuVars {
	struct Vars_ {
		int m_Selected;
	};

	extern Vars_ m_Vars;
}