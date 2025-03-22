#pragma once
#include "menu/base/submenu.hpp"

class WeaponModsMenu : public Menu::Submenu {
public:
	static WeaponModsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WeaponModsMenu()
		: Menu::Submenu() {}

	~WeaponModsMenu();
};

namespace WeaponModsMenuVars {
	struct Vars_ {
		bool m_InfiniteAmmo;
		bool m_InstantKill;
		bool m_RapidFire;
		bool m_LaserSight;
		bool m_ExplosiveAmmo;
		bool m_IncendiaryAmmo;
		bool m_DeleteGun;
		bool m_ForceGun;
		bool m_TeleportGun;
		bool m_AirstrikeGun;
		bool m_DefibrillatorGun;
		bool m_MoneyGun;
		bool m_ClownGun;
		bool m_ShrinkGun;
		bool m_DeadEyeGun;
		bool m_RopeGun;
		bool m_VehicleStealerGun;
		bool m_HashGun;
		bool m_ValkyrieGun;
		bool m_ValkryieRunning;

		Object m_ValkyrieObject;
		Cam m_ValkyrieCam;

		int m_ValkyrieTimer = 0;
		int m_TargetingMode;

		std::vector<std::pair<uint32_t, uint32_t>> m_WeaponGroups;

		Entity m_RopeGunEntities[2];
		Object m_RopeGunObject;
	};

	extern Vars_ m_Vars;
}