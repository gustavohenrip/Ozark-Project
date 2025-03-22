#include "hooks.hpp"
#include "menu/base/submenus/main/weapon/weapon_disables.hpp"

void Menu::Hooks::SetCurrentPedWeaponHook(Rage::Invoker::NativeContext* Context) {
	if (!WeaponDisablesMenuVars::m_Vars.m_InteriorWeapons && !WeaponDisablesMenuVars::m_Vars.m_PassiveWeapons) {
		Native::SetCurrentPedWeapon(Context->GetArgument<Ped>(0), Context->GetArgument<uint32_t>(1), Context->GetArgument<BOOL>(2));
	}
}

void Menu::Hooks::DisableControlActionHook(Rage::Invoker::NativeContext* Context) {
	if (!WeaponDisablesMenuVars::m_Vars.m_InteriorWeapons && !WeaponDisablesMenuVars::m_Vars.m_PassiveWeapons) {
		Native::DisableControlAction(Context->GetArgument<int>(0), Context->GetArgument<int>(1), Context->GetArgument<BOOL>(2));
	}
}