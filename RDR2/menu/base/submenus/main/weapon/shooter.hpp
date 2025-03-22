#pragma once
#include "menu/base/submenu.hpp"

class shooterMenu : public menu::submenu {
public:
	static shooterMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	shooterMenu()
		: menu::submenu()
	{}

	~shooterMenu();
};

namespace shooterMenuVars {
	struct vars_ {
		Object shooterObj;
		int shooterType;
		bool shooterToggle;
		int trains;
		int wagons;
		int boats;
		int stagecoach;
		int alligators;
		int dogs;
		int sparrows;
		int snakes;
		int males;
		int females;
		int horses;
		Hash hash;
	};

	extern vars_ vars;
}