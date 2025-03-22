#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"
#include "rage/types/network_types.hpp"
#include "rage/types/script_types.hpp"
#include "rage/types/generic_types.hpp"
#include "rage/types/bit_buffer.hpp"

namespace Rage::Engine {
	uint64_t GetEntityAddress(::Entity Entity);
	::Entity GetEntityHandleFromAddress(uint64_t Address);
	uint64_t GetModelInfo(uint32_t ModelHash);
	uint64_t GetVehiclePaint(int Category);
	uint64_t GetSpriteTexture(const char* Dict, const char* Name);
	Rage::Network::SyncTree* GetSyncTreeFromCloneType(uint32_t CloneType);
	std::vector<uint32_t> GetHashListFromHashTable(int Count, uint64_t Table);
	Rage::Network::NetGamePlayer* GetNetGamePlayerFromIndex(int Index);
	Rage::Network::NetGamePlayer* GetHostNetGamePlayer();
	Rage::Network::NetGamePlayer* GetNetGamePlayerFromHandle(uint64_t Handle);
	Rage::Network::NetObj* GetNetworkObjectFromNetworkID(uint64_t PlayerManager, uint16_t NetworkID, bool Unk);
	Rage::Types::ParticleFX* GetParticleFXInfo(uint32_t* Name, uint32_t* Dict);
	void SetVehicleGravity(uint64_t EntityAddress, float Gravity);
	void AddToClockTime(int Hours, int Minutes, int Seconds);
	void TriggerScriptEvent(int A, int* B, int C, int Bitset);
	void SendIncrementStatEvent(uint32_t Hash, uint32_t Value, Rage::Network::NetGamePlayer* NetPlayer);
	void HandleRotationValuesFromOrder(Math::Matrix* Matrix, Math::Vector3_<float>* Rotation, int Order);
	void SendTextMessageNetMsg(Rage::Network::NetMsgIdentifier* Target, Rage::Network::TextMessageNetMsg* Identifier);
	void SendSessionInfoRequest(Rage::Network::RockstarIdentifier* Targets, int Count);
	uint64_t SocialClubGetGameInfoAddress(const char* Identifier);
	const char* SocialClubGetGameInfoString(const char* Identifier);
	int SocialClubGetGameInfoInt(const char* Identifier);
	uint64_t SocialClubGetGameInfoUInt64(const char* Identifier);
	bool GetSessionInfoFromGs(Rage::Network::GsSession* Info, const char* Gs, int Unk1, int Unk2);
	void JoinPlayerViaSessionInfo(Rage::Network::NetMsgIdentifier* Identifier, Rage::Network::GsSession* SessionInfo, int Flag);
	void SendNetworkEventReply(uint64_t Table, uint8_t* SetupBuffer, Rage::Network::NetGamePlayer* Sender, Rage::Network::NetGamePlayer* Receiver, int EventIndex, int EventBitset);
	void SendNetworkEventAck(uint64_t Table, uint8_t* SetupBuffer, Rage::Network::NetGamePlayer* Sender, Rage::Network::NetGamePlayer* Receiver, int EventIndex);
	bool SetupGiveControlEvent(uint8_t* Out, Rage::BitBuffer* BitBuffer);
	void SendFriendRequest(uint64_t RockstarID);
	void SendGlobalChatMessage(uint64_t Manager, int PlayerKey, Rage::Network::GlobalMsgContext* Context);
	int GetClientPlayerKey(uint64_t Manager, int* SecondaryKey, int Flag);
	bool SendPresenceEvent(Rage::Network::RockstarIdentifier* Identifier, uint8_t* Payload);
	void MigrateScriptHost(Rage::Script::GameScriptHandlerNetworkComponent* Component, Rage::Network::NetGamePlayer* Player);
	void SendMatchmakingRequest();
	uint64_t ReadXmlString(uint64_t Context, const char* Name, int Unk1, int Unk2);
	const char* ReadXmlAttributeString(Rage::Types::XmlNode* Node, const char* Name, int Unk1);
	bool AddBasketToQueue(Rage::Network::NetShopQueue* Queue, int* TransactionID, int Category, int Action, int Flag);
	void AddItemToBasket(Rage::Network::NetShopQueue* Queue, int* Data);
	void RegisterTexture(std::string FilePath, std::string DictionaryName);
	bool ProcessBasketTransaction(int Category, int Action, int Destination, std::vector<Transaction> Items);
	bool IsBasketTransactionQueueBusy();
	void SendRagdollEvent(int Player);
	uint64_t GetScriptLocals(const char* Name);
}