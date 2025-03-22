#include "engine.hpp"
#include "utils/caller.hpp"
#include "global/variables.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/player_manager.hpp"

namespace Rage::Engine {
	uint64_t GetEntityAddress(::Entity Entity) {
		return Caller::Call<uint64_t>(Global::Vars::g_GetEntityAddress, Entity);
	}

	Entity GetEntityHandleFromAddress(uint64_t Address) {
		return Caller::Call<Entity>(Global::Vars::g_GetEntityHandleFromAddress, Address, 0);
	}

	std::vector<uint32_t> GetHashListFromHashTable(int Count, uint64_t Table) {
		std::vector<uint32_t> Hashes;

		if (Count && Table) {
			uint64_t* TablePtr = (uint64_t*)Table;
			
			for (int i = 0; i < Count; i++) {
				if (TablePtr[i]) {
					uint32_t Hash = *(uint32_t*)(TablePtr[i] + 0x10);

					if (Hash) {
						Hashes.push_back(Hash);
					}
				}
			}
		}

		return Hashes;
	}

	Rage::Network::NetGamePlayer* GetNetGamePlayerFromIndex(int Index) {
		return Caller::Call<Rage::Network::NetGamePlayer*>(Global::Vars::g_GetNetGamePlayerFromIndex, Index);
	}

	Rage::Network::NetGamePlayer* GetHostNetGamePlayer() {
		return Caller::Call<Rage::Network::NetGamePlayer*>(Global::Vars::g_GetHostNetGamePlayer);
	}

	Rage::Network::NetGamePlayer* GetNetGamePlayerFromHandle(uint64_t Handle) {
		return Caller::Call<Rage::Network::NetGamePlayer*>(Global::Vars::g_GetNetGamePlayerFromHandle, Handle);
	}

	Rage::Network::NetObj* GetNetworkObjectFromNetworkID(uint64_t PlayerManager, uint16_t NetworkID, bool Unk) {
		return Caller::Call<Rage::Network::NetObj*>(Global::Vars::g_GetNetworkObjectFromNetworkID, PlayerManager, NetworkID, Unk);
	}

	Rage::Types::ParticleFX* GetParticleFXInfo(uint32_t* Name, uint32_t* Dict) {
		return Caller::Call<Rage::Types::ParticleFX*>(Global::Vars::g_GetParticleInfo, Name, Dict);
	}

	void SetVehicleGravity(uint64_t EntityAddress, float Gravity) {
		Caller::Call<int>(Global::Vars::g_SetVehicleGravity, EntityAddress, Gravity);
	}

	void AddToClockTime(int Hours, int Minutes, int Seconds) {
		Caller::Call<int>(Global::Vars::g_AddToClockTime, Hours, Minutes, Seconds);
	}

	uint64_t GetModelInfo(uint32_t ModelHash) {
		int Offset = 0xFFFF;
		return Caller::Call<uint64_t>(Global::Vars::g_GetModelInfo, ModelHash, &Offset);
	}

	uint64_t GetVehiclePaint(int Category) {
		return Caller::Call<uint64_t>(Global::Vars::g_GetVehiclePaintArray, Category, true);
	}

	uint64_t GetSpriteTexture(const char* Dict, const char* Name) {
		return Caller::Call<uint64_t>(Global::Vars::g_GetSpriteTexture, Dict, Name);
	}

	Rage::Network::SyncTree* GetSyncTreeFromCloneType(uint32_t CloneType) {
		return Caller::Call<Rage::Network::SyncTree*>(Global::Vars::g_GetSyncTreeFromCloneType, 0, CloneType);
	}

	void TriggerScriptEvent(int A, int* B, int C, int Bitset) {
		Caller::Call<int>(Global::Vars::g_TriggerScriptEvent, A, B, C, Bitset);
	}

	void SendIncrementStatEvent(uint32_t Hash, uint32_t Value, Rage::Network::NetGamePlayer* NetPlayer) {
		Caller::Call<int>(Global::Vars::g_SendIncrementStatEvent, Hash, Value, NetPlayer);
	}

	void HandleRotationValuesFromOrder(Math::Matrix* Matrix, Math::Vector3_<float>* Rotation, int Order) {
		Caller::Call<int>(Global::Vars::g_HandleRotationValuesFromOrder, Matrix, Rotation, Order);
	}

