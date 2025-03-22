#pragma once
#include "menu/base/submenu.hpp"

class NetworkOverseerSelectedMenu : public Menu::Submenu {
public:
	static NetworkOverseerSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkOverseerSelectedMenu()
		: Menu::Submenu() {}

	~NetworkOverseerSelectedMenu();
};

namespace NetworkOverseerSelectedMenuVars {
	struct Vars_ {
		std::size_t m_Selected;

		bool m_Enabled;
		bool m_Notify;
		bool m_BlockJoinHost;
		bool m_Explode;
		bool m_Crash;
		bool m_Kick;
		bool m_Cage;
		bool m_KickFromVehicle;
		bool m_SetOnFire;
		bool m_Ragdoll;
		bool m_GlitchPhysics;
		bool m_BlackScreen;
		bool m_Blame;
		bool m_GiveWantedLevel;
		bool m_SendToIsland;
		bool m_RemoveWeapons;
		bool m_HostileClone;
	};

	extern Vars_ m_Vars;
}