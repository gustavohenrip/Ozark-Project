  #include "protection.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../../network.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/control_manager.hpp"
#include "rage/entity/entity.hpp"
#include "utils/log.hpp"
#include "utils/va.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "event_reactions.hpp"

using namespace networkProtectionMenuVars;

namespace networkProtectionMenuVars {
	vars_ vars;

	scrollStruct<int> joinPresence[] = {
		"Friends", 0,
		"RID Joiners", 1,
		"Both", 2
	};

	scrollStruct<int> smartIncludes[] = {
		"Modders", 0,
		"Ozark", 1,
		"Both", 2
	};

	scrollStruct<int> maliciousIncludes[] = {
		"Block", 0,
		"Redirect", 1,
	};

	bool isWeaponModel(Object object) {
		for (int i = 0; i < 29; i++) {
			if (object == native::getCurrentPedWeaponEntityIndex(menu::getLocalPlayer().ped, i)) {
				return true;
			}
		}

		return false;
	}
}

void networkProtectionMenu::init() {
	setName("Protection");
	setParentSubmenu<networkMenu>();

	addOption(submenuOption("Event Reactions")
		.addTranslation()
		.addSubmenu<networkEventReactionsMenu>());

	addOption(numberOption<int>(TOGGLE, "Fake Lag")
		.addTranslation()
		.addToggle(vars.fakeLag)
		.addNumber(vars.choke, "%ims Choke", 1).addMin(5).addMax(50).canBeSaved(name.getOriginal()));

	/*addOption(scrollOption<int>(TOGGLE, "Smart Protection")
		.addTranslation()
		.addScroll(vars.smartProtectionIncludes, 0, NUMOF(smartIncludes), smartIncludes)
		.addToggle(vars.smartProtection).canBeSaved(name.getOriginal())
		.addOnClick([] { if(vars.smartProtection) menu::getNotify()->right("Blocking ~e~everything~q~ from potential modders"); }));*/

	//addOption(toggleOption("Spoof Location")
	//	.addTranslation()
	//	.addOnClick([] { global::vars::g_spoofLocationPed = 0; })
	//	.addToggle(vars.spoofLocation).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Notify Attacks")
		.addTranslation()
		.addToggle(vars.notifyAttacks).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Redirect Attacks")
		.addTranslation()
		.addToggle(vars.redirectAttacks).canBeSaved(name.getOriginal()));

	addOption(breakOption("Network Events")
		.addTranslation());

	addOption(toggleOption("Give Control")
		.addTranslation()
		.addToggle(vars.giveControl).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Clock")
		.addTranslation()
		.addToggle(vars.clock).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Weather")
		.addTranslation()
		.addToggle(vars.weather).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Fire")
		.addTranslation()
		.addToggle(vars.fire).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Explosion")
		.addTranslation()
		.addToggle(vars.explosion).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Paralyse")
		.addTranslation()
		.addToggle(vars.paralyse).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Reports")
		.addTranslation()
		.addToggle(vars.stats).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Particles")
		.addTranslation()
		.addToggle(vars.particles).canBeSaved(name.getOriginal()));

	addOption(breakOption("Entity")
		.addTranslation());

	addOption(toggleOption("Attachment")
		.addTranslation()
		.addToggle(vars.attachment).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Clone")
		.addTranslation()
		.addToggle(vars.clone).canBeSaved(name.getOriginal()));


	addOption(scrollOption<int>(TOGGLE, "Malicious Entities")
		.addTranslation()
		.addScroll(vars.maliciousEntityIncludes, 0, NUMOF(maliciousIncludes), maliciousIncludes)
		.addToggle(vars.maliciousEntities).canBeSaved(name.getOriginal()));

	addOption(breakOption("Presence")
		.addTranslation());

	addOption(toggleOption("Notify Joining")
		.addTranslation()
		.addToggle(vars.presenceNotifyJoins).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Rockstar Message")
		.addTranslation()
		.addToggle(vars.presenceRockstarMessage).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Crew Message")
		.addTranslation()
		.addToggle(vars.presenceCrewMessage).canBeSaved(name.getOriginal()));

	addOption(toggleOption("Force Join")
		.addTranslation()
		.addToggle(vars.presenceForceJoin).canBeSaved(name.getOriginal()));

	addOption(scrollOption<int>(TOGGLE, "Joining")
		.addTranslation()
		.addScroll(vars.presenceJoinSelection, 0, NUMOF(joinPresence), joinPresence)
		.addToggle(vars.presenceJoin).canBeSaved(name.getOriginal()));
}

/*Called while in submenu*/
void networkProtectionMenu::update() {}

/*Called once on submenu open*/
void networkProtectionMenu::updateOnce() {}

