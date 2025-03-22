#include "animations.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../animation.hpp"
#include "utils/fibers.hpp"
#include "utils/fiber_pool.hpp"

using namespace AnimationsMenuVars;

namespace AnimationsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<std::pair<const char*, const char*>> Sexual[] = {
		{ TranslationString("Doggystyle 1", true), { "rcmpaparazzo_2", "shag_loop_poppy" } },
		{ TranslationString("Doggystyle 2", true), { "rcmpaparazzo_2", "shag_loop_a" } },
		{ TranslationString("Shaking Ass", true), { "switch@trevor@mocks_lapdance", "001443_01_trvs_28_idle_stripper" } },
		{ TranslationString("Slow Humping", true), { "misscarsteal2pimpsex", "shagloop_pimp" } }
	};

	ScrollStruct<std::pair<const char*, const char*>> Animals[] = {
		{ TranslationString("Monkey", true), { "missfbi5ig_30monkeys", "monkey_b_freakout_loop" } },
		{ TranslationString("Chop Hump", true), { "missfra0_chop_find", "hump_loop_chop" } },
		{ TranslationString("Chop Swim", true), { "creatures@rottweiler@swim@", "swim" } }
	};

	ScrollStruct<std::pair<const char*, const char*>> Actions[] = {
		{ TranslationString("Air Guitar", true), { "anim@mp_player_intcelebrationfemale@air_guitar", "air_guitar" } },
		{ TranslationString("Blow Kiss", true), { "anim@mp_player_intcelebrationfemale@blow_kiss", "blow_kiss" } },
		{ TranslationString("Bro Hug", true), { "anim@mp_player_intcelebrationpaired@f_m_bro_hug", "bro_hug_right" } },
		{ TranslationString("Challenge", true), { "misscommon@response", "face_palm" } },
		{ TranslationString("Face Palm", true), { "anim@mp_player_intcelebrationmale@face_palm", "" } },
		{ TranslationString("Finger", true), { "anim@mp_player_intcelebrationmale@finger", "finger" } },
		{ TranslationString("Hands Up", true), { "mp_pol_bust_out", "guard_handsup_loop" } },
		{ TranslationString("Hump Air", true), { "anim@mp_player_intcelebrationfemale@air_shagging", "air_shagging" } },
		{ TranslationString("Jazz Hands", true), { "anim@mp_player_intcelebrationmale@jazz_hands", "jazz_hands" } },
		{ TranslationString("Nose Pick", true), { "anim@mp_player_intcelebrationmale@nose_pick", "nose_pick" } },
		{ TranslationString("Photographer", true), { "anim@mp_player_intcelebrationmale@photography", "photography" } },
		{ TranslationString("Salute", true), { "anim@mp_player_intcelebrationmale@salute", "salute" } },
		{ TranslationString("Shush", true), { "anim@mp_player_intcelebrationmale@shush", "shush" } },
		{ TranslationString("Slow Clap", true), { "anim@mp_player_intcelebrationmale@slow_clap", "slow_clap" } },
		{ TranslationString("Smoke", true), { "anim@mp_player_intcelebrationmale@smoke_flick", "smoke_flick" } },
		{ TranslationString("Surrender", true), { "anim@mp_player_intcelebrationmale@surrender", "surrender" } },
		{ TranslationString("Synth", true), { "anim@mp_player_intcelebrationfemale@air_synth", "air_synth" } },
		{ TranslationString("Thumbs Up", true), { "anim@mp_player_intcelebrationmale@thumbs_up", "thumbs_up" } },
		{ TranslationString("Wank", true), { "mp_player_intwank", "mp_player_int_wank" } }
	};

	ScrollStruct<std::pair<const char*, const char*>> Dance[] = {
		{ TranslationString("Casual", true), { "rcmnigel1bnmt_1b", "dance_loop_tyler" } },
		{ TranslationString("Clown", true), { "rcm_barry2", "clown_idle_6" } },
		{ TranslationString("Pole", true), { "mini@strip_club@pole_dance@pole_dance3", "pd_dance_03" } },
		{ TranslationString("Private", true), { "mini@strip_club@private_dance@part2", "priv_dance_p2" } },
		{ TranslationString("Receive", true), { "mp_am_stripper", "lap_dance_player" } },
		{ TranslationString("Sexual", true), { "mini@strip_club@pole_dance@pole_a_2_stage", "pole_a_2_stage" } },
		{ TranslationString("Yacht", true), { "oddjobs@assassinate@multi@yachttarget@lapdance", "yacht_ld_f" } }
	};

	ScrollStruct<std::pair<const char*, const char*>> Misc[] = {
		{ TranslationString("Electrocute", true), { "ragdoll@human", "electrocute" } },
		{ TranslationString("Hover", true), { "swimming@base", "dive_idle" } },
		{ TranslationString("Jump", true), { "move_jump", "jump_launch_l_to_skydive" } },
		{ TranslationString("Meditate", true), { "rcmcollect_paperleadinout@", "meditiate_idle" } },
		{ TranslationString("Party", true), { "rcmfanatic1celebrate", "celebrate" } },
		{ TranslationString("Pissing", true), { "misscarsteal2peeing", "peeing_loop" } },
		{ TranslationString("Push Ups", true), { "rcmfanatic3", "ef_3_rcm_loop_maryann" } },
		{ TranslationString("Run", true), { "move_m@alien", "alien_run" } },
		{ TranslationString("Shitting", true), { "missfbi3ig_0", "shit_loop_trev" } },
		{ TranslationString("Showering", true), { "mp_safehouseshower@male@", "male_shower_idle_b" } },
		{ TranslationString("Swim", true), { "swimming@scuba", "dive_idle" } },
		{ TranslationString("Vomit", true), { "missfam5_blackout", "vomit" } },
		{ TranslationString("Wave Forward", true), { "friends@frj@ig_1", "wave_d" } },
		{ TranslationString("Wave Hands High", true), { "random@prisoner_lift", "arms_waving" } },
		{ TranslationString("Wave One Arm", true), { "random@shop_gunstore", "_greeting" } }
	};

	void StartAnimation(std::pair<const char*, const char*> Anim) {
		Utils::GetFiberPool()->Push([=] {
			int Flag = AnimationRepeat;
			if (m_Vars.m_Controllable) Flag |= AnimationEnablePlayerControl | AnimationUpperBody;
			if (m_Vars.m_Contort) Flag |= AnimationCreature;

			while (!Native::HasAnimDictLoaded(Anim.first)) {
				Native::RequestAnimDict(Anim.first);
				Utils::WaitFiber(100);
			}

			Native::ClearPedTasksImmediately(Menu::GetLocalPlayer().m_Ped);
			Native::TaskPlayAnim(Menu::GetLocalPlayer().m_Ped, Anim.first, Anim.second, 8.f, 8.f, -1, Flag, 0.f, false, false, false);
		});
	}
}

