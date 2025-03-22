#pragma once
#include "menu/base/submenu.hpp"

class ExplosionEditorMenu : public Menu::Submenu {
public:
	static ExplosionEditorMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ExplosionEditorMenu()
		: Menu::Submenu() {}

	~ExplosionEditorMenu();
};

namespace ExplosionEditorMenuVars {
	struct Vars_ {
		int m_SelectedExplosion = 0;
		int m_SelectedFX = 0;

		Rage::Types::ExplosionData m_ExplosionData;
		Rage::Types::ExplosionFX m_ExplosionFX;

		Rage::Types::ExplosionData* m_ExplosionDataPtr;
		Rage::Types::ExplosionFX* m_ExplosionFXPtr;
	};

	extern Vars_ m_Vars;
}