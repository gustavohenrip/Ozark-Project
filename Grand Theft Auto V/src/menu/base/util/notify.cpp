#include "notify.hpp"
#include "menu/base/renderer.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/helpers.hpp"
#include "utils/log.hpp"

namespace Menu {
	TranslationString T_Blocked("Blocked", true, true);
	TranslationString T_IncomingEvent("Incoming Event", true, true);
	TranslationString T_Event("Event", true, true);
	TranslationString T_From("From", true, true);
	TranslationString T_Redirected("Redirected", true, true);

	void Notify::Update() {
		float RenderingY = 0.01f;
		for (std::size_t i = 0; i < m_Context.size(); i++) {
			auto& V = m_Context[i];
			if (RenderingY > 0.7f) {
				// Over limit, handle making sure next notifies don't expire
				V.m_EndTime = GetTickCount() + 4000;
				continue;
			}

			// Handle alpha
			if (V.m_EndTime - GetTickCount() <= 2000) {
				if (V.m_AlphaStartTime == 0) V.m_AlphaStartTime = GetTickCount();
				V.m_Alpha = 200 - (((GetTickCount() - V.m_AlphaStartTime) * 200) / 2000);

				
				if (V.m_Alpha < 50) {
					// V.m_X -= ((V.m_X + 0.2f) - ((V.m_Alpha * (V.m_X + 0.2f)) / 50));
				}
			}

			// Manual not specified
			if (V.m_Lines == 0 || V.m_FontHeight == 0.0f) {
				Native::SetTextFont(0);
				Native::SetTextScale(0.f, 0.315f);
				Native::SetTextWrap(V.m_X + 0.004f, m_Padding);
				Native::_BeginTextCommandLineCount("STRING");
				Native::AddTextComponentSubstringPlayerName(V.m_Text.c_str());
				V.m_Lines = Native::_EndTextCommandGetLineCount(V.m_X + 0.004f, RenderingY + 0.005f);
				V.m_FontHeight = Native::_GetTextScaleHeight(0.315f, 0);
			}

			// Render
			float Extra = 0.0019375f;
			float Height = 0.0315f;

			if (V.m_IsLine) {
				std::string FullLine = "";
				float LongestWidth = 0.f;

				V.m_W = Menu::GetRenderer()->GetStringWidth(V.m_VecLines[0].c_str(), 0, 0.315f) + 0.008f;

				for (std::size_t i = 0; i < V.m_VecLines.size(); i++) {
					auto L = V.m_VecLines[i];

					float Width = Menu::GetRenderer()->GetStringWidth(L.c_str(), 0, 0.315f) + 0.008f;
					if (V.m_W < Width) {
						V.m_W = Width;
					}

					FullLine += L;

					if (i != (V.m_VecLines.size() - 1)) {
						FullLine += '\n';
					}
				}

				V.m_Lines = (int)V.m_VecLines.size();
				V.m_Text = FullLine;

				Height = (V.m_Lines * V.m_FontHeight) + (V.m_Lines * 0.004f) + (0.004f * 3);
			} else {
				if (V.m_Lines > 1) {
					Height = (V.m_Lines * V.m_FontHeight) + (V.m_Lines * 0.004f) + (0.004f * 3);
				} else {
					V.m_W = Menu::GetRenderer()->GetStringWidth(V.m_Text.c_str(), 0, 0.315f) + 0.008f;
				}
			}

			Native::ClearAllHelpMessages();
			GetRenderer()->DrawRect({ V.m_X, RenderingY }, { V.m_W, Height }, { 0, 0, 0, V.m_Alpha });
			GetRenderer()->DrawRect({ V.m_X, RenderingY }, { V.m_W, 0.002f + Extra }, V.m_BarColor.WithOpacity(V.m_Alpha));

			Math::Vector2<float> Wrap = V.m_IsLine ? Math::Vector2 <float>(0.f, 1.f) : Math::Vector2<float>(V.m_X + 0.004f, m_Padding);
			GetRenderer()->DrawText(V.m_Text.c_str(), { V.m_X + 0.004f, RenderingY + 0.005f + 0.002f }, 0.315f, 0, { 255, 255, 255, V.m_Alpha }, JUSTIFY_LEFT, false, Wrap);
			
			RenderingY += Height + 0.003f;
		}

		for (std::size_t i = 0; i < m_Context.size(); i++) {
			auto& V = m_Context[i];

			if (V.m_EndTime < GetTickCount()) {
				auto Vit = std::find(m_TextContext.begin(), m_TextContext.end(), V.m_Text);
				if (Vit != m_TextContext.end()) m_TextContext.erase(Vit);

				m_Context.erase(m_Context.begin() + i);
				continue;
			}
		}
	}

	void Notify::NotifyStackedLines(std::vector<std::string> Lines, Color Col) {
		// Add context - 4s for notify to show
		m_Context.push_back({ "", 0, GetTickCount() + 4000, 0, 200, Col, true, Lines });
	}

	void Notify::NotifyStacked(std::string Text, Color Col) {
		// Add context - 4s for notify to show
		if (Text.length() < 99) LOG_CUSTOM_INFO("Notify", "%s", Text.c_str());

		m_TextContext.push_back(Text);
		m_Context.push_back({ Text, 0, GetTickCount() + 5000, 0, 200, Col, false, {} });
	}

	void Notify::NotifyStackedProtectionEvent(std::string Sender, std::string Event, bool Redirected, bool Blocked) {
		uint32_t Hash = Native::GetHashKey(std::string(Sender + Event + std::to_string(Redirected) + std::to_string(Blocked)).c_str());
		
		if (m_ProtectionSpam[Hash] > GetTickCount()) {
			return;
		}

		m_ProtectionSpam[Hash] = GetTickCount() + 5000;

		if (!Sender.empty()) {
			if (strcmp(Sender.c_str(), "~c~Unknown~w~")) {
				Sender = Menu::Helpers::CleanPlayerName(Sender);
			}
		}

		std::vector<std::string> Lines;
		if (Blocked) Lines.push_back(std::string(TRANSLATE(T_Blocked)) + " " + TRANSLATE(T_IncomingEvent));
		else Lines.push_back(TRANSLATE(T_IncomingEvent));

		Lines.push_back(std::string(TRANSLATE(T_Event)) + ": " + Event);
		
		if (strcmp(Sender.c_str(), "~c~Unknown~w~")) {
			Lines.push_back(std::string(TRANSLATE(T_From)) + ": " + Sender + "~w~");
		}

		if (Redirected) {
			Lines.push_back(TRANSLATE(T_Redirected));
		}

		LOG_CUSTOM_INFO("Notify", "%s from %s", Event.c_str(), Sender.c_str());
		NotifyStackedLines(Lines, Global::UIVars::g_NotificationError);
	}

	void Notify::Subtitle(const char* Text) {
		Native::BeginTextCommandPrint("STRING");
		Native::AddTextComponentSubstringPlayerName(Text);
		Native::EndTextCommandPrint(3000, true);
	}

	Notify* GetNotify() {
		static Notify Instance;
		return &Instance;
	}
}