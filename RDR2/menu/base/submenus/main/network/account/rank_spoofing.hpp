#pragma once
#include "menu/base/submenu.hpp"

class networkAccountRankSpooferMenu : public menu::submenu {
public:
	static networkAccountRankSpooferMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAccountRankSpooferMenu()
		: menu::submenu()
	{}

	~networkAccountRankSpooferMenu();
};

namespace networkAccountRankSpooferMenuVars {
	struct vars_ {
		bool _1337;
		bool randomize;
		bool incrementing;
		bool saveToConfig;
		uint32_t originalRank;
		uint32_t spoofedRank = 1337;
		char previewName[50];
		std::shared_ptr<buttonOption> previewButton;
		char hackerName[50];
		std::shared_ptr<toggleOption> hackerButton;
	};

	extern vars_ vars;
}