#pragma once
#include "menu/base/submenu.hpp"

class AmmoModifierEditMenu : public Menu::Submenu {
public:
	static AmmoModifierEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AmmoModifierEditMenu()
		: Menu::Submenu() {}

	~AmmoModifierEditMenu();
};

namespace AmmoModifierEditMenuVars {
	struct EditContext {
		Rage::Types::AmmoInfo* m_AmmoInfo;
		Rage::Types::AimingInfo* m_AimingInfo;
		int m_FireType;
		uint32_t m_Audio;
		uint32_t m_Hash;
		uint32_t m_Swap;
	};
	
	struct Vars_ {
		uint32_t m_CurrentHash;
		std::vector<EditContext> m_Modifiers;
		Rage::Types::Weapon* m_CachedWeapon;

		float m_Damage = 0.f;
		float m_Force = 0.f;
		float m_Speed = 0.f;
	};

	Rage::Types::Weapon* GetActiveWeapon();
	void ResetModifier(uint32_t Hash);
	extern Vars_ m_Vars;
}