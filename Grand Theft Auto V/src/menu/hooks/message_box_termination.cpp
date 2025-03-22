#include "hooks.hpp"
#include "utils/utils.hpp"
#include "utils/log.hpp"

void Menu::Hooks::MessageBoxTerminationHook(uint32_t Ecx, uint32_t Edx) {
	LOG_INFO("MessageBoxTermination [%x][%x] %s", Ecx, Edx, Utils::GetStackTrace().c_str());
}