#include "hooks.hpp"
#include "menu/base/base.hpp"
#include "utils/log.hpp"

const char* menu::hooks::getGxtTextHook(uint64_t ptr, const char* label) {
	if (label) {
		if (!strcmp(label, "OzarkKeyboardTitle")) {
			return menu::getBase()->keyboardTitle;
		}
	}

	return orginalGetGxtText(ptr, label);
}