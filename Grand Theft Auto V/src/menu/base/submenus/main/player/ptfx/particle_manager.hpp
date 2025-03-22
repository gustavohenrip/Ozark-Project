#pragma once
#include "menu/base/submenu.hpp"

class ParticleManagerMenu : public Menu::Submenu {
public:
	static ParticleManagerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ParticleManagerMenu()
		: Menu::Submenu() {}

	~ParticleManagerMenu();
};

namespace ParticleManagerMenuVars {
	struct LoopedParticle {
		const char* m_Dict;
		const char* m_Texture;
		int m_Bone;
		float m_Scale;
		int m_Wait;
		int m_Handle;
		bool m_LoopRequired;
		int m_Timer;
	};

	struct Vars_ {
		int m_Type;
		int m_Wait;
		int m_SelectedSocket;
		bool m_Looped;
		float m_Scale = 1.f;

		std::vector<LoopedParticle> m_LoopedParticles;

		RadioContext m_Radio = { "commonmenu", "shop_makeup_icon" };
	};

	extern Vars_ m_Vars;
}