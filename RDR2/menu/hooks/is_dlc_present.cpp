#include "hooks.hpp"

void menu::hooks::isDlcPresentHook(rage::invoker::nativeContext* context) {
	//TODO: Check for dev mode hash

	context->setReturn(0, true);
}