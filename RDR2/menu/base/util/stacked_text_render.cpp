#include "stacked_text_render.hpp"
#include "rage/invoker/natives.hpp"

namespace menu::util {
	void stackedTextRender::render() {
		int drawIndex = 0;
		/*for (std::size_t i = 0; i < messages.size(); i++) {
			if (messages[i].second > native::getGameTimer() - 200) {
				menu::getRenderer()->drawText(messages[i].first.c_str(), { 0.18f, 0.72f + (drawIndex * 0.020f) }, 0.30f, 4, { 255, 255, 255, 255 });
				drawIndex++;
			}
		}*/

		std::vector<std::string> pools;
		for (auto it = msgs.begin(); it != msgs.end(); ++it) {
			if (it->second.rendering) {
				if (strstr(it->second.contents.c_str(), "Pool")) {
					pools.push_back(it->second.contents);
				} else {
					menu::getRenderer()->drawText(it->second.contents.c_str(), { 0.18f, 0.72f + (drawIndex * 0.020f) }, 0.30f, 4, { 255, 255, 255, 255 });
					drawIndex++;
				}
			}
		}

		for (auto pool : pools) {
			menu::getRenderer()->drawText(pool.c_str(), { 0.18f, 0.72f + (drawIndex * 0.020f) }, 0.30f, 4, { 255, 255, 255, 255 });
			drawIndex++;
		}
	}

	void stackedTextRender::update(std::string id, std::string text) {
		auto& msg = msgs[id];
		msg.contents = text;
	}

	void stackedTextRender::alloc(std::string id) {
		auto& msg = msgs[id];
		msg.rendering = true;
		msg.contents.clear();
	}

	void stackedTextRender::dealloc(std::string id) {
		auto& msg = msgs[id];
		msg.rendering = false;
		msg.contents.clear();
	}

	stackedTextRender* getStackedTextRenderer() {
		static stackedTextRender instance;
		return &instance;
	}
}