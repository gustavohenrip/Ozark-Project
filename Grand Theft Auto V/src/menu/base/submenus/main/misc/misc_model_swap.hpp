#pragma once
#include "menu/base/submenu.hpp"

class MiscModelSwapMenu : public Menu::Submenu {
public:
	static MiscModelSwapMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscModelSwapMenu()
		: Menu::Submenu() {}

	~MiscModelSwapMenu();
};

namespace MiscModelSwapMenuVars {
	struct ModelSwap {
		uint32_t m_OriginalModel;
		uint32_t m_NewModel;

		uint16_t m_OriginalOffset;

		// UI
		const char* m_OriginalName = "";
		const char* m_NewName = "";
		bool m_Toggled = true;
	};

	struct Vars_ {
		std::vector<ModelSwap> m_Swaps;
	};

	void AddSwap(const char* Original, const char* New);
	void RemoveSwap(uint32_t OriginalHash);
	extern Vars_ m_Vars;
}