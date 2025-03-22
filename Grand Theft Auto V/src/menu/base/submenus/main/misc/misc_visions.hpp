#pragma once
#include "menu/base/submenu.hpp"

class MiscVisionsMenu : public Menu::Submenu {
public:
	static MiscVisionsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscVisionsMenu()
		: Menu::Submenu() {}

	~MiscVisionsMenu();
};

namespace MiscVisionsMenuVars {
	struct Vars_ {
		bool m_ThermalVision;
		uint8_t m_Backup[5];

		RadioContext m_Context = { "commonmenu", "shop_mask_icon" };
	};

	extern Vars_ m_Vars;
}