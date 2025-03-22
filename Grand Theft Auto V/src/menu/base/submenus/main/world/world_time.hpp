#pragma once
#include "menu/base/submenu.hpp"

class WorldTimeMenu : public Menu::Submenu {
public:
	static WorldTimeMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldTimeMenu()
		: Menu::Submenu() {}

	~WorldTimeMenu();
};

namespace WorldTimeMenuVars {
	struct Vars_ {
		bool m_AlwaysDay;
		bool m_AlwaysNight;
		bool m_Freeze;
		bool m_Accelerate;
		bool m_Epilepsy;
		bool m_SyncWithSystem;
		bool m_Sync;

		int m_FreezeTime[3];
		int m_Timescale;
	};

	void SyncTime();
	extern Vars_ m_Vars;
}