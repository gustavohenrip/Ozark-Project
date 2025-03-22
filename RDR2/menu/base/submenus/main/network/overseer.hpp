#pragma once
#include "menu/base/submenu.hpp"

class networkOverseerMenu : public menu::submenu {
public:
	static networkOverseerMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkOverseerMenu()
		: menu::submenu()
	{}

	~networkOverseerMenu();
};

namespace networkOverseerMenuVars {
#define OVERSEER_ENABLED 1
#define OVERSEER_NOTIFY 2
#define OVERSEER_CRASH 4
#define OVERSEER_EXPLODE_LOOP 8
#define OVERSEER_PARALYSE 16
#define OVERSEER_BLOCK_JOIN 32
#define OVERSEER_BLACKLIST 64

	struct overseer {
		std::string name;
		std::string reason;
		int rid;
		int flags;
	};

	struct vars_ {
		std::vector<std::pair<std::string, overseer>> players;
		bool enableOverseer;
		std::vector<uint64_t> queue;
		std::vector<std::string> blockJoiners;
		std::vector<std::pair<std::string, std::string>> blacklist;
	};

	void load(networkOverseerMenu* ptr);
	bool save(std::string name, int rid, std::string reason);
	void process(int playerID, int flag);
	extern vars_ vars;
}