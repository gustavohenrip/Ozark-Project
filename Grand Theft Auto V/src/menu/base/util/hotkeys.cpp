#include "hotkeys.hpp"
#include "menu/base/base.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/submenu_handler.hpp"
#include "utils/config.hpp"

namespace Menu::Util {
	std::stack<std::string> GetOptionNameStack(Option* Parent) {
		if (Parent->GetSubmenuNameStackPtr()) {
			return *Parent->GetSubmenuNameStackPtr();
		}

		return std::stack<std::string>();
	}

	void Hotkey::Update() {
		if (Menu::GetBase()->m_DisableInputThisFrame) return;

		int LastKey = GetInput()->m_LastKey;
		if (GetInput()->IsJustReleased(true, LastKey)) {
			for (auto& Key : m_RegisteredHotkeys) {
				if (Key.m_Key == LastKey) {
					if (!Native::_IsTextChatActive()) {
						Key.m_Parent->InvokeHotkey();
					}
				}
			}
		}
	}

	void Hotkey::ReadHotkey(Option* Parent) {
		std::stack<std::string> NameStack = GetOptionNameStack(Parent);
		if (NameStack.size()) {
			bool Toggled = Utils::GetConfig()->ReadBool(NameStack, "Toggled", false, { "Hotkeys", Parent->GetName().GetOriginal() });
			if (Toggled) {
				int Key = Utils::GetConfig()->ReadInt(NameStack, "Key", -1, { "Hotkeys", Parent->GetName().GetOriginal() });
				Parent->setHotkey(Key);
				m_RegisteredHotkeys.push_back({ Key, &*Parent });
			}
		}
	}

	void Hotkey::RegisterHotkey(int Key, Option* Parent) {
		m_RegisteredHotkeys.push_back({ Key, Parent });

		std::stack<std::string> NameStack = GetOptionNameStack(Parent);
		if (NameStack.size()) {
			Utils::GetConfig()->WriteInt(NameStack, "Key", Key, { "Hotkeys", Parent->GetName().GetOriginal() });
			Utils::GetConfig()->WriteBool(NameStack, "Toggled", true, { "Hotkeys", Parent->GetName().GetOriginal() });
		}
	}

	void Hotkey::UnregisterHotkey(Option* Parent) {
		for (std::size_t i = 0; i < m_RegisteredHotkeys.size(); i++) {
			auto& Hotkey = m_RegisteredHotkeys[i];
			if (Hotkey.m_Parent == Parent) {
				Hotkey.m_Parent->setHotkey(-1);
				m_RegisteredHotkeys.erase(begin(m_RegisteredHotkeys) + i);
				break;
			}
		}

		std::stack<std::string> NameStack = GetOptionNameStack(Parent);
		if (NameStack.size()) {
			Utils::GetConfig()->WriteBool(NameStack, "Toggled", false, { "Hotkeys", Parent->GetName().GetOriginal() });
		}
	}

	Hotkey* GetHotkey() {
		static Hotkey Instance;
		return &Instance;
	}
}