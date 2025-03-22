#pragma once
#include "menu/base/submenu.hpp"

class networkPresenceMenu : public menu::submenu {
public:
	static networkPresenceMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkPresenceMenu()
		: menu::submenu()
	{}

	~networkPresenceMenu();
};

namespace networkPresenceMenuVars {
	struct vars_ {
		uint64_t rockstarMessageSendTo;
		uint64_t crewMessageSendTo;
		uint64_t joinRidSendTo = 0;
		uint64_t joinMeSendTo;

		std::string rockstarMessageSendToName;
		std::string crewMessageSendToName;
		std::string joinRidSendToName;
		std::string joinMeSendToName;

		std::shared_ptr<buttonOption> rockstarMessageButton;
		std::shared_ptr<buttonOption> crewMessageButton;
		std::shared_ptr<buttonOption> joinRidButton;
		std::shared_ptr<buttonOption> joinMeButton;
	};

	extern vars_ vars;
}