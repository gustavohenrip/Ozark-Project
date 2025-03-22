#include "all_player_esp.hpp"
#include "menu/base/submenu_handler.hpp"
#include "all_players.hpp"
#include "../player_esp.hpp"
#include"rage/session/session.hpp"
#include "menu/base/util/timers.hpp"

using namespace networkAllEspMenuVars;

namespace networkAllEspMenuVars {
	vars_ vars;


}

void networkAllEspMenu::init() {
	setName("ESP");
	setParentSubmenu<networkAllPlayersMenu>();

	addOption(toggleOption("Name")
		.addTranslation()
		.addToggle(vars.nameESP).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Lines")
	 	.addTranslation()
		.addToggle(vars.lines).canBeSaved(name.getOriginal()));

	addOption(scrollOption<uint8_t>(TOGGLE, "2D")
		.addTranslation()
		.addScroll(vars._2dBoxType, 0, NUMOF(networkPlayerEspMenuVars::screenType), networkPlayerEspMenuVars::screenType)
		.addToggle(vars._2dbox).canBeSaved(name.getOriginal()));

	addOption(toggleOption("3D Box")
		.addTranslation()
		.addToggle(vars._3dbox).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Player Blips")
		.addToggle(vars.blips)
		.addOnClick([] {
			menu::getPlayerManager()->foreach([=](menu::playerVars& lobby) {
				rage::network::netPlayerInfo* info = rage::network::getNetworkTask()->getNetPlayerInfo(lobby.id);
				//printf("Removing blip\n");
				native::removeBlip(&vars.playerBlip[info]);
				vars.playerBlip[info] = 0;
			}); }));

	addOption(breakOption("Config")
		.addTranslation());

	addOption(numberOption<float>(TOGGLE, "Name Distance Limit")
		.addTranslation()
		.addNumber(vars.radius[0], "%.0f ft", 5.f).addMin(5.f).addMax(5000.f)
		.addToggle(vars.withinRadius[0]));

	addOption(numberOption<float>(TOGGLE, "Lines Distance Limit")
		.addTranslation()
		.addNumber(vars.radius[1], "%.0f ft", 5.f).addMin(5.f).addMax(5000.f)
		.addToggle(vars.withinRadius[1]));

	addOption(breakOption("Name Color")
		.addTranslation()
		.addRequirement([] {return vars.nameESP; }));

	addOption(numberOption<int>(SCROLL, "[R] Red")
		.addTranslation()
		.addNumber(vars.espColor[0].r, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnHover([] { menu::getRenderer()->previewColor(vars.espColor[0]); })
		.addRequirement([] { return vars.nameESP; }));

	addOption(numberOption<int>(SCROLL, "[G] Green")
		.addTranslation()
		.addNumber(vars.espColor[0].g, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnHover([] { menu::getRenderer()->previewColor(vars.espColor[0]); })
		.addRequirement([] { return vars.nameESP; }));

	addOption(numberOption<int>(SCROLL, "[B] Blue")
		.addTranslation()
		.addNumber(vars.espColor[0].b, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnHover([] { menu::getRenderer()->previewColor(vars.espColor[0]); })
		.addRequirement([] { return vars.nameESP; }));

	addOption(breakOption("Line Color")
		.addTranslation()
		.addRequirement([] { return vars.lines || vars._2dbox || vars._3dbox; }));

	addOption(numberOption<int>(SCROLL, "[R] Red")
		.addTranslation()
		.addNumber(vars.espColor[1].r, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnHover([] { menu::getRenderer()->previewColor(vars.espColor[1]); })
		.addRequirement([] { return vars.lines || vars._2dbox || vars._3dbox; }));

	addOption(numberOption<int>(SCROLL, "[G] Green")
		.addTranslation()
		.addNumber(vars.espColor[1].g, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnHover([] { menu::getRenderer()->previewColor(vars.espColor[1]); })
		.addRequirement([] { return vars.lines || vars._2dbox || vars._3dbox; }));

	addOption(numberOption<int>(SCROLL, "[B] Blue")
		.addTranslation()
		.addNumber(vars.espColor[1].b, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop()
		.addOnHover([] { menu::getRenderer()->previewColor(vars.espColor[1]); })
		.addRequirement([] { return vars.lines || vars._2dbox || vars._3dbox; }));
}

/*Called while in submenu*/
void networkAllEspMenu::update() {}

/*Called once on submenu open*/
void networkAllEspMenu::updateOnce() {}

/*Called always*/
void networkAllEspMenu::featureUpdate() {
	menu::getPlayerManager()->foreach([=] (menu::playerVars& lobby) {

		if (vars.blips) {

			static int blipTimeout[32];
			rage::network::netPlayerInfo* info = rage::network::getNetworkTask()->getNetPlayerInfo(lobby.id);
			menu::util::getTimers()->runTimedFunction(&blipTimeout[lobby.id], 60000, [&] {
				native::removeBlip(&vars.playerBlip[info]);
				vars.playerBlip[info] = 0;
			});
			if (!vars.playerBlip[info]) {
				//vars.playerBlip[info] = native::addBlipForEntity(-1749618580, native::getPlayerPedScriptIndex(lobby.id));
				vars.playerBlip[info] = native::addBlipForEntity(-1337945352, lobby.ped);
				//native::setBlipSprite(vars.playerBlip[info], 460296925, TRUE);
				native::setBlipNameToPlayerName(vars.playerBlip[info], lobby.id);
			}
		}

		if (lobby.id != menu::getLocalPlayer().id) {
			if (vars.nameESP) {
				if (vars.withinRadius[0] && native::vDist(lobby.coords, menu::getLocalPlayer().coords) > vars.radius[0]) {
					goto jmp1;
				}

				networkPlayerEspMenuVars::nameEsp(lobby, vars.espColor[0]);
			}

		jmp1:
			if (vars.withinRadius[1] && native::vDist(lobby.coords, menu::getLocalPlayer().coords) > vars.radius[1]) {
				return;
			}

			if (vars.lines) {
				networkPlayerEspMenuVars::line(lobby.ped, vars.espColor[1]);
			}

			if (vars._2dbox) {
				networkPlayerEspMenuVars::screenEsp(lobby.ped, vars.espColor[1], vars._2dBoxType);
			}

			if (vars._3dbox) {
				networkPlayerEspMenuVars::box(lobby.ped, vars.espColor[1]);
			}
		}
	});
}

networkAllEspMenu* _instance;
networkAllEspMenu* networkAllEspMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAllEspMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAllEspMenu::~networkAllEspMenu() { delete _instance; }