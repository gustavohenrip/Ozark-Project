#pragma once
#include "menu/base/submenu.hpp"

class WorldMapMenu : public Menu::Submenu {
public:
	static WorldMapMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldMapMenu()
		: Menu::Submenu() {}

	~WorldMapMenu();
};

namespace WorldMapMenuVars {
	struct Vars_ {
		bool m_NorthYankton;
		bool m_Carrier;
		bool m_HeistYacht;
		bool m_Yacht;
		bool m_DesertUFO;
		bool m_FortZancudoUFO;
		bool m_ClickinBell;
		bool m_JeweleryStore;
		bool m_Morgue;
		bool m_FIBLobby;
		bool m_LifeInvaderLobby;
		bool m_DestroyedHospital;
		bool m_CargoShip;
		bool m_TrainCrash;
		bool m_BrokenStiltHouse;
		bool m_PlaneCrash;
	};

	extern Vars_ m_Vars;
}