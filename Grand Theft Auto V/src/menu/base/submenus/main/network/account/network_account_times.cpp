#include "network_account_times.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account.hpp"
#include "menu/base/util/stats.hpp"

using namespace NetworkAccountTimesMenuVars;

namespace NetworkAccountTimesMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> StatNames[] = {
		{ TranslationString("In Multiplayer", true), "MP_PLAYING_TIME" },
		{ TranslationString("In Car", true), "TIME_IN_CAR" },
		{ TranslationString("In Cover", true), "TIME_IN_COVER" },
		{ TranslationString("In Water", true), "TIME_IN_WATER" },
		{ TranslationString("As a Passenger", true), "TIME_AS_A_PASSENGER" },
		{ TranslationString("As a Driver", true), "TIME_AS_A_DRIVER" },
		{ TranslationString("Flying", true), "TIME_SPENT_FLYING" },
		{ TranslationString("Swimming", true), "TIME_SWIMMING" },
		{ TranslationString("Underwater", true), "TIME_UNDERWATER" },
		{ TranslationString("Walking", true), "TIME_WALKING" },
		{ TranslationString("On Lost Bike", true), "TIME_ON_LOST_BIKE" },
		{ TranslationString("Riding a Bicycle", true), "TIME_DRIVING_BICYCLE" },
		{ TranslationString("Riding a Bike", true), "TIME_DRIVING_BIKE" },
		{ TranslationString("Driving a Boat", true), "TIME_DRIVING_BOAT" },
		{ TranslationString("Driving a Car", true), "TIME_DRIVING_CAR" },
		{ TranslationString("Flying a Helicopter", true), "TIME_DRIVING_HELI" },
		{ TranslationString("Flying a Plane", true), "TIME_DRIVING_PLANE" },
		{ TranslationString("Driving a Quad Bike", true), "TIME_DRIVING_QUADBIKE" },
		{ TranslationString("Driving a Submarine", true), "TIME_DRIVING_SUBMARINE" },
		{ TranslationString("Specating Crew Members", true), "TIME_SPECT_CREW_MEMBERS" },
		{ TranslationString("Spectating Freeroam", true), "TIME_SPECT_FREEROAM" },
		{ TranslationString("Spectating Friends", true), "TIME_SPECT_FRIENDS" },
		{ TranslationString("Spectating Friends (Non Crew)", true), "TIME_SPECT_FRIEND_NOT_CREW" },
		{ TranslationString("Spectating Jobs", true), "TIME_SPECT_JOBS" },
		{ TranslationString("Spectating Strangers", true), "TIME_SPECT_STRANGERS" },
	};

	void SetTime() {
		int Math = (m_Vars.m_Days * 86400000) + (m_Vars.m_Hours * 3600000) + (m_Vars.m_Minutes * 60000) + (m_Vars.m_Seconds * 1000) + m_Vars.m_Milliseconds;
		Stats::Interface<int>::Set(StatNames[m_Vars.m_Type].m_Result, Math);
	}

	void Initialize() {
		int Total = Stats::Interface<int>::Get(StatNames[m_Vars.m_Type].m_Result);
		m_Vars.m_Days = Total / 86400000;
		Total = Total % 86400000;
		m_Vars.m_Hours = Total / 3600000;
		Total = Total % 3600000;
		m_Vars.m_Minutes = Total / 60000;
		Total = Total % 60000;
		m_Vars.m_Seconds = Total / 1000;
		m_Vars.m_Milliseconds = Total % 1000;
	}
}

void NetworkAccountTimesMenu::Init() {
	SetName("Times");
	SetParentSubmenu<NetworkAccountMenu>();

	addOption(ScrollOption<const char*>(SCROLL, "Type")
		.addTranslation()
		.addScroll(m_Vars.m_Type, 0, NUMOF(StatNames), StatNames)
		.addOnClick(Initialize));

	addOption(NumberOption<int>(SCROLL, "Days")
		.addTranslation()
		.addNumber(m_Vars.m_Days, "%i", 1).addMin(0).addMax(std::numeric_limits<int>::max())
		.addOnClick(SetTime));

	addOption(NumberOption<int>(SCROLL, "Hours")
		.addTranslation()
		.addNumber(m_Vars.m_Hours, "%i", 1).addMin(0).addMax(24)
		.addOnClick(SetTime));

	addOption(NumberOption<int>(SCROLL, "Minutes")
		.addTranslation()
		.addNumber(m_Vars.m_Minutes, "%i", 1).addMin(0).addMax(60)
		.addOnClick(SetTime));

	addOption(NumberOption<int>(SCROLL, "Seconds")
		.addTranslation()
		.addNumber(m_Vars.m_Seconds, "%i", 1).addMin(0).addMax(60)
		.addOnClick(SetTime));

	addOption(NumberOption<int>(SCROLL, "Milliseconds")
		.addTranslation()
		.addNumber(m_Vars.m_Milliseconds, "%i", 1).addMin(0).addMax(1000)
		.addOnClick(SetTime));
}

void NetworkAccountTimesMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountTimesMenu::UpdateOnce() {
	Initialize();
}

/*Called always*/
void NetworkAccountTimesMenu::FeatureUpdate() {}

NetworkAccountTimesMenu* _instance;
NetworkAccountTimesMenu* NetworkAccountTimesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountTimesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountTimesMenu::~NetworkAccountTimesMenu() { delete _instance; }