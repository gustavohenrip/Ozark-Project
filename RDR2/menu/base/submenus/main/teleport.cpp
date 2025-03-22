#include "teleport.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "teleport/popular_locations.hpp"
#include "teleport/post_office_locations.hpp"
#include "teleport/shop_locations.hpp"
#include "teleport/train_locations.hpp"
#include "teleport/directional.hpp"
#include "teleport/collectibles.hpp"
#include "menu/base/util/raycast.hpp"
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"
#include "teleport/save_and_load_teleport.hpp"
#include "menu/base/util/notify.hpp"

using namespace teleportMenuVars;

namespace teleportMenuVars {
	vars_ vars;

	void teleportToLocation(math::vector3<float> coords) {
		if (coords.x == 0.f) return;

		menu::util::getFiberPool()->run([=] {
			float z = 0.f;
			if (coords.z) {
				z = coords.z;
				goto teleport;
			}

			uint32_t timeout = GetTickCount() + 6000;
			float zIncrement = 10.f;
			while (!native::getGroundZFor3dCoord(coords.x, coords.y, coords.z + zIncrement, &z, 0) && timeout > GetTickCount()) {
				native::setFocusPosAndLevel(math::vector3<float>(coords.x, coords.y, coords.z + zIncrement), { 0.f,0.f,0.f });
				menu::util::waitFiber(60);
				zIncrement += 5.f;
			}
		teleport:
			native::setEntityCoords(menu::getLocalPlayer().entity, { coords.x, coords.y, z });
			native::clearFocus();
		});
	}

	Player getClosestPlayer() {
		struct data { int index; float dist; };
		data data;
		data.index = 1;
		data.dist = native::vDist(menu::getLocalPlayer().coords, native::getEntityCoords(native::getPlayerPed(1)));
		for (Player i = 1; i < 0x20; i++) {
			if (native::networkIsPlayerConnected(i) && i != menu::getLocalPlayer().id) {
				float dist = native::vDist(menu::getLocalPlayer().coords, native::getEntityCoords(native::getPlayerPed(i)));
				if (dist < data.dist) {
					data.index = i;
					data.dist = dist;
				}
			}
		}
		return data.index;
	}
}

void teleportMenu::init() {
	setName("Teleport");
	setParentSubmenu<mainMenu>();

	addOption(submenuOption("Save and Load")
		.addTranslation()
		.addSubmenu<saveLoadTeleportMenu>());

	addOption(toggleOption("Auto Waypoint")
		.addTranslation()
		.addToggle(vars.autoWaypoint).canBeSaved(name.getOriginal()));

	addOption(buttonOption("To Closest Player")
		.addTranslation()
		.addOnClick([] { teleportToLocation(native::getEntityCoords(native::getPlayerPed(getClosestPlayer()))); }));

	addOption(buttonOption("To Last Mount")
		.addTranslation()
		.addOnClick([] { math::vector3<float> mount = native::getEntityCoords(native::getLastPedMount(menu::getLocalPlayer().ped)); teleportToLocation(mount); }));

	addOption(submenuOption("Directional")
		.addTranslation()
		.addSubmenu<directionalMenu>());

	addOption(buttonOption("To Madam Nazar")
		.addOnClick([] {
			if (menu::getLocalPlayer().entity) {
				auto coords = menu::globals(1051439).at(4661).at(3).as<math::vector3<float>>();
				native::setEntityCoords(menu::getLocalPlayer().entity, coords);
			}
		}));

	addOption(buttonOption("To my Camp")
		.addOnClick([] {
			if (menu::getLocalPlayer().entity) {
				auto camp = menu::globals(1134390).at(native::playerId(), 83).at(38).at(3).as<int32_t>();
				auto coords = menu::globals(1131196).at(camp, 27).at(20).as<math::vector3<float>>();
				native::setEntityCoords(menu::getLocalPlayer().entity, coords);
			}
		}));

	addOption(buttonOption("To Moonshine Shack")
		.addOnClick([] {
			if (menu::getLocalPlayer().entity) {
				std::unordered_map<int32_t, math::vector3<float>> shacks;
				shacks[0] = { 1783.4565f, -823.0287f,  42.6925f };
				shacks[1] = { -1086.47f, 694.842f, 104.22f };
				shacks[2] = { -2777.22f, -3051.89f, 11.58f };
				shacks[3] = { 1624.38f, 830.30f, 145.21f };
				shacks[4] = { -1859.86f, -1730.06f, -109.52f };
				if (auto index = menu::globals(1298536).at(native::playerId(), 87).at(5).at(12).as<int32_t>() != -1) {
					native::setEntityCoords(menu::getLocalPlayer().entity, shacks[index]);
				} else {
					menu::getNotify()->right("~e~Error~q~: You do not own a moonshine shack.");
				}
			}
		}));

	/*
	int ped = PLAYER::PLAYER_PED_ID();    
                if (PED::IS_PED_IN_ANY_VEHICLE(ped, false))
                    ped = PLAYER::GET_PLAYERS_LAST_VEHICLE();
                else if (PED::IS_PED_ON_MOUNT(ped))
                    ped = PED::GET_MOUNT(ped);
                Vector3 coords = { 0.f, 0.f, 0.f };
                int shacknumber = YSCGlobal(1298536).Add(PLAYER::PLAYER_ID(), 87).Add(5).Add(12).As<int>();
                if (shacknumber == -1)
                {
                    Game::Print::PrintStringBottomCentre("buy shak");
                    return;
                }
                switch (shacknumber)
                {
                case 0:
                    coords = { 1783.4565, -823.0287,  42.6925 };
                    break;
                case 1:
                    coords = { -1086.47f, 694.842f, 104.22f };
                    break;
                case 2:
                    coords = { -2777.22f, -3051.89f, 11.58f };
                    break;
                case 3:
                    coords = { 1624.38f, 830.30f, 145.21f };
                    break;
                case 4:
                    coords = { -1859.86f, -1730.06f, -109.52f };
                    break;
                }
                if (coords.x != 0.f && coords.y != 0.f && coords.z != 0.f)
                {
                    ENTITY::SET_ENTITY_COORDS(ped, coords.x, coords.y + 1.0f, coords.z, false, false, false, false);
                }
	
	*/

	addOption(breakOption("Locations")
		.addTranslation());

	addOption(submenuOption("Collectibles")
		.addTranslation()
		.addSubmenu<collectiblesMenu>());

	addOption(submenuOption("Popular")
		.addTranslation()
		.addSubmenu<popularLocationsMenu>());

	addOption(submenuOption("Post Office")
		.addTranslation()
		.addSubmenu<postOfficeLocationsMenu>());

	addOption(submenuOption("Shops")
		.addTranslation()
		.addSubmenu<shopLocationsMenu>());

	addOption(submenuOption("Train Stations")
		.addTranslation()
		.addSubmenu<trainLocationsMenu>());
}

/*Called while in submenu*/
void teleportMenu::update() {}

/*Called once on submenu open*/
void teleportMenu::updateOnce() {}

/*Called always*/
void teleportMenu::featureUpdate() {
	if (vars.autoWaypoint) {
		if (native::isWaypointActive() && !vars.hasTeleportedFlag) {
			math::vector3<float> waypoint = native::getWaypointCoords();
			teleportToLocation(waypoint);
			vars.hasTeleportedFlag = true;
			vars.waypointTimer = GetTickCount() + 3000;
		}

		if (vars.waypointTimer < GetTickCount()) {
			vars.hasTeleportedFlag = false;
		}
	}
}

teleportMenu* _instance;
teleportMenu* teleportMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new teleportMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

teleportMenu::~teleportMenu() { delete _instance; }