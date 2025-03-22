#include "player.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "player/appearance.hpp"
#include "player/movement.hpp"
#include "player/animation.hpp"
#include "player/particle_fx.hpp"
#include "menu/base/util/global.hpp"

using namespace PlayerMenuVars;

namespace PlayerMenuVars {
	Vars_ m_Vars;

	void CopsTurnBlindEye() {
		if (m_Vars.m_CopsTurnBlindEye) {
			Menu::Global(2540612).At(4619).As<int>() = 5;
			Menu::Global(2540612).At(4622).As<int>() = Native::GetNetworkTime() + 637000;
		} else {
			Menu::Global(2540612).At(4619).As<int>() = 0;
			Menu::Global(2540612).At(4622).As<int>() = Native::GetNetworkTime();
		}
	}

	void OffTheRadar() {
		if (m_Vars.m_OffTheRadar) {
			Menu::Global(2426097).At(Menu::GetLocalPlayer().m_ID, 443).At(204).As<int>() = 1;
			Menu::Global(2440277).At(70).As<int>() = Native::GetNetworkTime() + 757000;
		} else {
			Menu::Global(2426097).At(Menu::GetLocalPlayer().m_ID, 443).At(204).As<int>() = 0;
			Menu::Global(2440277).At(70).As<int>() = Native::GetNetworkTime();
		}
	}

	void RevealHiddenPlayers() {
		if (m_Vars.m_RevealHiddenPlayers) {
			Menu::Global(2426097).At(Menu::GetLocalPlayer().m_ID, 443).At(207).As<int>() = 1;
			Menu::Global(2440277).At(71).As<int>() = Native::GetNetworkTime() + 757000;
		} else {
			Menu::Global(2426097).At(Menu::GetLocalPlayer().m_ID, 443).At(207).As<int>() = 0;
			Menu::Global(2440277).At(71).As<int>() = Native::GetNetworkTime();
		}
	}

	void NoRagdoll() {
		Native::SetPedCanRagdoll(Menu::GetLocalPlayer().m_Ped, !m_Vars.m_DisableRagdoll);
		Native::SetPedCanRagdollFromPlayerImpact(Menu::GetLocalPlayer().m_Ped, !m_Vars.m_DisableRagdoll);
		Native::SetPedCanBeKnockedOffVehicle(Menu::GetLocalPlayer().m_Ped, !m_Vars.m_DisableRagdoll);
	}

	void Badsport(bool Toggle) {
		if (!Toggle) Native::StatSetBool(0x8C1C0FAF, false, true);
		Native::StatSetFloat(0xBE89A9D2, Toggle ? 300.f : 0.f, true);
	}
}

void PlayerMenu::Init() {
	SetName("Player");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Appearance")
		.addTranslation().addHotkey()
		.addSubmenu<AppearanceMenu>());

	addOption(SubmenuOption("Movement")
		.addTranslation().addHotkey()
		.addSubmenu<MovementMenu>());

	addOption(SubmenuOption("Animation")
		.addTranslation().addHotkey()
		.addSubmenu<AnimationMenu>());

	addOption(SubmenuOption("Particle FX")
		.addTranslation().addHotkey()
		.addSubmenu<ParticleFXMenu>());

	addOption(ToggleOption("Godmode")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Godmode).canBeSaved(m_ParentNameStack)
		.addOnClick([] {
			if (!m_Vars.m_Godmode) {
				Native::SetEntityInvincible(Menu::GetLocalPlayer().m_Ped, false);
			}
		}));

	addOption(ToggleOption("Disable Police")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisablePolice).canBeSaved(m_ParentNameStack)
		.addOnClick([] {
			if (!m_Vars.m_DisablePolice) {
				Native::SetMaxWantedLevel(5);
			}
		}));

	addOption(ToggleOption("Disable Ragdoll")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableRagdoll).canBeSaved(m_ParentNameStack)
		.addOnClick(NoRagdoll));

	addOption(ToggleOption("Off the Radar")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_OffTheRadar).canBeSaved(m_ParentNameStack)
		.addOnClick(OffTheRadar));

	addOption(ToggleOption("Cops Turn Blind Eye")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_CopsTurnBlindEye).canBeSaved(m_ParentNameStack)
		.addOnClick(CopsTurnBlindEye));

	addOption(ToggleOption("Reveal Hidden Players")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RevealHiddenPlayers).canBeSaved(m_ParentNameStack)
		.addOnClick(RevealHiddenPlayers));

	addOption(ButtonOption("Bullshark Testosterone")
		.addTranslation().addHotkey()
		.addOnClick([] { Menu::Global(2440277).At(4006).As<bool>() = true; }));

	addOption(ButtonOption("Suicide")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::ApplyDamageToPed(Menu::GetLocalPlayer().m_Ped, 30000, true); }));

	addOption(ButtonOption("Clone")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::ClonePed(Menu::GetLocalPlayer().m_Ped, Menu::GetLocalPlayer().m_Heading, true, false); }));

	addOption(ButtonOption("Health & Armor Regeneration")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::SetPedArmour(Menu::GetLocalPlayer().m_Ped, 100);
			Native::SetEntityHealth(Menu::GetLocalPlayer().m_Ped, Native::GetEntityMaxHealth(Menu::GetLocalPlayer().m_Ped));
		}));

	addOption(ToggleOption("Reduced Collision")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ReducedCollision).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Invisibility")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Invisibility).canBeSaved(m_ParentNameStack)
		.addOnClick([] { if (!m_Vars.m_Invisibility) Native::SetEntityVisible(Menu::GetLocalPlayer().m_Ped, true, true); }));

	addOption(ToggleOption("Superman")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Superman));

	addOption(ToggleOption("Badsport")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Badsport)
		.addOnClick([] {
			Badsport(m_Vars.m_Badsport);
		}).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Breathe Fire")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_BreatheFire)
		.addOnClick([] { if (!m_Vars.m_BreatheFire) Native::RemoveParticleFx(m_Vars.m_BreatheFireHandle, false); }).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Swim Anywhere")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_SwimAnywhere).canBeSaved(m_ParentNameStack)
		.addTooltip("~y~Notice: ~w~Local only"));
}

