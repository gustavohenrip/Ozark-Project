#pragma once
#include "menu/base/submenu.hpp"

class weaponModsMenu : public menu::submenu {
public:
	static weaponModsMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	weaponModsMenu()
		: menu::submenu()
	{}

	~weaponModsMenu();
};

namespace weaponModsMenuVars {
	struct vars_ {
		bool infiniteAmmo;
		bool infiniteAmmo2;
		bool neverReload;
		bool neverDegrade;
		bool forceGun;
		bool rapidFire;
		bool smiteGun;
		bool lightningStrikeGun;
		bool deleteGun;
		bool hashGun;
		bool weaponDamage;
		float weaponDamageVal = 1.f;
		bool meleeDamage;
		float meleeDamageVal = 1.f;
		float defenseDamageVal = 1.f;
		bool defibrillatorGun;
		bool shrinkGun;
		bool teleportGun;
		bool explosiveAmmo;
		bool instantKill;
		bool throwableGun;
		int currentThrowable;
	};
	void smitePed(Ped ped);
	extern vars_ vars;
}