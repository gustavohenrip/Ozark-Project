#include "submenu.hpp"
#include "base.hpp"
#include "submenu_handler.hpp"
#include "renderer.hpp"
#include "menu/base/util/instructionals.hpp"
#include "global/auth_vars.hpp"
#include <algorithm>

namespace Menu {
	void Submenu::Init() {}
	void Submenu::UpdateOnce() {}
	void Submenu::Update() {}
	void Submenu::FeatureUpdate() {}

	void Submenu::UpdateMenus() {
		Update();

		if (GetRenderer()->IsRenderingCustomHeader()) {
			std::string Name = m_Name.Get();
			std::transform(begin(Name), end(Name), begin(Name), ::toupper);
			GetRenderer()->DrawText(Name.c_str(), { Global::UIVars::g_MenuX + .004f, Global::UIVars::g_MenuY - .03f }, 0.33f, Global::UIVars::g_HeaderFont, Global::UIVars::g_MenuHeaderTextColor);
		} else {
			if (Global::Auth::g_Username == "California") {
				GetRenderer()->DrawText("OZARK CALI EDITION", { Global::UIVars::g_MenuX + .004f, Global::UIVars::g_MenuY - .03f }, 0.33f, Global::UIVars::g_HeaderFont, Global::UIVars::g_MenuHeaderTextColor);
			} else GetRenderer()->DrawText("OZARK " MENU_VERSION, { Global::UIVars::g_MenuX + .004f, Global::UIVars::g_MenuY - .03f }, 0.33f, Global::UIVars::g_HeaderFont, Global::UIVars::g_MenuHeaderTextColor);
		}

		GetRenderer()->RenderTitle(m_Name.Get().c_str());

		GetSubmenuHandler()->SetTotalOptions(0);
		GetSubmenuHandler()->SetCurrentOptionWithoutBreaks(0);
		GetSubmenuHandler()->SetTotalOptionsWithoutBreaks(0);

		bool CurrentSlider = false;
		bool CurrentInput = false;
		bool CurrentHotkey = false;

		if (m_Options.size() > 0) {
			std::vector<std::shared_ptr<Option>> tempOptions;
			tempOptions.reserve(m_Options.size());

			int relativePosition = 0;
			int visibilityOffset = 0;

			for (std::size_t i = 0; i < m_Options.size(); i++) {
				if (m_Options.at(i)->IsVisible()) {
					tempOptions.push_back(m_Options.at(i));
					visibilityOffset++;

					GetSubmenuHandler()->SetTotalOptions(GetSubmenuHandler()->GetTotalOptions() + 1);
					if (!m_Options.at(i)->IsBreak()) {
						if (visibilityOffset <= GetBase()->m_CurrentOption) {
							GetSubmenuHandler()->SetCurrentOptionWithoutBreaks(GetSubmenuHandler()->GetCurrentOptionWithoutBreaks() + 1);
						}

						GetSubmenuHandler()->SetTotalOptionsWithoutBreaks(GetSubmenuHandler()->GetTotalOptionsWithoutBreaks() + 1);
					}
				}
			}

			for (int i = GetBase()->m_ScrollOffset; i < GetBase()->m_ScrollOffset + GetBase()->m_MaxOptions; i++) {
				if (i >= 0 && i < tempOptions.size()) {
					if (relativePosition >= GetBase()->m_MaxOptions) break;
					tempOptions.at(i).get()->Render(relativePosition);
					// mouse
					if (relativePosition == GetBase()->m_CurrentOption - GetBase()->m_ScrollOffset) {
						if (GetBase()->m_Open) {
							GetRenderer()->RenderTooltip(tempOptions.at(i).get()->GetTooltip().Get());
							tempOptions.at(i).get()->RenderSelected(relativePosition, m_ParentNameStack);
							CurrentSlider = tempOptions.at(i).get()->IsSlider();
							CurrentInput = tempOptions.at(i).get()->IsInput();
							CurrentHotkey = tempOptions.at(i).get()->HasHotkey();
						}
					}

					relativePosition++;
				}
			}
		}

		if (m_DefaultInstructionals) {
			Instructionals::Standard(CurrentSlider, CurrentInput, CurrentHotkey);
		}
	}

	void Submenu::ClearOptionsOffset(int offset) {
		if (offset > m_Options.size()) return;

		for (auto& Option : m_Options) {
			if (Option->HasHotkey()) {
				Menu::Util::GetHotkey()->UnregisterHotkey(&*Option);
			}
		}

		m_Options.resize(offset);
	}

	void Submenu::SetName(std::string n, bool translation, bool searchable) {
		if (!n.empty() && n.length() > 0) {
			m_Name.Set(n.c_str());
			m_Name.SetHasTranslation(translation);
			m_CanBeSearched = searchable;
		}
	}
}