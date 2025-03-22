#include "explosion_editor.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../weapon_mods.hpp"

using namespace ExplosionEditorMenuVars;

namespace ExplosionEditorMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> ExplosionNames[74];

	ScrollStruct<uint32_t> ExplosionFXList[] = {
		{ TranslationString("GRENADE", true), 0x8CBD7381 },
		{ TranslationString("STICKYBOMB", true), 0x6818372E },
		{ TranslationString("MOLOTOV", true), 0x289C84AB },
		{ TranslationString("ROCKET", true), 0xC18A7083 },
		{ TranslationString("TANKSHELL", true), 0xC6109DA9 },
		{ TranslationString("PETROL_PUMP", true), 0x5C685140 },
		{ TranslationString("VEHICLE", true), 0x1FDD8CC7 },
		{ TranslationString("BIKE", true), 0x2E89B6C6 },
		{ TranslationString("DIR_STEAM", true), 0xB096860 },
		{ TranslationString("DIR_FLAME", true), 0x61BEF304 },
		{ TranslationString("DIR_WATER_HYDRANT", true), 0xC9E6266C },
		{ TranslationString("DIR_GAS_CANISTER", true), 0xDCD1237B },
		{ TranslationString("TRUCK", true), 0x272C2A1C },
		{ TranslationString("BULLET", true), 0x4A602D1B },
		{ TranslationString("SMOKE_GRENADE", true), 0x1B171B0A },
		{ TranslationString("BZGAS", true), 0xC36F9FF4 },
		{ TranslationString("FBI4_TRUCK_DOORS", true), 0x68EC3F3E },
		{ TranslationString("GAS_CANISTER", true), 0x531AE6D7 },
		{ TranslationString("EXTINGUISHER", true), 0x3C212E68 },
		{ TranslationString("TRAIN", true), 0x7DF5D791 },
		{ TranslationString("BARREL", true), 0x26A49BC },
		{ TranslationString("PROPANE", true), 0x2A8D8114 },
		{ TranslationString("BLIMP", true), 0xBA683EF5 },
		{ TranslationString("FLARE", true), 0x4DFE55EF },
		{ TranslationString("PLANE_ROCKET", true), 0x549AE743 },
		{ TranslationString("TANKER", true), 0x6136E279 },
		{ TranslationString("PLANE", true), 0x174DDF67 },
		{ TranslationString("BOAT", true), 0x9AE5CE85 },
		{ TranslationString("TREV3_TRAILER", true), 0x567CAF1C },
		{ TranslationString("BLIMP2", true), 0x9077376B },
		{ TranslationString("BIRDCRAP", true), 0x8B716982 },
		{ TranslationString("FIREWORK", true), 0x1499FEF9 },
		{ TranslationString("SNOWBALL", true), 0x4B0245BA },
		{ TranslationString("AIR_DEFENCE", true), 0x3C84F30B },
		{ TranslationString("PIPEBOMB", true), 0x2E3BC2D6 },
		{ TranslationString("BOMB_CLUSTER", true), 0x7007891E },
		{ TranslationString("BOMB_GAS", true), 0xAD0A7E5D },
		{ TranslationString("BOMB_INCENDIARY", true), 0x62F8744D },
		{ TranslationString("BOMB_STANDARD", true), 0x30A5254A },
		{ TranslationString("TORPEDO_UNDERWATER", true), 0x8D16EE2E },
		{ TranslationString("MINE_UNDERWATER", true), 0x4F7CD2DC },
		{ TranslationString("OPPRESSOR2_CANNON", true), 0x3c4ccaab },
		{ TranslationString("MORTAR_KINETIC", true), 0x996d8865 },
		{ TranslationString("VEHMINE_KINETIC", true), 0xd0cb2c03 },
		{ TranslationString("VEHMINE_EMP", true), 0xeec9e329 },
		{ TranslationString("VEHMINE_SPIKE", true), 0xd1c5557c },
		{ TranslationString("VEHMINE_SLICK", true), 0x98bc33a3 },
		{ TranslationString("VEHMINE_TAR", true), 0x1325b17c },
		{ TranslationString("BURIEDMINE", true), 0x25a8cebf },
		{ TranslationString("RAYGUN", true), 0x86ad156c },
		{ TranslationString("RCTANK_ROCKET", true), 0x26cb9d76 },
		{ TranslationString("SUBMARINE_BIG", true), 0x4D807070 },
	};

	Rage::Types::ExplosionData* GetExplosion(uint32_t ExplosionHash) {
		for (std::size_t i = 0; i < 74; i++) {
			auto Explosion = &Global::Vars::g_ExplosionData[i];
			if (Native::GetHashKey(Explosion->m_ExplosionName) == ExplosionHash) {
				return Explosion;
			}
		}

		return nullptr;
	}

	Rage::Types::ExplosionFX* GetExplosionFX(uint32_t ExplosionFXHash) {
		for (std::size_t i = 0; i < 52; i++) {
			auto ExplosionFX = &Global::Vars::g_ExplosionFX[i];
			if (ExplosionFX->m_NameHash == ExplosionFXHash) {
				return ExplosionFX;
			}
		}

		return nullptr;
	}

	std::size_t GetExplosionFXEntry(uint32_t Hash) {
		for (std::size_t i = 0; i < NUMOF(ExplosionFXList); i++) {
			if (ExplosionFXList[i].m_Result == Hash) {
				return i;
			}
		}

		return 0;
	}

	void UpdateEditor() {
		if (ExplosionNames[0].m_Result != 1337) {
			for (std::size_t i = 0; i < NUMOF(ExplosionNames); i++) {
				auto Explosion = &Global::Vars::g_ExplosionData[i];
				ExplosionNames[i].m_Name.Set(Native::_GetTextSubstringSlice(Explosion->m_ExplosionName, 8, (int)strlen(Explosion->m_ExplosionName)));
				ExplosionNames[i].m_Result = 1337;
			}
		}

		m_Vars.m_ExplosionDataPtr = &Global::Vars::g_ExplosionData[m_Vars.m_SelectedExplosion];
		memcpy(&m_Vars.m_ExplosionData, m_Vars.m_ExplosionDataPtr, sizeof(Rage::Types::ExplosionData));

		m_Vars.m_SelectedFX = (int)GetExplosionFXEntry(m_Vars.m_ExplosionDataPtr->m_VFXTagNameHash);

		m_Vars.m_ExplosionFXPtr = GetExplosionFX(m_Vars.m_ExplosionDataPtr->m_VFXTagNameHash);
		if (m_Vars.m_ExplosionFXPtr) {
			memcpy(&m_Vars.m_ExplosionFX, m_Vars.m_ExplosionFXPtr, sizeof(Rage::Types::ExplosionFX));
		}
	}
}

