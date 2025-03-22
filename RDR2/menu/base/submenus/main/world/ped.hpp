#pragma once
#include "menu/base/submenu.hpp"

class pedMenu : public menu::submenu {
public:
	static pedMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	pedMenu()
		: menu::submenu()
	{}

	~pedMenu();
};

namespace pedMenuVars {
	struct vars_ {
		bool grabDeadPeds;
		bool esp = true;
		bool smite;
		int teleportIndex;
		float scale = 1.0f;
		bool scaleToggle;
	};

	void esp();
	void launchUp(Entity entity);
	extern vars_ vars;
}