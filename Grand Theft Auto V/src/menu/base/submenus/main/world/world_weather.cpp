#include "world_weather.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "menu/base/util/global.hpp"
#include "utils/memory/memory.hpp"

using namespace WorldWeatherMenuVars;

namespace WorldWeatherMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> Weathers[] = {
		{ TranslationString("Sunny", true), "CLEAR" }, // 1st row, 1st icon
		{ TranslationString("Extra Sunny", true), "EXTRASUNNY" }, // 1st row, 1st icon
		{ TranslationString("Rain", true), "RAIN" }, // 1st row, 4th icon
		{ TranslationString("Clearing", true), "CLEARING" }, // 1st row, 3rd icon
		{ TranslationString("Cloudy", true), "CLOUDS" }, // 1st row, 2nd icon
		{ TranslationString("Smoggy", true), "SMOG" }, // 4th row, 4th icon
		{ TranslationString("Foggy", true), "FOGGY" }, // 4th row, 4th icon
		{ TranslationString("Stormy", true), "THUNDER" }, // 2nd row, 3rd icon
		{ TranslationString("Overcast", true), "OVERCAST" }, // 1st row, 2nd icon
		{ TranslationString("Light Snow", true), "SNOWLIGHT" }, // 2nd row, 1st icon
		{ TranslationString("Snow", true), "SNOW" }, // 2nd row, 1st icon
		{ TranslationString("Blizzard", true), "BLIZZARD" }, // 2nd row, 1st icon
		{ TranslationString("Christmas", true), "XMAS" }, // christmas
		{ TranslationString("Neutral", true), "NEUTRAL" }, // nuetral.png
		{ TranslationString("Halloween", true), "HALLOWEEN" } // halloween.png
	};

	ScrollStruct<int> Clouds[] = {
		{ TranslationString("Clear", true), 0 },
		{ TranslationString("Rain", true), 0 },
		{ TranslationString("Horizonband1", true), 0 },
		{ TranslationString("Horizonband2", true), 0 },
		{ TranslationString("Horizonband3", true), 0 },
		{ TranslationString("Puffs", true), 0 },
		{ TranslationString("Wispy", true), 0 },
		{ TranslationString("Horizon", true), 0 },
		{ TranslationString("Contrails", true), 0 },
		{ TranslationString("Altostratus", true), 0 },
		{ TranslationString("Nimbus", true), 0 },
		{ TranslationString("Cirrus", true), 0 },
		{ TranslationString("Cirrocumulus", true), 0 },
		{ TranslationString("Stratoscumulus", true), 0 },
		{ TranslationString("Stripey", true), 0 },
		{ TranslationString("Horsey", true), 0 },
		{ TranslationString("Shower", true), 0 },
		{ TranslationString("Cloudy 01", true), 0 },
		{ TranslationString("Clear 01", true), 0 },
		{ TranslationString("Snowy 01", true), 0 },
		{ TranslationString("Stormy 01", true), 0 }
	};

	ScrollStruct<float> Rains[] = {
		{ TranslationString("None", true), 0.f },
		{ TranslationString("Light", true), 0.3f },
		{ TranslationString("Heavy", true), 1.5f },
		{ TranslationString("Heavier", true), 3.5f },
		{ TranslationString("Insane", true), 5.5f },
		{ TranslationString("Off the Charts", true), 50.f },
	};

	ScrollStruct<float> WindSpeeds[] = {
		{ TranslationString("None", true), 0.f },
		{ TranslationString("Light", true), 0.3f },
		{ TranslationString("Fast", true), 1.5f },
		{ TranslationString("Faster", true), 3.5f },
		{ TranslationString("Insane", true), 5.5f },
		{ TranslationString("Off the Charts", true), 50.f },
	};

	void SetWeather(const char* Weather) {
		if (m_Vars.m_Sync) {
			Memory::Nop(Global::Vars::g_SendWeatherUpdate + 0xA1, 2);
			Memory::Nop(Global::Vars::g_SendWeatherUpdate + 0xA6, 2);
		}

		Native::SetWeatherTypeNowPersist(Weather);
		Native::SetWeatherTypeNow(Weather);
		Native::SetOverrideWeather(Weather);

		if (m_Vars.m_Sync) {
			Memory::WriteVector(Global::Vars::g_SendWeatherUpdate + 0xA1, { 0x74, 0x31 });
			Memory::WriteVector(Global::Vars::g_SendWeatherUpdate + 0xA6, { 0x74, 0x2C });
		}
	}

	void ToggleXmas() { // good
		Menu::Global(262145).At(4724).As<int>() = m_Vars.m_XmasSnow;
		Menu::Global(262145).At(8928).As<int>() = !m_Vars.m_XmasSnow;
		Menu::Global(262145).At(8929).As<int>() = !m_Vars.m_XmasSnow;
		Menu::Global(262145).At(8930).As<int>() = !m_Vars.m_XmasSnow;
		Menu::Global(262145).At(8931).As<int>() = !m_Vars.m_XmasSnow;
		Menu::Global(262145).At(8936).As<int>() = m_Vars.m_XmasSnow ? 255 : 0;
		Menu::Global(262145).At(8937).As<int>() = m_Vars.m_XmasSnow ? 255 : 0;
	}
}

