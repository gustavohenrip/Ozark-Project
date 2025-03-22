#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/player_manager.hpp"

class NetworkPlayersAbusiveMenu : public Menu::Submenu {
public:
	static NetworkPlayersAbusiveMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersAbusiveMenu()
		: Menu::Submenu() {}

	~NetworkPlayersAbusiveMenu();
};

namespace NetworkPlayersAbusiveMenuVars {
	struct Vars_ {
		bool m_AlwaysWanted;
		bool m_DisableTasks;
		bool m_Taze;
		bool m_Ragdoll;
		bool m_KickFromVehicle;
		bool m_GlitchPhysics;
		bool m_Forcefield;
		bool m_RainRockets;
		int m_Explode;
		int m_CEO;
	};

	void GiveWantedLevel(Menu::PlayerVars Player, int Level);
	void SetOnFire(Menu::PlayerVars Player);
	void Explode(Menu::PlayerVars Player, int Type);
	void CEOBan(Menu::PlayerVars Player);
	void CEOKick(Menu::PlayerVars Player);
	void Blame(Menu::PlayerVars Player);
	void Rape(Menu::PlayerVars Player);
	void BlackScreen(Menu::PlayerVars Player);
	void GlitchPhysics(Menu::PlayerVars Player);
	void TazePlayer(Menu::PlayerVars Player);
	void KickFromVehiclePlayer(Menu::PlayerVars Player);
	void RagdollPlayer(Menu::PlayerVars Player);
	void DumpInfo(Menu::PlayerVars Player);
	void SendPlayerTo(Menu::PlayerVars Player);
	extern Vars_ m_Vars;
}