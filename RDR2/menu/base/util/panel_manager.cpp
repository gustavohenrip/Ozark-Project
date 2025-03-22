#include "panel_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/submenus/main/network/player_esp.hpp"
#include "menu/base/submenus/main/settings/video_settings.hpp"
#include "menu/base/submenus/main/network.hpp"
#include "menu/base/submenus/main/network/overseer.hpp"
#include "menu/base/submenus/main/misc.hpp"
#include "menu/base/util/timers.hpp"

namespace menu {
	panel::panel(const char* title, color color) {
		auto manager = getPanelManager();
		int maxCount = manager->panelOptionCountLeft;

		if (manager->panelOptionCountRight > maxCount) {
			maxCount = manager->panelOptionCountRight;
		}

		manager->panelOptionCountLeft = manager->panelTickLeft - 1;
		manager->panelOptionCountRight = manager->panelOptionCountRight - 1;

		float x = global::uivars::menuX >= .40f ? global::uivars::menuX - .283f : global::uivars::menuX + .234f;
		float xText = global::uivars::menuX >= .40f ? global::uivars::menuX - .28f : global::uivars::menuX + .2372f;
		// bg
		getRenderer()->drawRect({ x, (global::uivars::menuY - .127f) + 0.0315f }, { 0.28f, (0.0315f * maxCount) }, { 0, 0, 0, 180 });

		// bg addition (bottom
		getRenderer()->drawRect({ x, (global::uivars::menuY - .127f) + (0.0315f * (maxCount + 1)) }, { 0.28f, 0.0315f / 4.f }, { 0, 0, 0, 180 });

		// bg line
		// getRenderer()->drawRect({ 0.417f + 0.28f, 0.2f + 0.0315f }, { 0.001f, (0.0315f * maxCount) + (0.0315f / 4.f) }, { 255, 255, 255, 255 });

		// title bg
		getRenderer()->drawRect({ x, (global::uivars::menuY - .127f) }, { 0.28f, 0.0315f }, { 0, 0, 0, 180 });

		// title bg line
		// getRenderer()->drawRect({ 0.417f + 0.28f, 0.2f }, { 0.001f, 0.0315f }, { 255, 255, 255, 255 });
		getRenderer()->drawText(title, { xText, global::uivars::menuY - .124f }, 0.4f, 6, color);

		manager->panelTickLeft = 1;
		manager->panelTickRight = 1;
	}

	void panel::item(const char* name, const char* value) {
		auto manager = getPanelManager();
		if (manager->panelTickLeft > manager->panelTickRight) {
			panelRightItem(name, value);
		} else {
			panelLeftItem(name, value);
		}
	}

	void panel::item(const char* name, const char* spriteLeft, const char* spriteRight, color rgba) {
		auto manager = getPanelManager();
		if (manager->panelTickLeft > manager->panelTickRight) {
			panelRightSpriteItem(name, spriteLeft, spriteRight, rgba);
		} else {
			panelLeftSpriteItem(name, spriteLeft, spriteRight, rgba);
		}
	}

