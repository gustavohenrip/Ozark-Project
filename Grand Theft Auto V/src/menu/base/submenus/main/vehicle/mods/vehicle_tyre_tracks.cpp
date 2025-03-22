#include "vehicle_tyre_tracks.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_mods.hpp"
#include "menu/base/util/instructionals.hpp"
#include "menu/base/submenus/main/helpers/rainbow_config.hpp"

using namespace VehicleModsTyreTracksMenuVars;

namespace VehicleModsTyreTracksMenuVars {
	Vars_ m_Vars;

	void Persistent() {
		Rage::Types::VfxWheel* VfxWheel = Global::Vars::g_VfxWheel;
		Rage::Types::VfxWheel* VfxWheelCache = m_Vars.m_Cache;

		for (uint32_t i = 0; i < (uint32_t)(Global::Vars::g_VfxWheelCount * 4); i++) {
			if (m_Vars.m_Persistent) {
				VfxWheel[i].m_Type1 = 53;
				VfxWheel[i].m_Type2 = 53;
				VfxWheel[i].m_Type3 = 64;
				VfxWheel[i].m_Type4 = 255;
				VfxWheel[i].m_Unk1 = TRUE;
				VfxWheel[i].m_Unk2 = TRUE;
				VfxWheel[i].m_Unk3 = TRUE;
				VfxWheel[i].m_Unk4 = TRUE;
				VfxWheel[i].m_PressureMin = 0.0f;
				VfxWheel[i].m_PressureMax = 0.0f;
			} else {
				VfxWheel[i].m_Type1 = VfxWheelCache[i].m_Type1;
				VfxWheel[i].m_Type2 = VfxWheelCache[i].m_Type2;
				VfxWheel[i].m_Type3 = VfxWheelCache[i].m_Type3;
				VfxWheel[i].m_Type4 = VfxWheelCache[i].m_Type4;
				VfxWheel[i].m_Unk1 = VfxWheelCache[i].m_Unk1;
				VfxWheel[i].m_Unk2 = VfxWheelCache[i].m_Unk2;
				VfxWheel[i].m_Unk3 = VfxWheelCache[i].m_Unk3;
				VfxWheel[i].m_Unk4 = VfxWheelCache[i].m_Unk4;
				VfxWheel[i].m_PressureMin = VfxWheelCache[i].m_PressureMin;
				VfxWheel[i].m_PressureMax = VfxWheelCache[i].m_PressureMax;
			}
		}
	}

	void SetTireTrackColor(bool Reset, Color Col) {
		Rage::Types::VfxWheel* VfxWheel = Global::Vars::g_VfxWheel;
		Rage::Types::VfxWheel* VfxWheelCache = m_Vars.m_Cache;

		for (uint32_t i = 0; i < (uint32_t)(Global::Vars::g_VfxWheelCount * 4); i++) {
			if (!Reset) {
				VfxWheel[i].m_R = Col.R;
				VfxWheel[i].m_G = Col.G;
				VfxWheel[i].m_B = Col.B;
			} else {
				VfxWheel[i].m_R = VfxWheelCache[i].m_R;
				VfxWheel[i].m_G = VfxWheelCache[i].m_G;
				VfxWheel[i].m_B = VfxWheelCache[i].m_B;
			}
		}
	}
}

void VehicleModsTyreTracksMenu::Init() {
	SetName("Tyre Tracks");
	SetParentSubmenu<VehicleModsMenu>();

	m_DefaultInstructionals = false;

	// Initialize cache
	m_Vars.m_Cache = (Rage::Types::VfxWheel*)malloc((sizeof(Rage::Types::VfxWheel) * Global::Vars::g_VfxWheelCount) * 4);
	memcpy(m_Vars.m_Cache, Global::Vars::g_VfxWheel, (sizeof(Rage::Types::VfxWheel) * Global::Vars::g_VfxWheelCount) * 4);

	addOption(ToggleOption("Persistent Tracks")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Persistent)
		.addOnClick(Persistent).canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Color")
		.addTranslation());

	addOption(NumberOption<int>(SCROLL, "[R]ed")
		.addTranslation()
		.addNumber(m_Vars.m_Color.R, "%i", 1).addMin(0).addMax(255)
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_Color); }));

	addOption(NumberOption<int>(SCROLL, "[G]reen")
		.addTranslation()
		.addNumber(m_Vars.m_Color.G, "%i", 1).addMin(0).addMax(255)
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_Color); }));

	addOption(NumberOption<int>(SCROLL, "[B]lue")
		.addTranslation()
		.addNumber(m_Vars.m_Color.B, "%i", 1).addMin(0).addMax(255)
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_Color); }));

	addOption(ButtonOption("Apply Color")
		.addTranslation()
		.addOnClick([] { SetTireTrackColor(false, m_Vars.m_Color); }));

	addOption(ToggleOption("Rainbow")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RainbowToggle)
		.addOnClick([] {
			m_Vars.m_Rainbow.Toggle(m_Vars.m_RainbowToggle);
			if (m_Vars.m_RainbowToggle) {
				m_Vars.m_Rainbow.Add(&m_Vars.m_Color);
			} else {
				m_Vars.m_Rainbow.Remove(&m_Vars.m_Color);
			}
		}).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Save to Config")
		.addTranslation()
		.addToggle(m_Vars.m_SaveToConfig).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Reset to Default")
		.addTranslation().addHotkey()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([=] {
			m_Vars.m_RainbowToggle = false;
			m_Vars.m_Rainbow.Stop();
			SetTireTrackColor(true, Color());
		}));


	if (m_Vars.m_SaveToConfig) {
		m_Vars.m_RainbowToggle = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Rainbow", false);
		m_Vars.m_Persistent = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Persistent Tracks", false);
		Utils::GetConfig()->ReadColor(m_ParentNameStack, "Color", &m_Vars.m_Color);

		if (m_Vars.m_RainbowToggle) {
			m_Vars.m_Rainbow.Toggle(m_Vars.m_RainbowToggle);
			m_Vars.m_Rainbow.Add(&m_Vars.m_Color);
		}

		if (m_Vars.m_Persistent) {
			Persistent();
		}
	}
}

void VehicleModsTyreTracksMenu::Update() {
	if (Menu::GetBase()->m_CurrentOption == 6) {
		Instructionals::Rainbow();

		if (Menu::Util::GetInput()->IsJustPressed(false, ControlFrontendX)) {
			RainbowConfigMenuVars::m_Vars.m_Rainbow = &m_Vars.m_Rainbow;
			RainbowConfigMenu::GetInstance()->SetParentSubmenu<VehicleModsTyreTracksMenu>();
			Menu::GetSubmenuHandler()->SetSubmenu(RainbowConfigMenu::GetInstance());
		}
	} else Instructionals::Standard();
}

/*Called once on submenu open*/
void VehicleModsTyreTracksMenu::UpdateOnce() {
	if (m_Vars.m_SaveToConfig) {
		Utils::GetConfig()->WriteColor(m_ParentNameStack, "Color", m_Vars.m_Color);
	}
}

/*Called always*/
void VehicleModsTyreTracksMenu::FeatureUpdate() {
	if (m_Vars.m_RainbowToggle) {
		m_Vars.m_Rainbow.Run();
		SetTireTrackColor(false, m_Vars.m_Color);
	}
}

VehicleModsTyreTracksMenu* _instance;
VehicleModsTyreTracksMenu* VehicleModsTyreTracksMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleModsTyreTracksMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleModsTyreTracksMenu::~VehicleModsTyreTracksMenu() { delete _instance; free(m_Vars.m_Cache); }