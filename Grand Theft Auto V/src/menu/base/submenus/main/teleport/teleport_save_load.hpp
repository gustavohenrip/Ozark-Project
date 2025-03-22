#pragma once
#include "menu/base/submenu.hpp"

class TeleportSaveLoadMenu : public Menu::Submenu {
public:
	static TeleportSaveLoadMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TeleportSaveLoadMenu()
		: Menu::Submenu() {}

	~TeleportSaveLoadMenu();
};

namespace TeleportSaveLoadMenuVars {
	struct Vars_ {
		std::vector<std::pair<std::string, Math::Vector3<float>>> m_Locations;
	};

	extern Vars_ m_Vars;
}