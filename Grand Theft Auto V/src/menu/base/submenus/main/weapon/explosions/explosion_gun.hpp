#pragma once
#include "menu/base/submenu.hpp"

class ExplosionGunMenu : public Menu::Submenu {
public:
	static ExplosionGunMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ExplosionGunMenu()
		: Menu::Submenu() {}

	~ExplosionGunMenu();
};

namespace ExplosionGunMenuVars {
	struct Vars_ {
		int m_Type;
		int m_ChangeOwnerValue;
		int m_ListSize;
		bool m_Toggle;
		bool m_Audible = true;
		bool m_Invisible = false;
		bool m_ChangeOwner = false;
		float m_Radius = 10.f;
		float m_CameraShake = 0.f;
		ScrollStruct<Ped> m_ChangeOwnerList[32];
	};

	extern Vars_ m_Vars;
}