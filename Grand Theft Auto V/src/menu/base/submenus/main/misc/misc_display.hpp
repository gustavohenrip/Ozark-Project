#pragma once
#include "menu/base/submenu.hpp"

class MiscDisplayMenu : public Menu::Submenu {
public:
	static MiscDisplayMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscDisplayMenu()
		: Menu::Submenu() {}

	~MiscDisplayMenu();
};

namespace MiscDisplayMenuVars {
	struct Vars_ {
		bool m_FPS;
		bool m_Position;
		bool m_FreeSlots;
		bool m_ModderCount;
		bool m_EntityPools;

		int m_FPSVar;
		int m_DrawFPSCache[2];
		int m_DrawFPSAverage = 0;
		int m_DrawFPSAverageTotal = 0;
		int m_DrawFPSAverageCount = 0;
	};

	extern Vars_ m_Vars;
}