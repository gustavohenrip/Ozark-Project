#pragma once
#include "stdafx.hpp"
#include "options/option.hpp"
#include "options/button_option.hpp"
#include "options/break_option.hpp"
#include "options/toggle_option.hpp"
#include "options/number_option.hpp"
#include "options/scroll_option.hpp"
#include "utils/translation_string.hpp"
#include "options/radio_option.hpp"

namespace menu {
	class submenu {
	public:
		submenu()
			: name(translationString("")), parent(nullptr), oldCurrentOption(0), oldScrollOffset(0), canBeSearched(true)
		{}

		void updateMenus();
		virtual void init();
		virtual void updateOnce();
		virtual void update();
		virtual void featureUpdate();

		template<typename T>
		std::shared_ptr<T> addOption(T& option) {
			auto ptr = std::make_shared<T>(option);
			options.push_back(ptr);
			return ptr;
		}

		template<typename T>
		std::shared_ptr<T>& addOption(std::shared_ptr<T>& option) {
			options.push_back(option);
			return option;
		}

		template<typename T>
		void setParentSubmenu() {
			parent = T::getInstance();
		}

		void addString(std::string str) { strings.push_back(translationString(str, true)); }

		std::string getString(std::string str) {
			for (auto& s : strings) if (strcmp(str.c_str(), s.getOriginal().c_str()) == 0) return s.get();
			return str;
		}

		std::vector<translationString>& getStrings() { return strings; }

		void setName(std::string n, bool translation = true, bool searchable = true);
		void clearOptionsOffset(int offset);

		std::vector<std::shared_ptr<option>> options;
		std::vector<translationString> strings;
		translationString name;
		submenu* parent;
		int oldCurrentOption;
		int oldScrollOffset;
		bool canBeSearched;
	};
}