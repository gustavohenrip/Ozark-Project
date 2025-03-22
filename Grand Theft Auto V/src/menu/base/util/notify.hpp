#pragma once
#include "stdafx.hpp"
#include "global/ui_vars.hpp"

namespace Menu {
	struct NotifyContext {
		std::string m_Text;
		int m_Lines;
		float m_FontHeight;
		uint32_t m_EndTime;
		uint32_t m_AlphaStartTime;
		int m_Alpha = 200; // Alpha for entire context (text, rect)
		Color m_BarColor;
		float m_X = 0.005f;
		float m_W = 0.223f;

		bool m_IsLine = false;
		std::vector<std::string> m_VecLines;

		NotifyContext() {}

		NotifyContext(std::string A, int B, uint32_t C, uint32_t D, int E, Color F, bool G = false, std::vector<std::string> H = {}) {
			m_Text = A;
			m_Lines = B;
			m_EndTime = C;
			m_AlphaStartTime = D;
			m_Alpha = E;
			m_BarColor = F;
			m_IsLine = G;
			m_VecLines = H;
		}
	};

	class Notify {
	public:
		void Update();
		void NotifyStacked(std::string Text, Color Col = ::Global::UIVars::g_TitleHeaderColor);
		void NotifyStackedLines(std::vector<std::string> Lines, Color Col = ::Global::UIVars::g_TitleHeaderColor);
		void Subtitle(const char* Text);
		void NotifyStackedProtectionEvent(std::string Sender, std::string Event, bool Redirected, bool Blocked);

		std::vector<std::string>& GetTextContext() { return m_TextContext; }

		float m_Padding = 0.225f;
	private:
		std::vector<NotifyContext> m_Context;
		std::vector<std::string> m_TextContext;
		std::unordered_map<uint32_t, uint32_t> m_ProtectionSpam;
	};

	Notify* GetNotify();
}