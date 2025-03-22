#pragma once
#include "menu/base/submenu.hpp"

class miscMenu : public menu::submenu {
public:
	static miscMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	miscMenu()
		: menu::submenu()
	{}

	~miscMenu();
};

namespace miscMenuVars {
	struct vars_ {
		bool noclip;
		bool noIdleKick;
		bool noclipMonitoring;
		bool noclipBind;
		bool bypassProfanityFilter;
		int selectedMetric;

		math::vector3<float> noclipCoords;

		float noclipSpeed = 1.f;
	};

	extern scrollStruct<float> metric[3];
	extern vars_ vars;
}