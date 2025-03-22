#include "base.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/submenu_handler.hpp"
#include "rage/invoker/natives.hpp"
#include "util/player_manager.hpp"
#include "util/stacked_text_render.hpp"

namespace menu {
	void base::update() {
		if (disableInputThisFrame) {
			disableInputThisFrame = false;
			isInputDisabled = true;
		} else {
			isInputDisabled = false;
		}

		util::getInput()->update();
		util::getStackedTextRenderer()->render();

		if (getBase()->open) {
			// native::setInputExclusive(0, INPUT_FRONTEND_ACCEPT);
			native::setInputExclusive(0, INPUT_FRONTEND_CANCEL);
			native::setInputExclusive(0, INPUT_FRONTEND_DOWN);
			native::setInputExclusive(0, INPUT_FRONTEND_UP);
			native::setInputExclusive(0, INPUT_FRONTEND_LEFT);
			native::setInputExclusive(0, INPUT_FRONTEND_RIGHT);
			native::disableControlAction(0, INPUT_FRONTEND_UP, TRUE);
			native::disableControlAction(0, INPUT_FRONTEND_DOWN, TRUE);
			native::disableControlAction(0, INPUT_FRONTEND_LEFT, TRUE);
			native::disableControlAction(0, INPUT_FRONTEND_RIGHT, TRUE);
			native::disableControlAction(0, INPUT_FRONTEND_RB, TRUE);
			native::disableControlAction(0, INPUT_FRONTEND_CANCEL, TRUE);
			native::disableControlAction(0, INPUT_MP_TEXT_CHAT_ALL, TRUE);
			native::disableControlAction(0, INPUT_MP_TEXT_CHAT_TEAM, TRUE);
			native::disableControlAction(0, INPUT_MP_TEXT_CHAT_FRIENDS, TRUE);
			native::disableControlAction(0, INPUT_MP_TEXT_CHAT_CREW, TRUE);
			native::disableControlAction(0, INPUT_CINEMATIC_SLOWMO, TRUE);
			native::disableControlAction(0, INPUT_PHONE, TRUE);
			native::disableControlAction(0, INPUT_SELECT_CHARACTER_MICHAEL, TRUE);
			native::disableControlAction(0, INPUT_SELECT_CHARACTER_FRANKLIN, TRUE);
			native::disableControlAction(0, INPUT_SELECT_CHARACTER_TREVOR, TRUE);
			native::disableControlAction(0, INPUT_SELECT_CHARACTER_MULTIPLAYER, TRUE);
			native::disableControlAction(0, INPUT_MELEE_ATTACK, TRUE);
			native::disableControlAction(0, INPUT_HORSE_ATTACK, TRUE);
			native::disableControlAction(0, INPUT_HORSE_ATTACK2, TRUE);
			native::disableControlAction(0, INPUT_CELLPHONE_UP, TRUE);
			native::disableControlAction(0, INPUT_SCRIPT_PAD_UP, TRUE);
			native::setCinematicButtonActive(FALSE);
			native::disableFrontendThisFrame();

			getRenderer()->render();
			getSubmenuHandler()->update();
		} else {
			if (getBase()->openTooltip) {
				getRenderer()->drawOpenTooltip();
			}
		}
	}
	
	void base::asyncUpdate() {
		getPlayerManager()->update();
	}

	bool base::isOptionSelected(int sub) {
		return getRenderer()->getRenderingOptionsCount() == sub + 1;
	}

	base* getBase() {
		static base instance;
		return &instance;
	}
}