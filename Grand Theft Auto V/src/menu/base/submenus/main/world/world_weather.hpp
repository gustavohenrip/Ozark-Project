#pragma once
#include "menu/base/submenu.hpp"

class WorldWeatherMenu : public Menu::Submenu {
public:
	static WorldWeatherMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WorldWeatherMenu()
		: Menu::Submenu() {}

	~WorldWeatherMenu();
};

namespace WorldWeatherMenuVars {
	struct Vars_ {
		bool m_Lightning;
		bool m_XmasSnow;
		bool m_Epilepsy;
		bool m_Sync;

		int m_Weather;
		int m_Clouds;
		int m_Rain;
		int m_Wind;

		float m_WindSpeed = 100000.f;
	};

	void SetWeather(const char* Weather);
	extern ScrollStruct<const char*> Weathers[15];
	extern Vars_ m_Vars;
}