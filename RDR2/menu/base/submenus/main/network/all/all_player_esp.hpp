#pragma once
#include "menu/base/submenu.hpp"

class networkAllEspMenu : public menu::submenu {
public:
	static networkAllEspMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAllEspMenu()
		: menu::submenu()
	{}

	~networkAllEspMenu();
};

namespace networkAllEspMenuVars {
	struct vars_ {
		bool nameESP;
		bool lines;
		bool _2dbox;
		int _2dBoxType;
		bool _3dbox;
		bool blips;
		std::unordered_map<rage::network::netPlayerInfo*, Blip> playerBlip;
		bool withinRadius[3];

		float radius[2] = { 100.f, 100.f };
		color espColor[2] = {
			 color(60, 130, 255, 255),
			 color(60, 130, 255, 255)
		};
	};

	extern vars_ vars;
}