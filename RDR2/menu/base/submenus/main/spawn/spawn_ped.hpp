#pragma once
#include "menu/base/submenu.hpp"

class spawnPedMenu : public menu::submenu {
public:
	static spawnPedMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnPedMenu()
		: menu::submenu()
	{}

	~spawnPedMenu();
};

namespace spawnPedMenuVars {
	struct spawnedPedData {
		char name[60];
		Hash hash;
		Ped handle;
		int creationTime;
		bool invincibility;
		float scale = 1.f;
		bool creationOverlay;
		bool frozen;
		math::vector3<float> origin;
	};

	struct vars_ {
		std::vector<spawnedPedData> spawnedPeds;
	};

	void spawnPed(std::string visibleName, Hash hash, int variant = 0);
	extern vars_ vars;
}