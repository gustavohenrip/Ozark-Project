#include "visions.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../player.hpp"
#include "rage/engine.hpp"

using namespace visionMenuVars;

namespace visionMenuVars {
	vars_ vars;

	scrollStruct<DWORD> visionHashList[] = {
		"Default", 0,
		"Realistic", 0x63BA053,
		"Dark", 0x10397FB0,
		"Ocean", 0x6FA4C143,
		"Vibrant", 0x170BDD9D,
		"Moody", 0x4745FA62,
		"Sandy", 0x64BA7B05,
		"Very Bright", 0xDB54AC26,
		"Player Spotlight", 0x55B22100,
		"Small Render Distance", 0xC40C0DD0,
		"Blur", 0x93F88112,
		"Underwater", 0xa2663418
	};

	DWORD getHashFromID(int id) {
		for (std::size_t i = 0; i < global::vars::g_visionHashes.size(); i++) {
			if (global::vars::g_visionHashes[i].second == id) {
				return global::vars::g_visionHashes[i].first;
			}
		}

		return 0;
	}
}

void visionMenu::init() {
	setName("Visions");
	setParentSubmenu<playerMenu>();

	addOption(numberOption<int>(SCROLLSELECT, "Set Vision via ID")
		.addTranslation()
		.addNumber(vars.vision, "%i", 1).addMin(0).addMax(0)
		.addOnUpdate([] (numberOption<int>* option) { option->addMax((int)global::vars::g_visionHashes.size()); })
		.addOnClick([=] { rage::engine::setTimecycleModifierWithHash(getHashFromID(vars.vision)); }));

	addOption(buttonOption("Custom Input")
		.addTranslation()
		.addKeyboard("Enter ID, hash or name of vision", 100, [] (const char* res) {
			if (res) {
				if (res[0] == '0' && (res[1] == 'x' || res[1] == 'X')) {
					// hash
					rage::engine::setTimecycleModifierWithHash((DWORD)_strtoui64(res, 0, 0));
				} else {
					std::string str(res);
					if (std::find_if(str.begin(), str.end(), [] (unsigned char c) { return !std::isdigit(c); }) == str.end()) {
						// numeric
						rage::engine::setTimecycleModifierWithHash(atoi(res));
					} else {
						// str
						rage::engine::setTimecycleModifierWithHash(native::getHashKey(res));
					}
				}
			}
		}));

	addOption(breakOption("Preset Visions")
		.addTranslation());
	
	for (int i = 0; i < NUMOF(visionHashList); i++) {
		addOption(buttonOption(visionHashList[i].name)
			.addOnClick([=] { rage::engine::setTimecycleModifierWithHash(visionHashList[i].result); }));
	}
}

/*Called while in submenu*/
void visionMenu::update() {}

/*Called once on submenu open*/
void visionMenu::updateOnce() {
	if (global::vars::g_visionHashes.empty()) {
		if (global::vars::g_visionHashList) {
			WORD count = *(WORD*)(global::vars::g_visionHashList + 0x48);
			if (count) {
				uint64_t list = *(uint64_t*)(global::vars::g_visionHashList + 0x40);
				if (list) {
					for (WORD i = 0; i < count; i++) {
						uint64_t entry = *(uint64_t*)(list + (i * 8));
						if (entry) {
							DWORD hash = *(DWORD*)(entry + 0x10);
							if (hash) {
								auto vit = std::find_if(global::vars::g_visionHashes.begin(), global::vars::g_visionHashes.end(), [=] (std::pair<DWORD, int>& f) -> bool { return f.first == hash; });
								if (vit == global::vars::g_visionHashes.end()) {
									DWORD h = hash;
									global::vars::g_visionHashes.push_back(std::make_pair(hash, caller::call<int>(global::vars::g_getIndexFromHashList, global::vars::g_visionHashList, &h, "SetScriptModifierId", 0)));
								}
							}
						}
					}
				}
			}
		}
	}
}

/*Called always*/
void visionMenu::featureUpdate() {}

visionMenu* _instance;
visionMenu* visionMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new visionMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

visionMenu::~visionMenu() { delete _instance; }