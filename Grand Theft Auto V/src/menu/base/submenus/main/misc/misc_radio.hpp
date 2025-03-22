#pragma once
#include "menu/base/submenu.hpp"

class MiscRadioMenu : public Menu::Submenu {
public:
	static MiscRadioMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscRadioMenu()
		: Menu::Submenu() {}

	~MiscRadioMenu();
};

namespace MiscRadioMenuVars {
	struct Vars_ {
		bool m_MobileRadio;

		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}