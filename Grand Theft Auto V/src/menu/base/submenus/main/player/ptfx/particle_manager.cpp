#include "particle_manager.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../particle_fx.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/timers.hpp"

using namespace ParticleManagerMenuVars;

namespace ParticleManagerMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Type[] = {
		{ TranslationString("Attach to Socket", true), 0 },
		{ TranslationString("Current Position", true), 1 }
	};

	void StartParticle(const char* Dict, const char* Texture, bool Looped = false) {
		Menu::GetControlManager()->RequestParticleFX({ Dict, Texture }, [=](std::pair<const char*, const char*> Anim) {
			Native::_UseParticleFxAssetNextCall(Anim.first);

			int Handle = 0;
			if (m_Vars.m_Type == 0) {
				// Socket

				// Stop current if exists
				auto Vit = std::find_if(m_Vars.m_LoopedParticles.begin(), m_Vars.m_LoopedParticles.end(), [](LoopedParticle& Element) {
					return Element.m_Bone == Global::Arrays::g_PedBones[m_Vars.m_SelectedSocket].m_Result && Element.m_LoopRequired;
				});

				if (Vit != m_Vars.m_LoopedParticles.end()) {
					Native::RemoveParticleFx(Vit->m_Handle, true);
				}

				Handle = Looped ? Native::_StartParticleFxLoopedOnEntityBone2(Anim.second, Menu::GetLocalPlayer().m_Ped, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, Native::GetPedBoneIndex(Menu::GetLocalPlayer().m_Ped, Global::Arrays::g_PedBones[m_Vars.m_SelectedSocket].m_Result), m_Vars.m_Scale, false, false, false, 0, 0, 0)
					: Native::_StartParticleFxNonLoopedOnPedBone2(Anim.second, Menu::GetLocalPlayer().m_Ped, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, Global::Arrays::g_PedBones[m_Vars.m_SelectedSocket].m_Result, m_Vars.m_Scale, false, false, false);
			} else {
				// Current Position
				Math::Vector3<float> Position = Menu::GetLocalPlayer().m_Coords;
				Native::_StartParticleFxNonLoopedAtCoord2(Anim.second, Position.m_X, Position.m_Y, Position.m_Z, 0.f, 0.f, 0.f, m_Vars.m_Scale, false, false, false);
			}

			if ((m_Vars.m_Looped && m_Vars.m_Type == 0) || Looped) {
				m_Vars.m_LoopedParticles.push_back({ Anim.first, Anim.second, Global::Arrays::g_PedBones[m_Vars.m_SelectedSocket].m_Result, m_Vars.m_Scale, Global::Arrays::g_Timers[m_Vars.m_Wait].m_Result, Handle, Looped, 0 });
			}
		});
	}

	void UpdateParticleList(ParticleManagerMenu* This) {
		This->ClearOptionsOffset(7);

		if (m_Vars.m_Type == 0) {
			int Max = m_Vars.m_Looped ? NUMOF(Global::Arrays::g_Particles) : 20;
			m_Vars.m_Radio.Reset();
			m_Vars.m_Radio.m_Selected = 0;

			for (int i = 0; i < Max; i++) {
				auto Particle = Global::Arrays::g_Particles[i];
				This->addOption(RadioOption(Particle.m_Name.Get())
						.addRadio(m_Vars.m_Radio)
						.addOnClick([=] { StartParticle(Particle.m_Result.m_Dict, Particle.m_Result.m_Texture, Particle.m_Result.m_Looped); }));
			}
		} else {
			for (auto& Particle : Global::Arrays::g_Particles) {
				This->addOption(ButtonOption(Particle.m_Name.Get())
						.addOnClick([=] { StartParticle(Particle.m_Result.m_Dict, Particle.m_Result.m_Texture, Particle.m_Result.m_Looped); }));
			}
		}
	}
}

void ParticleManagerMenu::Init() {
	SetName("Particle Manager");
	SetParentSubmenu<ParticleFXMenu>();

	addOption(ButtonOption("Stop All Particles")
		.addTranslation()
		.addOnClick([] {
			for (auto& Particle : m_Vars.m_LoopedParticles) {
				Native::RemoveParticleFx(Particle.m_Handle, true);
			}

			m_Vars.m_LoopedParticles.clear();
		})
		.addTooltip("Stops running looped particles"));

	addOption(ScrollOption<int>(SCROLL, "Type")
		.addTranslation()
		.addScroll(m_Vars.m_Type, 0, NUMOF(Type), Type)
		.addOnClick([this] { UpdateParticleList(this); }));

	addOption(ScrollOption<int>(SCROLL, "Selected Socket")
		.addTranslation()
		.addScroll(m_Vars.m_SelectedSocket, 0, NUMOF(Global::Arrays::g_PedBones), Global::Arrays::g_PedBones)
		.addRequirement([] { return m_Vars.m_Type == 0; }));

	addOption(ScrollOption<int>(SCROLL, "Wait")
		.addTranslation()
		.addScroll(m_Vars.m_Wait, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers)
		.addRequirement([] { return m_Vars.m_Type == 0 && m_Vars.m_Looped; }));

	addOption(ToggleOption("Looped")
		.addTranslation()
		.addToggle(m_Vars.m_Looped)
		.addOnClick([this] { UpdateParticleList(this); })
		.addRequirement([] { return m_Vars.m_Type == 0; }));

	addOption(NumberOption<float>(SCROLL, "Scale")
		.addTranslation()
		.addNumber(m_Vars.m_Scale, "%.2f", 0.01f).addMin(0.f).setScrollSpeed(10));

	addOption(BreakOption("Particles")
		.addTranslation());
}

void ParticleManagerMenu::Update() {}

/*Called once on submenu open*/
void ParticleManagerMenu::UpdateOnce() {
	UpdateParticleList(this);
}

/*Called always*/
void ParticleManagerMenu::FeatureUpdate() {
	for (auto& Particle : m_Vars.m_LoopedParticles) {
		if (!Particle.m_LoopRequired) {
			Menu::Timers::RunTimedFunction(&Particle.m_Timer, Particle.m_Wait, [=] {
				Native::_UseParticleFxAssetNextCall(Particle.m_Dict);
				Native::_StartParticleFxNonLoopedOnPedBone2(Particle.m_Texture, Menu::GetLocalPlayer().m_Ped, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, Particle.m_Bone, Particle.m_Scale, false, false, false);
			});
		}
	}
}

ParticleManagerMenu* _instance;
ParticleManagerMenu* ParticleManagerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ParticleManagerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ParticleManagerMenu::~ParticleManagerMenu() { delete _instance; }