	void SendTextMessageNetMsg(Rage::Network::NetMsgIdentifier* Target, Rage::Network::TextMessageNetMsg* Identifier) {
		Caller::Call<int>(Global::Vars::g_SendTextMessageNetMsg, *(uint64_t*)(uint64_t)Global::Vars::g_NetworkBaseConfig, Target, Identifier);
	}

	void SendSessionInfoRequest(Rage::Network::RockstarIdentifier* Targets, int Count) {
		Caller::Call<int>(Global::Vars::g_SendSessionInfoRequest, Targets, Count);
	}

	uint64_t SocialClubGetGameInfoAddress(const char* Identifier) {
		if (Global::Vars::g_SocialClubGameInfoTable && Global::Vars::g_SocialClubGetGameInfoTable) {
			int Index = Caller::Call<int>(Global::Vars::g_SocialClubGetGameInfoTable, Identifier, Global::Vars::g_SocialClubGameInfoTable + 0x20, *(uint32_t*)(Global::Vars::g_SocialClubGameInfoTable + 0x14));
			if (Index >= 0) {
				return (((Global::Vars::g_SocialClubGameInfoTable + 0x20) + (Index * 0x148)));
			}
		}

		return 0;
	}

	const char* SocialClubGetGameInfoString(const char* Identifier) {
		uint64_t Table = SocialClubGetGameInfoAddress(Identifier);
		if (Table) {
			const char* Name = (char*)(Table + 0x40);
			if (Name && *Name != 0xFF) {
				return Name;
			}
		}

		return nullptr;
	}

	int SocialClubGetGameInfoInt(const char* Identifier) {
		uint64_t Table = SocialClubGetGameInfoAddress(Identifier);
		if (Table) {
			return *(int*)(Table + 0x40);
		}

		return 0;
	}

	uint64_t SocialClubGetGameInfoUInt64(const char* Identifier) {
		uint64_t Table = SocialClubGetGameInfoAddress(Identifier);
		if (Table) {
			return *(uint64_t*)(Table + 0x40);
		}

		return 0;
	}

	bool GetSessionInfoFromGs(Rage::Network::GsSession* Info, const char* Gs, int Unk1, int Unk2) {
		return Caller::Call<bool>(Global::Vars::g_GetSessionInfoFromGs, Info, Gs, Unk1, Unk2);
	}

	void JoinPlayerViaSessionInfo(Rage::Network::NetMsgIdentifier* Identifier, Rage::Network::GsSession* SessionInfo, int Flag) {
		Rage::Network::NetworkHandle EmptyHandle;
		Caller::Call<int>(Global::Vars::g_JoinPlayerViaSessionInfo, Identifier, 1, SessionInfo, "", &EmptyHandle, "", -1, -1, 0, 0, Flag);
	}

	void SendNetworkEventReply(uint64_t Table, uint8_t* SetupBuffer, Rage::Network::NetGamePlayer* Sender, Rage::Network::NetGamePlayer* Receiver, int EventIndex, int EventBitset) {
		Caller::Call<int>(Global::Vars::g_SendNetworkEventReply, Table, SetupBuffer, Sender, Receiver, EventIndex, EventBitset);
	}

	void SendNetworkEventAck(uint64_t Table, uint8_t* SetupBuffer, Rage::Network::NetGamePlayer* Sender, Rage::Network::NetGamePlayer* Receiver, int EventIndex) {
		Caller::Call<int>(Global::Vars::g_SendNetworkEventAck, Table, SetupBuffer, Sender, Receiver, EventIndex);
	}

	bool SetupGiveControlEvent(uint8_t* Out, Rage::BitBuffer* BitBuffer) {
		return Caller::Call<bool>(Global::Vars::g_SetupGiveControlEvent, Out, BitBuffer);
	}

	void SendFriendRequest(uint64_t RockstarID) {
		Rage::Network::NetMsgIdentifier Yes;
		Yes.m_RockstarID = RockstarID;
		Yes.m_Type = 3;

		Caller::Call<int>(Global::Vars::g_SendFriendRequest, &Yes, "");
	}

	void SendGlobalChatMessage(uint64_t Manager, int PlayerKey, Rage::Network::GlobalMsgContext* Context) {
		Caller::Call<int>(Global::Vars::g_SendGlobalChatMessage, Manager, PlayerKey, Context, 1, (uint64_t)0);
	}

