#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorResprayMenu : public Menu::Submenu {
public:
	static VehicleEditorResprayMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorResprayMenu()
		: Menu::Submenu() {}

	~VehicleEditorResprayMenu();
};

namespace VehicleEditorResprayMenuVars {
	struct Vars_ {
		bool m_UpdateCache;
		int m_Livery;
	};

	extern Vars_ m_Vars;
}