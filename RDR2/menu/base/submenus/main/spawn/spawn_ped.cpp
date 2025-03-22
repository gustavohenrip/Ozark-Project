#include "spawn_ped.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../spawn.hpp"
#include "spawn_settings.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/renderer.hpp"
#include "utils/va.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "spawn_ped_list.hpp"
#include "spawn_manage.hpp"
#include "rage/entity/entity.hpp"

using namespace spawnPedMenuVars;

namespace spawnPedMenuVars {
	vars_ vars;

	void spawnPed(std::string visibleName, Hash hash, int variant) {
		menu::getControlManager()->requestModel(hash, [=] (DWORD modelHash) {
			auto coords = menu::getLocalPlayer().coords + (native::getEntityForwardVector(menu::getLocalPlayer().entity) * 5.f);
			auto modelInfo = rage::engine::getModelInfo(modelHash);
			Ped ped = menu::util::getSpawnManager()->spawnPed(modelHash, coords, 0.f);
			if (ped) {
				if (spawnSettingsMenuVars::vars.pedSpawnWithBlip) {
					native::setBlipSprite(native::addBlipForEntity(-515518185, ped), -1606321000, TRUE);
				}
				_i<int>(0x77FF8D35EEC6BBC4, ped, variant, 0);
				if (spawnSettingsMenuVars::vars.pedSpawnSedated) {
					native::setPedConfigFlag(ped, 580, true);
					native::setPedKeepsTask(ped, true);
				}
				
				if (spawnSettingsMenuVars::vars.pedSpawnDead) {
					native::applyDamageToPed(ped, 30000, true, 0, 0);
				}

				if (spawnSettingsMenuVars::vars.pedSpawnWithParticle) {
					native::requestNamedPtfxAsset(native::getHashKey("scr_dis_ligtn_trees"));
					native::useParticleFXAsset("scr_dis_ligtn_trees");

					auto copyCoords = coords;
					copyCoords.z -= 5.f;
					native::startNetworkedParticleFXNonLoopedAtCoord("scr_crackpot_rc_lightening_exp", copyCoords, { 0.f, 0.f, 0.f }, 1.8f, FALSE, FALSE, FALSE);
				}

				if (spawnSettingsMenuVars::vars.pedSpawnWithInvincibility) {
					native::setEntityInvincible(ped, TRUE);
				}

				if (spawnSettingsMenuVars::vars.pedSpawnFrozen) {
					native::freezeEntityPosition(ped, TRUE);
				}

				if (spawnSettingsMenuVars::vars.pedAddToManager) {
					spawnedPedData data;
					data.creationOverlay = spawnSettingsMenuVars::vars.pedSpawnWithCreationOverlay;
					data.creationTime = native::getGameTimer();
					data.frozen = spawnSettingsMenuVars::vars.pedSpawnFrozen;
					data.handle = ped;
					data.hash = modelHash;
					data.invincibility = spawnSettingsMenuVars::vars.pedSpawnWithInvincibility;
					data.origin = coords;

					std::string n = visibleName;
					if (n.empty()) {
						std::stringstream stream;
						stream << "0x" << std::hex << modelHash;
						n = stream.str().c_str();
					}

					strcpy(data.name, n.c_str());

					vars.spawnedPeds.push_back(data);
				}
			}
		});
	}
}

void spawnPedMenu::init() {
	setName("Spawn Ped");
	setParentSubmenu<spawnMenu>();

	addOption(submenuOption("Spawn Settings")
		.addTranslation()
		.addSubmenu<spawnSettingsMenu>());

	addOption(submenuOption("Manage Spawned Peds")
		.addTranslation()
		.addSubmenu<spawnManageMenu>());

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter hash or name of model", 60, [] (const char* res) {
		if (res) {
			if (res[0] == '0' && (res[1] == 'x' || res[1] == 'X')) {
				// hash
				spawnPed(res, (DWORD)_strtoui64(res, 0, 0));
			} else {
				std::string str(res);
				if (std::find_if(str.begin(), str.end(), [] (unsigned char c) { return !std::isdigit(c); }) == str.end()) {
					// numeric
					spawnPed(res, (DWORD)atoi(res));
				} else {
					// str
					spawnPed(res, native::getHashKey(res));
				}
			}
		}
	}));

	addOption(breakOption("Peds")
		.addTranslation());

	addOption(submenuOption("Alligators")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelAlligators; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelAlligators); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Panthers")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelPanthers; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelPanthers); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Dogs")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelDogs; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelDogs); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Sparrows")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelSparrows; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelSparrows); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Snakes")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelSnake; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelSnake); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Males")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelMales; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelMales); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Females")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelFemales; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelFemales); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Horses")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.list = global::arrays::g_pedModelHorses; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedModelHorses); spawnPedListMenuVars::vars.listVariants = false; }));

	addOption(submenuOption("Legendary")
		.addTranslation()
		.addSubmenu<spawnPedListMenu>()
		.addOnClick([] { spawnPedListMenuVars::vars.listVariant = global::arrays::g_pedLegendary; spawnPedListMenuVars::vars.size = NUMOF(global::arrays::g_pedLegendary); spawnPedListMenuVars::vars.listVariants = true; }));
}

/*Called while in submenu*/
void spawnPedMenu::update() {}

/*Called once on submenu open*/
void spawnPedMenu::updateOnce() {}

/*Called always*/
void spawnPedMenu::featureUpdate() {
	if (vars.spawnedPeds.size()) {
		for (std::size_t i = 0; i < vars.spawnedPeds.size(); i++) {
			auto& ped = vars.spawnedPeds[i];

			if (!native::doesEntityExist(ped.handle)) {
				vars.spawnedPeds.erase(vars.spawnedPeds.begin() + i);
				continue;
			}

			if (ped.invincibility) {
				native::setEntityInvincible(ped.handle, TRUE);
			}

			if (ped.frozen) {
				native::freezeEntityPosition(ped.handle, TRUE);
			}

			if (ped.creationOverlay) {
				float screenX, screenY;
				if (native::getScreenCoordFromWorldCoord(native::getEntityCoords(ped.handle), &screenX, &screenY)) {
					if (menu::getLocalPlayer().coords.getMathmaticalDistance(native::getEntityCoords(ped.handle)) < 200.f) {
						int ms = (native::getGameTimer() - ped.creationTime) / 1000;
						int hour = ms / 3600;
						int min = (ms % 3600) / 60;
						int sec = ms % 60;

						menu::getRenderer()->drawText(utils::va::va("~e~%s~q~\n%s\n%02dh:%02dm:%02ds", ped.name, native::isEntityDead(ped.handle) ? "Dead" : "Alive", hour, min, sec), { screenX, screenY }, 0.20f, 4, { 255, 255, 255, 255 }, JUSTIFY_CENTER, true);
					}
				}
			}
		}
	}
}

spawnPedMenu* _instance;
spawnPedMenu* spawnPedMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new spawnPedMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

spawnPedMenu::~spawnPedMenu() { delete _instance; }