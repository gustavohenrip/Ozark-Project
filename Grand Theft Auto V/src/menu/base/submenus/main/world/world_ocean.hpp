#pragma once
#include "menu/base/submenu.hpp"

class WorldOceanMenu : public Menu::Submenu {
public:
	static WorldOceanMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldOceanMenu()
		: Menu::Submenu() {}

	~WorldOceanMenu();
};

namespace WorldOceanMenuVars {
	struct Vars_ {
		bool m_DisableOcean;
		bool m_ClearOcean;
		bool m_FierceOcean;
		bool m_PartWater;

		float m_WaveIntensity;
	};

	enum eOceanTasks {
		OCEAN_DISABLE,
		OCEAN_CLEAR,
		OCEAN_FIERCE
	};

	enum eOceanTypes {
		OCEAN_TYPE_OCEAN,
		OCEAN_TYPE_WAVE,
		OCEAN_TYPE_CALMING
	};

	void OceanLoop(eOceanTypes Type, std::function<void(uint64_t)> Callback);
	void OceanTask(eOceanTypes Type, eOceanTasks Task);
	extern Vars_ m_Vars;
}