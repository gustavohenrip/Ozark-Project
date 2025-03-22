#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/panel_manager.hpp"
#include "menu/base/util/player_manager.hpp"

class networkPlayerEspMenu : public menu::submenu {
public:
	static networkPlayerEspMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkPlayerEspMenu()
		: menu::submenu()
	{}

	~networkPlayerEspMenu();
};

namespace networkPlayerEspMenuVars {
	struct vars_ {

	};

	void line(Entity entity, color color);
	void box(Entity entity, color color);
	void screenEsp(Entity entity, color espColor, uint8_t flag = 0);
	bool hasEntityClearLosToEntity(math::vector3<float> coords, Entity entity2);
	void nameEsp(menu::playerVars& player, color espColor);
	extern scrollStruct<uint8_t> screenType[3];
	extern vars_ vars;
}