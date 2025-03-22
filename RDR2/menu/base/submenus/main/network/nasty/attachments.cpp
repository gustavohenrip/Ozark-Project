#include "attachments.hpp"
#include "menu/base/submenu_handler.hpp"
#include "nasty.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/spawn_manager.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace networkAttachmentsMenuVars;

namespace networkAttachmentsMenuVars {
	vars_ vars;

	scrollStruct<Hash> pedToThem[] = {
		{ "Blackwater Obese", 0x41907533 },
		{ "Rhodes Obese", 0x602993B2 },
		{ "Saint Denis Obese", 0x36E3135B },
		{ "Alligator 1", 0xA0B33A7B },
		{ "Coyote", 0x1CA6B883 },
	};

	scrollStruct<Hash> vehicleToThem[] = {
		"Steam Train", 0x6F8F7EE4,
		"Police Wagon", 0xB203C6B3,
		"Gatling Gun", 0x1EEBDBE5,
		"Hot Air Balloon", 0x5EB0BAE0,
	};

	void attach(menu::playerVars& player, int type) {
		if (type == 1) {
			native::attachEntityToEntity(menu::getLocalPlayer().entity, menu::getSelectedPlayer().entity, 0, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, FALSE, TRUE, TRUE, TRUE, 0, TRUE, TRUE, TRUE);
		}

		if (type == 2) {
			menu::getControlManager()->requestModel(pedToThem[menu::getSelectedPlayer().attachmentPedToThem].result, [] (DWORD model) {
				Ped ped = menu::util::getSpawnManager()->spawnPed(model, menu::getSelectedPlayer().coords, 0.f);
				if (ped) {
					native::attachEntityToEntity(ped, menu::getSelectedPlayer().entity, 0, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, FALSE, TRUE, TRUE, TRUE, 0, TRUE, TRUE, TRUE);
					menu::getSelectedPlayer().attachedPeds.push_back(ped);
				}
			});
		}

		if (type == 3) {
			menu::getControlManager()->requestModel(vehicleToThem[menu::getSelectedPlayer().attachmentVehicleToThem].result, [] (DWORD model) {
				Vehicle veh = menu::util::getSpawnManager()->spawnVehicle(model, menu::getSelectedPlayer().coords, 0.f);
				if (veh) {
					native::attachEntityToEntity(veh, menu::getSelectedPlayer().entity, 0, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, FALSE, TRUE, TRUE, TRUE, 0, TRUE, TRUE, TRUE);
					menu::getSelectedPlayer().attachedVehicles.push_back(veh);
				}
			});
		}
	}

	bool configRequirement() {
		return menu::getSelectedPlayer().attachedPeds.size() >= 1
			|| menu::getSelectedPlayer().attachedVehicles.size() >= 1
			|| native::isEntityAttachedToEntity(menu::getLocalPlayer().ped, menu::getSelectedPlayer().ped);
	}
}

void networkAttachmentsMenu::init() {
	setName("Attachments");
	setParentSubmenu<networkNastyMenu>();

	addOption(buttonOption("You to Them")
		.addTranslation()
		.addOnClick([] { attach(menu::getSelectedPlayer(), 1); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Ped to Them")
		.addTranslation()
		.addScroll(menu::getSelectedPlayer().attachmentPedToThem, 0, NUMOF(pedToThem), pedToThem)
		.addOnUpdate([] (scrollOption<Hash>* option) { option->addScroll(menu::getSelectedPlayer().attachmentPedToThem, 0, NUMOF(pedToThem), pedToThem); })
		.addOnClick([] { attach(menu::getSelectedPlayer(), 2); }));

	addOption(scrollOption<Hash>(SCROLLSELECT, "Vehicle to Them")
		.addTranslation()
		.addScroll(menu::getSelectedPlayer().attachmentVehicleToThem, 0, NUMOF(vehicleToThem), vehicleToThem)
		.addOnUpdate([] (scrollOption<Hash>* option) { option->addScroll(menu::getSelectedPlayer().attachmentVehicleToThem, 0, NUMOF(vehicleToThem), vehicleToThem); })
		.addOnClick([] { attach(menu::getSelectedPlayer(), 3); }));

	addOption(breakOption("Config")
		.addTranslation()
		.addRequirement(configRequirement));

	addOption(buttonOption("Detach (All Peds)")
		.addTranslation()
		.addOnClick([] {
			for (std::size_t i = 0; i < menu::getSelectedPlayer().attachedPeds.size(); i++) {
				native::setEntityAsMissionEntity(menu::getSelectedPlayer().attachedPeds[i], TRUE, TRUE);
				native::detachEntity(menu::getSelectedPlayer().attachedPeds[i], 1, 1);
			}

			menu::getSelectedPlayer().attachedPeds.clear();
		})
		.addRequirement([] { return menu::getSelectedPlayer().attachedPeds.size() >= 1; }));

	addOption(buttonOption("Detach (All Vehicles)")
		.addTranslation()
		.addOnClick([] {
			for (std::size_t i = 0; i < menu::getSelectedPlayer().attachedVehicles.size(); i++) {
				native::setEntityAsMissionEntity(menu::getSelectedPlayer().attachedVehicles[i], TRUE, TRUE);
				native::detachEntity(menu::getSelectedPlayer().attachedVehicles[i], 1, 1);
			}

			menu::getSelectedPlayer().attachedVehicles.clear();
		})
		.addRequirement([] { return menu::getSelectedPlayer().attachedVehicles.size() >= 1; }));

	addOption(buttonOption("Detach (You)")
		.addTranslation()
		.addOnClick([] { if (native::isEntityAttached(menu::getLocalPlayer().entity)) native::detachEntity(menu::getLocalPlayer().entity, TRUE, TRUE); })
		.addRequirement([] { return native::isEntityAttachedToEntity(menu::getLocalPlayer().ped, menu::getSelectedPlayer().ped); }));
}

/*Called while in submenu*/
void networkAttachmentsMenu::update() {
	if (menu::getBase()->currentOption > options.size()) {
		menu::getBase()->currentOption = 0;
		menu::getBase()->scrollOffset = 0;
	}

	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkAttachmentsMenu::updateOnce() {
}

/*Called always*/
void networkAttachmentsMenu::featureUpdate() {}

networkAttachmentsMenu* _instance;
networkAttachmentsMenu* networkAttachmentsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAttachmentsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAttachmentsMenu::~networkAttachmentsMenu() { delete _instance; }