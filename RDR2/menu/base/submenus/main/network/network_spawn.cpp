#include "network_spawn.hpp"
#include "menu/base/submenu_handler.hpp"
#include "players.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/util/control_manager.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace networkSpawnMenuVars;

namespace networkSpawnMenuVars {
	vars_ vars;

	void spawnVehicle(menu::playerVars& player, Hash hash) {
		Vehicle vehicle = 0;
		menu::getControlManager()->requestModel(hash, [&](DWORD modelHash) {
			vehicle = menu::util::getSpawnManager()->spawnVehicle(modelHash, native::getOffsetFromEntityInWorldCoords(player.ped, { 10.f, 0.f, 0.f }), 0.f);
			if (vehicle) {
				native::decorSetBool(vehicle, "wagon_block_honor", TRUE);

				if (vars.effect) {
					native::requestNamedPtfxAsset(native::getHashKey("scr_dis_ligtn_trees"));
					native::useParticleFXAsset("scr_dis_ligtn_trees");

					auto copyCoords = native::getEntityCoords(vehicle);
					copyCoords.z -= 5.f;
					native::startNetworkedParticleFXNonLoopedAtCoord("scr_crackpot_rc_lightening_exp", copyCoords, { 0.f, 0.f, 0.f }, 1.8f, FALSE, FALSE, FALSE);
				}

			}
		});
	}

	//menu::util::getSpawnManager()->spawnPed(hash, (pos + (cam * 8.f)), 0.f);
	void spawnPed(menu::playerVars& player, Hash hash) {
		menu::getControlManager()->requestModel(hash, [=](DWORD modelHash) {
		Ped ped = menu::util::getSpawnManager()->spawnPed(hash, player.coords, 0.f);

		if (vars.effect) {
			native::requestNamedPtfxAsset(native::getHashKey("scr_dis_ligtn_trees"));
			native::useParticleFXAsset("scr_dis_ligtn_trees");

			auto copyCoords = native::getEntityCoords(ped);
			copyCoords.z -= 5.f;
			native::startNetworkedParticleFXNonLoopedAtCoord("scr_crackpot_rc_lightening_exp", copyCoords, { 0.f, 0.f, 0.f }, 1.8f, FALSE, FALSE, FALSE);
		}
		});
	}
}

void networkSpawnMenu::init() {
	setName("Spawn");
	setParentSubmenu<networkPlayersMenu>();

	addOption(toggleOption("Spawn with Effect")
		.addTranslation()
		.addToggle(vars.effect));

	addOption(breakOption("Vehicles")
		.addTranslation());

	addOption(scrollOption<Hash>(SCROLLSELECT, "Trains")
		.addTranslation()
		.addScroll(vars.trains, 0, NUMOF(global::arrays::g_vehicleModelTrain), global::arrays::g_vehicleModelTrain)
		.addOnClick([] { spawnVehicle(menu::getSelectedPlayer(), global::arrays::g_vehicleModelTrain[vars.trains].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Wagons")
		.addTranslation()
		.addScroll(vars.wagons, 0, NUMOF(global::arrays::g_vehicleModelWagon), global::arrays::g_vehicleModelWagon)
		.addOnClick([] { spawnVehicle(menu::getSelectedPlayer(), global::arrays::g_vehicleModelWagon[vars.wagons].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Boats")
		.addTranslation()
		.addScroll(vars.boats, 0, NUMOF(global::arrays::g_vehicleModelBoat), global::arrays::g_vehicleModelBoat)
		.addOnClick([] { spawnVehicle(menu::getSelectedPlayer(), global::arrays::g_vehicleModelBoat[vars.boats].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Stagecoaches")
		.addTranslation()
		.addScroll(vars.stagecoaches, 0, NUMOF(global::arrays::g_vehicleModelStagecoach), global::arrays::g_vehicleModelStagecoach)
		.addOnClick([] { spawnVehicle(menu::getSelectedPlayer(), global::arrays::g_vehicleModelStagecoach[vars.stagecoaches].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Carts")
		.addTranslation()
		.addScroll(vars.carts, 0, NUMOF(global::arrays::g_vehicleModelCart), global::arrays::g_vehicleModelCart)
		.addOnClick([] { spawnVehicle(menu::getSelectedPlayer(), global::arrays::g_vehicleModelCart[vars.carts].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Special")
		.addTranslation()
		.addScroll(vars.special, 0, NUMOF(global::arrays::g_vehicleModelSpecial), global::arrays::g_vehicleModelSpecial)
		.addOnClick([] { spawnVehicle(menu::getSelectedPlayer(), global::arrays::g_vehicleModelSpecial[vars.special].result); }));

	addOption(breakOption("Peds")
		.addTranslation());

	addOption(scrollOption<Hash>(SCROLLSELECT, "Alligators")
		.addTranslation()
		.addScroll(vars.alligators, 0, NUMOF(global::arrays::g_pedModelAlligators), global::arrays::g_pedModelAlligators)
		.addOnClick([] { spawnPed(menu::getSelectedPlayer(), global::arrays::g_pedModelAlligators[vars.alligators].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Dogs")
		.addTranslation()
		.addScroll(vars.dogs, 0, NUMOF(global::arrays::g_pedModelDogs), global::arrays::g_pedModelDogs)
		.addOnClick([] { spawnPed(menu::getSelectedPlayer(), global::arrays::g_pedModelDogs[vars.dogs].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Sparrows")
		.addTranslation()
		.addScroll(vars.sparrows, 0, NUMOF(global::arrays::g_pedModelSparrows), global::arrays::g_pedModelSparrows)
		.addOnClick([] {spawnPed(menu::getSelectedPlayer(), global::arrays::g_pedModelSparrows[vars.sparrows].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Snakes")
		.addTranslation()
		.addScroll(vars.snakes, 0, NUMOF(global::arrays::g_pedModelSnake), global::arrays::g_pedModelSnake)
		.addOnClick([] { spawnPed(menu::getSelectedPlayer(), global::arrays::g_pedModelSnake[vars.snakes].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Males")
		.addTranslation()
		.addScroll(vars.males, 0, NUMOF(global::arrays::g_pedModelMales), global::arrays::g_pedModelMales)
		.addOnClick([] { spawnPed(menu::getSelectedPlayer(), global::arrays::g_pedModelMales[vars.males].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Females")
		.addTranslation()
		.addScroll(vars.females, 0, NUMOF(global::arrays::g_pedModelFemales), global::arrays::g_pedModelFemales)
		.addOnClick([] { spawnPed(menu::getSelectedPlayer(), global::arrays::g_pedModelFemales[vars.females].result); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Females")
		.addTranslation()
		.addScroll(vars.horses, 0, NUMOF(global::arrays::g_pedModelHorses), global::arrays::g_pedModelHorses)
		.addOnClick([] { spawnPed(menu::getSelectedPlayer(), global::arrays::g_pedModelHorses[vars.horses].result); }));
}

/*Called while in submenu*/
void networkSpawnMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkSpawnMenu::updateOnce() {}

/*Called always*/
void networkSpawnMenu::featureUpdate() {}

networkSpawnMenu* _instance;
networkSpawnMenu* networkSpawnMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkSpawnMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkSpawnMenu::~networkSpawnMenu() { delete _instance; }