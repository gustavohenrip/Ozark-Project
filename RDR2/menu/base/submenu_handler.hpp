#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"
#include "submenu.hpp"
#include "menu/base/options/submenu_option.hpp"
#include "menu/base/util/player_manager.hpp"

namespace menu {
	class submenuHandler {
	public:
		void initSubmenus();
		void update();
		void asyncUpdate();
		
		void addSubmenu(submenu* sub);
		void setSubmenu(submenu* sub);
		void setSubmenuPrevious(bool reset = false);

		int getTotalOptions() {
			return totalOptions;
		}

		void setTotalOptions(int total) {
			totalOptions = total;
		}

		submenu* getCurrentSubmenu() {
			return currentSubmenu;
		}

		submenu* getPreviousSubmenu() {
			return previousSubmenu;
		}

		int getCurrentOptionWithoutBreaks() {
			return currentOptionWithoutBreaks;
		}

		void setCurrentOptionWithoutBreaks(int total) {
			currentOptionWithoutBreaks = total;
		}

		int getTotalOptionsWithoutBreaks() {
			return totalOptionsWithoutBreaks;
		}

		void setTotalOptionsWithoutBreaks(int total) {
			totalOptionsWithoutBreaks = total;
		}

		std::vector<submenu*> getSubmenus() {
			return submenus;
		}
	private:
		submenu* mainSubmenu;
		submenu* currentSubmenu;
		submenu* previousSubmenu;
		submenu* nextSubmenu;
		std::vector<submenu*> submenus;
		int totalOptions;
		int currentOptionWithoutBreaks;
		int totalOptionsWithoutBreaks;
		int nextCurrentOption;
		int nextScrollOffset;
	}; submenuHandler* getSubmenuHandler();
}