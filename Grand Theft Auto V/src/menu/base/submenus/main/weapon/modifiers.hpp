#pragma once
#include "menu/base/submenu.hpp"

class WeaponModifierMenu : public Menu::Submenu {
public:
	static WeaponModifierMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WeaponModifierMenu()
		: Menu::Submenu() {}

	~WeaponModifierMenu();
};

namespace WeaponModifierMenuVars {
	struct Vars_ {
		bool m_DisableSpread;
		bool m_DisableSpin;
		bool m_DisableRecoil;
		bool m_InstantReload;
		bool m_DisableCooldown;
		bool m_Width;
		bool m_Height;
		bool m_WeaponDamageMultiplier;
		bool m_MeleeDamageMultiplier;
		bool m_DefenseDamageMultiplier;

		float m_WeaponDamageMultiplierVal = 1.f;
		float m_MeleeDamageMultiplierVal = 1.f;
		float m_DefenseDamageMultiplierVal = 1.f;
		float m_WidthVal = 1.f;
		float m_HeightVal = 1.f;

		std::unordered_map<Rage::Types::Weapon*, std::pair<float, float>> m_OriginalSpread;
		std::unordered_map<Rage::Types::Weapon*, float> m_OriginalRecoil;
		std::unordered_map<Rage::Types::Weapon*, float> m_OriginalAnimReloadRate;
		std::unordered_map<Rage::Types::Weapon*, float> m_OriginalTimeBetweenShots;
		std::unordered_map<Rage::Types::Weapon*, std::pair<float, float>> m_OriginalInstantSpin;
	};

	Rage::Types::Weapon* GetWeaponInfo();
	extern Vars_ m_Vars;
}