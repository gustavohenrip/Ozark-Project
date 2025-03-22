#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorWheelsMenu : public Menu::Submenu {
public:
	static VehicleEditorWheelsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorWheelsMenu()
		: Menu::Submenu() {}

	~VehicleEditorWheelsMenu();
};

namespace VehicleEditorWheelsMenuVars {
	struct Vars_ {
		bool m_UpdateCache;
		int m_PaintIndex;
	};

	extern Vars_ m_Vars;
}