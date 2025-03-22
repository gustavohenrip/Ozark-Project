#include "misc_panels.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/panels.hpp"
#include "panels/misc_panels_parent.hpp"
#include "menu/base/submenus/main/network/network_players.hpp"

using namespace MiscPanelsMenuVars;

namespace MiscPanelsMenuVars {
	Vars_ m_Vars;
}

void MiscPanelsMenu::Init() {
	SetName("Panels");
	SetParentSubmenu<MiscMenu>();

	for (auto Parent : Menu::Panels::GetPanelManager()->GetPanels()) {
		for (auto& Child : Parent->m_ChildrenPanels) {
			int Index = Utils::GetConfig()->ReadInt(m_ParentNameStack, "Index", Child.m_Index, { Parent->m_Name, Child.m_Name });
			int Column = Utils::GetConfig()->ReadInt(m_ParentNameStack, "Column", Child.m_Column, { Parent->m_Name, Child.m_Name });
			Child.m_Render = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Render", Child.m_Render, { Parent->m_Name, Child.m_Name });

			Menu::Panels::GetPanelManager()->Rearrange(Parent, Child.m_ID, Column, Index);
		}
	}
}

void MiscPanelsMenu::Update() {}

/*Called once on submenu open*/
void MiscPanelsMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	for (auto Parent : Menu::Panels::GetPanelManager()->GetPanels()) {
		addOption(SubmenuOption(Parent->m_Name)
			.addOnClick([=] {
				MiscPanelsParentMenuVars::m_Vars.m_Parent = Parent;
				MiscPanelsParentMenu::GetInstance()->SetName(Parent->m_Name);
			})
			.addOnHover([=] {
				for (auto P : Menu::Panels::GetPanelManager()->GetPanels()) {
					if (P != Parent) {
						P->m_Render = false;
					}
				}

				NetworkPlayersMenuVars::m_Vars.m_SelectedPlayer = Menu::GetLocalPlayer().m_ID; // for panel preview
				
				for (auto& Child : Parent->m_ChildrenPanels) {
					Menu::Panels::GetPanelManager()->NullStructure(Child);
				}

				Parent->m_Render = true;
			})
			.addSubmenu<MiscPanelsParentMenu>());
	}
}

/*Called always*/
void MiscPanelsMenu::FeatureUpdate() {}

MiscPanelsMenu* _instance;
MiscPanelsMenu* MiscPanelsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscPanelsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscPanelsMenu::~MiscPanelsMenu() { delete _instance; }