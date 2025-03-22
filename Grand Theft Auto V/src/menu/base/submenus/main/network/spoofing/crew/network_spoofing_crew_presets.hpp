#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingCrewPresetsMenu : public Menu::Submenu {
public:
	static NetworkSpoofingCrewPresetsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingCrewPresetsMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingCrewPresetsMenu();
};

namespace NetworkSpoofingCrewPresetsMenuVars {
	struct Vars_ {
		std::string* m_Editing;
	};

	extern Vars_ m_Vars;
}