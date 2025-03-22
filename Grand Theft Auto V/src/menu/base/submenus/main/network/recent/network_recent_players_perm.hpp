#pragma once
#include "menu/base/submenu.hpp"

class NetworkRecentPlayersPermanentMenu : public Menu::Submenu {
public:
	static NetworkRecentPlayersPermanentMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkRecentPlayersPermanentMenu()
		: Menu::Submenu() {}

	~NetworkRecentPlayersPermanentMenu();
};

namespace NetworkRecentPlayersPermanentMenuVars {
	struct Vars_ {
		std::vector<uint64_t> m_Queue;
	};

	extern Vars_ m_Vars;
}
