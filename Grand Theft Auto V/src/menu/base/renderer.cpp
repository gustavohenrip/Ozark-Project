#include "renderer.hpp"
#include "rage/invoker/natives.hpp"
#include "utils/math.hpp"
#include "menu/base/submenu_handler.hpp"

namespace Menu {
	bool Renderer::IsRenderingCustomHeader() {
		return Global::UIVars::g_InsideBennys
			|| Global::UIVars::g_InsideLSC
			|| Global::UIVars::g_InsideWeapon
			|| Global::UIVars::g_InsideCasino
			|| Global::UIVars::g_InsideCasinoLuckyWheel
			|| Global::UIVars::g_InsideCasinoSlotMachines
			|| Global::UIVars::g_InsideCasinoBlackjack
			|| Global::UIVars::g_InsideCasinoBlackjackHigh
			|| Global::UIVars::g_InsideCasinoPoker
			|| Global::UIVars::g_InsideCasinoPokerHigh
			|| Global::UIVars::g_InsideCasinoRoulette
			|| Global::UIVars::g_InsideCasinoRouletteHigh;
	}

	std::pair<std::pair<std::string, std::string>, bool&> CustomHeaders[] = {
		{ { "ShopUI_Title_Supermod", "ShopUI_Title_Supermod" }, Global::UIVars::g_InsideBennys },
		{ { "Ozark", "lsc" }, Global::UIVars::g_InsideLSC },
		{ { "shopui_title_gunclub", "shopui_title_gunclub" }, Global::UIVars::g_InsideWeapon },
		{ { "shopui_title_casino", "shopui_title_casino" }, Global::UIVars::g_InsideCasino },
		{ { "casinoui_lucky_wheel", "casinoui_lucky_wheel" }, Global::UIVars::g_InsideCasinoLuckyWheel },
		{ { "casinoui_cards_blackjack", "casinoui_cards_blackjack" }, Global::UIVars::g_InsideCasinoBlackjack },
		{ { "casinoui_cards_blackjack_high", "casinoui_cards_blackjack_high" }, Global::UIVars::g_InsideCasinoBlackjackHigh },
		{ { "casinoui_roulette", "casinoui_roulette" }, Global::UIVars::g_InsideCasinoRoulette },
		{ { "casinoui_roulette_high", "casinoui_roulette_high" }, Global::UIVars::g_InsideCasinoRouletteHigh },
		{ { "casinoui_cards_three", "casinoui_cards_three" }, Global::UIVars::g_InsideCasinoPoker },
		{ { "casinoui_cards_three_high", "casinoui_cards_three_high" }, Global::UIVars::g_InsideCasinoPokerHigh },
		{ { "casinoui_slots_diamond", "casinoui_slots_diamond" }, Global::UIVars::g_InsideCasinoSlotMachines },
	};

