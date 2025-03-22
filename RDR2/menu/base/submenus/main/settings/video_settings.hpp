#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/network_types.hpp"

class videoSettingsMenu : public menu::submenu {
public:
	static videoSettingsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	videoSettingsMenu()
		: menu::submenu()
	{}

	~videoSettingsMenu();
};

namespace videoSettingsMenuVars {
	struct vars_ {
		bool hideMe;
		bool hideIPs;
		bool hideRID;
		bool hideNames;
		std::unordered_map<rage::network::netPlayerInfoWrapper*, std::string> cachedPlayerInfo;
	};

	extern vars_ vars;
}