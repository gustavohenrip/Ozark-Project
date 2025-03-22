#include "esp.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../../main.hpp"
#include "utils/va.hpp"
#include "menu/base/submenus/main/helpers/esp_settings.hpp"
#include "menu/base/util/helpers.hpp"

using namespace ESPMenuVars;

namespace ESPMenuVars {
	Vars_ m_Vars;

	ScrollStruct<Menu::Helpers::eESPTypes> _3D[] = {
		{ TranslationString("Box - Inner Lines", true), Menu::Helpers::ESP_BOX },
		{ TranslationString("Box", true), Menu::Helpers::ESP_BOX_NO_INNER },
		{ TranslationString("Axis", true), Menu::Helpers::ESP_AXIS },
	};

	ScrollStruct<int> Types[] = {
		{ TranslationString("Without Distance", true), 0 },
		{ TranslationString("With Distance", true), 1 }
	};

	void NameESP(ESP E, Object Obj, std::string Name) {
		auto Coords = Native::GetEntityCoords(Obj, false);
		if (!Coords.IsZero()) {
			Math::Vector2<float> ScreenCoords;
			if (Native::GetScreenCoordFromWorldCoord(Coords.m_X, Coords.m_Y, Coords.m_Z, &ScreenCoords.m_X, &ScreenCoords.m_Y)) {
				Color SelectedColor = E.m_NameColor;
				if (E.m_NameLineOfSight) {
					if (Native::HasEntityClearLosToEntity(Menu::GetLocalPlayer().m_Ped, Obj, 17)) {
						SelectedColor = E.m_NameLineOfSightColor;
					}
				}

				Native::SetDrawOrigin(Coords.m_X, Coords.m_Y, Coords.m_Z, 0);

				if (*(uint32_t*)Global::Vars::g_DrawOriginIndex != 0xFFFFFFFF) {
					if (E.m_NameType == 0) {
						// Without Distance 
						const char* V = NULL;
						if (Name.empty()) {
							V = Utils::VA::VA("%X", Native::GetEntityModel(Obj));
						} else V = Utils::VA::VA("%s", Name.c_str());

						Menu::GetRenderer()->DrawText(V, { 0.f, 0.f }, E.m_NameScale, 0, SelectedColor, JUSTIFY_CENTER);
					} else {
						// With Distance
						const char* V = NULL;
						if (Name.empty()) {
							V = Utils::VA::VA("%X\n%.0fm", Native::GetEntityModel(Obj), Coords.GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords));
						} else V = Utils::VA::VA("%s\n%.0fm", Name.c_str(), Coords.GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords));

						Menu::GetRenderer()->DrawText(V, { 0.f, 0.f }, E.m_NameScale, 0, SelectedColor, JUSTIFY_CENTER);
					}
				}

				Native::ClearDrawOrigin();
			}
		}
	}

	void SnaplineESP(ESP E, Object Obj) {
		auto Coords = Native::GetEntityCoords(Obj, false);
		if (!Coords.IsZero()) {
			Math::Vector2<float> ScreenCoords;
			//if (Native::GetScreenCoordFromWorldCoord(Coords.m_X, Coords.m_Y, Coords.m_Z, &ScreenCoords.m_X, &ScreenCoords.m_Y)) {
				Color SelectedColor = E.m_SnaplineColor;
				if (E.m_SnaplineLineOfSight) {
					if (Native::HasEntityClearLosToEntity(Menu::GetLocalPlayer().m_Ped, Obj, 17)) {
						SelectedColor = E.m_SnaplineLineOfSightColor;
					}
				}

				Menu::Helpers::DrawSnapline(Menu::GetLocalPlayer().m_Entity, Obj, SelectedColor);
			//}
		}
	}

	void _3DESP(ESP E, Object Obj) {
		auto Coords = Native::GetEntityCoords(Obj, false);
		if (!Coords.IsZero()) {
			Math::Vector2<float> ScreenCoords;
			if (Native::GetScreenCoordFromWorldCoord(Coords.m_X, Coords.m_Y, Coords.m_Z, &ScreenCoords.m_X, &ScreenCoords.m_Y)) {
				Color SelectedColor = E.m_3DColor;
				if (E.m_3DLineOfSight) {
					if (Native::HasEntityClearLosToEntity(Menu::GetLocalPlayer().m_Ped, Obj, 17)) {
						SelectedColor = E.m_3DLineOfSightColor;
					}
				}

				Menu::Helpers::DrawESP(Obj, SelectedColor, _3D[E.m_3DType].m_Result, true);
			}
		}
	}
}

void ESPMenu::Init() {
	SetName("ESP");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Settings")
		.addTranslation()
		.addSubmenu<ESPSettingsMenu>());
}

void ESPMenu::Update() {}

/*Called once on submenu open*/
void ESPMenu::UpdateOnce() {
	ClearOptionsOffset(1);

	if (!m_Vars.m_ESP) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious(true);
		return;
	}

	addOption(ScrollOption<int>(TOGGLE, "Toggle Name")
		.addToggle(m_Vars.m_ESP->m_Name)
		.addScroll(m_Vars.m_ESP->m_NameType, 0, NUMOF(Types), Types));

	addOption(ToggleOption("Toggle Name LOS")
		.addToggle(m_Vars.m_ESP->m_NameLineOfSight)
		.addTooltip("Changes color when the entity is in your view"));

	addOption(ScrollOption<Menu::Helpers::eESPTypes>(TOGGLE, "Toggle 3D")
		.addToggle(m_Vars.m_ESP->m_3D)
		.addScroll(m_Vars.m_ESP->m_3DType, 0, NUMOF(_3D), _3D));

	addOption(ToggleOption("Toggle 3D LOS")
		.addToggle(m_Vars.m_ESP->m_3DLineOfSight)
		.addTooltip("Changes color when the entity is in your view"));

	addOption(ToggleOption("Toggle Snapline")
		.addToggle(m_Vars.m_ESP->m_Snapline));

	addOption(ToggleOption("Toggle Snapline LOS")
		.addToggle(m_Vars.m_ESP->m_SnaplineLineOfSight)
		.addTooltip("Changes color when the entity is in your view"));
}

/*Called always*/
void ESPMenu::FeatureUpdate() {}

ESPMenu* _instance;
ESPMenu* ESPMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ESPMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ESPMenu::~ESPMenu() { delete _instance; }