#include "misc_panels_parent.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc_panels.hpp"
#include "misc_panels_edit.hpp"

using namespace MiscPanelsParentMenuVars;

namespace MiscPanelsParentMenuVars {
	Vars_ m_Vars;
}

void MiscPanelsParentMenu::Init() {
	SetName("Parent");
	SetParentSubmenu<MiscPanelsMenu>();
}

void MiscPanelsParentMenu::Update() {}

/*Called once on submenu open*/
void MiscPanelsParentMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (!m_Vars.m_Parent || m_Vars.m_Parent->m_ChildrenPanels.size() <= 0) {
		Menu::GetSubmenuHandler()->SetSubmenuPrevious();
		return;
	}

	for (std::size_t i = 0; i < m_Vars.m_Parent->m_ChildrenPanels.size(); i++) {
		auto& Child = m_Vars.m_Parent->m_ChildrenPanels[i];

		addOption(SubmenuOption(Child.m_Name)
			.addOnClick([=] {
				MiscPanelsEditMenuVars::m_Vars.m_Index = i;
				MiscPanelsEditMenu::GetInstance()->SetName(Child.m_Name);
			})
			.addSubmenu<MiscPanelsEditMenu>());
	}
}

/*Called always*/
void MiscPanelsParentMenu::FeatureUpdate() {}

MiscPanelsParentMenu* _instance;
MiscPanelsParentMenu* MiscPanelsParentMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscPanelsParentMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscPanelsParentMenu::~MiscPanelsParentMenu() { delete _instance; }