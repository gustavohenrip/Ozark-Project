#pragma once
#include "menu/base/submenu.hpp"

class networkProtectionMenu : public menu::submenu {
public:
	static networkProtectionMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	networkProtectionMenu()
		: menu::submenu()
	{}

	~networkProtectionMenu();
};

namespace networkProtectionMenuVars {
	static uint32_t maliciousHashList[] = { 0x5EB0BAE0, 0x966D1C7A, 0x39c84a35, 0x3bf7829e, 0x5F3EE4D3, 0x53367A8A, 0x629ddf49, 0x7dd49b09 };

	struct vars_ {
		int choke = 5;
		int presenceJoinSelection;

		bool fakeLag;
		bool notifyAttacks;
		bool redirectAttacks;
		bool giveControl;
		bool clock;
		bool weather;
		bool fire;
		bool explosion;
		bool paralyse;
		bool stats;
		bool particles;
		bool attachment;
		bool clone;
		bool smartProtection;
		int smartProtectionIncludes;
		int maliciousEntityIncludes;
		bool spoofLocation;
		bool presenceRockstarMessage;
		bool presenceCrewMessage;
		bool presenceJoin;
		bool presenceNotifyJoins;
		bool maliciousEntities;
		bool presenceForceJoin;
		std::vector<std::pair<rage::network::netObject*, Player>> maliciousObjQueue;
	};

	bool isWeaponModel(Object object);
	extern vars_ vars;
}