#pragma once
#include "menu/base/submenu.hpp"
#include "rage/types/network_types.hpp"

class NetworkHostToolkitBlacklistMenu : public Menu::Submenu {
public:
	static NetworkHostToolkitBlacklistMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkHostToolkitBlacklistMenu()
		: Menu::Submenu() {}

	~NetworkHostToolkitBlacklistMenu();
};

namespace NetworkHostToolkitBlacklistMenuVars {
	struct Blacklist {
		Rage::Network::BlacklistedPlayer m_Player;
		bool m_API;
		std::string m_ResolvedName;
	};

	struct Vars_ {
		std::vector<Blacklist> m_Players;
	};

	extern Vars_ m_Vars;
}