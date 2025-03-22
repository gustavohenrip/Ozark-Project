#pragma once
#include "menu/base/submenu.hpp"

class saveLoadTeleportMenu : public menu::submenu {
public:
	static saveLoadTeleportMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	saveLoadTeleportMenu()
		: menu::submenu()
	{}

	~saveLoadTeleportMenu();
};

namespace saveLoadTeleportMenuVars {
	struct vars_ {
		std::vector<std::pair<std::string, math::vector3<float>>> locations;
	};

	extern vars_ vars;
}