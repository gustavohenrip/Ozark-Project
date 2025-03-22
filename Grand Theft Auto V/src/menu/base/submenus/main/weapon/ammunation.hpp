#pragma once
#include "menu/base/submenu.hpp"

class AmmunationMenu : public Menu::Submenu {
public:
	static AmmunationMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AmmunationMenu()
		: Menu::Submenu() {}

	~AmmunationMenu();
};

namespace AmmunationMenuVars {
	struct Vars_ {
	};

	namespace Scripts {
		const char* func_181(int iParam0, int iParam1);
	}

	extern Vars_ m_Vars;
}
