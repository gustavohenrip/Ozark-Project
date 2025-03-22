#pragma once
#include "menu/base/submenu.hpp"

class networkMessageSaveLoadMenu : public menu::submenu {
public:
	static networkMessageSaveLoadMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkMessageSaveLoadMenu()
		: menu::submenu()
	{}

	~networkMessageSaveLoadMenu();
};

namespace networkMessageSaveLoadMenuVars {
	struct vars_ {
		std::vector<std::pair<std::string, std::string>> messages;
	};

	extern vars_ vars;
}