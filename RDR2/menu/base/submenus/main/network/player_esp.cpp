#include "player_esp.hpp"
#include "menu/base/submenu_handler.hpp"
#include "players.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/raycast.hpp"
#include "all/all_player_esp.hpp"
#include "rage/engine.hpp"

using namespace networkPlayerEspMenuVars;

namespace networkPlayerEspMenuVars {
	vars_ vars;

	scrollStruct<uint8_t> screenType[3] = {
		"Box", 0,
		"Bones", 1,
		"Both", 2
	};

	void line(Entity entity, color color) {
		menu::getRenderer()->drawLine(menu::getLocalPlayer().coords, native::getEntityCoords(entity), color);
	}

	void box(Entity entity, color color) {
		math::vector3<float> minDimensions;
		math::vector3<float> maxDimensions;

		native::getModelDimensions(native::getEntityModel(entity), &minDimensions, &maxDimensions);

		math::vector3<float> diameter = { maxDimensions.x - minDimensions.x, maxDimensions.y - minDimensions.y, maxDimensions.z - minDimensions.z };
		math::vector3<float> radius = { diameter.x / 2, diameter.y / 2, diameter.z / 2 };

		math::vector3<float> faceUpLeft = native::getOffsetFromEntityInWorldCoords(entity, { radius.x * -1, radius.y, radius.z });
		math::vector3<float> faceUpRight = native::getOffsetFromEntityInWorldCoords(entity, { radius.x, radius.y, radius.z });
		math::vector3<float> faceBackLeft = native::getOffsetFromEntityInWorldCoords(entity, { radius.x * -1, radius.y, radius.z * -1 });
		math::vector3<float> faceBackRight = native::getOffsetFromEntityInWorldCoords(entity, { radius.x, radius.y, radius.z * -1 });

		math::vector3<float> backUpLeft = native::getOffsetFromEntityInWorldCoords(entity, { radius.x * -1, radius.y * -1, radius.z });
		math::vector3<float> backUpRight = native::getOffsetFromEntityInWorldCoords(entity, { radius.x, radius.y * -1, radius.z });
		math::vector3<float> backBackLeft = native::getOffsetFromEntityInWorldCoords(entity, { radius.x * -1, radius.y * -1, radius.z * -1 });
		math::vector3<float> backBackRight = native::getOffsetFromEntityInWorldCoords(entity, { radius.x, radius.y * -1, radius.z * -1 });

		menu::getRenderer()->drawLine(faceBackLeft, faceUpLeft, color);
		menu::getRenderer()->drawLine(faceBackRight, faceUpRight, color);
		menu::getRenderer()->drawLine(backBackLeft, backUpLeft, color);
		menu::getRenderer()->drawLine(backBackRight, backUpRight, color);
		menu::getRenderer()->drawLine(faceUpLeft, faceUpRight, color);
		menu::getRenderer()->drawLine(faceBackLeft, faceBackRight, color);
		menu::getRenderer()->drawLine(backUpLeft, backUpRight, color);
		menu::getRenderer()->drawLine(backBackLeft, backBackRight, color);

		menu::getRenderer()->drawLine(faceUpLeft, backUpLeft, color);
		menu::getRenderer()->drawLine(faceUpRight, backUpRight, color);
		menu::getRenderer()->drawLine(faceBackLeft, backBackLeft, color);
		menu::getRenderer()->drawLine(faceBackRight, backBackRight, color);
	}

	bool hasEntityClearLosToEntity(math::vector3<float> coords, Entity entity2) {
		math::vector3<float> points[2];
		points[0] = coords;
		points[1] = native::getPedBoneCoords(entity2, 0x5226, 0.f, 0.f, 0.f);
		menu::raycast cast = menu::raycastPointToPoint(points);
		
		if (cast.didHit()) {
			if (cast.getEntity() == entity2) {
				return true;
			}
		}

		return false;
	}

