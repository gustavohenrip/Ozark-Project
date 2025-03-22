#pragma once
#include "menu/base/submenu.hpp"

class spawnPedListMenu : public menu::submenu {
public:
	static spawnPedListMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	spawnPedListMenu()
		: menu::submenu()
	{}

	~spawnPedListMenu();
};

namespace spawnPedListMenuVars {
	struct vars_ {
		int size;
		scrollStruct<Hash>* list;
		scrollStruct<variants>* listVariant;
		bool listVariants;
	};

	extern vars_ vars;
}