#pragma once
#include "menu/base/submenu.hpp"

class networkJoinPlayerMenu : public menu::submenu {
public:
	static networkJoinPlayerMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkJoinPlayerMenu()
		: menu::submenu()
	{}

	~networkJoinPlayerMenu();
};

namespace networkJoinPlayerMenuVars {
	struct vars_ {
		std::shared_ptr<buttonOption > joinButton;
		std::vector<std::pair<std::pair<uint64_t, std::string>, std::pair<bool, std::shared_ptr<buttonOption>>>> players;
		std::vector<uint64_t> group;
		uint64_t ridToJoin;
		uint64_t ridToJoinCopy;
		int32_t submenuID;
	};

	extern vars_ vars;
}