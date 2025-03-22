#include "base.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/submenu_handler.hpp"
#include "util/stacked_text_renderer.hpp"
#include "util/player_manager.hpp"

namespace Menu {
	void Base::Update() {
		if (m_DisableInputThisFrame) {
			m_DisableInputThisFrame = false;
			m_IsInputDisabled = true;
		} else {
			m_IsInputDisabled = false;
		}

		Util::GetInput()->Update();
		Util::GetStackedTextRenderer()->Render();

		if (m_Open) {
			Native::SetInputExclusive(2, INPUT_FRONTEND_ACCEPT);
			Native::SetInputExclusive(2, INPUT_FRONTEND_CANCEL);
			Native::SetInputExclusive(2, INPUT_FRONTEND_DOWN);
			Native::SetInputExclusive(2, INPUT_FRONTEND_UP);
			Native::SetInputExclusive(2, INPUT_FRONTEND_LEFT);
			Native::SetInputExclusive(2, INPUT_FRONTEND_RIGHT);
			Native::DisableControlAction(0, INPUT_NEXT_CAMERA, true);
			Native::DisableControlAction(0, INPUT_VEH_SELECT_NEXT_WEAPON, true);
			Native::DisableControlAction(0, INPUT_FRONTEND_RIGHT, true);
			Native::DisableControlAction(0, INPUT_FRONTEND_DOWN, true);
			Native::DisableControlAction(0, INPUT_FRONTEND_LEFT, true);
			Native::DisableControlAction(0, INPUT_FRONTEND_UP, true);
			Native::DisableControlAction(0, INPUT_ARREST, true);
			Native::DisableControlAction(0, INPUT_CONTEXT, true);
			Native::DisableControlAction(0, INPUT_MELEE_BLOCK, true);
			Native::DisableControlAction(2, INPUT_FRONTEND_UP, true);
			Native::DisableControlAction(0, INPUT_HUD_SPECIAL, true);
			Native::DisableControlAction(0, INPUT_VEH_CIN_CAM, true);
			Native::DisableControlAction(2, INPUT_VEH_CIN_CAM, true);
			Native::DisableControlAction(0, INPUT_VEH_HEADLIGHT, true);
			Native::DisableControlAction(2, INPUT_FRONTEND_LEFT, true);
			Native::DisableControlAction(2, INPUT_FRONTEND_DOWN, true);
			Native::DisableControlAction(2, INPUT_FRONTEND_RDOWN, true);
			Native::DisableControlAction(2, INPUT_FRONTEND_RIGHT, true);
			Native::DisableControlAction(0, INPUT_CHARACTER_WHEEL, true);
			Native::DisableControlAction(0, INPUT_VEH_RADIO_WHEEL, true);
			Native::DisableControlAction(2, INPUT_FRONTEND_CANCEL, true);
			Native::DisableControlAction(0, INPUT_MELEE_ATTACK_LIGHT, true);
			Native::DisableControlAction(0, INPUT_MELEE_ATTACK_HEAVY, true);
			Native::DisableControlAction(0, INPUT_SELECT_CHARACTER_TREVOR, true);
			Native::DisableControlAction(0, INPUT_SELECT_CHARACTER_MICHAEL, true);
			Native::DisableControlAction(0, INPUT_SELECT_CHARACTER_FRANKLIN, true);
			Native::DisableControlAction(0, INPUT_SELECT_CHARACTER_MULTIPLAYER, true);

			GetRenderer()->Render();
			GetSubmenuHandler()->Update();
		} else {
			if (m_OpenTooltip) {
				GetRenderer()->RenderOpenTooltip();
			}
		}
	}

	void Base::AsyncUpdate() {
		GetPlayerManager()->Update();
	}

	bool Base::IsOptionSelected(int Submenu) {
		return GetRenderer()->GetRenderingOptionCount() == Submenu + 1;
	}

	Base* GetBase() {
		static Base Instance;
		return &Instance;
	}
}