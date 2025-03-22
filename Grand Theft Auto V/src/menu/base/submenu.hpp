#pragma once
#include "stdafx.hpp"
#include "options/option.hpp"
#include "options/button_option.hpp"
#include "options/break_option.hpp"
#include "options/number_option.hpp"
#include "options/toggle_option.hpp"
#include "options/scroll_option.hpp"
#include "options/radio_option.hpp"
#include "utils/translation_string.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/hotkeys.hpp"

namespace Menu {
	class Submenu {
	public:
		Submenu()
			: m_Name(TranslationString("")), m_Parent(nullptr), m_OldCurrentOption(0), m_OldScrollOffset(0), m_CanBeSearched(true) {}

		void UpdateMenus();
		virtual void Init();
		virtual void UpdateOnce();
		virtual void Update();
		virtual void FeatureUpdate();

		template<typename T>
		std::shared_ptr<T> addOption(T& option) {
			auto ptr = std::make_shared<T>(option);
			m_Options.push_back(ptr);

			ptr.get()->SetSubmenuNameStackPtr(&m_ParentNameStack);

			if (ptr.get()->HasHotkey()) {
				Menu::Util::GetHotkey()->ReadHotkey(ptr.get());
			}

			return ptr;
		}

		template<typename T>
		std::shared_ptr<T>& addOption(std::shared_ptr<T>& option) {
			m_Options.push_back(option);

			option.get()->SetSubmenuNameStackPtr(&m_ParentNameStack);

			if (option.get()->HasHotkey()) {
				Menu::Util::GetHotkey()->ReadHotkey(option.get());
			}

			return option;
		}

		template<typename T>
		void SetParentSubmenu() {
			m_Parent = T::GetInstance();
			m_ParentNameStack = GetParentNameStack();
		}

		void addString(std::string str) { m_Strings.push_back(TranslationString(str, true)); }

		std::string getString(std::string str) {
			for (auto& s : m_Strings)
				if (strcmp(str.c_str(), s.GetOriginal().c_str()) == 0)
					return s.Get();
			return str;
		}

		std::vector<TranslationString>& GetStrings() { return m_Strings; }

		std::stack<std::string> GetParentNameStack() {
			std::stack<std::string> Output;

			Output.push(m_Name.GetOriginal());

			Menu::Submenu* Sub = m_Parent;
			while (Sub) {
				Output.push(Sub->m_Name.GetOriginal());
				Sub = Sub->m_Parent;
			}

			return Output;
		}

		void SetName(std::string n, bool translation = true, bool searchable = true);
		void ClearOptionsOffset(int offset);

		std::vector<std::shared_ptr<Option>> m_Options;
		std::vector<TranslationString> m_Strings;
		TranslationString m_Name;
		Submenu* m_Parent;
		int m_OldCurrentOption;
		int m_OldScrollOffset;
		bool m_CanBeSearched;
		bool m_DefaultInstructionals = true;
		std::stack<std::string> m_ParentNameStack;
	};
}