#pragma once
#include "menu/base/submenu.hpp"

class ModelMenu : public Menu::Submenu {
public:
	static ModelMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ModelMenu()
		: Menu::Submenu() {}

	~ModelMenu();
};

namespace ModelMenuVars {
	struct Vars_ {
		int m_Popular;
		int m_Story;
		int m_Animals;
		int m_Emergency;
		int m_Role;
		int m_Ambient;
		int m_Misc;

		uint32_t m_LastModel = 0;
	};

	extern Vars_ m_Vars;
}