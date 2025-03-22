#include "network_casino_rigging_blackjack_cards.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_casino_rigging_blackjack_selected.hpp"
#include "network_casino_rigging_blackjack.hpp"

using namespace NetworkCasinoRiggingBlackjackCardsMenuVars;
using namespace NetworkCasinoRiggingBlackjackMenuVars;

namespace NetworkCasinoRiggingBlackjackCardsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<eBlackjackCards> Cards[] = {
		{ TranslationString("Ace of Clubs", true), ACE_OF_CLUBS },
		{ TranslationString("Two of Clubs", true), TWO_OF_CLUBS },
		{ TranslationString("Three of Clubs", true), THREE_OF_CLUBS },
		{ TranslationString("Four of Clubs", true), FOUR_OF_CLUBS },
		{ TranslationString("Five of Clubs", true), FIVE_OF_CLUBS },
		{ TranslationString("Six of Clubs", true), SIX_OF_CLUBS },
		{ TranslationString("Seven of Clubs", true), SEVEN_OF_CLUBS },
		{ TranslationString("Eight of Clubs", true), EIGHT_OF_CLUBS },
		{ TranslationString("Nine of Clubs", true), NINE_OF_CLUBS },
		{ TranslationString("Ten of Clubs", true), TEN_OF_CLUBS },
		{ TranslationString("Jack of Clubs", true), JACK_OF_CLUBS },
		{ TranslationString("Queen of Clubs", true), QUEEN_OF_CLUBS },
		{ TranslationString("King of Clubs", true), KING_OF_CLUBS },
		{ TranslationString("Ace of Diamonds", true), ACE_OF_DIAMONDS },
		{ TranslationString("Two of Diamonds", true), TWO_OF_DIAMONDS },
		{ TranslationString("Three of Diamonds", true), THREE_OF_DIAMONDS },
		{ TranslationString("Four of Diamonds", true), FOUR_OF_DIAMONDS },
		{ TranslationString("Five of Diamonds", true), FIVE_OF_DIAMONDS },
		{ TranslationString("Six of Diamonds", true), SIX_OF_DIAMONDS },
		{ TranslationString("Seven of Diamonds", true), SEVEN_OF_DIAMONDS },
		{ TranslationString("Eight of Diamonds", true), EIGHT_OF_DIAMONDS },
		{ TranslationString("Nine of Diamonds", true), NINE_OF_DIAMONDS },
		{ TranslationString("Ten of Diamonds", true), TEN_OF_DIAMONDS },
		{ TranslationString("Jack of Diamonds", true), JACK_OF_DIAMONDS },
		{ TranslationString("Queen of Diamonds", true), QUEEN_OF_DIAMONDS },
		{ TranslationString("King of Diamonds", true), KING_OF_DIAMONDS },
		{ TranslationString("Ace of Hearts", true), ACE_OF_HEARTS },
		{ TranslationString("Two of Hearts", true), TWO_OF_HEARTS },
		{ TranslationString("Three of Hearts", true), THREE_OF_HEARTS },
		{ TranslationString("Four of Hearts", true), FOUR_OF_HEARTS },
		{ TranslationString("Five of Hearts", true), FIVE_OF_HEARTS },
		{ TranslationString("Six of Hearts", true), SIX_OF_HEARTS },
		{ TranslationString("Seven of Hearts", true), SEVEN_OF_HEARTS },
		{ TranslationString("Eight of Hearts", true), EIGHT_OF_HEARTS },
		{ TranslationString("Nine of Hearts", true), NINE_OF_HEARTS },
		{ TranslationString("Ten of Hearts", true), TEN_OF_HEARTS },
		{ TranslationString("Jack of Hearts", true), JACK_OF_HEARTS },
		{ TranslationString("Queen of Hearts", true), QUEEN_OF_HEARTS },
		{ TranslationString("King of Hearts", true), KING_OF_HEARTS },
		{ TranslationString("Ace of Spades", true), ACE_OF_SPADES },
		{ TranslationString("Two of Spades", true), TWO_OF_SPADES },
		{ TranslationString("Three of Spades", true), THREE_OF_SPADES },
		{ TranslationString("Four of Spades", true), FOUR_OF_SPADES },
		{ TranslationString("Five of Spades", true), FIVE_OF_SPADES },
		{ TranslationString("Six of Spades", true), SIX_OF_SPADES },
		{ TranslationString("Seven of Spades", true), SEVEN_OF_SPADES },
		{ TranslationString("Eight of Spades", true), EIGHT_OF_SPADES },
		{ TranslationString("Nine of Spades", true), NINE_OF_SPADES },
		{ TranslationString("Ten of Spades", true), TEN_OF_SPADES },
		{ TranslationString("Jack of Spades", true), JACK_OF_SPADES },
		{ TranslationString("Queen of Spades", true), QUEEN_OF_SPADES },
		{ TranslationString("King of Spades", true), KING_OF_SPADES }
	};
}

void NetworkCasinoRiggingBlackjackCardsMenu::Init() {
	SetName("Blackjack");
	SetParentSubmenu<NetworkCasinoRiggingBlackjackSelectedMenu>();
}

void NetworkCasinoRiggingBlackjackCardsMenu::Update() {}

/*Called once on submenu open*/
void NetworkCasinoRiggingBlackjackCardsMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	auto& CardTuple = NetworkCasinoRiggingBlackjackSelectedMenuVars::m_Vars.m_Tables[NetworkCasinoRiggingBlackjackSelectedMenuVars::m_Vars.m_Selected].m_PlayerCards[NetworkCasinoRiggingBlackjackSelectedMenuVars::m_Vars.m_SelectedSeat];

	addOption(ScrollOption<eBlackjackCards>(TOGGLE, "First Card")
		.addToggle(CardTuple.m_First)
		.addScroll(CardTuple.m_FirstCard, 0, NUMOF(Cards), Cards));

	addOption(ScrollOption<eBlackjackCards>(TOGGLE, "Second Card")
		.addToggle(CardTuple.m_Second)
		.addScroll(CardTuple.m_SecondCard, 0, NUMOF(Cards), Cards));
}

/*Called always*/
void NetworkCasinoRiggingBlackjackCardsMenu::FeatureUpdate() {}

NetworkCasinoRiggingBlackjackCardsMenu* _instance;
NetworkCasinoRiggingBlackjackCardsMenu* NetworkCasinoRiggingBlackjackCardsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoRiggingBlackjackCardsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoRiggingBlackjackCardsMenu::~NetworkCasinoRiggingBlackjackCardsMenu() { delete _instance; }