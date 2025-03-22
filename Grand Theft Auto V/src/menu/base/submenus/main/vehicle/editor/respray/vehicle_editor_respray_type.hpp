#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorResprayTypeMenu : public Menu::Submenu {
public:
	static VehicleEditorResprayTypeMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorResprayTypeMenu()
		: Menu::Submenu() {}

	~VehicleEditorResprayTypeMenu();
};

namespace VehicleEditorResprayTypeMenuVars {
	enum ePaintIndexs {
		PRIMARY,
		SECONDARY,
		PEARLESCENT
	};

	struct Vars_ {
		bool m_UpdateCache;
		int m_Type;
		int m_PaintIndex[3];
		RadioContext m_Context = { "commonmenu", "shop_garage_icon" };
	};

	extern Vars_ m_Vars;
}