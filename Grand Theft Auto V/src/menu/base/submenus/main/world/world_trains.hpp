#pragma once
#include "menu/base/submenu.hpp"

class WorldTrainMenu : public Menu::Submenu {
public:
	static WorldTrainMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldTrainMenu()
		: Menu::Submenu() {}

	~WorldTrainMenu();
};

namespace WorldTrainMenuVars {
	struct Vars_ {
		bool m_AllowRandom = true;
		bool m_Spawn;
		bool m_Derail;
		bool m_Speed;

		int m_SpeedVal = 9;
		int m_Paint;
		Vehicle m_Train = 0;
	};

	extern Vars_ m_Vars;
}