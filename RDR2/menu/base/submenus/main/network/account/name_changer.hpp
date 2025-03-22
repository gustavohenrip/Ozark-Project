#pragma once
#include "menu/base/submenu.hpp"

class networkAccountNameChangerMenu : public menu::submenu {
public:
	static networkAccountNameChangerMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAccountNameChangerMenu()
		: menu::submenu()
	{}

	~networkAccountNameChangerMenu();
};

namespace networkAccountNameChangerMenuVars {
	struct vars_ {
		int color;
		int style;
		bool saveToConfig;
		std::string tempName;
		std::string spoofedName;
		char originalName[32];
		char previewName[50];
		std::shared_ptr<buttonOption> previewButton;
	};

	extern vars_ vars;
}