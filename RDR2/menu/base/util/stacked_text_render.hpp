#pragma once
#include "utils/singleton.hpp"
#include "utils/math.hpp"
#include "../renderer.hpp"

namespace menu::util {
	struct stackedRendererItem {
		bool rendering;
		std::string contents;
	};

	class stackedTextRender {
	public:
		void render();
		void update(std::string id, std::string text);
		void alloc(std::string id);
		void dealloc(std::string id);
	private:
		std::unordered_map<std::string, stackedRendererItem> msgs;
	}; stackedTextRender* getStackedTextRenderer();
}