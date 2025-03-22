#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/player_manager.hpp"

class NetworkPlayersTrollingMenu : public Menu::Submenu {
public:
	static NetworkPlayersTrollingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersTrollingMenu()
		: Menu::Submenu() {}

	~NetworkPlayersTrollingMenu();
};

namespace NetworkPlayersTrollingMenuVars {
	struct Vars_ {
		bool m_HostileTraffic;
		bool m_FakeMoneyDrop;
		int m_RamType;
		int m_RemoveWeaponGroupType;
		int m_JetType;
		int m_CageType;
		int m_CloneType;
		int m_Messages;
	};

	void CagePlayer(Menu::PlayerVars Player);
	void ClonePlayer(Menu::PlayerVars Player);

	extern Vars_ m_Vars;
}