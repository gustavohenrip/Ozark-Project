#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/util/rainbow.hpp"

class WorldAdvancedUISkyMenu : public Menu::Submenu {
public:
	static WorldAdvancedUISkyMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldAdvancedUISkyMenu()
		: Menu::Submenu() {}

	~WorldAdvancedUISkyMenu();
};

namespace WorldAdvancedUISkyMenuVars {
	struct SkyColor {
		bool m_SaveToConfig;
		bool m_RainbowToggle;
		float m_Luminance = 1.f;

		Color m_InternalColor;
		Math::Vector3_<float> m_Color;
		Math::Vector4<float>* m_MemoryColor;
		Menu::Rainbow::Rainbow m_Rainbow;
		std::vector<uint8_t> m_Cache;
	};

	struct Vars_ {
		SkyColor m_AzimuthEastColor;
		SkyColor m_AzimuthTransitionColor;
		SkyColor m_AzimuthWestColor;
		SkyColor m_CloudBaseMinusMidColour;
		SkyColor m_CloudMidColor;
		SkyColor m_CloudShadowMinusBaseColourTimesShadowStrength;
		SkyColor m_MoonColor;
		SkyColor m_SkyPlaneColor;
		SkyColor m_SmallCloudColorHdr;
		SkyColor m_SunColor;
		SkyColor m_ZenithColor;
		SkyColor m_ZenithTransitionColor;

		int m_Selected;
		bool m_Cached;
	};

	extern Vars_ m_Vars;
}