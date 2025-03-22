#pragma once
#include "menu/base/submenu.hpp"

class ParticleFXMenu : public Menu::Submenu {
public:
	static ParticleFXMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ParticleFXMenu()
		: Menu::Submenu() {}

	~ParticleFXMenu();
};

namespace ParticleFXMenuVars {
	struct Vars_ {
		
	};

	extern Vars_ m_Vars;
}