void PlayerMenu::Update() {}

/*Called once on submenu open*/
void PlayerMenu::UpdateOnce() {}

/*Called always*/
void PlayerMenu::FeatureUpdate() {
	if (m_Vars.m_Godmode) {
		Native::SetEntityInvincible(Menu::GetLocalPlayer().m_Ped, true);
	}

	if (m_Vars.m_DisableRagdoll) {
		NoRagdoll();
	}

	if (m_Vars.m_DisablePolice) {
		Native::SetMaxWantedLevel(0);
		Native::ClearPlayerWantedLevel(Menu::GetLocalPlayer().m_ID);
	}

	if (m_Vars.m_ReducedCollision) {
		Native::SetPedCapsule(Menu::GetLocalPlayer().m_Ped, 0.001f);
	}

	if (m_Vars.m_Invisibility) {
		Native::SetEntityVisible(Menu::GetLocalPlayer().m_Ped, false, false);
	}

	if (m_Vars.m_Superman) {
		int LeftRight = Native::GetControlValue(2, 188);
		int UpDown = Native::GetControlValue(2, 189);

		if (Native::GetEntityHeightAboveGround(Menu::GetLocalPlayer().m_Ped) < 3.f) {
			Native::GiveWeaponToPed(Menu::GetLocalPlayer().m_Ped, 0xFBAB5776, -1, true, true);

			Math::Vector3<float> Coords = Menu::GetLocalPlayer().m_Coords;
			Coords.m_Z += 100.f;
			Native::SetEntityCoords(Menu::GetLocalPlayer().m_Ped, Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);
		} else {
			if (UpDown == 254 || Menu::Util::GetInput()->IsPressed(true, 'S'))
				Native::ApplyForceToEntity(Menu::GetLocalPlayer().m_Ped, 1, 0.f, 3.f, 10.f, 0.f, 0.f, 0.f, false, true, true, true, false, true);

			if (UpDown == 0 || Menu::Util::GetInput()->IsPressed(true, 'W'))
				Native::ApplyForceToEntity(Menu::GetLocalPlayer().m_Ped, 1, 0.f, 3.f, 0.f, 0.f, 0.f, 0.f, false, true, true, true, false, true);

			if (Menu::Util::GetInput()->IsPressed(true, VK_SHIFT) || Menu::Util::GetInput()->IsPressed(false, ControlFrontendRt))
				Native::ApplyForceToEntity(Menu::GetLocalPlayer().m_Ped, 1, 0.f, 15.f, .3f, 0.f, 0.f, 0.f, false, true, true, true, false, true);

			if (LeftRight == 254 || Menu::Util::GetInput()->IsPressed(true, 'D'))
				Native::ApplyForceToEntity(Menu::GetLocalPlayer().m_Ped, 1, 1.20f, 0.f, 0.f, 0.f, 0.1f, 0.f, false, true, true, true, false, true);

			if (LeftRight == 0 || Menu::Util::GetInput()->IsPressed(true, 'A'))
				Native::ApplyForceToEntity(Menu::GetLocalPlayer().m_Ped, 1, -1.20f, 0.f, 0.f, 0.f, 0.1f, 0.f, false, true, true, true, false, true);
		}
	}

	if (m_Vars.m_OffTheRadar) {
		OffTheRadar();
	}

	if (m_Vars.m_RevealHiddenPlayers) {
		RevealHiddenPlayers();
	}

	if (m_Vars.m_CopsTurnBlindEye) {
		CopsTurnBlindEye();
	}

	if (m_Vars.m_SwimAnywhere) {
		if (Global::Vars::g_PedFactory) {
			auto Ped = Global::Vars::g_PedFactory->m_LocalPed;
			if (Ped) {
				Ped->SetFlags((1 << 1)); // IS_PED_SWIMMING
			}
		}
	}

	if (m_Vars.m_BreatheFire) {
		if (Native::HasNamedPtfxAssetLoaded("core")) {
			Native::_UseParticleFxAssetNextCall("core");
			if (!Native::DoesParticleFxLoopedExist(m_Vars.m_BreatheFireHandle)) {
				m_Vars.m_BreatheFireHandle = Native::StartParticleFxLoopedOnEntityBone("ent_sht_flame", Menu::GetLocalPlayer().m_Ped, -0.02f, 0.2f, 0.0f, 90.0f, 100.0f, 90.0f, Native::GetPedBoneIndex(Menu::GetLocalPlayer().m_Ped, 31086), 1.0f, false, false, false);
			}
		} else {
			Native::RequestNamedPtfxAsset("core");
		}
	}
}

PlayerMenu* _instance;
PlayerMenu* PlayerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PlayerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PlayerMenu::~PlayerMenu() { delete _instance; }