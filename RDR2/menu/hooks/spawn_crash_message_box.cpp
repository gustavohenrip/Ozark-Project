#include "hooks.hpp"
#include "menu/auth.hpp"
#include "utils/log.hpp"
#include "menu/init.hpp"

void menu::hooks::spawnCrashMessageBoxHook(int error, int fileHash, uint32_t fileLine, bool terminate) {
	char str[200];
	sprintf(str, "%X %X %X%s"_Protect, error, fileHash, fileLine, terminate ? " - terminate"_Protect : "");

#ifdef DEV
	info(str);
#else
#endif

	info("Unloading due to crash (%X %X %X)", error, fileHash, fileLine);
	menu::getInit()->unload();
}