/*Called always*/
void networkProtectionMenu::featureUpdate() {
	//if (vars.spoofLocation) {
	//	if (!global::vars::g_spoofLocationPed) {
	//		global::vars::g_spoofLocationPed = native::clonePed(menu::getLocalPlayer().ped, native::getEntityHeading(menu::getLocalPlayer().ped), TRUE, FALSE);
	//		native::setEntityCoords(global::vars::g_spoofLocationPed, { 4503.726f, -635.393f, 71.421f });
	//		native::freezeEntityPosition(global::vars::g_spoofLocationPed, TRUE);
	//		native::setEntityVisible(global::vars::g_spoofLocationPed, FALSE);
	//	}
	//}

	if (native::networkIsSessionActive() && menu::getLocalPlayer().connected) {

		//maliciousObjQueue
		if (vars.maliciousEntities) {
			if (!vars.maliciousObjQueue.empty()) {
				auto netObj = vars.maliciousObjQueue.back().first;
				auto& player = menu::getPlayer(vars.maliciousObjQueue.back().second);
				if (netObj) {
					if (netObj->physical) {
						auto handle = rage::engine::getEntityHandleFromAddress(netObj->physical);
						if (native::doesEntityExist(handle)) {
							if (native::vDist(native::getEntityCoords(handle), menu::getLocalPlayer().coords) < 15.f) {
								if (rage::entity::getEntityTask()->getNetObjOwner(handle) != menu::getLocalPlayer().id) {
									rage::entity::getEntityTask()->changeNetObjOwner(handle, menu::getLocalPlayer().id);
								} else {
									if (networkProtectionMenuVars::vars.notifyAttacks) {
										menu::getNotify()->right(std::string(player.name) + " tried to spawn a ~e~malicious~q~ entity on you");
									}
									if (networkProtectionMenuVars::vars.maliciousEntityIncludes == 0) {
										if (networkProtectionMenuVars::vars.notifyAttacks) {
											menu::getNotify()->right("Deleting...");
										}
										native::setEntityAsMissionEntity(handle, TRUE, TRUE);
										native::setEntityCoords(handle, { 4503.726f, -635.393f, 71.421f });
										native::deleteEntity2(&handle);
									} else {
										if (networkProtectionMenuVars::vars.notifyAttacks) {
											menu::getNotify()->right("Redirecting...");
										}
										native::setEntityCoords(handle, native::getEntityCoords(player.ped));
									}
									vars.maliciousObjQueue.pop_back();
								}
							}
						}
					}
				}
			}
		}

		if (vars.clone) {
			static int timer = 0;
			menu::util::getTimers()->runTimedFunction(&timer, 1000, [] {
				menu::pools::getPedPool()->run([] (Ped ped) {
					int owner = rage::entity::getEntityTask()->getNetObjOwner(ped);
					if (owner && owner != menu::getLocalPlayer().id) {
						if (ped != menu::getLocalPlayer().ped && ped != menu::getLocalPlayer().entity) {
							if (native::doesEntityExist(ped)) {
								if (!native::isPedAPlayer(ped)) {
									if (!native::isEntityDead(ped)) {
										if (native::getEntityModel(ped) == native::getEntityModel(menu::getLocalPlayer().ped)) {
											if (native::getEntityCoords(ped).getMathmaticalDistance(menu::getLocalPlayer().coords) < 25) {
												menu::getControlManager()->requestControl(ped, [] (Ped p, int oldOwner) {
													if (oldOwner <= 0) return;

													if (vars.notifyAttacks) {
														menu::getNotify()->right(utils::va::va("~e~%s~q~ tried to clone you", menu::getPlayer(oldOwner).name));
													}

													native::setEntityAsMissionEntity(p, TRUE, TRUE);
													native::detachEntity(p, TRUE, TRUE);
													native::setEntityCoords(p, { 4503.726f, -635.393f, 71.421f });
													native::deleteEntity(&p);

													if (vars.redirectAttacks) {
														native::clonePed(menu::getPlayer(oldOwner).ped, 0.f, TRUE, FALSE);
													}
												}, true);
											}
										}
									}
								}
							}
						}
					}
				});
			});
		}

		if (vars.attachment) {
			static int timer = 0;
			menu::util::getTimers()->runTimedFunction(&timer, 250, [] {
				auto pool = menu::pools::getAttachmentPool()->getPoolPointer();
				if (pool && pool->count && pool->size && pool->pool && pool->unk) {
					auto entityPool = rage::engine::decryptEntityPool(global::vars::g_encryptedEntityPool.table1, global::vars::g_encryptedEntityPool.table2, global::vars::g_encryptedEntityPool.addition);

					for (int i = 0; i < pool->count; i++) {
						if (entityPool) {
							if (*(uint32_t*)(entityPool + 0x18) - (*(uint32_t*)(entityPool + 0x28) & 0x3FFFFFFF) <= 256)
								break;
						}

						if (~(*(uint8_t*)(pool->unk + i) >> 7) & 1) {
							uint64_t objectPtr = pool->pool + (i * pool->size);
							if (objectPtr) {
								uint64_t pedPtr = *(uint64_t*)objectPtr;
								if (pedPtr == rage::engine::getPedFactory()->ped) {
									uint64_t attachedEntity = *(uint64_t*)(objectPtr + 0x18);
									if (attachedEntity) {
										auto handle = rage::engine::getEntityHandleFromAddress(attachedEntity);
										if (handle) {
											if (!isWeaponModel(handle)) {
												Hash model = native::getEntityModel(handle);
												if (model != 0xF5C1611E && model != 0xA7AF20C0) {
													menu::getControlManager()->requestControl(handle, [=] (Entity o, int oldOwner) {
														if (oldOwner > 0 && oldOwner <= 32) {
															if (vars.notifyAttacks) {
																menu::getNotify()->right(utils::va::va("~e~%s~q~ tried to attach an entity to you (0x%X)", menu::getPlayer(oldOwner).name, model));
															}

															native::setEntityAsMissionEntity(o, TRUE, TRUE);
															native::detachEntity(o, TRUE, TRUE);

															if (vars.redirectAttacks) {
																native::attachEntityToEntity(o, menu::getPlayer(oldOwner).ped, 0, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, FALSE, TRUE, TRUE, TRUE, 0, TRUE, TRUE, TRUE);
															} else {
																native::setEntityCoords(o, { 4503.726f, -635.393f, 71.421f });
																native::deleteEntity(&o);
															}
														}
													}, true);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			});
		}
	}
}

networkProtectionMenu* _instance;
networkProtectionMenu* networkProtectionMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkProtectionMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkProtectionMenu::~networkProtectionMenu() { delete _instance; }