#include "world_ocean.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "menu/base/util/global.hpp"
#include "world_ocean_quads.hpp"
#include "world_ocean_tuning.hpp"

using namespace WorldOceanMenuVars;

namespace WorldOceanMenuVars {
	Vars_ m_Vars;

	void OceanLoop(eOceanTypes Type, std::function<void(uint64_t)> Callback) {
		uint64_t QuadPool = 0;
		short QuadCount = 0;

		switch (Type) {
			case OCEAN_TYPE_OCEAN:
				QuadPool = Global::Vars::g_OceanQuads.m_Ocean->m_QuadPool;
				QuadCount = Global::Vars::g_OceanQuads.m_Ocean->m_QuadCount;
				break;

			case OCEAN_TYPE_WAVE:
				QuadPool = Global::Vars::g_OceanQuads.m_Wave->m_QuadPool;
				QuadCount = Global::Vars::g_OceanQuads.m_Wave->m_QuadCount;
				break;

			case OCEAN_TYPE_CALMING:
				QuadPool = Global::Vars::g_OceanQuads.m_Calming->m_QuadPool;
				QuadCount = Global::Vars::g_OceanQuads.m_Calming->m_QuadCount;
				break;
		}

		for (short i = 0; i < QuadCount; i++) {
			switch (Type) {
				case OCEAN_TYPE_OCEAN:
					Callback((QuadPool + (i * sizeof(Rage::Types::OceanQuad))));
					break;

				case OCEAN_TYPE_WAVE:
					Callback((QuadPool + (i * sizeof(Rage::Types::OceanWaveQuad))));
					break;

				case OCEAN_TYPE_CALMING:
					Callback((QuadPool + (i * sizeof(Rage::Types::OceanCalmingQuad))));
					break;
			}
		}
	}

	void OceanTask(eOceanTypes Type, eOceanTasks Task) {
		OceanLoop(Type, [=] (uint64_t Pool) {
			switch (Task) {
				case OCEAN_DISABLE: {
					Rage::Types::OceanQuad* Quad = (Rage::Types::OceanQuad*)(Pool);

					if (m_Vars.m_DisableOcean && Quad->m_Height == 0.0f) {
						Quad->m_Height = -10000.0f;
					} else if (!m_Vars.m_DisableOcean && Quad->m_Height == -10000.0f) {
						Quad->m_Height = 0.0f;
					}

					break;
				}

				case OCEAN_CLEAR: {
					Rage::Types::OceanQuad* Quad = (Rage::Types::OceanQuad*)(Pool);
					Quad->m_Alpha = m_Vars.m_ClearOcean ? 0x01010101 : 0x1A1A1A1A;
					break;
				}

				case OCEAN_FIERCE: {
					Rage::Types::OceanWaveQuad* Quad = (Rage::Types::OceanWaveQuad*)(Pool);
					Quad->m_Amplitude = m_Vars.m_FierceOcean ? Quad->m_Amplitude + 2000 : Quad->m_Amplitude - 2000;
					break;
				}
			}
		});
	}
}

void WorldOceanMenu::Init() {
	SetName("Ocean");
	SetParentSubmenu<WorldMenu>();

	addOption(SubmenuOption("Ocean Quads")
		.addTranslation()
		.addSubmenu<WorldOceanQuadsMenu>());

	addOption(SubmenuOption("Ocean Tuning")
		.addTranslation()
		.addSubmenu<WorldOceanTuningMenu>());

	addOption(ToggleOption("Disable Ocean")
		.addTranslation()
		.addToggle(m_Vars.m_DisableOcean)
		.addOnClick([] { OceanTask(OCEAN_TYPE_OCEAN, OCEAN_DISABLE); }));

	addOption(ToggleOption("Clear Ocean")
		.addTranslation()
		.addToggle(m_Vars.m_ClearOcean)
		.addOnClick([] { OceanTask(OCEAN_TYPE_OCEAN, OCEAN_CLEAR); }));

	addOption(ToggleOption("Fierce Ocean")
		.addTranslation()
		.addToggle(m_Vars.m_FierceOcean)
		.addOnClick([] { OceanTask(OCEAN_TYPE_WAVE, OCEAN_FIERCE); }));

	addOption(ToggleOption("Part Water")
		.addTranslation()
		.addToggle(m_Vars.m_PartWater));

	addOption(NumberOption<float>(SCROLL, "Wave Intensity")
		.addTranslation()
		.addNumber(m_Vars.m_WaveIntensity, "%.1f", 0.1f).addMin(0.f).addMax(10.f));
}

void WorldOceanMenu::Update() {}

/*Called once on submenu open*/
void WorldOceanMenu::UpdateOnce() {
	m_Vars.m_WaveIntensity = Native::_GetWavesIntensity();
}

/*Called always*/
void WorldOceanMenu::FeatureUpdate() {
	if (m_Vars.m_PartWater) {
		float offset[] = { -4, 4 };
		for (int i = 0; i < 5; i++) {
			Math::Vector3<float> CurCoords = Menu::GetLocalPlayer().m_Coords;
			if (i < 2)  CurCoords.m_X += offset[(i % 2 == 0)];
			else if (i < 4) CurCoords.m_Y += offset[(i % 2 == 0)];
			Native::ModifyWater(CurCoords.m_X, CurCoords.m_Y, -500000.0f, 0.2f);
		}
	}

	Native::_SetWavesIntensity(m_Vars.m_WaveIntensity);
}

WorldOceanMenu* _instance;
WorldOceanMenu* WorldOceanMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldOceanMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldOceanMenu::~WorldOceanMenu() { delete _instance; }