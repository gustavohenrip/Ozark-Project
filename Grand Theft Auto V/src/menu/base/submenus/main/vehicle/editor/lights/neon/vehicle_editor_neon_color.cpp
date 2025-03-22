#include "vehicle_editor_neon_color.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_editor_neon.hpp"
#include "../../vehicle_editor_lsc.hpp"
#include "../../../vehicle_editor.hpp"

using namespace VehicleEditorNeonColorMenuVars;

namespace VehicleEditorNeonColorMenuVars {
	Vars_ m_Vars;

	void UpdateColorPreview(Color Col) {
		for (int i = 0; i < 4; i++) {
			if (Native::_IsVehicleNeonLightEnabled(Menu::GetLocalPlayer().m_Vehicle, i)) {
				Native::_SetVehicleNeonLightsColour(Menu::GetLocalPlayer().m_Vehicle, Col.R, Col.G, Col.B);
			}
		}
	}

	void UpdateColor(Color Col) {
		VehicleEditorMenuVars::m_Vars.m_CachedStats.m_NeonColor = Col;
	}

	std::vector<std::pair<Color, int>> ColorMap = {
		{ { 222, 222, 255 }, 0 },
		{ { 2, 21, 255 }, 1 },
		{ { 3, 83, 255 }, 2 },
		{ { 0, 255, 140 }, 3 },
		{ { 94, 255, 1 }, 4 },
		{ { 255, 255, 0 }, 5 },
		{ { 255, 150, 0 }, 6 },
		{ { 255, 62, 0 }, 7 },
		{ { 255, 1, 1 }, 8 },
		{ { 255, 50, 100 }, 9 },
		{ { 255, 5, 190 }, 10 },
		{ { 35, 1, 255 }, 11 },
		{ { 15, 3, 255 }, 12 }
	};
}

void VehicleEditorNeonColorMenu::Init() {
	SetName("Neon Color");
	SetParentSubmenu<VehicleEditorNeonMenu>();

	addString("White");
	addString("Blue");
	addString("Electric Blue");
	addString("Mint Green");
	addString("Lime Green");
	addString("Yellow");
	addString("Golden Shower");
	addString("Orange");
	addString("Red");
	addString("Pony Pink");
	addString("Hot Pink");
	addString("Purple");
	addString("Blacklight");
}

void VehicleEditorNeonColorMenu::Update() {
	if (Menu::GetBase()->m_ScrollOffset < Menu::GetSubmenuHandler()->GetTotalOptions() - Menu::GetBase()->m_MaxOptions && Menu::GetBase()->m_CurrentOption - Menu::GetBase()->m_ScrollOffset == Menu::GetBase()->m_MaxOptions) Menu::GetBase()->m_ScrollOffset++;
}

/*Called once on submenu open*/
void VehicleEditorNeonColorMenu::UpdateOnce() {
	ClearOptionsOffset(0);
	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	std::vector<std::shared_ptr<RadioOption>> TempOptions;

	TempOptions.push_back(addOption(RadioOption(getString("White"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 222, 222, 255 }); })
		.addOnHover([] { UpdateColorPreview({ 222, 222, 255 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Blue"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 2, 21, 255 }); })
		.addOnHover([] { UpdateColorPreview({ 2, 21, 255 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Electric Blue"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 3, 83, 255 }); })
		.addOnHover([] { UpdateColorPreview({ 3, 83, 255 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Mint Green"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 0, 255, 140 }); })
		.addOnHover([] { UpdateColorPreview({ 0, 255, 140 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Lime Green"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 94, 255, 1 }); })
		.addOnHover([] { UpdateColorPreview({ 94, 255, 1 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Yellow"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 255, 255, 0 }); })
		.addOnHover([] { UpdateColorPreview({ 255, 255, 0 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Golden Shower"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 255, 150, 0 }); })
		.addOnHover([] { UpdateColorPreview({ 255, 150, 0 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Orange"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 255, 62, 0 }); })
		.addOnHover([] { UpdateColorPreview({ 255, 62, 0 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Red"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 255, 1, 1 }); })
		.addOnHover([] { UpdateColorPreview({ 255, 1, 1 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Pony Pink"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 255, 50, 100 }); })
		.addOnHover([] { UpdateColorPreview({ 255, 50, 100 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Hot Pink"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 255, 5, 190 }); })
		.addOnHover([] { UpdateColorPreview({ 255, 5, 190 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Purple"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 35, 1, 255 }); })
		.addOnHover([] { UpdateColorPreview({ 35, 1, 255 }); })));

	TempOptions.push_back(addOption(RadioOption(getString("Blacklight"))
		.addTranslation()
		.addRadio(m_Vars.m_Context)
		.addOnClick([] { UpdateColor({ 15, 3, 255 }); })
		.addOnHover([] { UpdateColorPreview({ 15, 3, 255 }); })));

	for (auto& Map : ColorMap) {
		auto Color = VehicleEditorMenuVars::m_Vars.m_CachedStats.m_NeonColor;
		if (Color.R == Map.first.R
			&& Color.G == Map.first.G
			&& Color.B == Map.first.B) {
			if (Map.second != 0) {
				TempOptions[Map.second]->UpdateSelected();
				Menu::GetBase()->m_CurrentOption = Map.second;
			}
			break;
		}
	}
}

/*Called always*/
void VehicleEditorNeonColorMenu::FeatureUpdate() {}

VehicleEditorNeonColorMenu* _instance;
VehicleEditorNeonColorMenu* VehicleEditorNeonColorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleEditorNeonColorMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleEditorNeonColorMenu::~VehicleEditorNeonColorMenu() { delete _instance; }