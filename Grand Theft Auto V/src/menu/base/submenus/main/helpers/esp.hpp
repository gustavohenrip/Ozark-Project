#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/rainbow.hpp"

class ESPMenu : public Menu::Submenu {
public:
	static ESPMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ESPMenu()
		: Menu::Submenu() {}

	~ESPMenu();
};

namespace ESPMenuVars {
	struct ESP {
		bool m_Name;
		bool m_NameLineOfSight;
		bool m_NameRainbowToggle;
		bool m_NameLineOfSightRainbowToggle;
		float m_NameScale = 0.14f;
		int m_NameType;
		Color m_NameColor = Color(255, 0, 0, 255);
		Color m_NameLineOfSightColor = Color(255, 255, 0, 255);
		Menu::Rainbow::Rainbow m_NameRainbow;
		Menu::Rainbow::Rainbow m_NameLineOfSightRainbow;

		bool m_Snapline;
		bool m_SnaplineLineOfSight;
		bool m_SnaplineRainbowToggle;
		bool m_SnaplineLineOfSightRainbowToggle;
		Color m_SnaplineColor = Color(255, 0, 0, 255);
		Color m_SnaplineLineOfSightColor = Color(255, 255, 0, 255);
		Menu::Rainbow::Rainbow m_SnaplineRainbow;
		Menu::Rainbow::Rainbow m_SnaplineLineOfSightRainbow;

		int m_3DType;
		bool m_3D;
		bool m_3DLineOfSight;
		bool m_3DRainbowToggle;
		bool m_3DLineOfSightRainbowToggle;
		Color m_3DColor = Color(255, 0, 0, 255);
		Color m_3DLineOfSightColor = Color(255, 255, 0, 255);
		Menu::Rainbow::Rainbow m_3DRainbow;
		Menu::Rainbow::Rainbow m_3DLineOfSightRainbow;
	};

	struct Vars_ {
		ESP* m_ESP;
		bool m_Player = false;
	};

	void _3DESP(ESP E, Object Obj);
	void SnaplineESP(ESP E, Object Obj);
	void NameESP(ESP E, Object Obj, std::string Name = "");
	extern Vars_ m_Vars;
}