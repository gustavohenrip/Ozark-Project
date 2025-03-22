#pragma once
#include "menu/base/submenu.hpp"

class TranslationSelectedMenu : public Menu::Submenu {
public:
	static TranslationSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TranslationSelectedMenu()
		: Menu::Submenu() {}

	~TranslationSelectedMenu();
};

namespace TranslationSelectedMenuVars {
	struct Vars_ {
		std::size_t m_Selected;
	};

	extern Vars_ m_Vars;
}