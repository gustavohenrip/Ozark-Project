#pragma once
#include "menu/base/submenu.hpp"
#include "global/arrays.hpp"

class modelChangeMenu : public menu::submenu {
public:
	static modelChangeMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	modelChangeMenu()
		: menu::submenu()
	{}

	~modelChangeMenu();
};

namespace modelChangeMenuVars {
	struct vars_ {
		int size;
		scrollStruct<Hash>* list;
	};

	extern vars_ vars;
}