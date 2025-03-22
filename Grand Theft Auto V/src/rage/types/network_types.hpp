#pragma once
#include "stdafx.hpp"
#include "utils/math.hpp"
#include "base_types.hpp"
#include "bit_buffer.hpp"

#pragma pack(push)
#pragma pack(1)
namespace Rage::Network {
	struct PlayerInfo;
	struct CameraManagerAngles;
	struct CameraAngles;
	struct PlayerAngles;
	struct NetObj;

	struct NetShopQueue {
		char _0x0000[32];
		void* m_First;
		void* m_Last;
		char _0x0030[9];
		bool m_IsBusy1;
		char _0x003A[14];
		bool m_IsBusy2;
		char _0x0049[7];
		bool m_IsBusy3;
	};

	struct GsSession {
		/*char _0x0000[0x8];
		uint64_t m_PeerToken;
		char _0x0010[0x38];
		uint64_t m_RockstarID;
		char _0x0050[0x20];*/

		// 0x18 = netmsgidentifier
		char _0x0000[0x8];
		uint64_t m_PeerToken;
		char _0x0010[0x8];
		uint64_t m_RockstarID;
		char _0x0020[0x50];
	};

	struct JoinRequest {
		char _0x0000[0x24];
		int m_DeveloperFlags;
	};

	struct PlayerGamerDataNode {
		char _0x0000[0xC8];
		int m_CrewID;
		char _0x00cc[0x12];
		char m_CrewName[0x19];
		char m_CrewAbbr[0x5];
		char m_CrewMotto[0x4C];
		char m_CrewRank[0x18];
	};

	struct GlobalMsgContext {
		char m_Message[0x100];
		uint64_t m_SenderChatToken;
		bool m_Flag0IsNeg1;
	};

	struct PlayerPeer {
		int m_PlayerKey;
		int m_SecondaryKey;
		char _0x0008[0x4];
		uint32_t m_ExternalIP;
		char _0x0010[0x20];
		uint8_t m_Flag0;
		char _0x0031[0xF];
		uint8_t m_Bitset;
	};

	struct GlobalChatRaw {
		char _0x0000[0x180];
		int m_Flag0;
		char _0x0184[0x1D24];
		PlayerPeer* m_PlayerPeers[0x20];
		int m_PlayerCount;
		char _0x1eac[0xC];
		uint64_t m_ManagerPtr;
		int m_Flag1;
		int m_Flag2;
		char _0x1ec8[0x30];
		uint8_t m_Bitset;
	};

	struct RsInfo {
		char m_ScsTicket[0x100];
		char _0x0100[0x190];
		uint64_t m_Peer;
		char _0x0298[0xA7];
		char m_Email[0x50];
		char _0x038f[0x20];
		char m_Name[0x20];
		char _0x03cf[0x29];
		uint32_t m_RockstarID;
	};

	struct NetworkCrewHandle {
		/*int m_Handle;
		char _0x0004[0x4];
		char m_CrewName[0x14];
		char _0x0000[0x6C];
		char m_CrewNameAbbr[0x4];
		char _0x0001[0x50];

		NetworkCrewHandle() {
			m_Handle = 0;
		}*/

		int m_Handle; // crew ID
		char _0x0004[0x4];
		char m_CrewName[0x18];
		char _0x0020[0x68];
		char m_CrewAbbr[0x4];
		char _0x008c[0x24];
		char m_CrewRank[0x18];
		char _0x00C8[0x14];
	};

	struct NetworkHandle {
		int m_Handle;
		char _0x0[48];
	};

	struct NetMsgIdentifier {
		uint64_t m_RockstarID;
		uint8_t m_Type;
		char _0x0009[0xB];
	};

	struct RockstarIdentifier {
		uint64_t m_RockstarID;
		uint32_t m_Type;
		uint32_t m_SubType;
	};

	struct TextMessageNetMsg {
		char m_Message[0x100];
		NetMsgIdentifier m_Sender;
	};

	struct NetworkGamerCmd {
		char _0x0000[0x68];
		uint64_t m_HostToken;
		uint64_t m_ChatToken;
		NetMsgIdentifier m_Identifier;
		char m_Name[0x14];
		char _0x0080[0x8];
		uint32_t m_MsgIP; // 0xA8
		char _0x008C[0x21C];
		NetMsgIdentifier m_PlayerIdentifiers[0x20];

		void SetIP(int Index, uint8_t IP) {
			uint8_t* Val = (uint8_t*)&m_MsgIP;
			Val[Index] = IP;
		}
	};

	struct GameCameraAngles {
		CameraManagerAngles* m_CameraManagerAngles;
		char _0x0008[56];
	};

	struct CameraManagerAngles {
		CameraAngles* m_CameraAngles;
	};

	struct CameraAngles {
		char _0x0000[0x3C0];
		PlayerAngles* m_FPSPlayerAngles;
		char _0x03C8[0x10];
		uint64_t m_Unknown;
	};

