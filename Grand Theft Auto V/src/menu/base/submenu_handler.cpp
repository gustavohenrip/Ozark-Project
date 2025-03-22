#include "submenu_handler.hpp"
#include "base.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/config.hpp"
#include "utils/va.hpp"

namespace Menu {
	void SubmenuHandler::InitSubmenus() {
		m_MainSubmenu = m_CurrentSubmenu = MainMenu::GetInstance(); // inits every submenu
	}

	void SubmenuHandler::Update() {
		if (m_CurrentSubmenu) {
			m_CurrentSubmenu->UpdateMenus();

			if (m_NextSubmenu) {
				GetBase()->m_CurrentOption = m_NextCurrentOption;
				GetBase()->m_ScrollOffset = m_NextScrollOffset;
				m_CurrentSubmenu = m_NextSubmenu;
				m_NextSubmenu = nullptr;
				m_CurrentSubmenu->UpdateOnce();
			}
		}
	}

	void SubmenuHandler::AsyncUpdate() {
		if (Native::NetworkIsPlayerConnected(Native::PlayerId())) {
			for (std::size_t i = 0; i < m_Submenus.size(); i++) {
				m_Submenus.at(i)->FeatureUpdate();
			}
		}
	}

	void SubmenuHandler::AddSubmenu(Submenu* Sub) {
		m_Submenus.push_back(Sub);
	}

	void SubmenuHandler::SetSubmenu(Submenu* Sub) {
		m_PreviousSubmenu = m_CurrentSubmenu;
		m_CurrentSubmenu->m_OldCurrentOption = GetBase()->m_CurrentOption;
		m_CurrentSubmenu->m_OldScrollOffset = GetBase()->m_ScrollOffset;
		m_NextCurrentOption = 0;
		m_NextScrollOffset = 0;
		m_NextSubmenu = Sub;
	}

	void SubmenuHandler::SetSubmenuPrevious(bool Reset) {
		m_PreviousSubmenu = m_CurrentSubmenu;
		if (m_CurrentSubmenu->m_Parent == nullptr) {
			// main menu
			m_NextSubmenu = m_MainSubmenu;
			m_NextCurrentOption = 0;
			m_NextScrollOffset = 0;
			GetBase()->m_Open = false;
		} else {
			m_NextSubmenu = m_CurrentSubmenu->m_Parent;

			if (Reset) {
				m_NextCurrentOption = 0;
				m_NextScrollOffset = 0;
			} else {
				m_NextCurrentOption = m_NextSubmenu->m_OldCurrentOption;
				m_NextScrollOffset = m_NextSubmenu->m_OldScrollOffset;
			}
		}

		Global::UIVars::g_RenderingTooltip.clear();
	}

	void SubmenuHandler::Unload() {
		for (std::size_t i = 0; i < m_Submenus.size(); i++) {
			delete m_Submenus[i];
		}
	}

	SubmenuHandler* GetSubmenuHandler() {
		static SubmenuHandler Instance;
		return &Instance;
	}
}