#include "misc_disables.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/global.hpp"

using namespace MiscDisablesMenuVars;

namespace MiscDisablesMenuVars {
	Vars_ m_Vars;

	void DisablePhone() {
		int Type = 19681; // updated
		int Bool_1 = 19864; // updated
		int PhonePosSet = 19609; // updated
		int PhonePosGet = 19618; // updated
		int Bitset_1 = 7552; // updated
		int Bitset_2 = 7551; // updated
		int Unk_1 = 21005; // updated

		if (*Menu::Global(Type).At(1).Get<bool>() != 1) {
			if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("cellphone_flashhand")) > 0 ||
				*Menu::Global(Type).At(1).Get<int>() > 3) {
				if (*Menu::Global(Bool_1).Get<bool>()) {
					Menu::Global(Bool_1).As<int>() = 0;
					Menu::Global(PhonePosSet).As<int>() = *Menu::Global(PhonePosGet).Get<int>();
					Menu::Global(PhonePosSet).At(1).As<int>() = *Menu::Global(PhonePosGet).At(1).Get<int>();
					Menu::Global(PhonePosSet).At(2).As<int>() = *Menu::Global(PhonePosGet).At(2).Get<int>();

					Native::SetMobilePhonePosition(*Menu::Global(PhonePosSet).Get<float>(), *Menu::Global(PhonePosSet).At(1).Get<float>(), *Menu::Global(PhonePosSet).At(2).Get<float>());
				}

				if (*Menu::Global(Type).At(1).Get<int>() == 10 || *Menu::Global(Type).At(1).Get<int>() == 9) {
					Native::SetBit((int*)Menu::Global(Bitset_1).Get<int>(), 16);
				}

				if (Native::IsMobilePhoneCallOngoing()) {
					Native::StopScriptedConversation(false);
				}

				Menu::Global(Unk_1).As<int>() = 5;
				Native::ClearBit(Menu::Global(Bitset_2).Get<int>(), 30);

				if (*Menu::Global(Type).At(1).Get<int>() != 1 && *Menu::Global(Type).At(1).Get<int>() != 0) {
					Menu::Global(Type).At(1).As<int>() = 3;
				}
			}

			Native::SetBit((int*)Menu::Global(Bitset_1).Get<int>(), 2);
		}
	}
}

void MiscDisablesMenu::Init() {
	SetName("Disables");
	SetParentSubmenu<MiscMenu>();

	addOption(ToggleOption("Disable Profanity Filter")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableProfanityFilter).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Phone")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisablePhone).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Calls")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableCalls).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Stunt Jumps")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableStuntJumps).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Cinematic Camera")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableCinematicCamera).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Minimap")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::DisplayRadar(true); })
		.addToggle(m_Vars.m_DisableMinimap).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable HUD")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::DisplayHud(true); })
		.addToggle(m_Vars.m_DisableHUD).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Chat on Open")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableChatOnOpen).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Restricted Areas")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableRestrictedAreas).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Notifications")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableNotifications).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Loading Prompts")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableLoadingPrompts).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Cutscenes")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableCutscenes).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Boundary Limit")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableBoundryLimit).canBeSaved(m_ParentNameStack));

	addOption(ButtonOption("Disable Mission Circles")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::TerminateAllScriptsWithThisName("fmmc_launcher"); }));
}

void MiscDisablesMenu::Update() {}

/*Called once on submenu open*/
void MiscDisablesMenu::UpdateOnce() {}

/*Called always*/
void MiscDisablesMenu::FeatureUpdate() {
	if (m_Vars.m_DisableBoundryLimit) {
		Native::_ExpandWorldLimits(-9000.f, -11000.f, 30.f);
		Native::_ExpandWorldLimits(10000.f, 12000.f, 30.f);
	}

	if (m_Vars.m_DisableCutscenes) {
		if (Native::IsCutsceneActive() || Native::IsCutscenePlaying()) {
			Native::StopCutsceneImmediately();
		}
	}

	if (m_Vars.m_DisableLoadingPrompts) {
		if (Native::_IsLoadingPromptBeingDisplayed()) {
			Native::_RemoveLoadingPrompt();
		}
	}

	if (m_Vars.m_DisableNotifications) {
		Native::ClearAllHelpMessages();
	}

	if (m_Vars.m_DisableMinimap) {
		Native::DisplayRadar(false);
	}

	if (m_Vars.m_DisableHUD) {
		Native::DisplayHud(false);
	}

	if (m_Vars.m_DisableCalls) {
		if (Native::IsMobilePhoneCallOngoing()) {
			Native::StopScriptedConversation(false);
		}
	}

	if (m_Vars.m_DisableStuntJumps) {
		if (Native::IsStuntJumpInProgress()) {
			Native::CancelStuntJump();
		}
	}

	if (m_Vars.m_DisableCinematicCamera) {
		Native::DisableControlAction(0, ControlVehicleCinCam, true);
	}

	if (m_Vars.m_DisablePhone) {
		DisablePhone();
	}

	if (m_Vars.m_DisableRestrictedAreas) {
		std::string AreaScripts[] = {
			"am_armybase",
			"restrictedareas",
			"re_armybase",
			"re_lossantosintl",
			"re_prison",
			"re_prisonvanbreak"
		};

		for (auto& Area : AreaScripts) {
			if (Native::DoesScriptExist(Area.c_str())) {
				Native::TerminateAllScriptsWithThisName(Area.c_str());
			}
		}
	}
}

MiscDisablesMenu* _instance;
MiscDisablesMenu* MiscDisablesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscDisablesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscDisablesMenu::~MiscDisablesMenu() { delete _instance; }