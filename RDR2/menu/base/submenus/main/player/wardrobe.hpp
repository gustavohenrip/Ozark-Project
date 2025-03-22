#pragma once
#include "menu/base/submenu.hpp"

class wardrobeMenu : public menu::submenu {
public:
	static wardrobeMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	wardrobeMenu()
		: menu::submenu()
	{}

	~wardrobeMenu();
};

namespace wardrobeMenuVars {
	struct vars_ {
		bool isPedMale;
		bool autoApplyOutfit;
		scrollStruct<int> savedOutfits[0xFFFF];
		int savedOutfitsSize;
		int currentSavedOutfit;
		int clear;
		int masks;
		int hats;
		int hair;
		int shirts;
		int vests;
		int pants;
		int spurs;
		int chaps;
		int head;
		int cloak;
		int badge;
		int skirts;
		int spats;
		int neckwear;
		int boots;
		int accessories;
		int ringsRight;
		int ringsleft;
		int bracelet;
		int guantlets;
		int necktie;
		int legs;
		int legAccessories;
		int loadouts;
		int suspenders;
		int satchel;
		int teeth;
		int gunbelt;
		int belt;
		int beltBuckle;
		int torso;
		int offhandHolster;
		int coats;
		int coatsMp;
		int ponchos;
		int armor;
		int eyes;
		int eyewear;
		int gloves;
		int mustache;
	};

	extern vars_ vars;
}