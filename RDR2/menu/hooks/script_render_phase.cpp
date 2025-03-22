#include "hooks.hpp"
#include "menu/base/renderer.hpp"
#include "utils/caller.hpp"
#include "menu/base/util/player_manager.hpp"

void menu::hooks::scriptImRenderPhaseHook() { 
	auto size = getRenderer()->getLineDrawRequests().size();
	if (size) {
		for (std::size_t i = 0; i < size; i++) {
			auto& data = getRenderer()->getLineDrawRequests().at(i);
			float points[8] = {
				data.first.x, data.first.y, data.first.z, 0.0f,
				data.second.x, data.second.y, data.second.z, 0.0f
			};

			caller::call<int>(global::vars::g_drawPoints, (uint64_t)&points, 2, 0, false, data.color);
		}

		getRenderer()->getLineDrawRequests().clear();
	}

	originalScriptImRenderPhase();
}