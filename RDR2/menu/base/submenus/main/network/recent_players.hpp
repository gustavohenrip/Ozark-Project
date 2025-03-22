#pragma once
#include "menu/base/submenu.hpp"

class networkRecentPlayersMenu : public menu::submenu {
public:
	static networkRecentPlayersMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkRecentPlayersMenu()
		: menu::submenu()
	{}

	~networkRecentPlayersMenu();
};

namespace networkRecentPlayersMenuVars {
	struct recentPlayer {
		std::string name;
		std::string ip;
		std::string firstEncountered;
		std::string lastEncountered;
		uint64_t rid;
		int internalEncountered;
	};

	struct vars_ {
		std::vector<uint64_t> queue;;
		std::vector<std::pair<std::string, recentPlayer>> players;
		std::vector<std::pair<std::string, recentPlayer>> permanentPlayers;
		std::vector<std::pair<std::string, recentPlayer>>* selectedList;
	};

	void save(std::string name, std::string ip, uint64_t rid);
	extern vars_ vars;
}