#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEventReactionsMenu : public Menu::Submenu {
public:
	static NetworkProtectionEventReactionsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEventReactionsMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEventReactionsMenu();
};

namespace NetworkProtectionEventReactionsMenuVars {
	enum eEventReactions {
		EVENT_DEATH,
		EVENT_AIM,
		EVENT_REPORT,
		EVENT_SPOOFED_NAME,
		EVENT_SPOOFED_PEER,
		EVENT_SPOOFED_RID,
		EVENT_SPOOFED_IP,
		EVENT_ARXAN,
		EVENT_CRASH_ATTEMPT,
		EVENT_SPECTATING_YOU,
		EVENT_MONEY_DROP,
		EVENT_QUANTUM_KICK,
		EVENT_SCRIPT_MIGRATION
	};

	struct EventReaction {
		bool m_Explode;
		bool m_Crash;
		bool m_Kick;
		bool m_AddToOverseer;
		bool m_Cage;
		bool m_KickFromVehicle;
		bool m_SetOnFire;
		bool m_Ragdoll;
		bool m_GlitchPhysics;
		bool m_BlackScreen;
		bool m_Blame;
		bool m_GiveWantedLevel;
		bool m_SendToIsland;
		bool m_RemoveWeapons;
		bool m_HostileClone;

		bool IsActive() {
			for (int i = 0; i < sizeof(EventReaction); i++) {
				if (*(bool*)((uint64_t)this + i)) return true;
			}

			return false;
		}
	};
	
	struct Vars_ {
		EventReaction m_Reactions[13];
	};

	void ProcessEvent(eEventReactions Event, Player ID);
	extern Vars_ m_Vars;
}