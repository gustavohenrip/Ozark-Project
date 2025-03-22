#pragma once
#include "stdafx.hpp"
#include "rage/invoker/native_invoker.hpp"
#include "rage/types/bit_buffer.hpp"

namespace Menu::Hooks {
	inline uint64_t g_2Take1CrashLocation;
	inline uint64_t g_GraphicCrashTable;

	inline uint64_t(*Originalsub_141358C18)(int, uint64_t);
	uint64_t sub_141358C18(int Index, uint64_t Override);

	inline uint64_t(*Originalsub_1415BBB74)(uint64_t, uint64_t);
	uint64_t sub_1415BBB74(uint64_t Rcx, uint64_t Rdx);

	inline void(*OriginalScriptWorldStateCrash)(uint64_t, uint32_t, uint32_t);
	void ScriptWorldStateCrashHook(uint64_t Rcx, uint32_t Edx, uint32_t R8d);

	inline uint64_t(*Originalsub_14113A75C)(uint64_t, uint64_t);
	uint64_t sub_14113A75C(uint64_t Rcx, uint64_t Rdx);

	inline void(*Originalsub_140A1BDF4)(uint64_t, uint64_t);
	void sub_140A1BDF4(uint64_t Rcx, uint64_t Rdx);

	inline eThreadState(*OriginalScriptThreadTick)(Rage::Script::GtaThread*, uint32_t);
	eThreadState ScriptThreadTickHook(Rage::Script::GtaThread* Thread, uint32_t Opcodes);

	inline uint64_t(*Originalsub_140E72620)(uint64_t);
	uint64_t sub_140E72620(uint64_t Rcx);

	inline void(*Originalsub_140E606D8)(uint64_t, uint64_t);
	void sub_140E606D8(uint64_t Rcx, uint64_t Rdx);

	inline bool(*Originalsub_140AEA62C)(uint64_t, uint64_t, uint64_t, uint32_t, uint64_t);
	bool sub_140AEA62C(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint32_t R9d, uint64_t XMM6);

	inline uint64_t(*Originalsub_140AFB59C)(uint64_t, int, int);
	uint64_t sub_140AFB59C(uint64_t Rcx, int Edx, int R8d);

