#pragma once
#include "menu/base/submenu.hpp"

class translationMenu : public menu::submenu {
public:
	static translationMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	translationMenu()
		: menu::submenu()
	{}

	~translationMenu();
};

namespace translationMenuVars {
	struct vars_ {
		std::vector<std::pair<std::string, struct stat>> cachedTranslations;
		int listSize;
		int updateTranslation;
		scrollStruct<int> translationList[100];
	};

	void load(const char* file);
	extern vars_ vars;
}