	void screenEsp2(Entity entity, color espColor) {
		math::vector2<float> screen[3];
		auto head = native::getOffsetFromEntityInWorldCoords(entity, { .0f, .0f, .35f });
		auto feet = native::getOffsetFromEntityInWorldCoords(entity, { .0f, .0f, -.35f });
		if (rage::engine::worldToScreen(head, &screen[0].x, &screen[0].y)
			&& rage::engine::worldToScreen(feet, &screen[1].x, &screen[1].y)
			&& rage::engine::worldToScreen(native::getEntityCoords(entity), &screen[2].x, &screen[2].y)) {
			color visColor = hasEntityClearLosToEntity(native::getGameplayCamCoord(), entity) ? espColor : color(242, 56, 56, 130);
			native::drawRect(screen[2].x, screen[2].y, ((screen[0].y - screen[1].y) / 2.f), (screen[0].y - screen[1].y) * 2.f, visColor.r, visColor.g, visColor.b, 130);
		}
	}

	

	void screenEsp(Entity entity, color espColor, uint8_t flag) {
		static Hash bones[5] = { 0x5226, 36029, 6286, 65245, 35502 };
		math::vector2<float> screen[3];
		if (native::isEntityOnScreen(entity) && native::vDist(menu::getLocalPlayer().coords, native::getEntityCoords(entity)) < 150.f) {
			if (flag == 0 || flag == 2) {
				static float _increments[3] = { .0f,  .37f, -.37f };
				for (std::size_t i = 0; i < 3; i++) {
					auto coords = native::getOffsetFromEntityInWorldCoords(entity, { .0f, .0f, _increments[i] });
					rage::engine::worldToScreen(coords, &screen[i].x, &screen[i].y);
				}
				color visColor = hasEntityClearLosToEntity(native::getGameplayCamCoord(), entity) ? espColor : color(242, 56, 56, 130);
				auto wh = (screen[1].y - screen[2].y);
				if (wh < 1.f && wh > -1.f) {
					native::drawRect(screen[0].x, screen[0].y, wh / 1.8f, wh * 2.f, visColor.r, visColor.g, visColor.b, 130);
				}
			}

			if (flag == 1 || flag == 2) {
				for (std::size_t i = 0; i < NUMOF(bones); i++) {
					auto coords = native::getPedBoneCoords(entity, bones[i], 0.f, 0.f, 0.f);
					if (rage::engine::worldToScreen(coords, &screen[0].x, &screen[0].y)) {
						static float increments[2] = { .09f, -.09f };
						for (size_t j = 1; j < 3; j++) {
							auto temp = coords;
							temp.z += increments[j - 1];
							rage::engine::worldToScreen(temp, &screen[j].x, &screen[j].y);
						}
						color visColor = hasEntityClearLosToEntity(native::getGameplayCamCoord(), entity) ? espColor : color(242, 56, 56, 255);
						auto wh = (screen[0].y - screen[1].y);
						if (wh < 1.f && wh > -1.f) {
							menu::getRenderer()->drawSprite("generic_textures", "tick_box", screen[0], { wh, (wh * 2.f) }, .0f, visColor);
						}
					}
				}
			}
		}
	}

	void nameEsp(menu::playerVars& player, color espColor) {
		if (native::isEntityOnScreen(player.ped)) {
			auto coords = native::getOffsetFromEntityInWorldCoords(player.ped, { .0f, .0f, .55f });
			if (rage::engine::worldToScreen(coords, &player.screen.x, &player.screen.y)) {
				menu::getRenderer()->drawText(player.name, player.screen, 0.25f, 4, espColor, JUSTIFY_CENTER);
			}
		}
	}
}

