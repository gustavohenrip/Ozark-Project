#include "world_ocean_tuning.hpp"
#include "menu/base/submenu_handler.hpp"
#include "world_ocean.hpp"

using namespace WorldOceanTuningMenuVars;

namespace WorldOceanTuningMenuVars {
	Vars_ m_Vars;
}

void WorldOceanTuningMenu::Init() {
	SetName("Ocean Tuning");
	SetParentSubmenu<WorldOceanMenu>();

	addOption(NumberOption<float>(SCROLL, "Ripple Scale")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_RippleScale, "%.2f", 0.01f).addMin(0.f).addMax(100.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_RippleScale = m_Vars.m_WaterTune.m_RippleScale; }));

	addOption(NumberOption<float>(SCROLL, "Foam Scale")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_OceanFoamScale, "%.2f", 0.01f).addMin(0.f).addMax(100.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_OceanFoamScale = m_Vars.m_WaterTune.m_OceanFoamScale; }));

	addOption(NumberOption<float>(SCROLL, "Specular Falloff")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_SpecularFalloff, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_SpecularFalloff = m_Vars.m_WaterTune.m_SpecularFalloff; }));

	addOption(NumberOption<float>(SCROLL, "Fog Pierce Intensity")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_FogPierceIntensity, "%.1f", 0.1f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_FogPierceIntensity = m_Vars.m_WaterTune.m_FogPierceIntensity; }));

	addOption(NumberOption<float>(SCROLL, "Refraction Blend")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_RefractionBlend, "%.1f", 0.1f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_RefractionBlend = m_Vars.m_WaterTune.m_RefractionBlend; }));

	addOption(NumberOption<float>(SCROLL, "Refraction Exponent")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_RefractionExponent, "%.2f", 0.25f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_RefractionExponent = m_Vars.m_WaterTune.m_RefractionExponent; }));

	addOption(NumberOption<float>(SCROLL, "Water Cycle Depth")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_WaterCycleDepth, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_WaterCycleDepth = m_Vars.m_WaterTune.m_WaterCycleDepth; }));

	addOption(NumberOption<float>(SCROLL, "Water Cycle Fade")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_WaterCycleFade, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_WaterCycleFade = m_Vars.m_WaterTune.m_WaterCycleFade; }));

	addOption(NumberOption<float>(SCROLL, "Water Lightning Depth")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_WaterLightningDepth, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_WaterLightningDepth = m_Vars.m_WaterTune.m_WaterLightningDepth; }));

	addOption(NumberOption<float>(SCROLL, "Water Lightning Fade")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_WaterLightningFade, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_WaterLightningFade = m_Vars.m_WaterTune.m_WaterLightningFade; }));

	addOption(NumberOption<float>(SCROLL, "Deep Water Mod Depth")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_DeepWaterModDepth, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_DeepWaterModDepth = m_Vars.m_WaterTune.m_DeepWaterModDepth; }));

	addOption(NumberOption<float>(SCROLL, "Deep Water Mod Fade")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_DeepWaterModFade, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_DeepWaterModFade = m_Vars.m_WaterTune.m_DeepWaterModFade; }));

	addOption(NumberOption<float>(SCROLL, "God Rays Lerp Start")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_GodRaysLerpStart, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_GodRaysLerpStart = m_Vars.m_WaterTune.m_GodRaysLerpStart; }));

	addOption(NumberOption<float>(SCROLL, "God Rays Lerp End")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_GodRaysLerpEnd, "%.0f", 1.f).addMin(0.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_GodRaysLerpEnd = m_Vars.m_WaterTune.m_GodRaysLerpEnd; }));

	addOption(NumberOption<float>(SCROLL, "Disturb Foam Scale")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_DisturbFoamScale, "%.2f", 0.01f).addMin(0.f).addMax(100.f).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_DisturbFoamScale = m_Vars.m_WaterTune.m_DisturbFoamScale; }));

	addOption(NumberOption<float>(SCROLL, "Fog Minimum - X")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_FogMin.m_X, "%.0f", 1.f).addMin(-std::numeric_limits<float>::max()).addMax(std::numeric_limits<float>::max()).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_FogMin.m_X = m_Vars.m_WaterTune.m_FogMin.m_X; }));

	addOption(NumberOption<float>(SCROLL, "Fog Minimum - Y")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_FogMin.m_Y, "%.0f", 1.f).addMin(-std::numeric_limits<float>::max()).addMax(std::numeric_limits<float>::max()).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_FogMin.m_Y = m_Vars.m_WaterTune.m_FogMin.m_Y; }));

	addOption(NumberOption<float>(SCROLL, "Fog Maximum - X")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_FogMax.m_X, "%.0f", 1.f).addMin(-std::numeric_limits<float>::max()).addMax(std::numeric_limits<float>::max()).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_FogMax.m_X = m_Vars.m_WaterTune.m_FogMax.m_X; }));

	addOption(NumberOption<float>(SCROLL, "Fog Maximum - Y")
		.addTranslation()
		.addNumber(m_Vars.m_WaterTune.m_FogMax.m_Y, "%.0f", 1.f).addMin(-std::numeric_limits<float>::max()).addMax(std::numeric_limits<float>::max()).setScrollSpeed(10)
		.addOnClick([] { Global::Vars::g_WaterTune->m_FogMax.m_Y = m_Vars.m_WaterTune.m_FogMax.m_Y; }));
}

void WorldOceanTuningMenu::Update() {}

/*Called once on submenu open*/
void WorldOceanTuningMenu::UpdateOnce() {
	memcpy(&m_Vars.m_WaterTune, Global::Vars::g_WaterTune, sizeof(m_Vars.m_WaterTune));
}

/*Called always*/
void WorldOceanTuningMenu::FeatureUpdate() {}

WorldOceanTuningMenu* _instance;
WorldOceanTuningMenu* WorldOceanTuningMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldOceanTuningMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldOceanTuningMenu::~WorldOceanTuningMenu() { delete _instance; }