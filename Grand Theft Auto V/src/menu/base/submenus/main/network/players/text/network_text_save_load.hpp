#pragma once
#include "menu/base/submenu.hpp"

class NetworkTextSaveLoadMenu : public Menu::Submenu {
public:
	static NetworkTextSaveLoadMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkTextSaveLoadMenu()
		: Menu::Submenu() {}

	~NetworkTextSaveLoadMenu();
};

namespace NetworkTextSaveLoadMenuVars {
	struct Vars_ {
		std::vector<std::pair<std::string, std::string>> m_Messages;
	};

	extern Vars_ m_Vars;
}