void networkPlayerEspMenu::init() {
	setName("ESP");
	setParentSubmenu<networkPlayersMenu>();

	addOption(toggleOption("Name")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().espName)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer().espName); }));

	addOption(toggleOption("Line")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().espLine)
		.addOnUpdate([](toggleOption* option) { option->addToggle(menu::getSelectedPlayer().espLine); }));

	addOption(scrollOption<uint8_t>(TOGGLE, "2D")
		.addTranslation()
		.addScroll(menu::getSelectedPlayer()._2dBoxType, 0, NUMOF(screenType) - 1, screenType)
		.addToggle(menu::getSelectedPlayer()._2dbox)
		.addOnUpdate([](scrollOption<uint8_t>* option) { option->addScroll(menu::getSelectedPlayer()._2dBoxType, 0, NUMOF(screenType), screenType)
			.addToggle(menu::getSelectedPlayer()._2dbox); }));

	addOption(toggleOption("3D Box")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer()._3dbox)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(menu::getSelectedPlayer()._3dbox); }));

	addOption(breakOption("Name Color")
		.addTranslation()
		.addRequirement([] { return menu::getSelectedPlayer().espName; }));

	addOption(numberOption<int>(SCROLL, "[R] Red")
		.addTranslation()
		.addNumber(menu::getSelectedPlayer().espNameColor.r, "%i", 1).addMin(0).addMax(255)
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(menu::getSelectedPlayer().espNameColor.r, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(menu::getSelectedPlayer().espNameColor); })
		.addRequirement([] { return menu::getSelectedPlayer().espName; }));

	addOption(numberOption<int>(SCROLL, "[G] Green")
		.addTranslation()
		.addNumber(menu::getSelectedPlayer().espNameColor.g, "%i", 1).addMin(0).addMax(255)
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(menu::getSelectedPlayer().espNameColor.g, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(menu::getSelectedPlayer().espNameColor); })
		.addRequirement([] { return menu::getSelectedPlayer().espName; }));

	addOption(numberOption<int>(SCROLL, "[B] Blue")
		.addTranslation()
		.addNumber(menu::getSelectedPlayer().espNameColor.b, "%i", 1).addMin(0).addMax(255)
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(menu::getSelectedPlayer().espNameColor.b, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(menu::getSelectedPlayer().espNameColor); })
		.addRequirement([] { return menu::getSelectedPlayer().espName; }));

	addOption(breakOption("Line Color")
		.addTranslation()
		.addRequirement([] { return menu::getSelectedPlayer().espLine || menu::getSelectedPlayer()._2dbox || menu::getSelectedPlayer()._3dbox; }));

	addOption(numberOption<int>(SCROLL, "[R] Red")
		.addTranslation()
		.addNumber(menu::getSelectedPlayer().espLineColor.r, "%i", 1).addMin(0).addMax(255)
		.addOnUpdate([] (numberOption<int>* option) { option->addNumber(menu::getSelectedPlayer().espLineColor.r, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(menu::getSelectedPlayer().espLineColor); })
		.addRequirement([] { return menu::getSelectedPlayer().espLine || menu::getSelectedPlayer()._2dbox || menu::getSelectedPlayer()._3dbox; }));

	addOption(numberOption<int>(SCROLL, "[G] Green")
		.addTranslation()
		.addNumber(menu::getSelectedPlayer().espLineColor.g, "%i", 1).addMin(0).addMax(255)
		.addOnUpdate([](numberOption<int>* option) { option->addNumber(menu::getSelectedPlayer().espLineColor.g, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(menu::getSelectedPlayer().espLineColor); })
		.addRequirement([] { return menu::getSelectedPlayer().espLine || menu::getSelectedPlayer()._2dbox || menu::getSelectedPlayer()._3dbox; }));

	addOption(numberOption<int>(SCROLL, "[B] Blue")
		.addTranslation()
		.addNumber(menu::getSelectedPlayer().espLineColor.b, "%i", 1).addMin(0).addMax(255)
		.addOnUpdate([](numberOption<int>* option) { option->addNumber(menu::getSelectedPlayer().espLineColor.b, "%i", 1).addMin(0).addMax(255).setScrollSpeed(2).canLoop(); })
		.addOnHover([] { menu::getRenderer()->previewColor(menu::getSelectedPlayer().espLineColor); })
		.addRequirement([] { return menu::getSelectedPlayer().espLine || menu::getSelectedPlayer()._2dbox || menu::getSelectedPlayer()._3dbox; }));
}

/*Called while in submenu*/
void networkPlayerEspMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkPlayerEspMenu::updateOnce() {}

/*Called always*/
void networkPlayerEspMenu::featureUpdate() {
	menu::getPlayerManager()->foreach([=](menu::playerVars& player) {
		if (player.espLine && !networkAllEspMenuVars::vars.lines) {
			line(player.ped, player.espLineColor);
		}

		if (player._2dbox && !networkAllEspMenuVars::vars._2dbox) {
			screenEsp(player.ped, player.espLineColor,screenType[player._2dBoxType].result);
		}

		if (player.espName && !networkAllEspMenuVars::vars.nameESP) {
			nameEsp(player, player.espNameColor);
		}

		if (player._3dbox && !networkAllEspMenuVars::vars._3dbox) {
			box(player.ped, player.espLineColor);
		}
	});
}

networkPlayerEspMenu* _instance;
networkPlayerEspMenu* networkPlayerEspMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkPlayerEspMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkPlayerEspMenu::~networkPlayerEspMenu() { delete _instance; }