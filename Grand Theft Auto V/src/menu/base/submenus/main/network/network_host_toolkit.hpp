#pragma once
#include "menu/base/submenu.hpp"

class NetworkHostToolkitMenu : public Menu::Submenu {
public:
	static NetworkHostToolkitMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkHostToolkitMenu()
		: Menu::Submenu() {}

	~NetworkHostToolkitMenu();
};

namespace NetworkHostToolkitMenuVars {
	struct Vars_ {
		bool m_ExtraSlot;
		int m_Transition;
	};

	bool ForceHostOfScript(const char* Script, Rage::Network::NetGamePlayer* Player, bool Notify = true);
	void TransitionToNewLobby(int ID);
	extern Vars_ m_Vars;
}