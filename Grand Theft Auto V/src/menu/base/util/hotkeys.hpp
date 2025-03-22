#pragma once
#include "stdafx.hpp"
#include "menu/base/options/option.hpp"

namespace Menu::Util {
	struct RegisteredHotkey {
		int m_Key;
		Option* m_Parent;
	};

	class Hotkey {
	public:
		void Update();

		void ReadHotkey(Option* Parent); // Init
		void RegisterHotkey(int Key, Option* Parent);
		void UnregisterHotkey(Option* Parent);
		std::vector<RegisteredHotkey> GetHotkeys() { return m_RegisteredHotkeys; }
	private:
		std::vector<RegisteredHotkey> m_RegisteredHotkeys;
	};

	Hotkey* GetHotkey();
}