	struct PlayerAngles {
		char _0x0000[0x40];
		Math::Vector3_<float> m_ViewAngles;
		char _0x0004C[0x14];
		Math::Vector3_<float> m_CrosshairLocation;
	};

	struct PlayerInfo {
		/*char _0x0000[0x34];
		uint32_t m_InternalIP;
		uint16_t m_InternalPort;
		char _0x003A[0x2];
		uint32_t m_RelayIP;
		uint16_t m_RelayPort;
		char _0x0042[0x2];
		uint32_t m_ExternalIP;
		uint16_t m_ExternalPort;
		char _0x004A[0xE];
		uint64_t m_RockstarID;
		uint64_t m_HostToken;
		uint64_t m_ChatToken;
		NetMsgIdentifier m_Identifier; // 0x14 in size
		char m_Name[0x14];
		char _0x0098[0xB8];
		float m_SwimSpeed;
		float m_RunSpeed;
		char _0x0158[0x78];
		void* m_PlayerPed;
		char _0x01D8[0x20];
		uint32_t m_FrameFlags;
		char _0x01FC[0x618];
		uint8_t m_WantedLevel1;
		char _0x0815[0x3];
		uint8_t m_WantedLevel2;
		char _0x0819[0x46F];
		float m_MeleeDamage;
		char _0x0C8C[0x224];
		char _new[0x120];
		uint32_t m_AimFlag;
		char _0x0228[0xC8];*/

		char _0x0000[0x20];
		uint64_t m_HostToken;
		uint64_t m_RockstarID;
		char _0x0030[0x3C];
		uint32_t m_ExternalIP;
		uint32_t m_ExternalPort;
		uint32_t m_InternalIP;
		uint32_t m_InternalPort;
		char _0x007C[0xC];
		uint64_t m_ChatToken;
		NetMsgIdentifier m_Identifier;
		char m_Name[0x14]; // A4
		char _0x00B8[0xB4];
		float m_SwimSpeed; // 16C
		float m_RunSpeed; // 170
		char _0x0174[0xA4];
		uint32_t m_FrameFlags; // 218
		char _0x021C[0xDD4];
		uint32_t m_AimFlag; // FF0



		bool IsFreeAiming() {
			return m_AimFlag >> 6 & 1;
		}

		void SetIP(int Index, uint8_t IP) {
			uint8_t* Val = (uint8_t*)&m_ExternalIP;
			Val[Index] = IP;
		}

		void SetRunSpeed(float Speed) {
			*(float*)((uint64_t)this + 0xCF0) = Speed;
		}
	};

	struct NetGamePlayer {
		char _0x0000[0x21];
		uint8_t m_ID;
		char _0x0022[0x7E];
		PlayerInfo* m_PlayerInfo; //0xA0
		char _0x00b8[0xF1];
		bool m_IsRockstarDeveloper; // 1A9
		bool m_IsRockstarQA; // 1AA
		bool m_IsCheater; // 1AB
		char _0x01AB[0x94];
		uint8_t m_ReportBits; // 240
		char _0x0241[0x3];
		int* m_ReportReasons; // 244
		char _0x024C[0x48];
		int m_MaxReportReasons; // 294

		bool IsReportedForReason(int Category, int Reason) {
			bool ValidBit = ((1 << (Category & 7)) & *(uint8_t*)((uint64_t)&m_ReportBits + (Category >> 3)));
			if (ValidBit) {
				if ((Category - 4) <= 1) {
					if (Reason) {
						ValidBit = false;

						if (m_MaxReportReasons) {
							int Increment = 0;
							int* Reasons = m_ReportReasons;

							while (Reason != *Reasons) {
								Increment++;
								Reasons++;

								if (Increment >= m_MaxReportReasons) {
									return ValidBit;
								}
							}

							ValidBit = true;
						}
					}
				}
			}

			return ValidBit;
		}

		const char* GetCrewMotto() {
			return (const char*)((uint64_t)this + 0x104);
		}

		const char* GetCrewName() {
			return (const char*)((uint64_t)this + 0xE6);
		}

		const char* GetCrewTag() {
			return (const char*)((uint64_t)this + 0xFF);
		}

		const char* GetCrewRank() {
			return (const char*)((uint64_t)this + 0x150);
		}

		int GetCrewID() {
			return *(int*)((uint64_t)this + 0x148);
		}
	};

	struct NetObj {
		void* m_VTable;
		eNetObjectTypes m_Type;
		int16_t m_NetworkID;
		char _0x000C[0x14];
		int8_t m_PlayerSyncIntervals[0x20];
		char _0x0040[0x9];
		uint8_t m_Owner; // 0x49
		char _0x004A[0x6];
		uint64_t m_Entity;
		char _0x0058[0x4C0];
		int16_t m_SpectatingNetworkID;
	};

	struct DataNodeCommonOperations {
		virtual ~DataNodeCommonOperations();
		virtual void Reader();
		virtual void Writer();
		virtual void Unk();
		virtual uint32_t SizeCalculator();
		virtual void Logger();
		virtual void Unk2();

		uint64_t m_Data;
	};

