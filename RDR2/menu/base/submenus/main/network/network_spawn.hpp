#pragma once
#include "menu/base/submenu.hpp"

class networkSpawnMenu : public menu::submenu {
public:
	static networkSpawnMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkSpawnMenu()
		: menu::submenu()
	{}

	~networkSpawnMenu();
};

namespace networkSpawnMenuVars {
	struct vars_ {
		int trains;
		int wagons;
		int boats;
		int stagecoaches;
		int carts;
		int special;
		int alligators;
		int dogs;
		int sparrows;
		int snakes;
		int males;
		int females;
		int horses;
		bool effect;
	};
	extern vars_ vars;
}