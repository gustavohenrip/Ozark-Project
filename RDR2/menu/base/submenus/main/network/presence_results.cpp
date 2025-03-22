#include "presence_results.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "presence/network_presence.hpp"
#include "rage/api/socialclub.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "rage/session/session.hpp"

using namespace networkPresenceSeachResultsMenuVars;

namespace networkPresenceSeachResultsMenuVars {
	vars_ vars;
}

void networkPresenceSeachResultsMenu::init() {
	setName("Presence");
	setParentSubmenu<networkMenu>();

	addOption(buttonOption("Search")
		.addKeyboard("Enter the rid/name to search", 100, [this](const char* res) {
			vars.searchList.clear();
			networkPresenceMenuVars::vars.joinRidSendToName.clear();
			std::string str(res);
			if (std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end()) {
				// numeric
				networkPresenceMenuVars::vars.joinRidSendTo = std::atoi(str.c_str());
				vars.loading = true;
				rage::api::getSocialclub()->getProfileFromRID(networkPresenceMenuVars::vars.joinRidSendTo, [&](bool success, rage::network::rockstarAccount acc) {
					if (success) {
						vars.searchList.clear();
						vars.searchList.push_back(std::make_pair(acc.name, networkPresenceMenuVars::vars.joinRidSendTo));
						vars.loading = false;
					}
				});
			}
			else {
				// str
				vars.loading = true;
				rage::api::getSocialclub()->searchProfiles(res, [&, this](bool success, std::vector<std::pair<std::string, uint64_t>> acc) {
					if (success) {
						vars.searchList = acc;
						vars.loading = false;
					}
					else {
						vars.loading = false;
						menu::getNotify()->right(getString("~e~Error~q~: Failed to resolve"));
					}
				});
			}

			}));

	addOption(breakOption("Results"));
}

/*Called while in submenu*/
void networkPresenceSeachResultsMenu::update() {
	clearOptionsOffset(2);
	std::for_each(vars.searchList.begin(), vars.searchList.end(), [=](std::pair<std::string, uint64_t> &user) {
		addOption(submenuOption(user.first)
			.addTranslation()
			.addOnClick([=] {
				networkPresenceMenuVars::vars.joinRidSendToName.clear();
				networkPresenceMenuVars::vars.joinRidSendToName = user.first;
				networkPresenceMenuVars::vars.joinRidSendTo = user.second;
				})
			.addSubmenu<networkPresenceMenu>());
	});
	if (!vars.searchList.size()) {
		if (vars.loading) {
			addOption(buttonOption("~m~Loading..."));
		} else {
			addOption(buttonOption("~m~No Results"));
		}
	}
	//std::pair<std::string, uint64_t>
}

/*Called once on submenu open*/
void networkPresenceSeachResultsMenu::updateOnce() {}

/*Called always*/
void networkPresenceSeachResultsMenu::featureUpdate() {}

networkPresenceSeachResultsMenu* _instance;
networkPresenceSeachResultsMenu* networkPresenceSeachResultsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkPresenceSeachResultsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkPresenceSeachResultsMenu::~networkPresenceSeachResultsMenu() { delete _instance; }