#include "explosion_gun.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../weapon_mods.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/raycast.hpp"

using namespace ExplosionGunMenuVars;

namespace ExplosionGunMenuVars {
	Vars_ m_Vars;
}

void ExplosionGunMenu::Init() {
	SetName("Explosion Gun");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ToggleOption("Toggle Explosion Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Toggle).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<ExplosionTypes>(SCROLL, "Explosion")
		.addTranslation()
		.addScroll(m_Vars.m_Type, 0, NUMOF(Global::Arrays::g_Explosions), Global::Arrays::g_Explosions).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Audible")
		.addTranslation()
		.addToggle(m_Vars.m_Audible).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Invisible")
		.addTranslation()
		.addToggle(m_Vars.m_Invisible).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Radius")
		.addTranslation()
		.addNumber(m_Vars.m_Radius, "%.1f", 0.1f).addMin(0.f).addMax(100.f).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Camera Shake")
		.addTranslation()
		.addNumber(m_Vars.m_CameraShake, "%.1f", 0.1f).addMin(0.f).addMax(100.f).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<Ped>(TOGGLE, "Owner")
		.addTranslation()
		.addToggle(m_Vars.m_ChangeOwner)
		.addScroll(m_Vars.m_ChangeOwnerValue, 0, NUMOF(m_Vars.m_ChangeOwnerList), m_Vars.m_ChangeOwnerList)
		.addOnUpdate([] (ScrollOption<Ped>* option) { option->addScroll(m_Vars.m_ChangeOwnerValue, 0, m_Vars.m_ListSize, m_Vars.m_ChangeOwnerList); }));
}

void ExplosionGunMenu::Update() {
	int size = 0;
	Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& var) {
		m_Vars.m_ChangeOwnerList[size].m_Name.Set(var.m_Name);
		m_Vars.m_ChangeOwnerList[size].m_Result = var.m_Ped;
		size++;
	});

	m_Vars.m_ListSize = size;
}

/*Called once on submenu open*/
void ExplosionGunMenu::UpdateOnce() {}

/*Called always*/
void ExplosionGunMenu::FeatureUpdate() {
	if (m_Vars.m_Toggle) {
		if (Menu::GetLocalPlayer().m_IsShooting) {
			Menu::Raycast Cast = Menu::RaycastInfront(9999.f);
			if (Cast.HitSomething()) {
				auto Coords = Cast.GetHitCoords();
				Native::AddOwnedExplosion(m_Vars.m_ChangeOwner ? m_Vars.m_ChangeOwnerList[m_Vars.m_ChangeOwnerValue].m_Result : Menu::GetLocalPlayer().m_Ped, Coords.m_X, Coords.m_Y, Coords.m_Z, m_Vars.m_Type, m_Vars.m_Radius, m_Vars.m_Audible, m_Vars.m_Invisible, m_Vars.m_CameraShake);
			}
		}
	}
}

ExplosionGunMenu* _instance;
ExplosionGunMenu* ExplosionGunMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ExplosionGunMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ExplosionGunMenu::~ExplosionGunMenu() { delete _instance; }