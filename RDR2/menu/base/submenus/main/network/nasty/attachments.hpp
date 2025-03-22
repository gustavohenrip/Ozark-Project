#pragma once
#include "menu/base/submenu.hpp"

class networkAttachmentsMenu : public menu::submenu {
public:
	static networkAttachmentsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkAttachmentsMenu()
		: menu::submenu()
	{}

	~networkAttachmentsMenu();
};

namespace networkAttachmentsMenuVars {
	struct vars_ {
		
	};

	extern vars_ vars;
}