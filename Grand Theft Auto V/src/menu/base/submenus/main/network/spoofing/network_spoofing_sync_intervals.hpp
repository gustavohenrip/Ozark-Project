#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingSyncIntervalsMenu : public Menu::Submenu {
public:
	static NetworkSpoofingSyncIntervalsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingSyncIntervalsMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingSyncIntervalsMenu();
};

namespace NetworkSpoofingSyncIntervalsMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}