#include "stacked_text_renderer.hpp"
#include "menu/base/renderer.hpp"
#include "rage/invoker/natives.hpp"
#include <algorithm>

namespace Menu::Util {
	void StackedTextRenderer::Render() {
		int DrawIndex = 0;
		int DrawCount = 0;

		for (auto& Msg : m_Messages) {
			if (Msg.second.m_Rendering && !Msg.second.m_Key.empty() && !Msg.second.m_Value.empty()) DrawCount++;
		}

		if (DrawCount > 0) {
			float W = 0.15f;
			float X = 1.f - W - 0.005f;
			float Y = 0.01f;
			float BezzelSize = 0.007875f / 2.f;

			// header
			Menu::GetRenderer()->DrawRect({ X, Y }, { W, BezzelSize }, Global::UIVars::g_TitleHeaderColor);

			// bg
			Menu::GetRenderer()->DrawRect({ X, Y + BezzelSize }, { W, (0.015f * DrawCount) + 0.005f }, { 0, 0, 0, 180 });

			for (int i = 0; i < 50; i++) {
				for (auto& Msg : m_Messages) {
					if (Msg.second.m_Rendering) {
						Native::HideHudComponentThisFrame(1);
						Native::HideHudComponentThisFrame(2);
						Native::HideHudComponentThisFrame(3);
						Native::HideHudComponentThisFrame(4);

						if (Msg.second.m_RenderID == i) {
							Menu::GetRenderer()->DrawText(Msg.second.m_Key.c_str(), { X + 0.002f, Y + (DrawIndex * 0.015f) + BezzelSize + 0.002f }, 0.20f, 0, { 255, 255, 255, 255 });
							Menu::GetRenderer()->DrawText(Msg.second.m_Value.c_str(), { 0.f, Y + (DrawIndex * 0.015f) + BezzelSize + 0.002f }, 0.20f, 0, { 255, 255, 255, 255 }, JUSTIFY_RIGHT, false, { 0.f, X + W - 0.002f });
							DrawIndex++;
							break;
						}
					}
				}
			}
		}
	}

	void StackedTextRenderer::Update(std::string id, std::string key, std::string value) {
		auto& Message = m_Messages[id];
		Message.m_Key = key;
		Message.m_Value = value;
	}

	void StackedTextRenderer::Alloc(std::string id, int renderID) {
		auto& Message = m_Messages[id];
		Message.m_Rendering = true;
		Message.m_RenderID = renderID;
		Message.m_Key.clear();
		Message.m_Value.clear();
	}

	void StackedTextRenderer::Dealloc(std::string id) {
		auto& Message = m_Messages[id];
		Message.m_Rendering = false;
		Message.m_Key.clear();
		Message.m_Value.clear();
	}

	StackedTextRenderer* GetStackedTextRenderer() {
		static StackedTextRenderer Instance;
		return &Instance;
	}
}