	void panel::itemFull(const char* name, const char* value) {
		auto manager = getPanelManager();
		if (manager->panelTickLeft != manager->panelTickRight) {
			if (manager->panelTickLeft > manager->panelTickRight) {
				manager->panelTickRight = manager->panelTickLeft;
			} else {
				manager->panelTickLeft = manager->panelTickRight;
			}
		}
		float wrap = global::uivars::menuX >= .40f ? 1.0f - (1.0f - (global::uivars::menuX + (0.28f / 2.f)) + .1470f) : 1.0f - (1.0f - ((global::uivars::menuX + .5170f) + (0.28f / 2.f)) + .1470f);
		float x = global::uivars::menuX >= .40f ? global::uivars::menuX - .28f : global::uivars::menuX + .237f;
		// draw name
		getRenderer()->drawText(name, { x, (global::uivars::menuY - .127f) + (manager->panelTickLeft * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 });

		// draw value
		getRenderer()->drawText(value, { 0.f, (global::uivars::menuY - .127f) + (manager->panelTickLeft * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 }, JUSTIFY_RIGHT, false, { 0.f, wrap });

		manager->panelTickLeft++;
		manager->panelTickRight++;
	}

	void panel::panelLeftItem(const char* name, const char* value) {
		auto manager = getPanelManager();

		float x = global::uivars::menuX >= .40f ? global::uivars::menuX - .283f : global::uivars::menuX + .24f;
		float x2 = global::uivars::menuX >= .40f ? global::uivars::menuX - .28f : global::uivars::menuX + .237f;
		float wrap = global::uivars::menuX >= .40f ? 1.0f - (1.0f - ((global::uivars::menuX - .1392f) + (0.28f / 2.f)) + .1470f) : 1.0f - (1.0f - ((global::uivars::menuX + .3844f) + (0.28f / 2.f)) + .1470f);
		// draw divider
		getRenderer()->drawRect({ x + 0.14f, (global::uivars::menuY - .127f) + (manager->panelTickLeft * 0.0315f) }, { 0.001f, 0.0315f }, { 74, 127, 184, 255 });

		// draw name
		getRenderer()->drawText(name, { x2, (global::uivars::menuY - .127f) + (manager->panelTickLeft * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 });

		// draw value
		getRenderer()->drawText(value, { 0.f, (global::uivars::menuY - .127f) + (manager->panelTickLeft * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 }, JUSTIFY_RIGHT, false, { 0.f, wrap });

		manager->panelTickLeft++;
	}

	void panel::panelRightItem(const char* name, const char* value) {
		auto manager = getPanelManager();
		float x = global::uivars::menuX >= .40f ? global::uivars::menuX - .283f : global::uivars::menuX + .24f;
		float wrap = global::uivars::menuX >= .40f ? 1.0f - (1.0f - (global::uivars::menuX + (0.28f / 2.f)) + .1470f) : 1.0f - (1.0f - ((global::uivars::menuX + .5170f) + (0.28f / 2.f)) + .1470f);

		// draw name
		getRenderer()->drawText(name, { x + 0.14f + 0.004f, (global::uivars::menuY - .127f) + (manager->panelTickRight * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 });

		// draw value
		getRenderer()->drawText(value, { 0.f, (global::uivars::menuY - .127f) + (manager->panelTickRight * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 }, JUSTIFY_RIGHT, false, { 0.f, wrap });

		manager->panelTickRight++;
	}

	void panel::panelLeftSpriteItem(const char* name, const char* spriteLeft, const char* spriteRight, color rgba) {
		auto manager = getPanelManager();
		float x = global::uivars::menuX >= .40f ? global::uivars::menuX - .283f : global::uivars::menuX + .24f;
		float x2 = global::uivars::menuX >= .40f ? global::uivars::menuX - .275f : global::uivars::menuX + .2421f;
		float x3 = global::uivars::menuX >= .40f ? global::uivars::menuX - .28f : global::uivars::menuX + .2368f;
		// draw divider
		getRenderer()->drawRect({ x + 0.14f, (global::uivars::menuY - .127f) + (manager->panelTickLeft * 0.0315f) }, { 0.001f, 0.0315f }, { 74, 127, 184, 255 });

		// sprite
		getRenderer()->drawSprite(spriteLeft, spriteRight, { x2, (global::uivars::menuY - .127f) + 0.017f + (manager->panelTickLeft * 0.0315f) }, { 0.007f, 0.011f }, 0.f, rgba);

		// draw name
		getRenderer()->drawText(name, { x3 + 0.011f, (global::uivars::menuY - .127f) + (manager->panelTickLeft * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 });

		manager->panelTickLeft++;
	}

	void panel::panelRightSpriteItem(const char* name, const char* spriteLeft, const char* spriteRight, color rgba) {
		auto manager = getPanelManager();
		float x = global::uivars::menuX >= .40f ? global::uivars::menuX - .144f : global::uivars::menuX + .24f;
		float x2 = global::uivars::menuX >= .40f ? global::uivars::menuX - .1285f : global::uivars::menuX + .24f;

		// sprite
		getRenderer()->drawSprite(spriteLeft, spriteRight, { x, (global::uivars::menuY - .127f) + 0.017f + (manager->panelTickRight * 0.0315f) }, { 0.007f, 0.011f }, 0.f, rgba);

		// draw name
		getRenderer()->drawText(name, { x2, (global::uivars::menuY - .127f) + (manager->panelTickRight * 0.0315f) + 0.006f }, 0.315f, 4, { 255, 255, 255, 255 });

		manager->panelTickRight++;
	}

	void panelManager::panelEntity(const char* title, Entity handle, int creationTime, int type) {
		if (!getBase()->panels) return;
		if (!native::doesEntityExist(handle)) return;

		int ms = (native::getGameTimer() - creationTime) / 1000;
		int hour = ms / 3600;
		int min = (ms % 3600) / 60;
		int sec = ms % 60;

		panel p(title);

		color rgba(0, 0, 0);
		if (type == 1) {
			rgba = color(204, 0, 0); // red
		} else if (type == 2) {
			rgba = color(254, 243, 144); // yellow
		} else if (type == 3) {
			rgba = color(255, 159, 116); // orange
		}

		p.item(utils::va::va("%s", type == 1 ? "Ped" : type == 2 ? "Vehicle" : "Object"), "generic_textures", "counter_bg_1b", rgba);
		p.item("Age", utils::va::va("%02dh:%02dm:%02ds", hour, min, sec));
		p.item("Model", utils::va::va("0x%X", native::getEntityModel(handle)));
		p.item("Distance (Feet)", utils::va::va("%.2f", native::getEntityCoords(handle).getMathmaticalDistance(menu::getLocalPlayer().coords)));
		p.item("Height (Above Ground)", utils::va::va("%.2f", native::getEntityHeightAboveGround(handle)));
		p.item("Speed", utils::va::va("%.0f %s", native::getEntitySpeed(handle) * miscMenuVars::metric[miscMenuVars::vars.selectedMetric].result, miscMenuVars::metric[miscMenuVars::vars.selectedMetric].name.c_str()));
		p.item("Visible", native::isEntityOnScreen(handle) ? "~t6~Yes" : "~e~No");

		int health = native::getEntityHealth(handle);
		int maxHealth = native::getEntityMaxHealth(handle, FALSE);

		if (health == 0 && maxHealth == 0) {
			p.item("Health", "~m~Invalid");
		} else if (health == 0 && maxHealth != 0) {
			p.item("Health", "~e~Dead");
		} else if (health == maxHealth) {
			p.item("Health", "~t6~Full");
		} else {
			p.item("Health", utils::va::va("~o~%.0f%%", (float)((health * 100) / maxHealth)));
		}

		float heading = native::getEntityHeading(handle);
		if (heading >= 0.f && heading <= 90.f) {
			p.item("Heading", utils::va::va("North (%.0f)", heading));
		} else if (heading >= 90.f && heading <= 180.f) {
			p.item("Heading", utils::va::va("East (%.0f)", heading));
		} else if (heading >= 180.f && heading <= 270.f) {
			p.item("Heading", utils::va::va("South (%.0f)", heading));
		} else if (heading >= 270.f && heading <= 360.f) {
			p.item("Heading", utils::va::va("West (%.0f)", heading));
		} else p.item("Heading", "~m~Invalid");
		
		p.item("Attached", native::isEntityAttached(handle) ? "~t6~Yes" : "~e~No");

		// ped
		if (type == 1) {
			Hash weaponHash;
			native::getCurrentPedWeapon(handle, &weaponHash, FALSE, FALSE, FALSE);

			bool rendered = false;
			if (weaponHash) {
				auto name = native::getWeaponName(weaponHash);
				if (name) {
					name = native::getLabelText(name);
					if (name) {
						rendered = true;
						p.itemFull("Weapon", name);
					}
				}
			}

			if (!rendered) {
				p.itemFull("Weapon", "~m~Unarmed");
			}
		}
		
		// vehicle
		if (type == 2) {
			bool rendered = false;

			Ped driverPed = native::getPedInVehicleSeat(handle, -1);
			if (driverPed && native::doesEntityExist(driverPed)) {
				if (native::isPedAPlayer(driverPed)) {
					rendered = true;
					if (driverPed == menu::getLocalPlayer().ped) {
						p.itemFull("Vehicle Driver", menu::getLocalPlayer().name);
					} else {
						p.itemFull("Vehicle Driver", utils::va::va("Player - %s", menu::getPlayer(native::networkGetPlayerIndexFromPed(driverPed)).name));
					}
				} else {
					rendered = true;
					p.itemFull("Vehicle Driver", utils::va::va("Ped (0x%X)", native::getEntityModel(driverPed)));
				}
			}

			if (!rendered) {
				p.itemFull("Vehicle Driver", "~m~None");
			}

			p.item("Free Seats", native::areAnyVehicleSeatsFree(handle) ? "~t6~Yes" : "~e~No");
		}

		auto coords = native::getEntityCoords(handle);
		auto rotation = native::getEntityRotation(handle, 2);

		p.item("Pos:X", utils::va::va("%.2f", coords.x));
		p.item("Pos:Y", utils::va::va("%.2f", coords.y));
		p.item("Pos:Z", utils::va::va("%.2f", coords.z));
		p.item("Rot:X", utils::va::va("%.2f", rotation.x));
		p.item("Rot:Y", utils::va::va("%.2f", rotation.y));
		p.item("Rot:Z", utils::va::va("%.2f", rotation.z));
	}

	void panelManager::panelPool(const char* title, entityPool* pool, int type) {
		if (!getBase()->panels) return;

#undef max
		if (pool) {
			int visibleCount = 0;
			int count = 0;
			float closestDistance = std::numeric_limits<float>::max();

			for (int i = 0; i < pool->count; i++) {
				if (~(*(uint8_t*)(pool->unk + i) >> 7) & 1) {
					count++;
				}
			}

			color rgba(0, 0, 0);
			if (type == 1) {
				rgba = color(204, 0, 0); // red

				pools::getPedPool()->run([&] (Ped ped) {
					if (native::isEntityOnScreen(ped)) visibleCount++;
					auto distance = native::getEntityCoords(ped).getMathmaticalDistance(menu::getLocalPlayer().coords);
					if (distance < closestDistance) {
						closestDistance = distance;
					}
				});
			} else if (type == 2) {
				rgba = color(254, 243, 144); // yellow

				pools::getVehiclePool()->run([&] (Vehicle vehicle) {
					if (native::isEntityOnScreen(vehicle)) visibleCount++;
					auto distance = native::getEntityCoords(vehicle).getMathmaticalDistance(menu::getLocalPlayer().coords);
					if (distance < closestDistance) {
						closestDistance = distance;
					}
				});
			} else if (type == 3) {
				rgba = color(255, 159, 116); // orange

				pools::getObjectPool()->run([&] (Object object) {
					if (native::isEntityOnScreen(object)) visibleCount++;
					auto distance = native::getEntityCoords(object).getMathmaticalDistance(menu::getLocalPlayer().coords);
					if (distance < closestDistance) {
						closestDistance = distance;
					}
				});
			} else if (type == 4) {
				rgba = color(0, 159, 116); // green

				pools::getPickupPool()->run([&] (Pickup pickup) {
					if (native::isEntityOnScreen(pickup)) visibleCount++;
					auto distance = native::getEntityCoords(pickup).getMathmaticalDistance(menu::getLocalPlayer().coords);
					if (distance < closestDistance) {
						closestDistance = distance;
					}
				});
			}

			if (closestDistance == std::numeric_limits<float>::max()) {
				closestDistance = 0.f;
			}

			panel p(title, rgba);
			p.item(utils::va::va("%s Pool", type == 1 ? "Ped" : type == 2 ? "Vehicle" : type == 3 ? "Object" : "Pickup"), "generic_textures", "counter_bg_1b", rgba);
			p.item("Count", utils::va::va("%i", count));
			p.item("On Screen", utils::va::va("%i", visibleCount));
			p.item("Closest (Feet)", utils::va::va("%.2f", closestDistance));
		}
	}

	void panelManager::panelPlayer(Player id) {
		auto& player = menu::getPlayer(id);
		if (!player.espLine) {
			networkPlayerEspMenuVars::line(player.entity, player.espLineColor);
		}

		if (!getBase()->panels) return;

		panel p(player.name, { 244, 60, 88, 255 });
		// menu::globals(1139395).at(player.id, 34).as<int>() old
		//  std::string(utils::va::va("%i", menu::globals(1145091).at(player.id, 35).as<int>())).c_str() old
		p.item("Rank", "Needs Updating");

		int health = native::getEntityHealth(player.ped);
		int maxHealth = native::getEntityMaxHealth(player.ped, TRUE);

		if (health == 0 && maxHealth == 0) {
			p.item("Health", "~m~Invalid");
		} else if (health == 0 && maxHealth != 0) {
			p.item("Health", "~e~Dead");
		} else if (health == maxHealth) {
			p.item("Health", "~t6~Full");
		} else {
			p.item("Health", std::string(utils::va::va("~o~%i%%", ((health * 100) / maxHealth))).c_str());
		}

		auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
		if (net && net->playerInfo) {
			p.item("Posse", std::string(net->posseName).empty() ? "~m~None" : net->posseName);
		} else p.item("Posse", "~m~None");

		p.item("Visible", native::isEntityOnScreen(player.ped) ? "~t6~Yes" : "~e~No");
		p.item("Model", std::string(utils::va::va("0x%X", native::getEntityModel(player.ped))).c_str());
		p.item("Height (Above Ground)", std::string(utils::va::va("%.2f", native::getEntityHeightAboveGround(player.ped))).c_str());
		p.item("Distance (Feet)", std::string(utils::va::va("%.2f", player.coords.getMathmaticalDistance(menu::getLocalPlayer().coords))).c_str());
		p.item("Speed", std::string(utils::va::va("%.0f %s", native::getEntitySpeed(player.ped) * miscMenuVars::metric[miscMenuVars::vars.selectedMetric].result, miscMenuVars::metric[miscMenuVars::vars.selectedMetric].name.c_str())).c_str());

		float heading = native::getEntityHeading(player.ped);
		if (heading >= 0.f && heading <= 90.f) {
			p.item("Heading", std::string(utils::va::va("North (%.0f)", heading)).c_str());
		} else if (heading >= 90.f && heading <= 180.f) {
			p.item("Heading", std::string(utils::va::va("East (%.0f)", heading)).c_str());
		} else if (heading >= 180.f && heading <= 270.f) {
			p.item("Heading", std::string(utils::va::va("South (%.0f)", heading)).c_str());
		} else if (heading >= 270.f && heading <= 360.f) {
			p.item("Heading", std::string(utils::va::va("West (%.0f)", heading)).c_str());
		} else p.item("Heading", "~m~Invalid");

		int ammo = native::getAmmoInPedWeapon(player.ped, player.weaponHash);
		p.item("Ammo", ammo > 0 ? std::to_string(ammo).c_str() : "~m~None");

		bool rendered = false;
		if (player.weaponHash) {
			auto name = native::getWeaponName(player.weaponHash);
			if (name) {
				name = native::getLabelText(name);
				if (name) {
					rendered = true;
					p.itemFull("Weapon", name);
				}
			}
		}

		if (!rendered) {
			p.itemFull("Weapon", "~m~Unarmed");
		}

		p.item("On Mount", native::isPedOnMount(player.ped) ? "~t6~Yes" : "~e~No");
		p.item("Is Aiming", native::isPlayerFreeAiming(player.id) ? "~t6~Yes" : "~e~No");

		auto rotation = native::getEntityRotation(player.ped, 2);
		p.item("Pos:X", std::string(utils::va::va("%.2f", player.coords.x)).c_str());
		p.item("Rot:X", std::string(utils::va::va("%.2f", rotation.x)).c_str());
		p.item("Pos:Y", std::string(utils::va::va("%.2f", player.coords.y)).c_str());
		p.item("Rot:Y", std::string(utils::va::va("%.2f", rotation.y)).c_str());
		p.item("Pos:Z", std::string(utils::va::va("%.2f", player.coords.z)).c_str());
		p.item("Rot:Z", std::string(utils::va::va("%.2f", rotation.z)).c_str());

		if (net && net->playerInfo) {
			p.item("IP", (videoSettingsMenuVars::vars.hideIPs || player.id == getLocalPlayer().id) ? "~m~Hidden" : std::string(utils::va::va("%i.%i.%i.%i", net->playerInfo->netInfo.externalIp[3], net->playerInfo->netInfo.externalIp[2], net->playerInfo->netInfo.externalIp[1], net->playerInfo->netInfo.externalIp[0])).c_str());
			p.item("RID", videoSettingsMenuVars::vars.hideRID ? "~m~Hidden" : std::string(utils::va::va("%i", net->playerInfo->netInfo.rockstarId)).c_str());
		} else {
			p.item("IP", "~m~Hidden");
			p.item("RID", "~m~Hidden");
		}

		if (!videoSettingsMenuVars::vars.hideIPs) {
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					uint32_t ip = *(uint32_t*)(&net->playerInfo->netInfo.externalIp);
					if (global::vars::g_relayUsage.find(net->playerInfo->netInfo.rockstarId2) != global::vars::g_relayUsage.end()) {
						if (global::vars::g_geo.find(global::vars::g_relayUsage[net->playerInfo->netInfo.rockstarId2]) == global::vars::g_geo.end()) {
							global::vars::g_geoQueue.push_back(global::vars::g_relayUsage[net->playerInfo->netInfo.rockstarId2]);
						} else {
							ip = global::vars::g_relayUsage[net->playerInfo->netInfo.rockstarId2];
						}
					}

					auto it = global::vars::g_geo.find(ip);
					if (it != global::vars::g_geo.end()) {
						p.item("Zip", it->second.zip.c_str());
						p.item("Timezone", it->second.timezone.c_str());
						p.item("Region", it->second.regionName.c_str());
						p.item("City", it->second.city.c_str());
						p.item("Country", it->second.country.c_str());
						p.item("Country Code", it->second.countryCode.c_str());
						if (it->second.isp.length()) p.itemFull("ISP", it->second.isp.c_str());
						if (it->second.org.length()) p.itemFull("Org", it->second.org.c_str());
					}
				}
			}
		}

		// notes
		menu::getPlayerManager()->updateNotes(id);

		if (!player.notes.empty()) {
			player.modder = true;
			p.itemFull("", (std::string("~t8~") + player.notes).c_str());
		}
	}

	void panelManager::panelFile(std::string name, struct stat st) {
		if (!getBase()->panels) return;

		auto created = localtime(&st.st_ctime);
		auto modified = localtime(&st.st_mtime);

		char createdBuffer[100];
		char modifiedBuffer[100];
		
		strftime(createdBuffer, 100, "%A %d %B %T %p", created);
		strftime(modifiedBuffer, 100, "%A %d %B %T %p", modified);

		panel p("Theme", color(255, 234, 0));
		p.itemFull("Name", name.c_str());
		p.itemFull("Created", createdBuffer);
		p.itemFull("Last Modified", modifiedBuffer);
	}

	void panelManager::panelOverseer(std::string rid, std::string name, std::string reason, int flags) {
		if (!getBase()->panels) return;

		panel p("Overseer", color(187, 0, 255));

		p.item("Name", name.c_str());
		p.item("RID", rid.c_str());
		p.itemFull("Reason Added", reason.c_str());

		p.item("Enabled", (flags & OVERSEER_ENABLED) ? "~t6~YES" : "~e~NO");
		p.item("Notify", (flags & OVERSEER_NOTIFY) ? "~t6~YES" : "~e~NO");
		p.item("Crash", (flags & OVERSEER_CRASH) ? "~t6~YES" : "~e~NO");
		p.item("Explode Loop", (flags & OVERSEER_EXPLODE_LOOP) ? "~t6~YES" : "~e~NO");
		p.item("Paralyse", (flags & OVERSEER_PARALYSE) ? "~t6~YES" : "~e~NO");

		bool inSession = false;
		menu::getPlayerManager()->foreach([&] (menu::playerVars& player) {
			auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					if (info->netInfo.rockstarId2 == atoi(rid.c_str())) {
						inSession = true;
					}
				}
			}
		}, true);

		p.item("In Session", inSession ? "~t6~YES" : "~e~NO");
	}

	void panelManager::panelRecentPlayer(std::string name, std::string first, std::string last, std::string ip, std::string rid) {
		if (!getBase()->panels) return;

		panel p("Recent Player", color(187, 0, 255));

		p.item("Name", name.c_str());
		p.item("RID", rid.c_str());
		p.item("IP", ip.c_str());
		p.item("Online", global::vars::g_onlineRockstarIds[std::stoull(rid)] ? "~t6~YES" : "~e~NO");

		bool inSession = false;
		menu::getPlayerManager()->foreach([&] (menu::playerVars& player) {
			auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					if (info->netInfo.rockstarId2 == atoi(rid.c_str())) {
						inSession = true;
					}
				}
			}
		}, true);

		p.item("In Session", inSession ? "~t6~YES" : "~e~NO");

		p.itemFull("First Encountered", first.c_str());
		p.itemFull("Last Encountered", last.c_str());
	}

	void panelManager::panelFriend(std::string name, std::string rid, networkHandle netHandle, bool inRdr2) {
		if (!getBase()->panels) return;

		panel p(name.c_str(), color(0, 206, 209));

		p.item("RID", rid.c_str());
		p.item("Multiplayer", _i<BOOL>(0x665161D250850A9F, &netHandle.handle[0]) ? "~t6~YES" : "~e~NO");
		p.item("Online",  _i<BOOL>(0xE348D1404BD80146, &netHandle.handle[0]) ? "~t6~YES" : "~e~NO");

		bool inSession = false;
		menu::getPlayerManager()->foreach([&](menu::playerVars& player) {
			auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					if (info->netInfo.rockstarId2 == atoi(rid.c_str())) {
						inSession = true;
					}
				}
			}
			}, true);

		p.item("In Session", inSession ? "~t6~YES" : "~e~NO");
		p.itemFull("In Red Dead redemption 2", inRdr2 ? "~t6~YES" : "~e~NO");

	}

	void panelManager::panelPresence(std::string name, std::string rid) {
		if (!getBase()->panels) return;

		panel p(name.c_str(), color(0, 206, 209));

		p.item("RID", rid.c_str());
		p.item("Online", global::vars::g_onlineRockstarIds[std::stoull(rid)] ? "~t6~YES" : "~e~NO");

		bool inSession = false;
		menu::getPlayerManager()->foreach([&](menu::playerVars& player) {
			auto net = rage::engine::getNetGamePlayerFromIndex(player.id);
			if (net) {
				auto info = net->playerInfo;
				if (info) {
					if (info->netInfo.rockstarId2 == atoi(rid.c_str())) {
						inSession = true;
					}
				}
			}
			}, true);

		p.itemFull("In Session", inSession ? "~t6~YES" : "~e~NO");

	}

	panelManager* getPanelManager() {
		static panelManager instance;
		return &instance;
	}
}