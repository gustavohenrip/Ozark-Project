#pragma once
#include "menu/base/submenu.hpp"

class NetworkTextPresetMenu : public Menu::Submenu {
public:
	static NetworkTextPresetMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkTextPresetMenu()
		: Menu::Submenu() {}

	~NetworkTextPresetMenu();
};

namespace NetworkTextPresetMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}