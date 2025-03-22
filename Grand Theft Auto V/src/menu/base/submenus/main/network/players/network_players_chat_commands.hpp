#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/network_types.hpp"

class NetworkPlayersChatCommandsMenu : public Menu::Submenu {
public:
	static NetworkPlayersChatCommandsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkPlayersChatCommandsMenu()
		: Menu::Submenu() {}

	~NetworkPlayersChatCommandsMenu();
};

namespace NetworkPlayersChatCommandsMenuVars {
	struct Vars_ {
		bool m_ChatCommands;
		bool m_ChatCommand[11];
	};

	void CallCommand(const char* CommandName, Rage::Network::NetGamePlayer* Player, std::vector<std::string> Tokens);
	extern Vars_ m_Vars;
}