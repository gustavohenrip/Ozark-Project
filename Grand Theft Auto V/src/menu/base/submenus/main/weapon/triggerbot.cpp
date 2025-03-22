#include "triggerbot.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "menu/base/util/timers.hpp"
#include "aimbot.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/timers.hpp"

using namespace TriggerbotMenuVars;

namespace TriggerbotMenuVars {
	Vars_ m_Vars;
}

void TriggerbotMenu::Init() {
	SetName("Triggerbot");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ToggleOption("Toggle Triggerbot")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Enable).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Aim Entity")
		.addTranslation()
		.addScroll(m_Vars.m_AimEntity, 0, NUMOF(AimbotMenuVars::AimEntity), AimbotMenuVars::AimEntity).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(SCROLL, "Aim Tag")
		.addTranslation()
		.addScroll(m_Vars.m_AimTag, 0, NUMOF(Global::Arrays::g_PedBones), Global::Arrays::g_PedBones).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<int>(SCROLL, "Bullet Delay")
		.addTranslation()
		.addNumber(m_Vars.m_BulletBuffer, "%i", 20).addMin(0).addMax(2000).canBeSaved(m_ParentNameStack));
}

void TriggerbotMenu::Update() {}

/*Called once on submenu open*/
void TriggerbotMenu::UpdateOnce() {}

/*Called always*/
void TriggerbotMenu::FeatureUpdate() {
	if (m_Vars.m_Enable) {
		Entity AimedEntity = 0;
		static int AimTimer = 0;

		if (Native::GetEntityPlayerIsFreeAimingAt(Menu::GetLocalPlayer().m_ID, &AimedEntity)) {
			if (!Native::IsEntityAPed(AimedEntity) || Native::IsEntityDead(AimedEntity, 0)) return;
			if (m_Vars.m_AimEntity == 0 && !Native::IsPedAPlayer(AimedEntity)) return;
			if (m_Vars.m_AimEntity == 1 && Native::IsPedAPlayer(AimedEntity)) return;

			Math::Vector3<float> Bone = Native::GetPedBoneCoords(AimedEntity, Global::Arrays::g_PedBones[m_Vars.m_AimTag].m_Result, 0.f, 0.f, 0.f);
			if (!Bone.IsZero()) {
				Menu::Timers::RunTimedFunction(&AimTimer, m_Vars.m_BulletBuffer, [=] {
					Native::SetPedShootsAtCoord(Menu::GetLocalPlayer().m_Ped, Bone.m_X, Bone.m_Y, Bone.m_Z, true);
				});
			}
		}
	}
}

TriggerbotMenu* _instance;
TriggerbotMenu* TriggerbotMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TriggerbotMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TriggerbotMenu::~TriggerbotMenu() { delete _instance; }