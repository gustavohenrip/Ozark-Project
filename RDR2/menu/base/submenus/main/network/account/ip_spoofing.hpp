#pragma once
#include "menu/base/submenu.hpp"

class networkIPSpoofingMenu : public menu::submenu {
public:
	static networkIPSpoofingMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkIPSpoofingMenu()
		: menu::submenu()
	{}

	~networkIPSpoofingMenu();
};

namespace networkIPSpoofingMenuVars {
	struct vars_ {
		char previewName[50];
		std::string spoofingIP;
		std::string tempSpoofingIP;
		std::string defaultIP;
		bool configSaveIP;
		std::shared_ptr<buttonOption> applyButton;
	};

	extern vars_ vars;
}