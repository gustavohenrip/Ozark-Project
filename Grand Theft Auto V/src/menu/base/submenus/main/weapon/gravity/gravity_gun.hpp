#pragma once
#include "menu/base/submenu.hpp"

class GravityGunMenu : public Menu::Submenu {
public:
	static GravityGunMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	GravityGunMenu()
		: Menu::Submenu() {}

	~GravityGunMenu();
};

namespace GravityGunMenuVars {
	struct Vars_ {
		bool m_Toggle;
		bool m_Attraction;
		bool m_Peds;
		bool m_Vehicles;
		bool m_Objects;

		float m_AttractionDistance = 10.f;
		float m_Force = 4.f;
		float m_Distance = 10.f;

		Entity m_Entity;
	};

	extern Vars_ m_Vars;
}