#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorWheelTypeMenu : public Menu::Submenu {
public:
	static VehicleEditorWheelTypeMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorWheelTypeMenu()
		: Menu::Submenu() {}

	~VehicleEditorWheelTypeMenu();
};

namespace VehicleEditorWheelTypeMenuVars {
	struct Vars_ {
		bool m_UpdateCache;
		int m_CachedWheelType;
		int m_CachedWheelIndex;
		eVehicleModTypes m_ModType;
	};

	extern Vars_ m_Vars;
}