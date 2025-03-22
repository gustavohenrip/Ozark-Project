#include "world_ocean_quads.hpp"
#include "menu/base/submenu_handler.hpp"
#include "world_ocean.hpp"
#include "world_ocean_quads_edit.hpp"

using namespace WorldOceanQuadsMenuVars;

namespace WorldOceanQuadsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<WorldOceanMenuVars::eOceanTypes> OceanTypes[] = {
		{ TranslationString("Ocean", true), WorldOceanMenuVars::OCEAN_TYPE_OCEAN },
		{ TranslationString("Calming", true), WorldOceanMenuVars::OCEAN_TYPE_CALMING },
		{ TranslationString("Wave", true), WorldOceanMenuVars::OCEAN_TYPE_WAVE }
	};

	Math::Vector3<float> GetWaterQuadCoords(uint64_t WaterQuadAddress) {
		Rage::Types::OceanQuad* WaterQuadCoords = (Rage::Types::OceanQuad*)WaterQuadAddress;

		Math::Vector3<float> Coords = { 0.f, 0.f, 0.f };
		Coords.m_X = (float)(WaterQuadCoords->m_Min.m_X + WaterQuadCoords->m_Max.m_X) / 2.0f;
		Coords.m_Y = (float)(WaterQuadCoords->m_Min.m_Y + WaterQuadCoords->m_Max.m_Y) / 2.0f;

		if (m_Vars.m_Type == 0) {
			Coords.m_Z = WaterQuadCoords->m_Height;
		}

		return Coords;
	}

	void CacheClosest() {
		m_Vars.m_ClosestQuad = 0;

		float Distance = std::numeric_limits<float>::max();
		WorldOceanMenuVars::OceanLoop(OceanTypes[m_Vars.m_Type].m_Result, [&] (uint64_t Pool) {
			auto Coords = GetWaterQuadCoords(Pool);

			float D = Coords.GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords);
			if (D < Distance) {
				Distance = D;
				m_Vars.m_ClosestQuad = Pool;
			}
		});
	}

	void QuadESP() {
		if (m_Vars.m_ClosestQuad) {
			Menu::GetRenderer()->DrawLine(Menu::GetLocalPlayer().m_Coords, GetWaterQuadCoords(m_Vars.m_ClosestQuad), { 255, 255, 255, 255 });
		}
	}
}

void WorldOceanQuadsMenu::Init() {
	SetName("Ocean Quads");
	SetParentSubmenu<WorldOceanMenu>();

	addOption(ScrollOption<WorldOceanMenuVars::eOceanTypes>(SCROLL, "Quad Type")
		.addTranslation()
		.addScroll(m_Vars.m_Type, 0, NUMOF(OceanTypes), OceanTypes));

	addOption(SubmenuOption("Edit Nearest Quad")
		.addTranslation()
		.addSubmenu<WorldOceanQuadsEditMenu>()
		.addOnHover([] { CacheClosest(); QuadESP(); }));
}

void WorldOceanQuadsMenu::Update() {}

/*Called once on submenu open*/
void WorldOceanQuadsMenu::UpdateOnce() {}

/*Called always*/
void WorldOceanQuadsMenu::FeatureUpdate() {}

WorldOceanQuadsMenu* _instance;
WorldOceanQuadsMenu* WorldOceanQuadsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldOceanQuadsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldOceanQuadsMenu::~WorldOceanQuadsMenu() { delete _instance; }