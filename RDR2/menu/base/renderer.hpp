#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"
#include "utils/math.hpp"
#include "global/ui_vars.hpp"

enum eJustify {
	JUSTIFY_LEFT,
	JUSTIFY_RIGHT,
	JUSTIFY_CENTER,
};

struct lineDrawRequest {
	uint32_t color;
	math::vector3<float> first;
	math::vector3<float> second;
};

namespace menu {
	class renderer {
	public:
		void render();
		void drawRect(math::vector2<float> position, math::vector2<float> scale, color color);
		void drawSprite(const char* dict, const char* name, math::vector2<float> position, math::vector2<float> scale, float heading, color color);
		void drawSpriteAligned(const char* dict, const char* name, math::vector2<float> position, math::vector2<float> scale, float heading, color col);
		void drawLine(math::vector3<float> from, math::vector3<float> to, color color);
		void drawText(const char* text, math::vector2<float> position, float scale, int font, color color, eJustify justify = JUSTIFY_LEFT, bool outline = false, math::vector2<float> wrap = { 0.f, 0.f });
		void drawTitle(const char* name);
		void drawTooltip(const char* name);
		void drawOpenTooltip();
		void previewColor(color col);
		void displayTooltip(std::string tooltip);

		int getRenderingOptionsCount() { return renderingOptionsCount; }
		std::vector<lineDrawRequest>& getLineDrawRequests() { return lineDrawRequests; }
	private:
		std::vector<lineDrawRequest> lineDrawRequests;
		int renderingOptionsCount;
	}; renderer* getRenderer();
}