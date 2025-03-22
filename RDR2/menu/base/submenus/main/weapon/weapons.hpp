#pragma once
#include "menu/base/submenu.hpp"

class weaponsMenu : public menu::submenu {
public:
	static weaponsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	weaponsMenu()
		: menu::submenu()
	{}

	~weaponsMenu();
};

namespace weaponsMenuVars {
	struct vars_ {
		int ammoVar;
		int immobileWeapons;
	};

	void givePedWeapon(Ped ped, Hash weapon, int ammo);
	void giveAmmoToPed(Ped ped, Hash ammo);
	void dropAmmo(Ped ped);

	extern vars_ vars;
}