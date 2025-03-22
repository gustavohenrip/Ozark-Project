#include "misc_model_swap_add.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc_model_swap.hpp"

using namespace MiscModelSwapAddMenuVars;

namespace MiscModelSwapAddMenuVars {
	Vars_ m_Vars;
}

void MiscModelSwapAddMenu::Init() {
	SetName("+ Add Swap");
	SetParentSubmenu<MiscModelSwapMenu>();

	addOption(ButtonOption("Input Original Model")
		.addTranslation()
		.addKeyboard("Input the model! (Hint: name, hash or numeric)", 100, [] (const char* Model) { m_Vars.m_Original = Model; }));

	addOption(ButtonOption("Input New Model")
		.addTranslation()
		.addKeyboard("Input the model! (Hint: name, hash or numeric)", 100, [] (const char* Model) { m_Vars.m_New = Model; }));

	addOption(ButtonOption("Add Swap")
		.addTranslation()
		.addOnClick([] {
			if (!m_Vars.m_Original.empty() && !m_Vars.m_New.empty()) {
				MiscModelSwapMenuVars::AddSwap(m_Vars.m_Original.c_str(), m_Vars.m_New.c_str());

				Menu::GetSubmenuHandler()->SetSubmenuPrevious(true);;
			}
		}));
}

void MiscModelSwapAddMenu::Update() {}

/*Called once on submenu open*/
void MiscModelSwapAddMenu::UpdateOnce() {}

/*Called always*/
void MiscModelSwapAddMenu::FeatureUpdate() {}

MiscModelSwapAddMenu* _instance;
MiscModelSwapAddMenu* MiscModelSwapAddMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscModelSwapAddMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscModelSwapAddMenu::~MiscModelSwapAddMenu() { delete _instance; }