#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"
#include "global/ui_vars.hpp"
#include "rage/types/network_types.hpp"
#include "menu/base/util/player_manager.hpp"
#include "rage/types/bit_buffer.hpp"

namespace Menu::Helpers {
	enum eESPTypes {
		ESP_BOX,
		ESP_BOX_NO_INNER,
		ESP_AXIS
	};

	bool IsRockstarIP(uint32_t ExternalIP);
	void TriggerScriptEvent(int* Args, int Count, Player Bitset);
	void TriggerScriptEvent(std::vector<int> Args, int Count, Player Bitset);
	void DrawSnapline(Entity From, Entity To, Color Col);
	void DrawESP(Entity Ent, Color Col, eESPTypes Type, bool InnerLinesBox = false);
	Color GetColorFromBlip(int ColorID);
	void SendTextMessage(Player ID, Player Sender, const char* Message);
	std::string CleanPlayerName(std::string Name);
	Menu::PlayerVars& GetPlayerFromPeerMac(uint32_t Peer);
	Menu::PlayerVars& GetPlayerFromIP(uint32_t IP);
	Menu::PlayerVars& GetPlayerFromPeer(uint64_t Peer);
	Menu::PlayerVars& GetPlayerFromRID(uint64_t Peer);

	// Protex
	std::string GetVtableName(uint64_t Table);
	bool GetPlayerModelFromPlayerAppearanceDataNode(Rage::Network::DataNode* Node, uint32_t* OutModel, uint64_t* OutNode);
	void GetModelHashFromNode(Rage::Network::DataNode* Node, Rage::BitBuffer* BitBuffer, uint32_t* OutModel, std::string* OutNameDebugOnly);
}