#pragma once
#include "stdafx.hpp"
#include "submenu.hpp"
#include "menu/base/options/submenu_option.hpp"
#include "menu/base/util/player_manager.hpp"

namespace Menu {
	class SubmenuHandler {
	public:
		void InitSubmenus();
		void Update();
		void AsyncUpdate();
		void Unload();

		void AddSubmenu(Submenu* Sub);
		void SetSubmenu(Submenu* Sub);
		void SetSubmenuPrevious(bool Reset = false);

		int GetTotalOptions() { return m_TotalOptions; }
		void SetTotalOptions(int total) { m_TotalOptions = total; }

		Submenu* GetCurrentSubmenu() { return m_CurrentSubmenu; }
		Submenu* GetPreviousSubmenu() { return m_PreviousSubmenu; }

		int GetCurrentOptionWithoutBreaks() { return m_CurrentOptionWithoutBreaks; }
		void SetCurrentOptionWithoutBreaks(int total) { m_CurrentOptionWithoutBreaks = total; }

		int GetTotalOptionsWithoutBreaks() { return m_TotalOptionsWithoutBreaks; }
		void SetTotalOptionsWithoutBreaks(int total) { m_TotalOptionsWithoutBreaks = total; }

		std::vector<Submenu*> GetSubmenus() { return m_Submenus; }

		Submenu* m_PreviousSubmenu;
	private:
		Submenu* m_MainSubmenu;
		Submenu* m_CurrentSubmenu;
		Submenu* m_NextSubmenu;
		std::vector<Submenu*> m_Submenus;
		int m_TotalOptions;
		int m_CurrentOptionWithoutBreaks;
		int m_TotalOptionsWithoutBreaks;
		int m_NextCurrentOption;
		int m_NextScrollOffset;
	};
	
	SubmenuHandler* GetSubmenuHandler();
}