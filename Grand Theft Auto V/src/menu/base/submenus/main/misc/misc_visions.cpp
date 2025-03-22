#include "misc_visions.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "utils/memory/memory.hpp"

using namespace MiscVisionsMenuVars;

namespace MiscVisionsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> Visions[] {
		{ TranslationString("None", true), "" },
		{ TranslationString("Sunglasses", true), "sunglasses" },
		{ TranslationString("Camera BW", true), "CAMERA_BW" },
		{ TranslationString("Hint Cam", true), "Hint_cam" },
		{ TranslationString("UFO", true), "ufo" },
		{ TranslationString("UFO Deathray", true), "ufo_deathray" },
		{ TranslationString("Dying", true), "dying" },
		{ TranslationString("Underwater", true), "underwater" },
		{ TranslationString("Spectator", true), "spectator1" },
		{ TranslationString("Cops Splash", true), "CopsSPLASH" },
		{ TranslationString("Crane Cam", true), "crane_cam" },
		{ TranslationString("Secret Camera", true), "secret_camera" },
		{ TranslationString("Graveyard Shootout", true), "graveyard_shootout" },
		{ TranslationString("Cops", true), "cops" },
		{ TranslationString("Damage", true), "damage" },
		{ TranslationString("Bikers Splash", true), "BikersSPLASH" },
		{ TranslationString("Bikers", true), "Bikers" },
		{ TranslationString("Prologue Shootout", true), "prologue_shootout" },
		{ TranslationString("Vagos", true), "vagos" },
		{ TranslationString("Sniper", true), "sniper" },
	};
}

void MiscVisionsMenu::Init() {
	SetName("Visions");
	SetParentSubmenu<MiscMenu>();

	memcpy(m_Vars.m_Backup, (void*)(Global::Vars::g_SetSeethrough + 0x14), 5);

	addOption(ToggleOption("Thermal Vision")
		.addTranslation().addHotkey()
		.addOnClick([] {
			if (m_Vars.m_ThermalVision) {
				Memory::WriteVector(Global::Vars::g_SetSeethrough + 0x14, { 0xB3, 0x01, 0x90, 0x90, 0x90 });
				Native::SetSeethrough(true);
			} else {
				Memory::WriteMemcpy((void*)(Global::Vars::g_SetSeethrough + 0x14), m_Vars.m_Backup, 5);
				Native::SetSeethrough(false);
			}
		})
		.addToggle(m_Vars.m_ThermalVision).canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Visions")
		.addTranslation());

	for (int i = 0; i < NUMOF(Visions); i++) {
		addOption(RadioOption(Visions[i].m_Name.Get())
			.addTranslation()
			.addRadio(m_Vars.m_Context)
			.addOnClick([=] { if (i == 0) Native::ClearTimecycleModifier(); }));
	}
}

void MiscVisionsMenu::Update() {}

/*Called once on submenu open*/
void MiscVisionsMenu::UpdateOnce() {}

/*Called always*/
void MiscVisionsMenu::FeatureUpdate() {
	if (m_Vars.m_Context.m_Selected != 0) {
		Native::SetTimecycleModifier(Visions[m_Vars.m_Context.m_Selected].m_Result);
	}
}

MiscVisionsMenu* _instance;
MiscVisionsMenu* MiscVisionsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscVisionsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscVisionsMenu::~MiscVisionsMenu() { delete _instance; }