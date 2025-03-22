#pragma once
#include "menu/base/submenu.hpp"

class networkMessageMenu : public menu::submenu {
public:
	static networkMessageMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkMessageMenu()
		: menu::submenu()
	{}

	~networkMessageMenu();
};

namespace networkMessageMenuVars {
	struct vars_ {
		int32_t color;
		int32_t style;
		int32_t submenuIdentifier = 0;
		int32_t messageIdentifier = 0;
		uint64_t rockstarID = 0;
		std::shared_ptr<buttonOption> inputButton;
		char inputMessage[100];
		std::string message = "Hello, friend!";
	};

	extern scrollStruct<const char*> textColors[18];
	extern scrollStruct<const char*> textSettings[3];
	extern vars_ vars;
}