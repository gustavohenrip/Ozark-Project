#pragma once
#include "menu/base/submenu.hpp"

class MiscCameraAerialMenu : public Menu::Submenu {
public:
	static MiscCameraAerialMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscCameraAerialMenu()
		: Menu::Submenu() {}

	~MiscCameraAerialMenu();
};

namespace MiscCameraAerialMenuVars {
	struct Vars_ {
		bool m_ToggleCamera;
		int m_Distance = 100;
		Cam m_AerialCam;
	};

	extern Vars_ m_Vars;
}