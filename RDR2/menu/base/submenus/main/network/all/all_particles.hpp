#pragma once
#include "menu/base/submenu.hpp"

class networkAllParticleMenu : public menu::submenu {
public:
	static networkAllParticleMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAllParticleMenu()
		: menu::submenu()
	{}

	~networkAllParticleMenu();
};

namespace networkAllParticleMenuVars {
	struct vars_ {
		bool overrideScale;
		float scale = 2.f;
		int loopedTimeout = 10;
	};

	extern vars_ vars;
}