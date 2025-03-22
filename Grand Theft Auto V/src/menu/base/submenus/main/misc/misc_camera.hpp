#pragma once
#include "menu/base/submenu.hpp"

class MiscCameraMenu : public Menu::Submenu {
public:
	static MiscCameraMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscCameraMenu()
		: Menu::Submenu() {}

	~MiscCameraMenu();
};

namespace MiscCameraMenuVars {
	struct Vars_ {
		bool m_Freecam;
		bool m_CameraZoom;

		float m_CameraZoomVal = 1.f;
		Cam m_FreecamHandle;
	};

	extern Vars_ m_Vars;
}