void WorldWeatherMenu::Init() {
	SetName("Weather");
	SetParentSubmenu<WorldMenu>();

	addOption(ScrollOption<const char*>(SCROLL, "Modify Weather")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Weather, 0, NUMOF(Weathers), Weathers)
		.addOnClick([] { SetWeather(Weathers[m_Vars.m_Weather].m_Result); }));

	addOption(ScrollOption<int>(SCROLL, "Modify Clouds")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Clouds, 0, NUMOF(Clouds), Clouds)
		.addOnClick([] { if (m_Vars.m_Clouds == 0) Native::_ClearCloudHat(); else Native::_SetCloudHatTransition(Clouds[m_Vars.m_Clouds].m_Name.GetOriginal().c_str(), 0.5f); }));

	addOption(ScrollOption<float>(SCROLL, "Wind Speed")
		.addTranslation()
		.addScroll(m_Vars.m_Wind, 0, NUMOF(WindSpeeds), WindSpeeds)
		.addOnClick([] { m_Vars.m_WindSpeed = WindSpeeds[m_Vars.m_Wind].m_Result; }));

	addOption(ScrollOption<float>(SCROLL, "Rain Intensity")
		.addTranslation()
		.addScroll(m_Vars.m_Rain, 0, NUMOF(Rains), Rains)
		.addOnClick([] { Native::_SetRainFxIntensity(Rains[m_Vars.m_Rain].m_Result); }));

	addOption(ToggleOption("Weather Epilepsy")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Epilepsy));

	addOption(ToggleOption("Xmas Weather")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_XmasSnow).canBeSaved(m_ParentNameStack)
		.addOnClick(ToggleXmas));

	addOption(ToggleOption("Thunder and Lightning")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Lightning).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Sync to Session")
		.addTranslation()
		.addToggle(m_Vars.m_Sync));
}

void WorldWeatherMenu::Update() {}

/*Called once on submenu open*/
void WorldWeatherMenu::UpdateOnce() {}

/*Called always*/
void WorldWeatherMenu::FeatureUpdate() {
	if (m_Vars.m_Lightning) {
		Native::_CreateLightningThunder();
	}

	if (m_Vars.m_XmasSnow) {
		ToggleXmas();
	}

	if (m_Vars.m_Epilepsy) {
		SetWeather(Weathers[Native::GetRandomIntInRange(0, 12)].m_Result);
	}
}

WorldWeatherMenu* _instance;
WorldWeatherMenu* WorldWeatherMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldWeatherMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldWeatherMenu::~WorldWeatherMenu() { delete _instance; }