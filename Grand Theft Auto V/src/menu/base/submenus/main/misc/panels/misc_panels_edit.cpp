#include "misc_panels_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "misc_panels_parent.hpp"
#include "menu/base/util/timers.hpp"
#include "../misc_panels.hpp"

using namespace MiscPanelsEditMenuVars;

namespace MiscPanelsEditMenuVars {
	Vars_ m_Vars;
}

void MiscPanelsEditMenu::Init() {
	SetName("Edit");
	SetParentSubmenu<MiscPanelsParentMenu>();

	addString("Toggle");
	addString("Column");
	addString("Index");
}

void MiscPanelsEditMenu::Update() {
	static bool UpdateCache = true;
	static int TIMER = 0;

	static bool WriteConfig = true;
	static int TIMER2 = 0;

	Menu::Timers::RunTimedFunction(&TIMER, 100, [&] { UpdateCache = true; });
	Menu::Timers::RunTimedFunction(&TIMER2, 500, [&] { WriteConfig = true; });

	if (UpdateCache) {
		UpdateCache = false;

		ClearOptionsOffset(0);
		auto& Child = Menu::Panels::GetPanelManager()->GetChild(MiscPanelsParentMenuVars::m_Vars.m_Parent, m_Vars.m_ID); // account for sorting

		if (WriteConfig) {
			Utils::GetConfig()->WriteInt(MiscPanelsMenu::GetInstance()->m_ParentNameStack, "Index", Child.m_Index, { Child.m_Parent->m_Name, Child.m_Name });
			Utils::GetConfig()->WriteInt(MiscPanelsMenu::GetInstance()->m_ParentNameStack, "Column", Child.m_Column, { Child.m_Parent->m_Name, Child.m_Name });
			Utils::GetConfig()->WriteBool(MiscPanelsMenu::GetInstance()->m_ParentNameStack, "Render", Child.m_Render, { Child.m_Parent->m_Name, Child.m_Name });
		}

		addOption(ToggleOption(getString("Toggle"))
			.addToggle(Child.m_Render));

		if (Child.m_Parent->m_ChildrenPanels.size() == 1) {
			return;
		}

		int MaxColumn = 0;
		for (auto& ItChild : Child.m_Parent->m_ChildrenPanels) {
			if (ItChild.m_Column > MaxColumn) {
				MaxColumn = ItChild.m_Column;
			}
		}

		addOption(NumberOption<int>(SCROLL, getString("Column"))
			.addNumber(m_Vars.m_LocalColumn, "%i", 1).addMin(0).addMax(MaxColumn + 1)
			.addOnClick([=] { Menu::Panels::GetPanelManager()->Rearrange(Child.m_Parent, Child.m_ID, m_Vars.m_LocalColumn, m_Vars.m_LocalIndex); }));

		addOption(NumberOption<int>(SCROLL, getString("Index"))
			.addNumber(m_Vars.m_LocalIndex, "%i", 1).addMin(0).addMax(1000)
			.addOnClick([=] { Menu::Panels::GetPanelManager()->Rearrange(Child.m_Parent, Child.m_ID, m_Vars.m_LocalColumn, m_Vars.m_LocalIndex); }));
	}
}

/*Called once on submenu open*/
void MiscPanelsEditMenu::UpdateOnce() {
	auto& Child = MiscPanelsParentMenuVars::m_Vars.m_Parent->m_ChildrenPanels[m_Vars.m_Index];
	m_Vars.m_ID = Child.m_ID;
	m_Vars.m_LocalColumn = Child.m_Column;
	m_Vars.m_LocalIndex = Child.m_Index;
}

/*Called always*/
void MiscPanelsEditMenu::FeatureUpdate() {}

MiscPanelsEditMenu* _instance;
MiscPanelsEditMenu* MiscPanelsEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscPanelsEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscPanelsEditMenu::~MiscPanelsEditMenu() { delete _instance; }