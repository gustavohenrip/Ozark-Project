#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingCrewMenu : public Menu::Submenu {
public:
	static NetworkSpoofingCrewMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingCrewMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingCrewMenu();
};

namespace NetworkSpoofingCrewMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}