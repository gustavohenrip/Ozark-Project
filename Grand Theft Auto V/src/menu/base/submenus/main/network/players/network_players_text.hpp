#pragma once
#include "menu/base/submenu.hpp"

class NetworkPlayersTextMenu : public Menu::Submenu {
public:
	static NetworkPlayersTextMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersTextMenu()
		: Menu::Submenu() {}

	~NetworkPlayersTextMenu();
};

namespace NetworkPlayersTextMenuVars {
	struct Vars_ {
		bool m_SenderToggle;
		bool m_RandomizeSender;

		Player m_SenderID;
		int m_Sender;
		int m_ListSize;
		ScrollStruct<int> m_PlayerList[34];
	};

	extern ScrollStruct<const char*> TextFonts[6];
	extern ScrollStruct<const char*> TextColors[10];
	extern ScrollStruct<const char*> TextSizes[6];
	extern Vars_ m_Vars;
}