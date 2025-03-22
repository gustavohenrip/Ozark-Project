#include "ip_spoofing.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../account.hpp"
#include "global/auth_vars.hpp"
#include "rage/engine.hpp"
#include "utils/utils.hpp"
#include "utils/va.hpp"
#include "menu/base/util/notify.hpp"

using namespace networkIPSpoofingMenuVars;

namespace networkIPSpoofingMenuVars {
	vars_ vars;

	void spoofIP(networkIPSpoofingMenu* pthis, std::string ip, bool def = false) {
		if (def) {
			auto split = utils::split(vars.defaultIP + ".", ".");
			if (split.size() == 4) {
				utils::getConfig()->writeString("Spoofing: IP", "IP", vars.defaultIP.c_str());

				if (native::networkIsInSession()) {
					auto net = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
					if (net) {
						if (net->playerInfo) {
							net->playerInfo->netInfo.externalIp[3] = (BYTE)atoi(split[0].c_str());
							net->playerInfo->netInfo.externalIp[2] = (BYTE)atoi(split[1].c_str());
							net->playerInfo->netInfo.externalIp[1] = (BYTE)atoi(split[2].c_str());
							net->playerInfo->netInfo.externalIp[0] = (BYTE)atoi(split[3].c_str());
						}
					}
				}
			} else {
				menu::getNotify()->right(pthis->getString("~e~Error~q~: Invalid default IP"));
			}

			return;
		}

		auto split = utils::split(ip + ".", ".");
		if (split.size() == 4) {
			vars.spoofingIP = ip;
			if (vars.configSaveIP) {
				utils::getConfig()->writeString("Spoofing: IP", "IP", ip.c_str());
			}

			if (native::networkIsInSession()) {
				auto net = rage::engine::getNetworkPlayerManager()->localNetGamePlayer;
				if (net) {
					if (net->playerInfo) {
						net->playerInfo->netInfo.externalIp[3] = (BYTE)atoi(split[0].c_str());
						net->playerInfo->netInfo.externalIp[2] = (BYTE)atoi(split[1].c_str());
						net->playerInfo->netInfo.externalIp[1] = (BYTE)atoi(split[2].c_str());
						net->playerInfo->netInfo.externalIp[0] = (BYTE)atoi(split[3].c_str());
					}
				}
			}
		} else {
			menu::getNotify()->right(pthis->getString("~e~Error~q~: Invalid IP format (has to be xxx.xxx.xxx.xxx)"));
		}
	}
}

void networkIPSpoofingMenu::init() {
	setName("Spoofing: IP");
	setParentSubmenu<networkAccountMenu>();

	addString("~e~Error~q~: Invalid default IP");
	addString("~e~Error~q~: Invalid IP format (has to be xxx.xxx.xxx.xxx)");

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter IP", 15, [this] (const char* name) {
		std::string str = name + std::string(".");

		auto split = utils::split(str, ".");
		if (split.size() == 4) {
			vars.tempSpoofingIP = name;
		} else {
			menu::getNotify()->right(getString("~e~Error~q~: Invalid IP format (has to be xxx.xxx.xxx.xxx)"));
		}
	}));

	addOption(buttonOption("Reset to Default")
		.addTranslation()
		.addOnClick([this] { spoofIP(this, vars.defaultIP); }));

	addOption(toggleOption("Save to Config")
		.addTranslation()
		.addToggle(vars.configSaveIP).canBeSaved(name.getOriginal())
		.addOnClick([this] { spoofIP(this, "", true); }));

	vars.applyButton = addOption(buttonOption("")
		.addOnClick([this] { spoofIP(this, vars.tempSpoofingIP); }));

	addOption(breakOption("Presets")
		.addTranslation());

	addOption(buttonOption("Take Two Interactive")
		.addTranslation()
		.addOnClick([this] { spoofIP(this, utils::va::va("199.168.6%i.%i", native::getRandomIntInRange(0, 2), native::getRandomIntInRange(0, 255))); }));

	if (vars.configSaveIP) {
		vars.spoofingIP = vars.tempSpoofingIP = utils::getConfig()->readString("Spoofing: IP", "IP", vars.spoofingIP.c_str());
	}
}

/*Called while in submenu*/
void networkIPSpoofingMenu::update() {
	snprintf(vars.previewName, 50, "Apply: %s", vars.tempSpoofingIP.c_str());
	vars.applyButton->setName(vars.previewName);
}

/*Called once on submenu open*/
void networkIPSpoofingMenu::updateOnce() {}

/*Called always*/
void networkIPSpoofingMenu::featureUpdate() {
	if (vars.defaultIP.empty()) {
		uint64_t mapip = rage::engine::getGameInfoAddress("mapip");

		if (mapip) {
			BYTE* p = (BYTE*)(mapip + 0x40);
			vars.defaultIP = utils::va::va("%i.%i.%i.%i", p[3], p[2], p[1], p[0]);

			if (vars.tempSpoofingIP.empty()) {
				vars.tempSpoofingIP = vars.defaultIP;
			}
		}
	}
}

networkIPSpoofingMenu* _instance;
networkIPSpoofingMenu* networkIPSpoofingMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkIPSpoofingMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkIPSpoofingMenu::~networkIPSpoofingMenu() { delete _instance; }