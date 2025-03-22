#pragma once
#include "menu/base/submenu.hpp"

class mountMenu : public menu::submenu {
public:
	static mountMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	mountMenu()
		: menu::submenu()
	{}

	~mountMenu();
};

namespace mountMenuVars {
	struct vars_ {
		bool invincibility;
		bool invisibility;
		bool unlimitedStamina;
		bool fastRegenHealth;
		bool noRagdoll;
		bool superGallop;
		bool hornBoost;
		int superGallopSpeed = 10;
		float scale = 1.f;
	};

	extern vars_ vars;
}