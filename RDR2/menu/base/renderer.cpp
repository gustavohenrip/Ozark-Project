#include "renderer.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/engine.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/input.hpp"

namespace menu {
	void renderer::drawRect(math::vector2<float> position, math::vector2<float> scale, color color) {
		native::drawRect(position.x + (scale.x * 0.5f), position.y + (scale.y * 0.5f), scale.x, scale.y, color.r, color.g, color.b, color.a);
	}

	void renderer::drawSprite(const char* dict, const char* name, math::vector2<float> position, math::vector2<float> scale, float heading, color color) {
		if (!native::hasStreamedTextureDictLoaded(dict)) {
			native::requestStreamedTextureDict(dict);
		}
		
		native::drawSprite(dict, name, position.x, position.y, scale.x, scale.y, heading, color.r, color.g, color.b, color.a, FALSE);
	}

	void renderer::drawSpriteAligned(const char* dict, const char* name, math::vector2<float> position, math::vector2<float> scale, float heading, color col) {
		if (!native::hasStreamedTextureDictLoaded(dict)) {
			native::requestStreamedTextureDict(dict);
		}

		//Native::_Set2DLayer(1);
		native::drawSprite(dict, name, position.x + (scale.x * 0.5f), position.y + (scale.y * 0.5f), scale.x, scale.y, heading, col.r, col.g, col.b, col.a, FALSE);
	}

	void renderer::drawLine(math::vector3<float> from, math::vector3<float> to, color color) {
		lineDrawRequest req;
		req.first = from;
		req.second = to;
		req.color = ((color.a & 0xFF) << 24) + ((color.b & 0xFF) << 16) + ((color.g & 0xFF) << 8) + (color.r & 0xFF);
		lineDrawRequests.push_back(req);
	}

	void renderer::drawText(const char* text, math::vector2<float> position, float scale, int font, color color, eJustify justify, bool outline, math::vector2<float> wrap) {
		if (justify != JUSTIFY_LEFT) {
			if (justify == JUSTIFY_RIGHT) {
				rage::engine::setTextWrap(wrap.x, wrap.y);
			}

			rage::engine::setTextJustification(justify == JUSTIFY_CENTER ? 0 : 2);
		}

		native::setTextCentre(justify == JUSTIFY_CENTER);
		native::setTextScale(0.f, scale);
		native::setTextColor(color.r, color.g, color.b, color.a);
		rage::engine::setTextFont(font);
		if (outline) rage::engine::setTextOutline();
		native::drawText(native::createVarString(10, "LITERAL_STRING", text), position.x, position.y);
	}

	void renderer::drawOpenTooltip() {
		drawText("put me in coach\n~c~F4", { 0.5f, 0.075f }, 0.450f, 2, global::uivars::openToolTip, JUSTIFY_CENTER, true);
	}

	void renderer::drawTitle(const char* name) {
		drawText(name, { global::uivars::menuX + .004f, global::uivars::menuY - .03f }, 0.33f, global::uivars::headerFont, global::uivars::menuHeaderText);
	}

	void renderer::drawTooltip(const char* name) {}

	void renderer::previewColor(color col) {
		int count = 0;

		auto options = menu::getSubmenuHandler()->getCurrentSubmenu()->options;
		for (std::size_t i = 0; i < options.size(); i++) {
			if (options.at(i)->isVisible()) {
				count++;
			}
		}

		if (count > menu::getBase()->maxOptions) {
			drawRect({ global::uivars::menuX, global::uivars::menuY + (menu::getBase()->maxOptions * 0.0315f) }, { 0.23f, 0.0315f }, col);
		} else {
			drawRect({ global::uivars::menuX, global::uivars::menuY + (count * 0.0315f) }, { 0.23f, 0.0315f }, col);
		}
	}

	void renderer::displayTooltip(std::string toolTip) {
		if (!toolTip.empty()) {
			const char* yeet = native::createVarString(10, "LITERAL_STRING", toolTip.c_str());
			int32_t rcx[10] = { 5000, 0, 0, 0 };
			uint64_t rdx[10] = { 0x0, (uint64_t)yeet };
			_i<int>(0x049d5c615bd38bad, &rcx, &rdx, 1);
		}
	}

	void renderer::render() {
		renderingOptionsCount = math::clamp(getBase()->currentOption - getBase()->scrollOffset > getBase()->maxOptions ? getBase()->maxOptions : (getBase()->currentOption - getBase()->scrollOffset) + 1, 0, getBase()->maxOptions);
		int renderingOptions = getSubmenuHandler()->getTotalOptions() > getBase()->maxOptions ? getBase()->maxOptions : getSubmenuHandler()->getTotalOptions();
		int scrollerPosition = math::clamp(getBase()->currentOption - getBase()->scrollOffset > getBase()->maxOptions ? getBase()->maxOptions : getBase()->currentOption - getBase()->scrollOffset, 0, getBase()->maxOptions);
	
		// stop the menu flickering in pause menu
		native::setScriptGFxDrawBehindPauseMenu(TRUE);

		// header
		drawRect({ global::uivars::menuX, global::uivars::menuY - 0.127f }, { 0.23f, 0.09f }, global::uivars::titleHeader);

		// title
		drawText("ozark", { global::uivars::menuX + 0.115f, global::uivars::menuY - .114f }, 1.f, global::uivars::titleFont, global::uivars::titleText, JUSTIFY_CENTER, true);//0.815f

		// sub header
		drawRect({ global::uivars::menuX, global::uivars::menuY - 0.037f }, { 0.23f, 0.037f }, global::uivars::menuHeader);

		// background
		drawSpriteAligned("commonmenu", "gradient_bgd", { global::uivars::menuX, global::uivars::menuY }, { 0.23f, (renderingOptions * 0.0315f) }, 0.f, global::uivars::background);
		//drawRect({ global::uivars::menuX, global::uivars::menuY }, { 0.23f, (renderingOptions * 0.0315f) }, global::uivars::background);

		// scroller
		drawRect({ global::uivars::menuX, global::uivars::menuY + (scrollerPosition * 0.0315f) }, { 0.23f, 0.0315f }, global::uivars::scroller);

		// bottom box if option count > max 
		int count = 0;

		auto options = menu::getSubmenuHandler()->getCurrentSubmenu()->options;
		for (std::size_t i = 0; i < options.size(); i++) {
			if (options.at(i)->isVisible()) {
				count++;
			}
		}

		if (count > menu::getBase()->maxOptions) {
			drawRect({ global::uivars::menuX, global::uivars::menuY + (menu::getBase()->maxOptions * 0.0315f) }, { 0.23f, 0.0315f }, global::uivars::menuFooter);
			drawSprite("commonmenu", "shop_arrows_upanddown", { global::uivars::menuX + + 0.115f, global::uivars::menuY + (menu::getBase()->maxOptions * 0.0315f) + (0.0315f / 2.f) }, { 0.0215f, 0.0315f }, .0f, global::uivars::menuFooterArrows);
		}

		// option counter
		char counter[10];
		sprintf(counter, "%i / %i", menu::getBase()->currentOption + 1, (int)menu::getSubmenuHandler()->getCurrentSubmenu()->options.size());
		drawText(counter, { global::uivars::menuX + .004f, global::uivars::menuY - .03f }, 0.33f, global::uivars::headerFont, global::uivars::menuHeaderText, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (global::uivars::menuX + (0.315f / 2.f)) - .068f)) });
	}

	renderer* getRenderer() {
		static renderer instance;
		return &instance;
	}
}