void ExplosionEditorMenu::Init() {
	SetName("Explosion Editor");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ScrollOption<int>(SCROLL, "Explosion")
		.addTranslation()
		.addScroll(m_Vars.m_SelectedExplosion, 0, NUMOF(ExplosionNames), ExplosionNames)
		.addOnClick(UpdateEditor));

	addOption(ScrollOption<uint32_t>(SCROLL, "FX")
		.addTranslation()
		.addScroll(m_Vars.m_SelectedFX, 0, NUMOF(ExplosionFXList), ExplosionFXList)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_VFXTagNameHash = ExplosionFXList[m_Vars.m_SelectedFX].m_Result; }));

	addOption(NumberOption<float>(SCROLL, "FX Scale")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionFX.m_Scale, "%.1f", 0.1f).addMin(-1.f).addMax(100.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionFXPtr->m_Scale = m_Vars.m_ExplosionFX.m_Scale; })
		.addRequirement([] { return m_Vars.m_ExplosionFXPtr != nullptr; }));

	addOption(NumberOption<float>(SCROLL, "Center Damage")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_DamageAtCentre, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_DamageAtCentre = m_Vars.m_ExplosionData.m_DamageAtCentre; }));

	addOption(NumberOption<float>(SCROLL, "Edge Damage")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_DamageAtEdge, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_DamageAtEdge = m_Vars.m_ExplosionData.m_DamageAtEdge; }));

	addOption(NumberOption<float>(SCROLL, "Network Player Modifier")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_NetworkPlayerModifier, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_NetworkPlayerModifier = m_Vars.m_ExplosionData.m_NetworkPlayerModifier; }));

	addOption(NumberOption<float>(SCROLL, "Network Ped Modifier")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_NetworkPedModifier, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_NetworkPedModifier = m_Vars.m_ExplosionData.m_NetworkPedModifier; }));

	addOption(NumberOption<float>(SCROLL, "End Radius")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_EndRadius, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_EndRadius = m_Vars.m_ExplosionData.m_EndRadius; }));

	addOption(NumberOption<float>(SCROLL, "Init Speed")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_InitSpeed, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_InitSpeed = m_Vars.m_ExplosionData.m_InitSpeed; }));

	addOption(NumberOption<float>(SCROLL, "Decay Factor")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_DecayFactor, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_DecayFactor = m_Vars.m_ExplosionData.m_DecayFactor; }));

	addOption(NumberOption<float>(SCROLL, "Force Factor")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_ForceFactor, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_ForceFactor = m_Vars.m_ExplosionData.m_ForceFactor; }));

	addOption(NumberOption<float>(SCROLL, "Ragdoll Force Modifier")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_RagdollForceModifier, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_RagdollForceModifier = m_Vars.m_ExplosionData.m_RagdollForceModifier; }));

	addOption(NumberOption<float>(SCROLL, "Self Force Modifier")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_SelfForceModifier, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_SelfForceModifier = m_Vars.m_ExplosionData.m_SelfForceModifier; }));

	addOption(NumberOption<float>(SCROLL, "Directed Width")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_DirectedWidth, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_DirectedWidth = m_Vars.m_ExplosionData.m_DirectedWidth; }));

	addOption(NumberOption<float>(SCROLL, "Directed Lifetime")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_DirectedLifeTime, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_DirectedLifeTime = m_Vars.m_ExplosionData.m_DirectedLifeTime; }));

	addOption(NumberOption<float>(SCROLL, "Cam Shake")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_CamShake, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_CamShake = m_Vars.m_ExplosionData.m_CamShake; }));

	addOption(NumberOption<float>(SCROLL, "Cam Shake Rolloff Scaling")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_CamShakeRollOffScaling, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_CamShakeRollOffScaling = m_Vars.m_ExplosionData.m_CamShakeRollOffScaling; }));

	addOption(NumberOption<float>(SCROLL, "Shocking Event Visual Range")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_ShockingEventVisualRangeOverride, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_ShockingEventVisualRangeOverride = m_Vars.m_ExplosionData.m_ShockingEventVisualRangeOverride; }));

	addOption(NumberOption<float>(SCROLL, "Shocking Event Audio Range")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_ShockingEventAudioRangeOverride, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_ShockingEventAudioRangeOverride = m_Vars.m_ExplosionData.m_ShockingEventAudioRangeOverride; }));

	addOption(NumberOption<float>(SCROLL, "Frag Damage")
		.addTranslation()
		.addNumber(m_Vars.m_ExplosionData.m_FragDamage, "%.1f", 0.1f).addMin(-1.f).addMax(10000.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_FragDamage = m_Vars.m_ExplosionData.m_FragDamage; }));

	addOption(ToggleOption("Minor Explosion")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_MinorExplosion)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_MinorExplosion = m_Vars.m_ExplosionData.m_MinorExplosion; }));

	addOption(ToggleOption("Applies Continuous Damage")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_AppliesContinuousDamage)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_AppliesContinuousDamage = m_Vars.m_ExplosionData.m_AppliesContinuousDamage; }));

	addOption(ToggleOption("Post Process Collisions")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_PostProcessCollisionsWithNoForce)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_PostProcessCollisionsWithNoForce = m_Vars.m_ExplosionData.m_PostProcessCollisionsWithNoForce; }));

	addOption(ToggleOption("Damage Vehicles")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_DamageVehicles)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_DamageVehicles = m_Vars.m_ExplosionData.m_DamageVehicles; }));

	addOption(ToggleOption("Damage Objects")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_DamageObjects)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_DamageObjects = m_Vars.m_ExplosionData.m_DamageObjects; }));

	addOption(ToggleOption("Only Affect Living Peds")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_OnlyAffectsLivePeds)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_OnlyAffectsLivePeds = m_Vars.m_ExplosionData.m_OnlyAffectsLivePeds; }));

	addOption(ToggleOption("Ignore Exploding Entity")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_IgnoreExplodingEntity)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_IgnoreExplodingEntity = m_Vars.m_ExplosionData.m_IgnoreExplodingEntity; }));

	addOption(ToggleOption("No Occlusion")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_NoOcclusion)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_NoOcclusion = m_Vars.m_ExplosionData.m_NoOcclusion; }));

	addOption(ToggleOption("Explode Attach When Finished")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_ExplodeAttachEntityWhenFinished)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_ExplodeAttachEntityWhenFinished = m_Vars.m_ExplosionData.m_ExplodeAttachEntityWhenFinished; }));

	addOption(ToggleOption("Can Set Ped On Fire")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_CanSetPedOnFire)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_CanSetPedOnFire = m_Vars.m_ExplosionData.m_CanSetPedOnFire; }));

	addOption(ToggleOption("Can Set Player On Fire")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_CanSetPlayerOnFire)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_CanSetPlayerOnFire = m_Vars.m_ExplosionData.m_CanSetPlayerOnFire; }));

	addOption(ToggleOption("Supress Crime")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_SuppressCrime)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_SuppressCrime = m_Vars.m_ExplosionData.m_SuppressCrime; }));

	addOption(ToggleOption("Use Distance Damage Calculation")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_UseDistanceDamageCalc)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_UseDistanceDamageCalc = m_Vars.m_ExplosionData.m_UseDistanceDamageCalc; }));

	addOption(ToggleOption("Prevent Water Explosion VFX")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_PreventWaterExplosionVFX)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_PreventWaterExplosionVFX = m_Vars.m_ExplosionData.m_PreventWaterExplosionVFX; }));

	addOption(ToggleOption("Ignore Fire Ratio Check")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_IgnoreRatioCheckForFire)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_IgnoreRatioCheckForFire = m_Vars.m_ExplosionData.m_IgnoreRatioCheckForFire; }));

	addOption(ToggleOption("Allow Underwater Explosion")
		.addTranslation()
		.addToggle(m_Vars.m_ExplosionData.m_AllowUnderwaterExplosion)
		.addOnClick([] { m_Vars.m_ExplosionDataPtr->m_AllowUnderwaterExplosion = m_Vars.m_ExplosionData.m_AllowUnderwaterExplosion; }));

	for (std::size_t i = 0; i < NUMOF(ExplosionNames); i++) {
		ExplosionNames[i].m_Name.Set("");
		ExplosionNames[i].m_Result = 0;
	}
}

void ExplosionEditorMenu::Update() {}

/*Called once on submenu open*/
void ExplosionEditorMenu::UpdateOnce() {
	UpdateEditor();
}

/*Called always*/
void ExplosionEditorMenu::FeatureUpdate() {}

ExplosionEditorMenu* _instance;
ExplosionEditorMenu* ExplosionEditorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ExplosionEditorMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ExplosionEditorMenu::~ExplosionEditorMenu() { delete _instance; }