void AnimationsMenu::Init() {
	SetName("Animations");
	SetParentSubmenu<AnimationMenu>();

	addOption(ButtonOption("Stop All Animations")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::ClearPedTasksImmediately(Menu::GetLocalPlayer().m_Ped); }));

	addOption(ToggleOption("Controllable")
		.addTranslation()
		.addToggle(m_Vars.m_Controllable).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Contort")
		.addTranslation()
		.addToggle(m_Vars.m_Contort).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<std::pair<const char*, const char*>>(SCROLLSELECT, "Sexual")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Sexual, 0, NUMOF(Sexual), Sexual)
		.addOnClick([] { StartAnimation(Sexual[m_Vars.m_Sexual].m_Result); }));

	addOption(ScrollOption<std::pair<const char*, const char*>>(SCROLLSELECT, "Dance")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Dance, 0, NUMOF(Dance), Dance)
		.addOnClick([] { StartAnimation(Dance[m_Vars.m_Dance].m_Result); }));

	addOption(ScrollOption<std::pair<const char*, const char*>>(SCROLLSELECT, "Actions")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Actions, 0, NUMOF(Actions), Actions)
		.addOnClick([] { StartAnimation(Actions[m_Vars.m_Actions].m_Result); }));

	addOption(ScrollOption<std::pair<const char*, const char*>>(SCROLLSELECT, "Animals")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Animals, 0, NUMOF(Animals), Animals)
		.addOnClick([] { StartAnimation(Animals[m_Vars.m_Animals].m_Result); }));

	addOption(ScrollOption<std::pair<const char*, const char*>>(SCROLLSELECT, "Misc")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Misc, 0, NUMOF(Misc), Misc)
		.addOnClick([] { StartAnimation(Misc[m_Vars.m_Misc].m_Result); }));
}

void AnimationsMenu::Update() {}

/*Called once on submenu open*/
void AnimationsMenu::UpdateOnce() {}

/*Called always*/
void AnimationsMenu::FeatureUpdate() {}

AnimationsMenu* _instance;
AnimationsMenu* AnimationsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AnimationsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AnimationsMenu::~AnimationsMenu() { delete _instance; }