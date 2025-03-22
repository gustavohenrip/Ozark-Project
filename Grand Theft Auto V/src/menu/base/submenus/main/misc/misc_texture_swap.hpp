#pragma once
#include "menu/base/submenu.hpp"

class MiscTextureSwapMenu : public Menu::Submenu {
public:
	static MiscTextureSwapMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscTextureSwapMenu()
		: Menu::Submenu() {}

	~MiscTextureSwapMenu();
};

namespace MiscTextureSwapMenuVars {
	struct TextureSwapContext {
		std::pair<std::string, std::string> m_Original;
		std::pair<std::string, std::string> m_Target;
		Rage::Types::DictionaryItem m_OriginalItem;
	};

	struct TextureSwap {
		std::vector<TextureSwapContext> m_Contexts;
		std::string m_Name = "";
		bool m_Enabled = false;
		bool m_Initialized = false;
	};

	struct Vars_ {
		std::vector<TextureSwap> m_Swaps;
	};

	extern Vars_ m_Vars;
}
