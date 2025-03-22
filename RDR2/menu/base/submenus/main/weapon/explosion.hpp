#pragma once
#include "menu/base/submenu.hpp"

class explosionMenu : public menu::submenu {
public:
	static explosionMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	explosionMenu()
		: menu::submenu()
	{}

	~explosionMenu();
};

namespace explosionMenuVars {
	struct vars_ {
		bool explosionGun;
		int type;
		float radius = 1.0f;
		bool invisible = false;
		bool audible = true;
		float cameraShake = 0.0f;
		bool changeOwner;
		int changeOwnerVal;
		int listSize;
		scrollStruct<int> changeOwnerList[32];
	};
	extern scrollStruct<int> explosions[36];
	void explosionGun(Ped owner, int type, float radius, bool audible, bool invisible, float cameraShake);
	extern vars_ vars;
}