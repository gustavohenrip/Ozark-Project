#pragma once
#include "stdafx.hpp"
#include "global/ui_vars.hpp"
#include "utils/math.hpp"

enum eJustify {
	JUSTIFY_LEFT,
	JUSTIFY_RIGHT,
	JUSTIFY_CENTER,
};

namespace Menu {
#undef DrawText

	class Renderer {
	public:
		void Render();
		void RenderTitle(const char* Title);
		void RenderTooltip(std::string Tooltip);
		void RenderOpenTooltip();
		void PreviewColor(Color Col);

		float GetStringWidth(const char* String, int Font, float Fontsize);
		void ResetGlobe();

		void DrawRect(Math::Vector2<float> Position, Math::Vector2<float> Scale, Color Col);
		void DrawSprite(const char* Dict, const char* Name, Math::Vector2<float> Position, Math::Vector2<float> Scale, float Heading, Color Col);
		void DrawSpriteAligned(const char* Dict, const char* Name, Math::Vector2<float> Position, Math::Vector2<float> Scale, float Heading, Color Col);
		void DrawLine(Math::Vector3<float> From, Math::Vector3<float> To, Color Col);
		void DrawText(const char* Text, Math::Vector2<float> Position, float Scale, int Font, Color Col, eJustify Justify = JUSTIFY_LEFT, bool Outline = false, Math::Vector2<float> Wrap = { 0.f, 0.f });

		int GetRenderingOptionCount() { return m_RenderingOptionCount; }
		bool IsRenderingCustomHeader();
	private:
		int m_GlobeHandle;
		int m_RenderingOptionCount;
	};

	Renderer* GetRenderer();
}