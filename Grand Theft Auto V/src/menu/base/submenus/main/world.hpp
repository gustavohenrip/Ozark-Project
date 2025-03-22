#pragma once
#include "menu/base/submenu.hpp"

class WorldMenu : public Menu::Submenu {
public:
	static WorldMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldMenu()
		: Menu::Submenu() {}

	~WorldMenu();
};

namespace WorldMenuVars {
	struct Tracer {
		uint32_t m_SpawnTime;
		Math::Vector3_<float> m_SpawnPosition;
		Math::Vector3_<float> m_EndPosition;
		bool m_Populated = false;
	};

	struct Vars_ {
		bool m_DisableLights;
		bool m_RenderWaterOnMap;
		bool m_WorldWaypoint;

		bool m_DisableSky;
		float m_Fog;

		bool m_WorldTracers;
		int m_TracerCurrentIndex = 0;
		Tracer m_Tracers[500];
	};

	extern Vars_ m_Vars;
}