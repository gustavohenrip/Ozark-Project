#pragma once
#include "menu/base/submenu.hpp"

class ParticleGunMenu : public Menu::Submenu {
public:
	static ParticleGunMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ParticleGunMenu()
		: Menu::Submenu() {}

	~ParticleGunMenu();
};

namespace ParticleGunMenuVars {
	struct Vars_ {
		bool m_Toggle;
		float m_Scale = 1.f;
		RadioContext m_Radio = { "commonmenu", "shop_makeup_icon" };
	};

	extern Vars_ m_Vars;
}