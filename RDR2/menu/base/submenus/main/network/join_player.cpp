#include "join_player.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network.hpp"
#include "menu/base/util/presence.hpp"
#include "rage/session/session.hpp"
#include "../network/recent/selected_recent_player.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"

using namespace networkJoinPlayerMenuVars;

namespace networkJoinPlayerMenuVars {
	vars_ vars;

	void initialize() {
		std::size_t size = (std::size_t)_i<int>(0xA4A79DD2D9600654);// getNumOfConnectedPlayers
		vars.players.clear();
		vars.players.resize(size);
		static int count = 0;
		std::for_each(vars.players.begin(), vars.players.end(),
			[&](std::pair<std::pair<uint64_t, std::string>, std::pair<bool, std::shared_ptr<buttonOption>>>& data) {
				auto net = rage::engine::getNetGamePlayerFromIndex(count++);
				if (net) {
					auto info = net->playerInfo;
					if (info) {
						data.first.first = info->netInfo.rockstarId2;
						data.first.second = info->netInfo.name;
					}
				}

			});
		count = 0;
	}

}

void networkJoinPlayerMenu::init() {
	setName("Join Player");
	setParentSubmenu<networkSelectedRecentPlayerMenu>();

}

/*Called while in submenu*/
void networkJoinPlayerMenu::update() {
	std::string joinButton = std::string(!vars.players.empty() ? "Join with Group" : "Join Player");
	vars.joinButton->setName(joinButton.c_str());

	std::for_each(vars.players.begin(), vars.players.end(),
		[&](std::pair<std::pair<uint64_t, std::string>, std::pair<bool, std::shared_ptr<buttonOption>>>& data) {
			char joinButton[36];
			snprintf(joinButton, 36, data.second.first ? "~t6~%s" : "~m~%s", data.first.second);
			data.second.second->setName(joinButton);
		});
}

// 153430369

/*Called once on submenu open*/
void networkJoinPlayerMenu::updateOnce() {
	clearOptionsOffset(0);
	initialize();

	addOption(buttonOption("Cancel")
		.addOnClick([] { vars.group.clear(); }));

	vars.joinButton = addOption(buttonOption("")
		.addOnClick([] { 
			
			std::for_each(vars.players.begin(), vars.players.end(),
				[&](std::pair<std::pair<uint64_t, std::string>, std::pair<bool, std::shared_ptr<buttonOption>>>& data) {
					if (data.second.first) {
						menu::getPresence()->forceRidIntoSession(data.first.first, vars.ridToJoin);
						vars.group.push_back(data.first.first);
					}
				});
			menu::getPresence()->joinThem(vars.ridToJoin);

			vars.ridToJoinCopy = vars.ridToJoin;
			
			}));
	if (!vars.players.empty()) {
		addOption(breakOption("Include"));

		std::for_each(vars.players.begin(), vars.players.end(),
			[&](std::pair<std::pair<uint64_t, std::string>, std::pair<bool, std::shared_ptr<buttonOption>>>& data) {
				data.second.second = addOption(buttonOption("")
					.addOnClick([&] {
						data.second.first = !data.second.first;
						printf(data.second.first ? "added %s to group\n" : "removed %s from group\n", data.first.second);
					}));
			});
	}

}

/*Called always*/
void networkJoinPlayerMenu::featureUpdate() {

	if (!vars.group.empty()) {
		static bool bringGroup = false;
			menu::getPlayerManager()->foreach([&](menu::playerVars& lobby) {
				auto net = rage::network::getNetworkTask()->getNetPlayerInfo(lobby.id);
				if (net) {
					if (net->rockstarId2 == vars.ridToJoinCopy) {
						bringGroup = true;
					}
				}
			}, true);
			if (bringGroup) {
				std::for_each(vars.group.begin(), vars.group.end(),
					[](uint64_t& data) {
						menu::util::getFiberPool()->run([=] {	
							//menu::getPresence()->joinMe(data);
							menu::util::waitFiber(2000);
						});
					});
				bringGroup = false;
				vars.group.clear();
			}
	}

}

networkJoinPlayerMenu* _instance;
networkJoinPlayerMenu* networkJoinPlayerMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkJoinPlayerMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkJoinPlayerMenu::~networkJoinPlayerMenu() { delete _instance; }