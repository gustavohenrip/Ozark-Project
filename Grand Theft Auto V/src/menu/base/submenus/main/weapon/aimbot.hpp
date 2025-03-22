#pragma once
#include "menu/base/submenu.hpp"

class AimbotMenu : public Menu::Submenu {
public:
	static AimbotMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AimbotMenu()
		: Menu::Submenu() {}

	~AimbotMenu();
};

namespace AimbotMenuVars {
	struct Vars_ {
		bool m_Toggle;
		bool m_AimingRequired;
		bool m_AutoFire;
		bool m_AutoADS;
		bool m_ShowAimbotTarget;
		bool m_FOV;
		bool m_FOVDraw;

		int m_AimEntity;
		int m_AimFilter;
		int m_AimType;
		int m_AimTag;
		int m_AutoFireDelay;
		int m_AutoFireDelayTimer;
		int m_AimDistance;

		float m_FOVRange = 2.5f;
	};

	extern ScrollStruct<int> AimEntity[3];
	extern Vars_ m_Vars;
}