#include "hooks.hpp"
#include "menu/base/submenus/main/world.hpp"
#include "menu/base/util/global.hpp"

void menu::hooks::requestScriptHook(rage::invoker::nativeContext* context) {
	if (worldMenuVars::vars.bypassRestrictedAreas) {
		const char* script = context->getArgument<const char*>(0);
		if (script) {
			if (!strcmp(script, "BluegillMarsh_population")
				|| !strcmp(script, "Guama_population")) {
				return;
			}
		}
	}

	static const rage::invoker::nativeHandler handler = rage::invoker::getNativeHandler(0x46ED607DDD40D7FE);
	handler(context);
}