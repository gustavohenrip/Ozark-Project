#include "translations_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "translations.hpp"
#include "utils/va.hpp"

using namespace TranslationSelectedMenuVars;

namespace TranslationSelectedMenuVars {
	Vars_ m_Vars;
}

void TranslationSelectedMenu::Init() {
	SetName("Translation");
	SetParentSubmenu<TranslationMenu>();

	addOption(ButtonOption("Load")
		.addTranslation()
		.addOnClick([] { TranslationMenuVars::LoadTranslation(TranslationMenuVars::m_Vars.m_CachedTranslations[m_Vars.m_Selected].first); }));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			remove(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetTranslationPath().c_str(), TranslationMenuVars::m_Vars.m_CachedTranslations[m_Vars.m_Selected].first.c_str()));
			Menu::GetSubmenuHandler()->SetSubmenuPrevious(true);
		}));

	addOption(ButtonOption("Set as Load Default")
		.addTranslation()
		.addOnClick([this] { Utils::GetConfig()->WriteString(m_ParentNameStack, "Launch", TranslationMenuVars::m_Vars.m_CachedTranslations[m_Vars.m_Selected].first); }));

	std::string Launch = Utils::GetConfig()->ReadString(m_ParentNameStack, "Launch", "-none-");
	if (Launch.compare("-none-")) {
		TranslationMenuVars::LoadTranslation(Launch);
	}
}

void TranslationSelectedMenu::Update() {}

/*Called once on submenu open*/
void TranslationSelectedMenu::UpdateOnce() {}

/*Called always*/
void TranslationSelectedMenu::FeatureUpdate() {}

TranslationSelectedMenu* _instance;
TranslationSelectedMenu* TranslationSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TranslationSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TranslationSelectedMenu::~TranslationSelectedMenu() { delete _instance; }