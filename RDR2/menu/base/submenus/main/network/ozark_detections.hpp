#pragma once
#include "menu/base/submenu.hpp"
#define OZARK_HIDDEN_USER 1
#define OZARK_USER 2
#define OZARK_STAFF 4
#define OZARK_DONT_HURT_ME 8
#define OZARK_USER_DEVELOPER 16

class networkOzarkDetectsMenu : public menu::submenu {
public:
	static networkOzarkDetectsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkOzarkDetectsMenu()
		: menu::submenu()
	{}

	~networkOzarkDetectsMenu();
};

namespace networkOzarkDetectsMenuVars {
	struct vars_ {
		int ozarkBit = OZARK_HIDDEN_USER | OZARK_USER;
		bool detection = true;
		bool disableOzarkAccess;
		std::string ozarkDetects = "MC_EntityID";
	};

	extern vars_ vars;
}