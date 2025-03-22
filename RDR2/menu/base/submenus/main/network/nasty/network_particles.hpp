#pragma once
#include "menu/base/submenu.hpp"

class networkParticleMenu : public menu::submenu {
public:
	static networkParticleMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkParticleMenu()
		: menu::submenu()
	{}

	~networkParticleMenu();
};

namespace networkParticleMenuVars {
	struct vars_ {};

	extern vars_ vars;
}