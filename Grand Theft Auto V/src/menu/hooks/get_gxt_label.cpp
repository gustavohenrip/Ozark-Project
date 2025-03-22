#include "hooks.hpp"
#include "menu/base/base.hpp"
#include "utils/utils.hpp"

const char* Menu::Hooks::GetGxtLabelHook(uint64_t Table, const char* Label) {
	if (Label && !strcmp(Label, "OzarkKeyboardTitle")) {
		return Menu::GetBase()->m_KeyboardTitle;
	}

	return OriginalGetGxtLabel(Table, Label);
}