	struct DataNode {
		virtual ~DataNode() = 0;
		virtual bool IsDataNode() = 0;
		virtual bool IsParentNode() = 0;
		virtual bool Unk() = 0;
		virtual bool Unk1() = 0;
		virtual bool Unk2() = 0;
		virtual bool Write() = 0;
		virtual bool Read(uint32_t Edx, uint32_t R8d, Rage::BitBuffer* R9, uint64_t Stack_18) = 0;
		virtual uint32_t CalculateChildNodeSize(uint32_t Edx, uint32_t R8d, bool R9b) = 0;

		DataNode* m_Next; //0x0008
		DataNode* m_Previous; //0x0010
		uint64_t m_SyncTree; //0x0018
		DataNode* m_Parent; //0x0020
		uint32_t m_Flag; //0x0028
		uint32_t m_Flag3; //0x002C
		uint32_t m_Flag2; //0x0030
		char _0x0034[0x4]; //0x0034
		DataNode* m_FirstChild; //0x0038
		DataNode* m_LastChild; //0x0040
		uint64_t m_UnkData; //0x0048
		uint32_t m_ChildCount; //0x0050
		char _0x0054[0x54];
		DataNodeCommonOperations* m_DataNodeCommonOperations; // 0xA8

		bool IsWriting() {
			return *(bool*)((uint64_t)this + 0x40);
		}
	};

	struct Friends {
		char _0x0000[128]; //0x0000
		char m_Name[32]; //0x0080
		char _0x00A0[24]; //0x00A0
		uint64_t m_RockstarID; //0x00B8
		char _0x00C0[4]; //0x00C0
		int m_State; //0x00C4
		char _0x00C8[312]; //0x00C8
	};

	struct EventAddedGamer {
		/*char _0x0000[0x44];         // 0x0000
		uint32_t m_OldIP;			// 0x0044
		char _0x0048[0x10];         // 0x0048
		uint64_t m_RockstarID;      // 0x0058
		char _0x0060[0x24];         // 0x0060
		char m_Name[0x14];          // 0x0084
		char _0x0098[0x8];			// 0x0098
		uint32_t m_ExternalIP;      // 0x00A0*/

		char _0x0000[0x6C];
		uint32_t m_ExternalIP;		// 0x006C
		char _0x0070[0x20];			// 0x0070
		uint64_t m_RockstarID;		// 0x0090
		char _0x00098[0xC];			// 0x0098
		char m_Name[0x14];			// 0x00A4
	};

	struct BlacklistedPlayer {
		NetMsgIdentifier m_Player;
		uint32_t m_Timestamp;
		BlacklistedPlayer* m_Next;
		BlacklistedPlayer* m_Last;
	};

	struct NetworkBaseConfig {
		char _0x0000[0x19928]; // 0x184E8
		NetMsgIdentifier m_LastBlacklisted;
		uint32_t m_LastBlacklistedTimestamp;
		char _0x184b0[0x280];
		BlacklistedPlayer* m_FirstBlacklistedPlayer;
		BlacklistedPlayer* m_LastBlacklistedPlayer;

		void SetNetworkSessionWasInvited(bool Invited) {
			*(bool*)((uint64_t)this + 0x86A5) = Invited;
		}
	};

	struct KickPeer {
		uint64_t m_Peer;
		char _0x0008[5];
		bool m_Kicked;
		char _0x000E[2];
	};

	struct ListKickPeers {
		KickPeer m_Peers[0x20];
	};

	struct SyncTreeDataNodeInfo {
		char _0x0000[0x40];
		uint8_t m_Flag;
	};

	struct SyncTree {
		void* m_VTable;
		NetObj* m_Object;
		DataNode* m_ParentNode;
		char _0x0018[0x8];
		uint32_t m_DataNodeCount;
		char _0x0024[0x8];
		SyncTreeDataNodeInfo* m_NodeData;
	};

	struct GameInvite {
		char _0x0000[8]; //0x0000
		GsSession gssession_start; //0x0008
		uint64_t h; //0x0058						 - now at 0x78			
		uint64_t h2; //0x0060
		char s[125]; //0x0068						 - now at 0x88
		char n[17]; //0x00B5						 - now at 0x105
		char _0x00C6[2]; //0x00C6
		uint32_t gm; //0x00C8						 - now at 0x118
		char c[23]; //0x00CC						 - now at 0x11C
		char _0x00E3[5]; //0x00E3
		uint64_t cc; //0x00E8						 - now at 0x138
		uint64_t cc2;
		char mid[30]; //0x00F8						 - now at 0x148
		char _0x0116[2]; //0x0116
		uint32_t if_; //0x0118
		uint32_t it; //0x011C
		uint32_t l; //0x0120						 - now at 0x170
		uint32_t p; //0x0124
		uint32_t f; //0x0128
		uint32_t cr; //0x012C						 - now at 0x17C
		uint32_t u; //0x0130						 - now at 0x180
		uint32_t d; //0x0134						 - now at 0x184
		uint32_t jq; //0x0138
	};
}
#pragma pack(pop)