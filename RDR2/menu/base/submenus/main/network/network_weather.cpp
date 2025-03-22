#include "network_weather.hpp"
#include "menu/base/submenu_handler.hpp"
#include "rage/session/session.hpp"
#include "menu/base/submenus/main/world/weather.hpp"
#include "menu/base/util/timers.hpp"
#include "../network/players.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace networkWeatherMenuVars;

namespace networkWeatherMenuVars {
	vars_ vars;
}

void networkWeatherMenu::init() {
	setName("Weather");
	setParentSubmenu<networkPlayersMenu>();

	addOption(toggleOption("Rainbow")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().rainbowWeather)
		.addOnUpdate([](toggleOption* option) { option->addToggle(menu::getSelectedPlayer().rainbowWeather); }));

	addOption(breakOption("Presets"));

	for (std::size_t i = 0; i < NUMOF(weatherMenuVars::weather); i++) {
		addOption(buttonOption(weatherMenuVars::weather[i].name)
			.addOnClick([=] { rage::network::getNetworkTask()->triggerWeather(weatherMenuVars::weather[i].result, menu::getSelectedPlayer().id); }));
	}

}

/*Called while in submenu*/
void networkWeatherMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkWeatherMenu::updateOnce() {}

/*Called always*/
void networkWeatherMenu::featureUpdate() {
	menu::getPlayerManager()->foreach([](menu::playerVars& player) {
		if (player.rainbowWeather) {
			static int rainbowTimer[32];
			menu::util::getTimers()->runTimedFunction(&rainbowTimer[player.id], 500, [&] {
				rage::network::getNetworkTask()->triggerWeather(weatherMenuVars::weather[native::getRandomIntInRange(0, NUMOF(weatherMenuVars::weather) - 1)].result, player.id);
			});
		}
	});
}

networkWeatherMenu* _instance;
networkWeatherMenu* networkWeatherMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkWeatherMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkWeatherMenu::~networkWeatherMenu() { delete _instance; }