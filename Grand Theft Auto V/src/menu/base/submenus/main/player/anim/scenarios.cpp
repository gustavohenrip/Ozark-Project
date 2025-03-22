#include "scenarios.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../animation.hpp"

using namespace ScenarioMenuVars;

namespace ScenarioMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> Ambient[] = {
		{ TranslationString("Coffee", true), "WORLD_HUMAN_AA_COFFEE" },
		{ TranslationString("Smoking", true), "WORLD_HUMAN_AA_SMOKE" },
		{ TranslationString("Freeway Bum", true), "WORLD_HUMAN_BUM_FREEWAY" },
		{ TranslationString("Sleeping Bum", true), "WORLD_HUMAN_BUM_SLUMPED" },
		{ TranslationString("Cheer", true), "WORLD_HUMAN_CHEERING" },
		{ TranslationString("Partying", true), "WORLD_HUMAN_PARTYING" },
		{ TranslationString("Shine Flashlight", true), "WORLD_HUMAN_SECURITY_SHINE_TORCH" },
		{ TranslationString("Tourist Map", true), "WORLD_HUMAN_TOURIST_MAP" }
	};

	ScrollStruct<const char*> Activities[] = {
		{ TranslationString("Binoculars", true), "WORLD_HUMAN_BINOCULARS" },
		{ TranslationString("Drunk", true), "WORLD_HUMAN_DRINKING" },
		{ TranslationString("Human Statue", true), "WORLD_HUMAN_HUMAN_STATUE" },
		{ TranslationString("Musician", true), "WORLD_HUMAN_MUSICIAN" },
		{ TranslationString("Fishing", true), "WORLD_HUMAN_STAND_FISHING" },
		{ TranslationString("BBQ", true), "PROP_HUMAN_BBQ" },
		{ TranslationString("Yoga", true), "WORLD_HUMAN_YOGA" }
	};

	ScrollStruct<const char*> Animals[] = {
		{ TranslationString("Grazing Boar", true), "WORLD_BOAR_GRAZING" },
		{ TranslationString("Grazing Cow", true), "WORLD_COW_GRAZING" },
		{ TranslationString("Coyote Howl", true), "WORLD_COYOTE_HOWL" },
		{ TranslationString("Coyote Rest", true), "WORLD_COYOTE_REST" },
		{ TranslationString("Coyote Wander", true), "WORLD_COYOTE_WANDER" },
		{ TranslationString("Rottweiler Bark", true), "WORLD_DOG_BARKING_ROTTWEILER" }
	};

	ScrollStruct<const char*> Fitness[] = {
		{ TranslationString("Idle Jogger", true), "WORLD_HUMAN_JOG_STANDING" },
		{ TranslationString("Golf", true), "WORLD_HUMAN_GOLF_PLAYER" },
		{ TranslationString("Muscle Flex", true), "WORLD_HUMAN_MUSCLE_FLEX" },
		{ TranslationString("Free Weights", true), "WORLD_HUMAN_MUSCLE_FREE_WEIGHTS" },
		{ TranslationString("Tennis", true), "WORLD_HUMAN_TENNIS_PLAYER" },
		{ TranslationString("Bench Press", true), "PROP_HUMAN_SEAT_MUSCLE_BENCH_PRESS" }
	};

	ScrollStruct<const char*> Industrial[] = {
		{ TranslationString("Parking Attendant", true), "WORLD_HUMAN_CAR_PARK_ATTENDANT" },
		{ TranslationString("Drill", true), "WORLD_HUMAN_CONST_DRILL" },
		{ TranslationString("Leaf Blower", true), "WORLD_HUMAN_GARDENER_LEAF_BLOWER" },
		{ TranslationString("Hammering", true), "WORLD_HUMAN_HAMMERING" },
		{ TranslationString("Janitor", true), "WORLD_HUMAN_JANITOR" },
		{ TranslationString("Maid", true), "WORLD_HUMAN_MAID_CLEAN" },
		{ TranslationString("Welding", true), "WORLD_HUMAN_WELDING" }
	};
}

void ScenarioMenu::Init() {
	SetName("Scenarios");
	SetParentSubmenu<AnimationMenu>();

	addOption(ButtonOption("Stop All Scenarios")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::ClearPedTasksImmediately(Menu::GetLocalPlayer().m_Ped); }));

	addOption(ScrollOption<const char*>(SCROLLSELECT, "Ambient")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Ambient, 0, NUMOF(Ambient), Ambient)
		.addOnClick([] { Native::TaskStartScenarioInPlace(Menu::GetLocalPlayer().m_Ped, Ambient[m_Vars.m_Ambient].m_Result, 0, true); }));

	addOption(ScrollOption<const char*>(SCROLLSELECT, "Activities")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Activities, 0, NUMOF(Activities), Activities)
		.addOnClick([] { Native::TaskStartScenarioInPlace(Menu::GetLocalPlayer().m_Ped, Activities[m_Vars.m_Activities].m_Result, 0, true); }));

	addOption(ScrollOption<const char*>(SCROLLSELECT, "Animals")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Animals, 0, NUMOF(Animals), Animals)
		.addOnClick([] { Native::TaskStartScenarioInPlace(Menu::GetLocalPlayer().m_Ped, Animals[m_Vars.m_Animals].m_Result, 0, true); }));

	addOption(ScrollOption<const char*>(SCROLLSELECT, "Fitness")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Fitness, 0, NUMOF(Fitness), Fitness)
		.addOnClick([] { Native::TaskStartScenarioInPlace(Menu::GetLocalPlayer().m_Ped, Fitness[m_Vars.m_Fitness].m_Result, 0, true); }));

	addOption(ScrollOption<const char*>(SCROLLSELECT, "Industrial")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Industrial, 0, NUMOF(Industrial), Industrial)
		.addOnClick([] { Native::TaskStartScenarioInPlace(Menu::GetLocalPlayer().m_Ped, Industrial[m_Vars.m_Industrial].m_Result, 0, true); }));
}

void ScenarioMenu::Update() {}

/*Called once on submenu open*/
void ScenarioMenu::UpdateOnce() {}

/*Called always*/
void ScenarioMenu::FeatureUpdate() {}

ScenarioMenu* _instance;
ScenarioMenu* ScenarioMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ScenarioMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ScenarioMenu::~ScenarioMenu() { delete _instance; }