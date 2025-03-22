#pragma once
#include "menu/base/submenu.hpp"

class playerMenu : public menu::submenu {
public:
	static playerMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	playerMenu()
		: menu::submenu()
	{}

	~playerMenu();
};

namespace playerMenuVars {
	struct vars_ {
		bool invincibility;
		bool fasthealthRegen;
		bool neverWanted;
		bool noBounty;
		bool offTheRadar;
		bool invisibility;
		bool unlimitedStamina;
		bool superchargeCores;
		bool unlimitedDeadEye;
		bool infiniteEagleEye;
		bool noRagdoll;
		bool superJump;
		bool superRun;
		bool superSwim;
		bool forcefield;
		bool shrink;

		int superRunSpeed = 10;
	};

	extern vars_ vars;
}