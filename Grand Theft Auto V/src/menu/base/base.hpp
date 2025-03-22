#pragma once
#include "stdafx.hpp"
#include "utils/math.hpp"

typedef enum {
	SCROLL,
	TOGGLE,
	SCROLLSELECT
} ScrollOptionType;

namespace Menu {
	class Base {
	public:
		void Update();
		void AsyncUpdate();
		bool IsOptionSelected(int Submenu);

		bool m_Open = false;
		bool m_DisableInputThisFrame = false;
		bool m_IsInputDisabled = false;
		bool m_OpenTooltip = true;
		bool m_Panels = true;

		int m_MaxOptions = 13;
		int m_CurrentOption = 0;
		int m_CurrentOptionRendering = 0;
		int m_ScrollOffset = 0;
		int m_BreakScroll = 0;

		const char* m_KeyboardTitle = "";
	};
	
	Base* GetBase();
}