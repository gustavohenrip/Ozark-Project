#pragma once
#include "menu/base/submenu.hpp"

class trainMenu : public menu::submenu {
public:
	static trainMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	trainMenu()
		: menu::submenu()
	{}

	~trainMenu();
};

namespace trainMenuVars {
	struct vars_ {
		bool hasTrain;
		Vehicle train;
		float speed = 1.f;
	};

	extern vars_ vars;
}