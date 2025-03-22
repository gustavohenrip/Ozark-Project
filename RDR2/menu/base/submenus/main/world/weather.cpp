#include "weather.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "rage/session/session.hpp"

using namespace weatherMenuVars;

namespace weatherMenuVars {
	vars_ vars; 
	
	scrollStruct<Hash> weather[22] = {
		{"Clear", 0xf5a87b65},
		{"Cloudy",0x30fdaf5c},
		{"Misty", 0x5974e8e5},
		{"Thunder", 0xB677829F},
		{"Thunderstorm", 0x7C1C4A13},
		{"Sunny", 0x614a1f91},
		{"Snow", 0xefb6eff6},
		{"Snow Clearing", 0x641DFC11},
		{"Light Snow", 0x23fb812b},
		{"Heavy Snow", 0x2b402288},
		{"Blizzard", 0x27ea2814},
		{"Ground Blizzard", 0x7F622122},
		{"Hail", 0x75A9E268},
		{"Foggy", 0xd61bde01},
		{"Drizzle", 0x995C7F44},
		{"Hurricane", 0x320D0951},
		{"Overcast", 0xBB898D2D},
		{"Dark Overcast", 0x19D4F1D9},
		{"Rain", 0x54A69840},
		{"Sandstorm", 0xB17F6111},
		{"Shower", 0xE72679D5},
		{"Sleet", 0xCA71D7C},
	};

	void setWeather(Hash type, bool networked) {
		_i<int>(0xD85DFE5C131E4AE9);
		native::setWeatherType(type, 1, 1, 1, 0.1f, 0);
		if (networked) {
			rage::network::getNetworkTask()->triggerWeather(type);
		}
	}
}

void weatherMenu::init() {
	setName("Weather");
	setParentSubmenu<worldMenu>();

	addOption(toggleOption("Network Weather")
		.addTranslation()
		.addToggle(vars.networkWeather));

	addOption(toggleOption("Disco Weather")
		.addTranslation()
		.addToggle(vars.discoWeather));

	addOption(breakOption("Presets")
		.addTranslation());

	for (std::size_t i = 0; i < NUMOF(weather); i++) {
		addOption(buttonOption(weather[i].name)
			.addOnClick([=] { setWeather(weather[i].result, vars.networkWeather); }));
	}
}

/*Called while in submenu*/
void weatherMenu::update() {}

/*Called once on submenu open*/
void weatherMenu::updateOnce() {}

/*Called always*/
void weatherMenu::featureUpdate() {
	if (vars.discoWeather) {
		if (vars.discoTimer < GetTickCount()) {
			setWeather(weather[native::getRandomIntInRange(0, NUMOF(weather) - 1)].result, vars.networkWeather);
			vars.discoTimer = GetTickCount() + 300;
		}
	}
}

weatherMenu* _instance;
weatherMenu* weatherMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new weatherMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

weatherMenu::~weatherMenu() { delete _instance; }