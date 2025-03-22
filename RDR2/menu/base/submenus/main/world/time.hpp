#pragma once
#include "menu/base/submenu.hpp"

class timeMenu : public menu::submenu {
public:
	static timeMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	timeMenu()
		: menu::submenu()
	{}

	~timeMenu();
};

namespace timeMenuVars {
	struct vars_ {
		bool alwaysDay;
		bool alwaysNight;
		int hours;
		float scale = 1.f;
		bool syncWithNetworkTime;
		bool networkTime;
		bool syncWithSystemTime;
	};

	void setTime(math::vector3<int> time, bool networked);
	extern vars_ vars;
}