#pragma once
#include "menu/base/submenu.hpp"

class WardrobeSaveLoadMenu : public Menu::Submenu {
public:
	static WardrobeSaveLoadMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WardrobeSaveLoadMenu()
		: Menu::Submenu() {}

	~WardrobeSaveLoadMenu();
};

namespace WardrobeSaveLoadMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}