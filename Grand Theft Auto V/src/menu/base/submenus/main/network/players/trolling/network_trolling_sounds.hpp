#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/player_manager.hpp"

class NetworkTrollingSoundsMenu : public Menu::Submenu {
public:
	static NetworkTrollingSoundsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkTrollingSoundsMenu()
		: Menu::Submenu() {}

	~NetworkTrollingSoundsMenu();
};

namespace NetworkTrollingSoundsMenuVars {
	struct Vars_ {
		std::unordered_map<int, int> m_Sounds;
		std::vector<std::shared_ptr<ButtonOption>> m_OptionCache;
	};

	void StartSoundPlayer(Menu::PlayerVars Player, std::pair<const char*, const char*> Sound, bool Add = true, int OptionID = 0);
	extern Vars_ m_Vars;
}