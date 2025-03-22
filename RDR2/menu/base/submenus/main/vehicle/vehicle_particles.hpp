#pragma once
#include "menu/base/submenu.hpp"

class vehicleParticlesMenu : public menu::submenu {
public:
	static vehicleParticlesMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	vehicleParticlesMenu()
		: menu::submenu()
	{}

	~vehicleParticlesMenu();
};

namespace vehicleParticlesMenuVars {
	struct particleInfo {
		const char* displayName;
		const char* boneName;
		int boneIndex;
		bool enabled;
	};

	struct vars_ {
		bool enabled;
		int particle;
		float scale = 1.f;
		std::vector<particleInfo> values;
	};

	extern vars_ vars;
}