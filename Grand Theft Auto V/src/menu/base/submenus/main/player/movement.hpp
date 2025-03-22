#pragma once
#include "menu/base/submenu.hpp"

class MovementMenu : public Menu::Submenu {
public:
	static MovementMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MovementMenu()
		: Menu::Submenu() {}

	~MovementMenu();
};

namespace MovementMenuVars {
	struct Vars_ {
		bool m_NoClip;
		bool m_NoClipNotify = true;
		bool m_NoClipBind;

		bool m_SuperJump;
		bool m_SuperRun;
		bool m_SuperRunAlternative;
		bool m_SuperSwim;

		float m_SuperRunSpeed = 1.49f;
		float m_SuperRunAlternativeSpeed = 5.f;
		float m_SuperSwimSpeed = 1.49f;
		float m_NoClipSpeed = 1.f;
	};

	extern Vars_ m_Vars;
}