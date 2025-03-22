#include "world_ocean_quads_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "world_ocean_quads.hpp"

using namespace WorldOceanQuadsEditMenuVars;

namespace WorldOceanQuadsEditMenuVars {
	Vars_ m_Vars;
}

void WorldOceanQuadsEditMenu::Init() {
	SetName("Ocean Quads");
	SetParentSubmenu<WorldOceanQuadsMenu>();

	addString("Minimum - X");
	addString("Minimum - Y");
	addString("Maximum - X");
	addString("Maximum - Y");
	addString("Height");
	addString("Clear");
	addString("Dampening");
	addString("Amplitude");
	addString("Direction - X");
	addString("Direction - Y");
}

void WorldOceanQuadsEditMenu::Update() {
	WorldOceanQuadsMenuVars::QuadESP();
}

/*Called once on submenu open*/
void WorldOceanQuadsEditMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	m_Vars.m_Calming = (Rage::Types::OceanCalmingQuad*)WorldOceanQuadsMenuVars::m_Vars.m_ClosestQuad;
	m_Vars.m_Wave = (Rage::Types::OceanWaveQuad*)WorldOceanQuadsMenuVars::m_Vars.m_ClosestQuad;
	m_Vars.m_Ocean = (Rage::Types::OceanQuad*)WorldOceanQuadsMenuVars::m_Vars.m_ClosestQuad;

	addOption(NumberOption<short>(SCROLL, getString("Minimum - X"))
		.addNumber(m_Vars.m_Ocean->m_Min.m_X, "%i", 1).addMin(std::numeric_limits<short>::min()).addMax(std::numeric_limits<short>::max()).setScrollSpeed(10));

	addOption(NumberOption<short>(SCROLL, getString("Minimum - Y"))
		.addNumber(m_Vars.m_Ocean->m_Min.m_Y, "%i", 1).addMin(std::numeric_limits<short>::min()).addMax(std::numeric_limits<short>::max()).setScrollSpeed(10));

	addOption(NumberOption<short>(SCROLL, getString("Maximum - X"))
		.addNumber(m_Vars.m_Ocean->m_Max.m_X, "%i", 1).addMin(std::numeric_limits<short>::min()).addMax(std::numeric_limits<short>::max()).setScrollSpeed(10));

	addOption(NumberOption<short>(SCROLL, getString("Maximum - Y"))
		.addNumber(m_Vars.m_Ocean->m_Max.m_Y, "%i", 1).addMin(std::numeric_limits<short>::min()).addMax(std::numeric_limits<short>::max()).setScrollSpeed(10));

	/* Ocean */
	if (WorldOceanQuadsMenuVars::m_Vars.m_Type == 0) {
		addOption(NumberOption<float>(SCROLL, getString("Height"))
			.addNumber(m_Vars.m_Ocean->m_Height, "%.0f", 1.f).addMin(-10000.f).addMax(10000.f).setScrollSpeed(10));

		m_Vars.m_Clear = m_Vars.m_Ocean->m_Alpha != 0x1A1A1A1A;
		addOption(ToggleOption(getString("Clear"))
			.addToggle(m_Vars.m_Clear)
			.addOnClick([] { m_Vars.m_Ocean->m_Alpha = m_Vars.m_Clear ? 0x01010101 : 0x1A1A1A1A; }));
		return;
	}

	/* Calming */
	if (WorldOceanQuadsMenuVars::m_Vars.m_Type == 1) {
		addOption(NumberOption<float>(SCROLL, getString("Dampening"))
			.addNumber(m_Vars.m_Calming->m_Dampening, "%.1f", 0.1f).addMin(0.f).addMax(10000.f).setScrollSpeed(10));
		return;
	}

	/* Wave */
	if (WorldOceanQuadsMenuVars::m_Vars.m_Type == 2) {
		addOption(NumberOption<short>(SCROLL, getString("Amplitude"))
			.addNumber(m_Vars.m_Wave->m_Amplitude, "%i", 1).addMin(0).addMax(10000).setScrollSpeed(10));

		addOption(NumberOption<uint8_t>(SCROLL, getString("Direction - X"))
			.addNumber(m_Vars.m_Wave->m_Direction.m_X, "%i", 1).addMin(0).addMax(255));

		addOption(NumberOption<uint8_t>(SCROLL, getString("Direction - Y"))
			.addNumber(m_Vars.m_Wave->m_Direction.m_Y, "%i", 1).addMin(0).addMax(255));

		return;
	}
}

/*Called always*/
void WorldOceanQuadsEditMenu::FeatureUpdate() {}

WorldOceanQuadsEditMenu* _instance;
WorldOceanQuadsEditMenu* WorldOceanQuadsEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldOceanQuadsEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldOceanQuadsEditMenu::~WorldOceanQuadsEditMenu() { delete _instance; }