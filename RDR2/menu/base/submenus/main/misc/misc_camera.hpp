#pragma once
#include "menu/base/submenu.hpp"

class miscCameraMenu : public menu::submenu {
public:
	static miscCameraMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	miscCameraMenu()
		: menu::submenu()
	{}

	~miscCameraMenu();
};

namespace miscCameraMenuVars {
	struct vars_ {
		bool cameraZoom;
		bool freecam;
		bool birdsEyeView;
		int freecamHandle;
		int birdsEyeViewHandle;
		math::vector3<float> lastFreecamCoords;
		math::vector3<float> lastFreecamRotation;
		float cameraZoomVal;
	};

	extern vars_ vars;
}