	int GetClientPlayerKey(uint64_t Manager, int* SecondaryKey, int Flag) {
		return Caller::Call<int>(Global::Vars::g_GetClientPlayerKey, Manager, SecondaryKey, Flag);
	}
	
	bool SendPresenceEvent(Rage::Network::RockstarIdentifier* Identifier, uint8_t* Payload){
		return Caller::Call<bool>(Global::Vars::g_SendPresenceEvent, 0, Identifier, 1, Payload, 1, 0);
	}

	void MigrateScriptHost(Rage::Script::GameScriptHandlerNetworkComponent* Component, Rage::Network::NetGamePlayer* Player) {
		Caller::Call<int>(Global::Vars::g_MigrateScriptHost, Component, Player);
	}

	void SendMatchmakingRequest() {
		Caller::Call<int>(Global::Vars::g_SendMatchmakingRequest, *(uint64_t*)(uint64_t)Global::Vars::g_NetworkBaseConfig, 1, 1, 1, 1);
	}

	uint64_t ReadXmlString(uint64_t Context, const char* Name, int Unk1, int Unk2) {
		return  Caller::Call<uint64_t>(Global::Vars::g_ReadXmlString, Context, Name, Unk1, Unk2);
	}

	const char* ReadXmlAttributeString(Rage::Types::XmlNode* Node, const char* Name, int Unk1) {
		return Caller::Call<const char*>(Global::Vars::g_ReadXmlNodeString, Node, Name, Unk1);
	}

	bool AddBasketToQueue(Rage::Network::NetShopQueue* Queue, int* TransactionID, int Category, int Action, int Flag) {
		return Caller::Call<bool>(Global::Vars::g_AddBasketToQueue, Queue, TransactionID, Category, Action, Flag);
	}

	void AddItemToBasket(Rage::Network::NetShopQueue* Queue, int* Data) {
		Caller::Call<int>(Global::Vars::g_AddItemToBasket, Queue, Data);
	}

	void RegisterTexture(std::string FilePath, std::string DictionaryName) {
		int Index = 0;
		Caller::Call<int>(Global::Vars::g_RegisterTexture, &Index, FilePath.c_str(), true, DictionaryName.c_str(), false);
	}

	bool ProcessBasketTransaction(int Category, int Action, int Destination, std::vector<Transaction> Items) {
		if (Native::_NetworkShopBasketEnd())
			_i<int>(0xFA336E7F40C0A0D0);

		int TransactionID = std::numeric_limits<int>::max();

		bool Result = AddBasketToQueue(Global::Vars::g_NetShopQueue, &TransactionID, Category, Action, Destination);
		if (!Result || !TransactionID) {
			return false;
		}

		for (std::size_t i = 0; i < Items.size(); i++) {
			Transaction Item = Items[i];
			AddItemToBasket(Global::Vars::g_NetShopQueue, (int*)&Item);
		}

		if (!Caller::Call<bool>(Global::Vars::g_ProcessTransactionQueue, Global::Vars::g_NetShopQueue, TransactionID, 1337)) {
			return false;
		}

		return true;
	}

	bool IsBasketTransactionQueueBusy() {
		return Global::Vars::g_NetShopQueue->m_IsBusy1 || Global::Vars::g_NetShopQueue->m_IsBusy2 || Global::Vars::g_NetShopQueue->m_IsBusy3;
	}

	void SendRagdollEvent(int Player) {
		if (Menu::GetPlayer(Player).m_PedPointer) {
			if (Menu::GetPlayer(Player).m_PedPointer->GetNetObj()) {
				Caller::Call<int>(Global::Vars::g_SendRagdollEvent, Menu::GetPlayer(Player).m_PedPointer->GetNetObj()->m_NetworkID);
			}
		}
	}

	uint64_t GetScriptLocals(const char* Name) {
		for (uint16_t i = 0; i < *(uint16_t*)(Global::Vars::g_ScriptHandlerPoolCount); i++) {
			Rage::Script::GtaThread* Thread = *(Rage::Script::GtaThread**)(Global::Vars::g_ScriptHandlerPool + (i * 8));
			if (IsValidPtr(Thread)) {
				if (!strcmp(Thread->m_Name, Name)) {
					return (uint64_t)Thread->m_Stack;
				}
			}
		}

		return 0;
	}
}