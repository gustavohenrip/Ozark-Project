#include "animation.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../player.hpp"
#include "anim/scenarios.hpp"
#include "anim/clipsets.hpp"
#include "anim/animations.hpp"

using namespace AnimationMenuVars;

namespace AnimationMenuVars {
	Vars_ m_Vars;
}

void AnimationMenu::Init() {
	SetName("Animation");
	SetParentSubmenu<PlayerMenu>();

	addOption(SubmenuOption("Animations")
		.addTranslation().addHotkey()
		.addSubmenu<AnimationsMenu>());

	addOption(SubmenuOption("Scenarios")
		.addTranslation().addHotkey()
		.addSubmenu<ScenarioMenu>());

	addOption(SubmenuOption("Clipsets")
		.addTranslation().addHotkey()
		.addSubmenu<ClipsetMenu>());
}

void AnimationMenu::Update() {}

/*Called once on submenu open*/
void AnimationMenu::UpdateOnce() {}

/*Called always*/
void AnimationMenu::FeatureUpdate() {}

AnimationMenu* _instance;
AnimationMenu* AnimationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AnimationMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AnimationMenu::~AnimationMenu() { delete _instance; }