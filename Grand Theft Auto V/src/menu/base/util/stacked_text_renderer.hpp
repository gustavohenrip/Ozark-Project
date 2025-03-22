#pragma once
#include "stdafx.hpp"

namespace Menu::Util {
	struct StackedRendererItem {
		bool m_Rendering;
		int m_RenderID;
		std::string m_Key;
		std::string m_Value;
	};

	class StackedTextRenderer {
	public:
		void Render();
		void Update(std::string id, std::string key, std::string value);
		void Alloc(std::string id, int renderID = -1);
		void Dealloc(std::string id);
	private:
		std::unordered_map<std::string, StackedRendererItem> m_Messages;
	};

	StackedTextRenderer* GetStackedTextRenderer();
}