#pragma once
#include "menu/base/submenu.hpp"

class ParticleHandTrailsMenu : public Menu::Submenu {
public:
	static ParticleHandTrailsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ParticleHandTrailsMenu()
		: Menu::Submenu() {}

	~ParticleHandTrailsMenu();
};

namespace ParticleHandTrailsMenuVars {
	struct Vars_ {
		bool m_Trails;
		int m_TrailBoneLeft;
		int m_TrailBoneRight;

		Color m_TrailColor = { 255, 0, 0, 255 };
	};

	extern Vars_ m_Vars;
}