	void Renderer::Render() {
		if (Global::Vars::g_StopRendering) return;

		m_RenderingOptionCount = Math::Clamp(GetBase()->m_CurrentOption - GetBase()->m_ScrollOffset > GetBase()->m_MaxOptions ? GetBase()->m_MaxOptions : (GetBase()->m_CurrentOption - GetBase()->m_ScrollOffset) + 1, 0, GetBase()->m_MaxOptions);
		int renderingOptions = GetSubmenuHandler()->GetTotalOptions() > GetBase()->m_MaxOptions ? GetBase()->m_MaxOptions : GetSubmenuHandler()->GetTotalOptions();
		int scrollerPosition = Math::Clamp(GetBase()->m_CurrentOption - GetBase()->m_ScrollOffset > GetBase()->m_MaxOptions ? GetBase()->m_MaxOptions : GetBase()->m_CurrentOption - GetBase()->m_ScrollOffset, 0, GetBase()->m_MaxOptions);

		// header
		Color HeaderColor = Global::UIVars::g_TitleHeaderColor;

		for (auto& H : CustomHeaders) {
			if (H.second) {
				DrawSpriteAligned(H.first.first.c_str(), H.first.second.c_str(), { Global::UIVars::g_MenuX, Global::UIVars::g_MenuY - 0.127f }, { 0.23f, 0.09f }, 0.f, { 255, 255, 255, 255 });
				break;
			}
		}

		if (!IsRenderingCustomHeader()) {
			// main header
			DrawRect({ Global::UIVars::g_MenuX, Global::UIVars::g_MenuY - 0.127f }, { 0.23f, 0.09f }, HeaderColor);

			// draw globe
			if (Global::UIVars::g_MenuGlobe) {
				if (!Native::HasScaleformMovieLoaded(m_GlobeHandle)) {
					m_GlobeHandle = Native::RequestScaleformMovie("MP_MENU_GLARE");
				} else {
					float Rotation = Native::GetGameplayCamRot(2).m_Z;
					Rotation -= round(Rotation / 360.0f) * 360.0f;
					if (Rotation < 0.f) {
						Rotation += 360.0f;
					}

					Native::_PushScaleformMovieFunction(m_GlobeHandle, "SET_DATA_SLOT");
					Native::_PushScaleformMovieFunctionParameterFloat(Rotation);
					Native::_PopScaleformMovieFunctionVoid();
				}

				Native::DrawScaleformMovie(m_GlobeHandle, Global::UIVars::g_MenuX + 0.451f, Global::UIVars::g_MenuY + 0.323f, 0.98f, 1.f, 255, 255, 255, 255, 0);
			}
		}

		// sub header
		DrawRect({ Global::UIVars::g_MenuX, Global::UIVars::g_MenuY - 0.037f }, { 0.23f, 0.037124f }, Global::UIVars::g_MenuHeaderColor);

		// background
		// DrawRect({ Global::UIVars::g_MenuX, Global::UIVars::g_MenuY }, { 0.23f, (renderingOptions * 0.0315f) }, Global::UIVars::g_BackgroundColor);
		DrawSpriteAligned("commonmenu", "gradient_bgd", { Global::UIVars::g_MenuX, Global::UIVars::g_MenuY }, { 0.23f, (renderingOptions * 0.0315f) }, 0.f, Global::UIVars::g_BackgroundColor);

		// scroller
		DrawRect({ Global::UIVars::g_MenuX, Global::UIVars::g_MenuY + (scrollerPosition * 0.0315f) }, { 0.23f, 0.0315f }, Global::UIVars::g_ScrollerColor);

		// bottom box if option count > max 
		int Count = 0;
		auto Options = GetSubmenuHandler()->GetCurrentSubmenu()->m_Options;

		for (std::size_t i = 0; i < Options.size(); i++) {
			if (Options.at(i)->IsVisible()) {
				Count++;
			}
		}

		if (Count > GetBase()->m_MaxOptions) {
			DrawRect({ Global::UIVars::g_MenuX, Global::UIVars::g_MenuY + (GetBase()->m_MaxOptions * 0.0315f) }, { 0.23f, 0.0315f }, Global::UIVars::g_MenuFooterColor);
			DrawSprite("commonmenu", "shop_arrows_upanddown", { Global::UIVars::g_MenuX + 0.115f, Global::UIVars::g_MenuY + (GetBase()->m_MaxOptions * 0.0315f) + (0.0315f / 2.f) }, { 0.0215f, 0.0315f }, 0.f, { 255, 255, 255, Global::UIVars::g_MenuFooterColor.A });
		}

		// option counter
		char counter[10];
		sprintf(counter, "%i / %i", GetBase()->m_CurrentOption + 1, (int)GetSubmenuHandler()->GetCurrentSubmenu()->m_Options.size());
		DrawText(counter, { Global::UIVars::g_MenuX + .004f, Global::UIVars::g_MenuY - .03f }, 0.33f, Global::UIVars::g_HeaderFont, Global::UIVars::g_MenuHeaderTextColor, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - .068f)) });
	}

	void Renderer::RenderTitle(const char* Title) {
		if (IsRenderingCustomHeader()) return;

		int Font = Global::UIVars::g_TitleFont;
		bool Outline = false;
		float Scale = 0.9f;
		float Y = Global::UIVars::g_MenuY - 0.102f;

		if (GetSubmenuHandler()->GetCurrentSubmenu()) {
			if (GetSubmenuHandler()->GetCurrentSubmenu()->m_Name.GetOriginal() == "Ozark") {
				Font = 7;
				Outline = true;
				Scale = 1.f;
				Y = Global::UIVars::g_MenuY - 0.105f;
			}
		}

		DrawText(Title, { Global::UIVars::g_MenuX + 0.115f, Y }, Scale, Font, Global::UIVars::g_TitleTextColor, JUSTIFY_CENTER, Outline);
	}

	float Renderer::GetStringWidth(const char* String, int Font, float Fontsize) {
		// if (Font == FontChaletLondon) Fontsize *= 0.75f;
		Native::_BeginTextCommandWidth("STRING");
		Native::AddTextComponentSubstringPlayerName(String);
		Native::SetTextFont(Font);
		Native::SetTextScale(Fontsize, Fontsize);
		return Native::_EndTextCommandGetWidth(Font);
	}

	void Renderer::ResetGlobe() {
		if (Native::HasScaleformMovieLoaded(m_GlobeHandle)) {
			Native::SetScaleformMovieAsNoLongerNeeded(&m_GlobeHandle);
		}
	}

	void Renderer::RenderTooltip(std::string Tooltip) {
		if (!Tooltip.empty()) {
			Global::UIVars::g_RenderingTooltip = Tooltip;

			float Y = 0.f;
			
			// bottom box if option count > max 
			int Count = 0;
			auto Options = GetSubmenuHandler()->GetCurrentSubmenu()->m_Options;

			for (std::size_t i = 0; i < Options.size(); i++) {
				if (Options.at(i)->IsVisible()) {
					Count++;
				}
			}

			if (Count > GetBase()->m_MaxOptions) {
				Y = Global::UIVars::g_MenuY + (GetBase()->m_MaxOptions * 0.0315f) + 0.0315f;
			} else {
				Y = Global::UIVars::g_MenuY + (Count * 0.0315f);
			}

			Y += 0.002f;

			if (Global::UIVars::g_RenderingColor) {
				Global::UIVars::g_RenderingColor = false;
				Y += 0.0317f;
			}

			// Setup render
			Native::SetTextFont(Global::UIVars::g_OptionFont);
			Native::SetTextScale(0.f, 0.315f);
			Native::SetTextWrap(Global::UIVars::g_MenuX + 0.004f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - Global::UIVars::g_MenuWrap)));
			Native::_BeginTextCommandLineCount("STRING");
			Native::AddTextComponentSubstringPlayerName(Tooltip.c_str());
			int Lines = Native::_EndTextCommandGetLineCount(Global::UIVars::g_MenuX + 0.004f, Y + 0.005f);

			float Height = 0.0315f;
			if (Lines > 1) {
				Height = (Lines * 0.027f);
			}

			DrawSpriteAligned("commonmenu", "gradient_bgd", { Global::UIVars::g_MenuX, Y }, { 0.23f, Height }, 180.f, Global::UIVars::g_MenuFooterColor);
			DrawRect({ Global::UIVars::g_MenuX, Y }, { 0.23f, 0.001f }, { 255, 255, 255, Global::UIVars::g_MenuFooterColor.A });

			DrawText(Tooltip.c_str(), { Global::UIVars::g_MenuX + 0.004f, Y + 0.005f }, 0.315f, Global::UIVars::g_OptionFont, { 255, 255, 255, 255 }, JUSTIFY_LEFT, false, { Global::UIVars::g_MenuX + 0.004f, (1.0f - (1.0f - (Global::UIVars::g_MenuX + (0.315f / 2.f)) - Global::UIVars::g_MenuWrap)) });
		}
	}

	void Renderer::RenderOpenTooltip() {
		DrawText("put me in coach\n~c~F4", { 0.5f, 0.075f }, 0.450f, 2, Global::UIVars::g_OpenToolTipColor, JUSTIFY_CENTER, true);
	}

	void Renderer::PreviewColor(Color Col) {
		Global::UIVars::g_RenderingColor = true;

		int Count = 0;
		auto Options = GetSubmenuHandler()->GetCurrentSubmenu()->m_Options;

		for (std::size_t i = 0; i < Options.size(); i++) {
			if (Options.at(i)->IsVisible()) {
				Count++;
			}
		}

		if (Count > GetBase()->m_MaxOptions) {
			DrawRect({ Global::UIVars::g_MenuX, Global::UIVars::g_MenuY + ((GetBase()->m_MaxOptions + 1) * 0.0315f) }, { 0.23f, 0.0315f }, Col);
		} else {
			DrawRect({ Global::UIVars::g_MenuX, Global::UIVars::g_MenuY + (Count * 0.0315f) }, { 0.23f, 0.0315f }, Col);
		}
	}

	void Renderer::DrawRect(Math::Vector2<float> Position, Math::Vector2<float> Scale, Color Col) {
		if (Global::Vars::g_StopRendering) return;
		Native::DrawRect(Position.m_X + (Scale.m_X * 0.5f), Position.m_Y + (Scale.m_Y * 0.5f), Scale.m_X, Scale.m_Y, Col.R, Col.G, Col.B, Col.A, 0);
	}

	void Renderer::DrawSprite(const char* Dict, const char* Name, Math::Vector2<float> Position, Math::Vector2<float> Scale, float Heading, Color Col) {
		if (Global::Vars::g_StopRendering) return;
		if (!Native::HasStreamedTextureDictLoaded(Dict)) {
			Native::RequestStreamedTextureDict(Dict, false);
		}

		Native::DrawSprite(Dict, Name, Position.m_X, Position.m_Y, Scale.m_X, Scale.m_Y, Heading, Col.R, Col.G, Col.B, Col.A, FALSE);
	}

	void Renderer::DrawSpriteAligned(const char* Dict, const char* Name, Math::Vector2<float> Position, Math::Vector2<float> Scale, float Heading, Color Col) {
		if (Global::Vars::g_StopRendering) return;
		if (!Native::HasStreamedTextureDictLoaded(Dict)) {
			Native::RequestStreamedTextureDict(Dict, false);
		}

		Native::DrawSprite(Dict, Name, Position.m_X + (Scale.m_X * 0.5f), Position.m_Y + (Scale.m_Y * 0.5f), Scale.m_X, Scale.m_Y, Heading, Col.R, Col.G, Col.B, Col.A, FALSE);
	}

	void Renderer::DrawLine(Math::Vector3<float> From, Math::Vector3<float> To, Color Col) {
		if (Global::Vars::g_StopRendering) return;
		Native::DrawLine(From.m_X, From.m_Y, From.m_Z, To.m_X, To.m_Y, To.m_Z, Col.R, Col.G, Col.B, Col.A);
	}

	void Renderer::DrawText(const char* Text, Math::Vector2<float> Position, float Scale, int Font, Color Col, eJustify Justify, bool Outline, Math::Vector2<float> Wrap) {
		if (Global::Vars::g_StopRendering) return;
		if (Wrap.m_Y != 0.f) Native::SetTextWrap(Wrap.m_X, Wrap.m_Y);

		if (Justify != JUSTIFY_LEFT) {
			Native::SetTextJustification(Justify == JUSTIFY_CENTER ? 0 : 2);
		}

		Native::SetTextCentre(Justify == JUSTIFY_CENTER);
		Native::SetTextScale(0.f, Scale);
		Native::SetTextColour(Col.R, Col.G, Col.B, Col.A);
		Native::SetTextFont(Font);
		if (Outline) Native::SetTextOutline();

		Native::BeginTextCommandDisplayText("STRING");
		Native::AddTextComponentSubstringPlayerName(Text);
		Native::EndTextCommandDisplayText(Position.m_X, Position.m_Y, 0);
	}

	Renderer* GetRenderer() {
		static Renderer Instance;
		return &Instance;
	}
}