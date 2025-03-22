#pragma once
#include "menu/base/submenu.hpp"

class TranslationMenu : public Menu::Submenu {
public:
	static TranslationMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TranslationMenu()
		: Menu::Submenu() {}

	~TranslationMenu();
};

namespace TranslationMenuVars {
	struct Vars_ {
		std::vector<std::pair<std::string, struct stat>> m_CachedTranslations;
		int m_ListSize;
		int m_UpdateTranslation;
		ScrollStruct<int> m_TranslationList[100];
	};

	void LoadTranslation(std::string File);
	extern Vars_ m_Vars;
}