	inline void(*Originalsub_140E8407C)(uint64_t, uint64_t, uint64_t, uint64_t);
	void sub_140E8407C(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline uint64_t(*Originalsub_140730894)(uint64_t, uint64_t);
	uint64_t sub_140730894(uint64_t Rcx, uint64_t Rdx);

	inline uint64_t(*OriginalAccessNetObj)(uint16_t*);
	uint64_t AccessNetObjHook(uint16_t* Rcx);

	inline uint64_t(*OriginalCutscenePedWrite)(bool);
	uint64_t CutscenePedWriteHook(bool Reset);

	inline uint64_t(*Originalsub_140632DD4)(uint64_t, uint64_t, uint32_t, uint32_t, uint32_t, bool, bool);
	uint64_t sub_140632DD4(uint64_t Rcx, uint64_t Rdx, uint32_t R8d, uint32_t R9d, uint32_t Stack_28, bool Stack_20, bool Stack_18);

	inline bool(*Originalsub_140E7C4E8)(uint64_t, uint64_t, uint64_t, uint32_t, uint64_t, uint64_t, uint64_t, uint64_t, bool, bool);
	bool sub_140E7C4E8(uint64_t Rcx, uint64_t Rdx, uint64_t R8d, uint32_t R9d, uint64_t Stack_210, uint64_t Stack_208, uint64_t Stack_200, uint64_t Stack_1F8, bool Stack_1F0, bool Stack_1E8);

	inline void(*Originalsub_141644B58)(uint64_t, uint64_t, uint64_t);
	void sub_141644B58(uint64_t Rcx, uint64_t Rdx, uint64_t R8);

	inline uint64_t(*Originalsub_1411175B0)(uint64_t);
	uint64_t sub_1411175B0(uint64_t Rcx);

	inline uint64_t(*OriginalRockstarIDWriter)(uint64_t, uint64_t);
	uint64_t RockstarIDWriterHook(uint64_t Rcx, uint64_t Rdx);

	inline void(*OriginalTrailerDetachmentCrash)(uint64_t, uint64_t);
	void TrailerDetachmentCrashHook(uint64_t Trailer, uint64_t Flag);

	inline void(*OriginalUpdateScriptForClone)(uint64_t, Rage::Network::NetObj*, uint32_t, uint8_t);
	void UpdateScriptForCloneHook(uint64_t Rcx, Rage::Network::NetObj* Rdx, uint32_t R8d, uint8_t R9b);

	inline void(*OriginalRockstarIntegrityCheckStart)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckStartHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckEnd)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckEndHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckStart2)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckStart2Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckEnd2)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckEnd2Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckStart3)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckStart3Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckEnd3)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckEnd3Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckStart4)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckStart4Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckEnd4)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckEnd4Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckStart5)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckStart5Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalRockstarIntegrityCheckEnd5)(uint64_t, uint64_t, uint64_t, uint64_t);
	void RockstarIntegrityCheckEnd5Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	int StatGetIntHook(Rage::Invoker::NativeContext* Context);
	void SetCurrentPedWeaponHook(Rage::Invoker::NativeContext* Context);
	void DisableControlActionHook(Rage::Invoker::NativeContext* Context);
	void SetWarningMessage2Hook(Rage::Invoker::NativeContext* Context);
	void TerminateThisThreadHook(Rage::Invoker::NativeContext* Context);

	inline void(*OriginalSyncPedOrientation)(uint64_t, uint64_t);
	void SyncPedOrientationHook(uint64_t Rcx, uint64_t Rdx);

	inline void(*OriginalSyncEntityMatrix)(uint64_t, uint64_t);
	void SyncEntityMatrixHook(uint64_t Rcx, uint64_t Rdx);

	inline void(*OriginalApplyWeatherMeta)(uint64_t);
	void ApplyWeatherMetaHook(uint64_t Meta);

	inline int(*OriginalScProfanityCheckStringStatus)(uint64_t, uint64_t, uint64_t);
	int ScProfanityCheckStringStatusHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8);

	inline int(*OriginalGetChatProfanityStatus)(uint64_t, uint64_t, uint64_t);
	int GetChatProfanityStatusHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8);

	inline uint64_t(*OriginalFormatGameEngineString)(uint64_t, uint64_t, uint32_t, const char*, uint32_t);
	uint64_t FormatGameEngineStringHook(uint64_t Rcx, uint64_t Rdx, uint32_t R8d, const char* R9, uint32_t Stack);

	inline int(*OriginalGetGameEngineStringLineCount)(uint64_t, const char*, float, uint64_t, bool);
	int GetGameEngineStringLineCountHook(uint64_t Rcx, const char* Rdx, float Xmm2, uint64_t R9, bool Stack);

	inline void(*OriginalPhysicalAttachDataNode)(uint64_t, uint64_t);
	void PhysicalAttachDataNodeHook(uint64_t Rcx, uint64_t Rdx);

	inline void(*OriginalPedAttachDataNode)(uint64_t, uint64_t);
	void PedAttachDataNodeHook(uint64_t Rcx, uint64_t Rdx);

	inline bool(*OriginalReadTextMessageBitbuffer)(Rage::Network::TextMessageNetMsg*, uint64_t);
	bool ReadTextMessageBitbufferHook(Rage::Network::TextMessageNetMsg* Identifier, uint64_t Rdx);

	inline bool(*OriginalReadChatMessageBitbuffer)(Rage::Network::GlobalMsgContext*, uint64_t);
	bool ReadChatMessageBitbufferHook(Rage::Network::GlobalMsgContext* Identifier, uint64_t Rdx);

	inline uint64_t(*OriginalReceiveTextMsg)(uint64_t, uint64_t, uint64_t);
	uint64_t ReceiveTextMsgHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8);

	inline uint64_t(*OriginalReceiveChatMsg)(uint64_t, uint64_t, uint64_t);
	uint64_t ReceiveChatMsgHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8);

	inline bool(*OriginalSendAddGamerToSessionCmd)(uint64_t, Rage::Network::NetworkGamerCmd*);
	bool SendAddGamerToSessionCmdHook(uint64_t Rcx, Rage::Network::NetworkGamerCmd* Cmd);

	// inline uint64_t(*OriginalHandleMsgJoinRequest)(uint64_t, Rage::Network::NetworkGamerCmd*, uint64_t, uint16_t);
	// uint64_t HandleMsgJoinRequestHook(uint64_t Rcx, Rage::Network::NetworkGamerCmd* GamerCmd, uint64_t R8, uint16_t R9w);

	inline bool(*OriginalHostHandleJoinRequest)(uint64_t, uint64_t, uint64_t, uint64_t);
	bool HostHandleJoinRequestHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline void(*OriginalHandleNetworkEvent)(uint64_t, Rage::Network::NetGamePlayer*, Rage::Network::NetGamePlayer*, int16_t, int, int, uint64_t, Rage::BitBuffer*);
	void HandleNetworkEventHook(uint64_t This, Rage::Network::NetGamePlayer* Sender, Rage::Network::NetGamePlayer* Receiver, int16_t EventID, int EventIndex, int EventBitset, uint64_t BitbufferSize, Rage::BitBuffer* BitBuffer);

	inline bool(*OriginalReceiveBroadcastVariables)(uint64_t, Rage::Network::NetGamePlayer*, Rage::BitBuffer*, uint32_t, uint16_t);
	bool ReceiveBroadcastVariablesHook(uint64_t This, Rage::Network::NetGamePlayer* Sender, Rage::BitBuffer* BitBuffer, uint32_t BitCount, uint16_t NetworkID);

	inline bool(*OriginalHandleMsgNewScriptHost)(uint64_t Rcx, uint64_t Rdx);
	bool HandleMsgNewScriptHostHook(uint64_t Rcx, uint64_t Rdx);

	inline bool(*OriginalHandleMsgKickPlayer)(uint64_t, uint64_t);
	bool HandleMsgKickPlayerHook(uint64_t Rcx, uint64_t Rdx);

	inline bool(*OriginalWriteUintToDataWriterBuffer)(Rage::BitBufferSyncInstance*, uint32_t*, uint32_t, uint32_t);
	bool WriteUintToDataWriterBufferHook(Rage::BitBufferSyncInstance* This, uint32_t* Input, uint32_t BitsToWrite, uint32_t R9d);

	inline bool(*OriginalWriteArrayToDataWriterBuffer)(Rage::BitBufferSyncInstance*, uint32_t*, uint32_t, uint32_t);
	bool WriteArrayToDataWriterBufferHook(Rage::BitBufferSyncInstance* Instance, uint32_t* Input, uint32_t BitsToWrite, uint32_t R9d);

	inline bool(*OriginalReceiveCloneCreate)(uint64_t, Rage::Network::NetGamePlayer*, uint64_t, uint16_t, uint16_t, uint16_t, Rage::BitBuffer*, int);
	bool ReceiveCloneCreateHook(uint64_t This, Rage::Network::NetGamePlayer* NetGamePlayer, uint64_t r8, uint16_t CloneType, uint16_t NetworkID, uint16_t Flag, Rage::BitBuffer* BitBuffer, int Timestamp);

	inline uint64_t(*OriginalReceiveCloneSync)(uint64_t, Rage::Network::NetGamePlayer*, uint64_t, uint16_t, uint16_t, Rage::BitBuffer*, uint16_t, uint32_t);
	uint64_t ReceiveCloneSyncHook(uint64_t This, Rage::Network::NetGamePlayer* NetGamePlayer, uint64_t r8, uint16_t CloneType, uint16_t NetworkID, Rage::BitBuffer* BitBuffer, uint16_t SequenceID, uint32_t Timestamp);

	inline bool(*OriginalReceiveCloneRemove)(uint64_t, Rage::Network::NetGamePlayer*, uint64_t, uint16_t, uint64_t);
	bool ReceiveCloneRemoveHook(uint64_t This, Rage::Network::NetGamePlayer* NetGamePlayer, uint64_t R8, uint16_t NetworkID, uint64_t Stack);

	inline void(*OriginalPackCloneCreate)(uint64_t, Rage::Network::NetObj*, Rage::Network::NetGamePlayer*, uint64_t);
	void PackCloneCreateHook(uint64_t This, Rage::Network::NetObj* Object, Rage::Network::NetGamePlayer* Player, uint64_t SyncData);

	inline Math::Vector4<float>*(*OrignalSyncPedCoords)(Rage::Network::NetObj*, Math::Vector4<float>*);
	Math::Vector4<float>* SyncPedCoordsHook(Rage::Network::NetObj* Object, Math::Vector4<float>* Out);

	inline void(*OriginalUnbindAttachmentExtension)(uint64_t, uint8_t, uint8_t, uint64_t);
	void UnbindAttachmentExtensionHook(uint64_t Target, uint8_t DL, uint8_t R8b, uint64_t Override);

	inline uint32_t(*OriginalGetHashKey)(const char*, uint32_t);
	uint32_t GetHashKeyHook(const char* Text, uint32_t Edx);

	inline bool(*OriginalSyncPlayerModel)(uint64_t);
	bool SyncPlayerModelHook(uint64_t PlayerAppearanceDataNode);

	inline uint64_t(*OriginalHandleNetworkGameEvent)(uint64_t);
	uint64_t HandleNetworkGameEventHook(uint64_t This);

	inline bool(*OriginalReadSessionInfoResponse)(uint64_t);
	bool ReadSessionInfoResponseHook(uint64_t Rcx);

	inline void(*OriginalApplyPlayerPhysicalIndex)(uint64_t, Rage::Network::NetGamePlayer*, uint8_t);
	void ApplyPlayerPhysicalIndexHook(uint64_t Rcx, Rage::Network::NetGamePlayer* NetGamePlayer, uint8_t R8b);

	inline bool(*OriginalHandlePresenceEvent)(uint64_t, uint64_t, const char**, uint64_t);
	bool HandlePresenceEventHook(uint64_t Rcx, uint64_t Rdx, const char** R8, uint64_t R9);

	inline bool(*OriginalPrepareRequestUri)(uint64_t, uint64_t);
	bool PrepareRequestUriHook(uint64_t Rcx, uint64_t Rdx);
	
	inline bool(*OriginalParseNewsStoryFromJson)(uint64_t);
	bool ParseNewsStoryFromJsonHook(uint64_t This);

	inline bool(*OriginalSendJoinRequest)(Rage::Network::JoinRequest*, uint64_t, int, uint32_t*);
	bool SendJoinRequestHook(Rage::Network::JoinRequest* Rcx, uint64_t Rdx, int R8d, uint32_t* R9d);

	inline void(*OriginalPlayerGamerDataNode)(Rage::Network::PlayerGamerDataNode*, Rage::BitBufferSyncInstance*);
	void PlayerGamerDataNodeHook(Rage::Network::PlayerGamerDataNode* Data, Rage::BitBufferSyncInstance* Buffer);

	inline void(*OriginalSendGameMessage)(uint64_t, int, Rage::Network::GlobalMsgContext*, uint64_t, uint64_t);
	void SendGameMessageHook(uint64_t Manager, int PlayerKey, Rage::Network::GlobalMsgContext* Context, uint64_t Unk1, uint64_t Unk2);

	inline bool(*OriginalHasRosPrivilege)(uint64_t, int);
	bool HasRosPrivilegeHook(uint64_t Table, int Type);

	inline FARPROC(*OriginalGetProcAddress)(HMODULE, LPCSTR);
	FARPROC GetProcAddressHook(HMODULE moduleHandle, LPCSTR functionName);

	inline bool(*OriginalGetPeerToken)(uint64_t*);
	bool GetPeerTokenHook(uint64_t* Out);

	inline const char*(*OriginalGetGxtLabel)(uint64_t, const char*);
	const char* GetGxtLabelHook(uint64_t Table, const char* Label);

	inline void(*OriginalPedMovementDataNode)(uint64_t, uint64_t);
	void PedMovementDataNodeHook(uint64_t Rcx, uint64_t Rdx);

	inline bool(*OriginalGetScItemString)(uint64_t, uint64_t, const char*, char*);
	bool GetScItemStringHook(uint64_t This, uint64_t Table, const char* Type, char* Buffer);

	inline bool(*OriginalPackCloneSync)(uint64_t, Rage::Network::NetObj*, Rage::Network::NetGamePlayer*);
	bool PackCloneSyncHook(uint64_t This, Rage::Network::NetObj* NetObj, Rage::Network::NetGamePlayer* NetGamePlayer);

	inline bool(*OriginalWriteNetMsgIdentifier)(uint64_t, uint64_t, int);
	bool WriteNetMsgIdentifierHook(uint64_t Rcx, uint64_t Rdx, int R8d);

	inline bool(*OriginalSyncRockstarID)(uint64_t, uint64_t, uint32_t, uint64_t);
	bool SyncRockstarIDHook(uint64_t Rcx, uint64_t Rdx, uint32_t R8d, uint64_t R9);

	inline bool(*OriginalWriteUnsignedLongLongToBitBuffer)(uint64_t, uint64_t, uint64_t);
	bool WriteUnsignedLongLongToBitBufferHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8);

	inline void(*OriginalProcessRelayUsage)(uint8_t, uint64_t, Rage::Network::EventAddedGamer*, uint32_t);
	void ProcessRelayUsageHook(uint8_t Cl, uint64_t Rdx, Rage::Network::EventAddedGamer* Gamer, uint32_t R9d);

	inline void(*OriginalMessageBoxTermination)(uint32_t, uint32_t);
	void MessageBoxTerminationHook(uint32_t Ecx, uint32_t Edx);

	inline bool(*OriginalReadMatchmakingResponse)(uint64_t, uint64_t, uint64_t, uint64_t);
	bool ReadMatchmakingResponseHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9);

	inline bool(*OriginalWriteMatchmakingRequest)(uint64_t, uint32_t, uint32_t, uint64_t, uint32_t, uint64_t, uint64_t);
	bool WriteMatchmakingRequestHook(uint64_t Rcx, uint32_t RsIndex, uint32_t AvailableSlots, uint64_t FilterInfo, uint32_t MaxResults, uint64_t Stack_20, uint64_t Stack_18);

	inline bool(*OriginalWriteEncryptedStringToRequest)(uint64_t, const char*, char*, bool, uint64_t, uint64_t);
	bool WriteEncryptedStringToRequestHook(uint64_t Rcx, const char* Name, char* Value, bool Encrypted, uint64_t Stack_3C8, uint64_t Stack_3C0);

	inline bool(*OriginalHostDesyncPlayerRequest)(uint64_t, uint64_t, Rage::Network::ListKickPeers*, uint32_t);
	bool HostDesyncPlayerRequestHook(uint64_t Rcx, uint64_t Rdx, Rage::Network::ListKickPeers* R8, uint32_t R9d);

	inline uint64_t(*OriginalHandleNegotiationMsg)(uint64_t, uint64_t, uint64_t);
	uint64_t HandleNegotiationMsgHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8);

	inline bool(*OriginalReadBlacklistMessage)(uint64_t, uint64_t);
	bool ReadBlacklistMessageHook(uint64_t Rcx, uint64_t Rdx);

	inline bool(*OriginalReadUnsignedIntFromBitBuffer)(Rage::BitBufferSyncInstance*, uint32_t*, int);
	bool ReadUnsignedIntFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint32_t* Output, int Count);

	inline bool(*OriginalReadUnsignedShortFromBitBuffer)(Rage::BitBufferSyncInstance*, uint16_t*, int);
	bool ReadUnsignedShortFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint16_t* Output, int Count);

	inline bool(*OriginalReadUnsignedCharFromBitBuffer)(Rage::BitBufferSyncInstance*, uint8_t*, int);
	bool ReadUnsignedCharFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint8_t* Output, int Count);

	inline bool(*OriginalReadIntFromBitBuffer)(Rage::BitBufferSyncInstance*, int*, int);
	bool ReadIntFromBitBufferHook(Rage::BitBufferSyncInstance* This, int* Output, int Count);

	inline bool(*OriginalReadShortFromBitBuffer)(Rage::BitBufferSyncInstance*, short*, int);
	bool ReadShortFromBitBufferHook(Rage::BitBufferSyncInstance* This, short* Output, int Count);

	inline bool(*OriginalReadCharFromBitBuffer)(Rage::BitBufferSyncInstance*, char*, int);
	bool ReadCharFromBitBufferHook(Rage::BitBufferSyncInstance* This, char* Output, int Count);

	inline bool(*OriginalReadBoolFromBitBuffer)(Rage::BitBufferSyncInstance*, bool*);
	bool ReadBoolFromBitBufferHook(Rage::BitBufferSyncInstance* This, bool* Output);

	inline bool(*OriginalReadLongLongFromBitBuffer)(Rage::BitBufferSyncInstance*, long long*, int);
	bool ReadLongLongFromBitBufferHook(Rage::BitBufferSyncInstance* This, long long* Output, int Count);

	inline bool(*OriginalReadUnsignedLongLongFromBitBuffer)(Rage::BitBufferSyncInstance*, uint64_t*, int);
	bool ReadUnsignedLongLongFromBitBufferHook(Rage::BitBufferSyncInstance* This, uint64_t* Output, int Count);

	inline bool(*OriginalReadFloatFromBitBuffer)(Rage::BitBufferSyncInstance*, float*, float, int);
	bool ReadFloatFromBitBufferHook(Rage::BitBufferSyncInstance* This, float* Output, float Mod, int Count);

	inline bool(*OriginalReadUnsignedFloatFromBitBuffer)(Rage::BitBufferSyncInstance*, float*, float, int);
	bool ReadUnsignedFloatFromBitBufferHook(Rage::BitBufferSyncInstance* This, float* Output, float Mod, int Count);

	inline bool(*OriginalReadNetworkIdFromBitBuffer)(Rage::BitBufferSyncInstance*, short*);
	bool ReadNetworkIdFromBitBufferHook(Rage::BitBufferSyncInstance* This, short* Output);

	inline bool(*OriginalReadArrayFromBitBuffer)(Rage::BitBufferSyncInstance*, int*, int, int);
	bool ReadArrayFromBitBufferHook(Rage::BitBufferSyncInstance* This, int* Output, int Count, int Unk);

	inline bool(*OriginalReadStringFromBitBuffer)(Rage::BitBufferSyncInstance*, char*, int);
	bool ReadStringFromBitBufferHook(Rage::BitBufferSyncInstance* This, char* Output, int Length);

	inline void(*OriginalReadNodeData)(Rage::Network::SyncTree*, uint32_t, uint32_t, Rage::BitBuffer*, uint64_t);
	void ReadNodeDataHook(Rage::Network::SyncTree* Tree, uint32_t Bit, uint32_t R8d, Rage::BitBuffer* BitBuffer, uint64_t LoggingInterface);

	inline bool(*OriginalWritePlayerGameStateDataNode)(uint64_t, uint64_t);
	bool WritePlayerGameStateDataNodeHook(uint64_t Rcx, uint64_t Rdx);

	inline void(*OriginalWritePedHealthDataNode)(uint64_t, uint64_t);
	void WritePedHealthDataNodeHook(uint64_t Rcx, uint64_t Rdx);

	inline bool(*OriginalShootBullet)(uint64_t, uint64_t, float*, float*);
	bool ShootBulletHook(uint64_t WeaponInstance, uint64_t Rdx, float* StartPos, float* TargetPos);

	inline uint64_t(*OriginalGetRenderTexture)(uint64_t);
	uint64_t GetRenderTextureHook(uint64_t Table);

	inline bool(*OriginalRenderScriptTextures)(uint64_t, uint32_t, uint32_t);
	bool RenderScriptTexturesHook(uint64_t Table, uint32_t Edx, uint32_t R8d);

	inline void(*OriginalUpdatePlayerScriptStatus)(Rage::Script::GameScriptHandlerNetworkComponent*, uint64_t, uint64_t);
	void UpdatePlayerScriptStatusHook(Rage::Script::GameScriptHandlerNetworkComponent* Component, uint64_t Rdx, uint64_t R8);

	inline bool(*OriginalParseDataNode)(Rage::Network::DataNode*, uint32_t, uint64_t, Rage::Network::NetObj*, Rage::BitBuffer*, uint32_t, uint64_t, bool, uint64_t);
	bool ParseDataNodeHook(Rage::Network::DataNode* Node, uint32_t Edx, uint64_t R8, Rage::Network::NetObj* Object, Rage::BitBuffer* Buffer, uint32_t BitSize, uint64_t Unk1, bool Unk2, uint64_t Unk3);

	inline bool(*OriginalParseDataNode2)(Rage::Network::DataNode*, Rage::Network::NetObj*);
	bool ParseDataNode2Hook(Rage::Network::DataNode* Node, Rage::Network::NetObj* Object);
}