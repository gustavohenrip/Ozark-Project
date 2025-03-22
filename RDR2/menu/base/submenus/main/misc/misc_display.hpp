#pragma once
#include "menu/base/submenu.hpp"

class miscDisplayMenu : public menu::submenu {
public:
	static miscDisplayMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	miscDisplayMenu()
		: menu::submenu()
	{}

	~miscDisplayMenu();
};

namespace miscDisplayMenuVars {
	struct vars_ {
		int displayFPSVar;
		int drawFPSCache[2];
		int drawFPSAverage = 0;
		int drawFPSAverageTotal = 0;
		int drawFPSAverageCount = 0;

		bool displayPools;
		bool displayFPS;
		bool displayPosition;
		bool displayNetworkPool;

		std::size_t displayNetworkPoolId = INT_MAX;
		std::size_t displayFPSId = INT_MAX;
		std::size_t displayPositionId = INT_MAX;
		std::size_t displayPoolsId[5] = { INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX };
	};

	extern vars_ vars;
}