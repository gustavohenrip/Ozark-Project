#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/player_manager.hpp"

class NetworkPlayersFriendlyMenu : public Menu::Submenu {
public:
	static NetworkPlayersFriendlyMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersFriendlyMenu()
		: Menu::Submenu() {}

	~NetworkPlayersFriendlyMenu();
};

namespace NetworkPlayersFriendlyMenuVars {
	struct Vars_ {
		int m_Commend;
		int m_BodyguardCount = 1;
		bool m_DemiGodmode;
		bool m_OffTheRadar;
		bool m_DisablePolice;
		bool m_Revenge;
	};

	void GiveAllWeapons(Menu::PlayerVars Player);
	void GiveOffTheRadar(Menu::PlayerVars Player);
	void RemoveWantedLevel(Menu::PlayerVars Player);
	void RemoveAttachments(Menu::PlayerVars Player);
	void CommendPlayer(Menu::PlayerVars Player, int Commend);
	void SpawnBodyguards(Menu::PlayerVars Player, int Count);

	extern Vars_ m_Vars;
}