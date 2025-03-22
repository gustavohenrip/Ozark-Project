#include "hooks.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/engine.hpp"
#include "rage/entity/entity.hpp"
#include "rage/session/session.hpp"
#include "../base/renderer.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/base.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/init.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "utils/log.hpp"
#include "utils/memory/pattern.hpp"
#include "global/arrays.hpp"

using namespace menu;
using namespace menu::util;

uint8_t menu::hooks::getPeerIdHook(uint64_t *rcx) {
	auto ret = originalGetPeerId(rcx);

	//*rcx = 0XFFFFFFFFFFFFFFFF;
	static int32_t count = 0;
	if (count++ < 20) {
		printf("[peerID] %llX\n", *rcx);
	}
	return ret;
}

void menu::hooks::waitHook(rage::invoker::nativeContext* context) {
	static const rage::invoker::nativeHandler handler = rage::invoker::getNativeHandler(0x4EDE34FBADD967A6);
	handler(context);
	static int frame = 0;
	//if (menu::util::getInput()->isPressed(true, VK_DELETE)) {
	//	getInit()->unload();
	//}
	if (frame < rage::engine::getFrameCount()) {
		frame = rage::engine::getFrameCount();

		static bool init = false;
		if (!init) {
			init = true;

			getSubmenuHandler()->initSubmenus();
			getFiberManager()->init();
			getScriptHookFiberManager()->init();
			getFiberPool()->init();

			getFiberManager()->add("CMU"_Protect, [] {
				getControlManager()->update();
			});

			getFiberManager()->add("BU"_Protect, [] {
				getBase()->update();
			});

			getFiberManager()->add("SU"_Protect, [] {
				getSubmenuHandler()->asyncUpdate();
				getBase()->asyncUpdate();
			});
			getNotify()->right("Welcome to ~pa~Ozark~q~!"_Protect);
		}

		if (!global::vars::g_unloading) {
			getFiberManager()->update();
			getScriptHookFiberManager()->update();
		}
	}
}