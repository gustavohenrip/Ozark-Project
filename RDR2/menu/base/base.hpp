#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"
#include "utils/math.hpp"

typedef enum {
	SCROLL, // Scroll through the numbers/strings and execute the addFunction function every time the user scrolls. Pressing the option opens up the custom input window
	TOGGLE, // Add a toggle to enable/disable the feature. Acts the same as the SCROLL type.
	SCROLLSELECT // Same as scroll, just without custom input, and the addFunction function is called on press
} ScrollOptionType;

namespace menu {
	class base {
	public:
		void update();
		void asyncUpdate();
		bool isOptionSelected(int sub);

		bool open = false;
		bool disableInputThisFrame = false;
		bool isInputDisabled = false;
		int maxOptions = 14;
		int currentOption = 0;
		int currentOptionRendering = 0;
		int scrollOffset = 0;
		int breakScroll = 0;
		const char* keyboardTitle = "";
		bool openTooltip = true;
		bool panels = true;
	}; base* getBase();
}