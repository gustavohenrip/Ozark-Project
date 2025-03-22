#pragma once
#include "menu/base/submenu.hpp"

class weatherMenu : public menu::submenu {
public:
	static weatherMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	weatherMenu()
		: menu::submenu()
	{}

	~weatherMenu();
};

namespace weatherMenuVars {
	struct vars_ {
		bool discoWeather;
		bool networkWeather;
		uint32_t discoTimer;
	};
	extern scrollStruct<Hash> weather[22];
	void setWeather(Hash type, bool networked);
	extern vars_ vars;
}