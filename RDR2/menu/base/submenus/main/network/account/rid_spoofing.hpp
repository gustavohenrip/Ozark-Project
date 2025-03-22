#pragma once
#include "menu/base/submenu.hpp"

class networkRockstarIDSpoofingMenu : public menu::submenu {
public:
	static networkRockstarIDSpoofingMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkRockstarIDSpoofingMenu()
		: menu::submenu()
	{}

	~networkRockstarIDSpoofingMenu();
};

namespace networkRockstarIDSpoofingMenuVars {
	struct vars_ {
		char previewName[50];
		uint64_t spoofingRockstarID;
		uint64_t tempSpoofingRockstarID;
		bool configSaveRockstarID;
		std::shared_ptr<buttonOption> applyButton;
	};

	extern vars_ vars;
}