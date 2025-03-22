#include "teleport_store.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"
#include "teleport_store_selected.hpp"

using namespace TeleportStoreMenuVars;

namespace TeleportStoreMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> TeleportLocStoreCategories[] = {
		{ TranslationString("Ammunation", true), 0 },
		{ TranslationString("Barber Shop", true), 1 },
		{ TranslationString("Binco", true), 2 },
		{ TranslationString("Discount", true), 3 },
		{ TranslationString("Los Santos Customs", true), 4 },
		{ TranslationString("Ponsonbys", true), 5 },
		{ TranslationString("Suburban", true), 6 },
		{ TranslationString("Tattoo", true), 7 },
		{ TranslationString("Other", true), 8 }
	};
}

void TeleportStoreMenu::Init() {
	SetName("Store");
	SetParentSubmenu<TeleportMenu>();

	for (auto& Cat : TeleportLocStoreCategories) {
		addOption(SubmenuOption(Cat.m_Name.Get())
			.addTranslation()
			.addSubmenu<TeleportStoreSelectedMenu>()
			.addOnClick([=] { TeleportStoreSelectedMenuVars::m_Vars.m_Selected = Cat.m_Result; }));
	}
}

void TeleportStoreMenu::Update() {}

/*Called once on submenu open*/
void TeleportStoreMenu::UpdateOnce() {}

/*Called always*/
void TeleportStoreMenu::FeatureUpdate() {}

TeleportStoreMenu* _instance;
TeleportStoreMenu* TeleportStoreMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TeleportStoreMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TeleportStoreMenu::~TeleportStoreMenu() { delete _instance; }