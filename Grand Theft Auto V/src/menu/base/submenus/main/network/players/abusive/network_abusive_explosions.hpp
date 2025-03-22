#pragma once
#include "menu/base/submenu.hpp"

class NetworkAbusiveExplosionsMenu : public Menu::Submenu {
public:
	static NetworkAbusiveExplosionsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAbusiveExplosionsMenu()
		: Menu::Submenu() {}

	~NetworkAbusiveExplosionsMenu();
};

namespace NetworkAbusiveExplosionsMenuVars {
	struct Vars_ {
		int m_ExplosionType;
		int m_ExplosionVisual;
		float m_ExplosionRadius;
		float m_ExplosionCameraShake;
		bool m_ExplosionSound;
		bool m_ExplosionLoop;
	};

	extern Vars_ m_Vars;
}