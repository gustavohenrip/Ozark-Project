#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"
#include "utils/math.hpp"
#include "rage/types/base_types.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/renderer.hpp"

namespace menu {
	struct playerVars {
		const char* name;
		Player id;
		Ped ped;
		Hash weaponHash;
		Entity entity;
		math::vector3<float> coords;
		bool modder;
		bool exists;
		bool connected;
		bool inVehicle;
		bool isTeleportActive;
		bool isSessionHost;
		bool isFriend;
		bool isScriptHost;
		bool isOzarkUser;
		bool isOzarkUserDev;
		bool isOzarkHiddenUser;
		bool isOzarkPassive;
		bool isOnMount;
		rage::network::netGamePlayer* netGamePlayer;
		Entity mount;
		Vehicle vehicle;
		math::vector2<float> screen;
		std::string notes = "";

		// net
		bool explodeLoop;
		bool nearbyExplosions;
		bool rainbowWeather;
		bool bulletBarrage;
		bool espName;
		bool espLine;
		bool _2dbox;
		int _2dBoxType;
		bool _3dbox;
		bool forcefield;
		int setOnFireHandle;
		bool setOnFire;
		bool paralyse;
		bool hostilePeds;
		bool fakeMoney;
		bool explosiveAmmo;
		bool antiAim;
		bool infiniteDeadeye;
		int antiAimType;
		int teleportType;
		bool spectate;
		int reportType;
		int explosiveAmmoType;
		int attachmentPedToThem;
		int attachmentVehicleToThem;
		bool crashPlayer;
		bool socialclubSpam;
		uint32_t crashTimer;
		color espLineColor = color(60, 130, 255, 255);
		color espNameColor = color(255, 255, 255, 255);
		std::vector<Ped> attachedPeds;
		std::vector<Vehicle> attachedVehicles;
		bool particleOverrideScale;
		float particleScale = 2.f;
		int particleLoopedTimeout = 10;
	};

	class playerManager {
	public:
		playerVars players[32];

		void update();
		void updateNotes(int player);
		void foreach(std::function<void(playerVars&)> callback, bool bypass = false);
		playerVars& getPlayer(int index);
		playerVars& getLocalPlayer();
	}; playerManager* getPlayerManager();

	playerVars& getPlayer(int index);
	playerVars& getLocalPlayer();
	playerVars& getSelectedPlayer();
}