#pragma once
#include "stdafx.hpp"

namespace rage::network {
	struct relayUser {
		char _0x0000[0x20];
		uint64_t rid;
		char _0x0028[0xAC];
		char name[0x20];
	};

	struct relayNetwork {
		char _0x0000[0x8];
		uint32_t ip;
		uint32_t port;
	};

	struct rockstarAccount {
		std::string name;
		uint64_t rockstarId;
		int total;

		rockstarAccount()
			: name(""), rockstarId(0), total(0)
		{}

		rockstarAccount(std::string n, uint64_t rid, int t)
			: name(n), rockstarId(rid), total(t)
		{}
	};

	struct netPlayerInfo {
		uint64_t peerID;// 00
		uint64_t rockstarId;// 8
		//char _0x0010[0x34];
		char _0x0010[0x3C]; //4c
		BYTE externalIp[4];
		int externalPort;
		BYTE internalIp[4];
		int internalPort;
		char _0x0054[0x3C]; // 0098
		uint64_t syncPeerID;
		char _0x0098[0x8];//0xA8
		uint64_t rockstarId2;
		char _0x00A8[0xC];
		char name[0x20];

		uint32_t getIP() {
			return *(uint32_t*)((uint64_t)&externalIp);
		}
	};

	struct netPlayerInfoWrapper {
		char _0[0x150]; // add rax, 150h < gets to the real CPlayerInfo inside of this ptr
		netPlayerInfo netInfo;
	};

	struct localPlayerInfo
	{
		char _0x0000[68]; //0x0000
		uint32_t onlineIP; //0x0044
		char _0x0048[24]; //0x0048
		uint64_t hostToken; //0x0060
		char _0x0068[8]; //0x0068
		uint64_t rockstarID; //0x0070
		char _0x0078[12]; //0x0078
		char name[20]; //0x0084
		char _0x0098[184]; //0x0098
		float swimSpeed; //0x0150
		char _0x0154[24]; //0x0154
		float stealthWalkSpeed; //0x016C
		char _0x0170[136]; //0x0170
		uint32_t frameFlags; //0x01F8
		char _0x01FC[1588]; //0x01FC
		bool isWanted; //0x0830
		char _0x0831[51]; //0x0831
		int32_t wantedLevelDisplay; //0x0864
		int32_t wantedLevel; //0x0868
		char _0x086C[1124]; //0x086C
		float runSpeed; //0x0CD0
		float stamina; //0x0CD4
	}; //Size: 0x0CD8

	struct netGamePlayer {
		virtual void destruct() = 0;
		virtual void init(int natType) = 0;
		virtual void init2(int, int, int) = 0;
		virtual void shutdown() = 0;
		virtual bool isPhysical() = 0;
		virtual const char* getLogName() = 0;
		virtual uint64_t getPeerId() = 0;
		virtual bool isOnSameTeam(netGamePlayer* player) = 0;
		virtual void activeUpdate() = 0;
		virtual bool isHost() = 0;
		virtual void unk1() = 0;
		virtual void unk2() = 0;
		virtual netPlayerInfo* getNetPlayerInfo() = 0;

		char _0x0000[0x10];
		BYTE activePlayerIndex;
		BYTE playerID; // physical index
		char _0x001A[0x10E];
		netPlayerInfoWrapper* playerInfo;
		char _0x0130[0x130];
		char posseName[32];// 0x260
	};

	struct networkEvent {
		char _0x0000[0x28];
		void(*setup)(networkEvent*, uint64_t, netGamePlayer*, uint64_t);
		bool(*execute)(networkEvent*, netGamePlayer*, uint64_t);
	};

	struct networkEventContext {
		networkEvent* vtable;
		uint16_t id;
	};

	struct netObject;
	struct netObjectVirtual {
		char _0x0000[0x38];
		bool(*hasPhysicalEntity)(netObject*);
		char _0x0040[0x70];
		uint64_t(*getPhysicalEntity)(netObject*);
		char _0x00B8[0x80];
		uint32_t* (*getType)(netObject*);
	};

	struct netObject {
		// netObjectVirtual* vtable;
		//char _0x0008[61 - 8]; // remove (- 8) if not using virtual
		//uint8_t playerID; // 0x45

		virtual void ctor() = 0;
		virtual void unk1() = 0;
		virtual void unk2() = 0;
		virtual void unk3() = 0;
		virtual void unk4() = 0;
		virtual void unk5() = 0;
		virtual uint64_t getTree() = 0;
		virtual bool hasPhysicalEntity() = 0;
		virtual void unk6() = 0;
		virtual void unk7() = 0;
		virtual void unk8() = 0;
		virtual void unk9() = 0;
		virtual void unk10() = 0;
		virtual void unk11() = 0;
		virtual void unk12() = 0;
		virtual void unk13() = 0;
		virtual void unk14() = 0;
		virtual void unk15() = 0;
		virtual void unk16() = 0;
		virtual void unk17() = 0;
		virtual void unk18() = 0;
		virtual void unk19() = 0;
		virtual uint64_t getPhysicalEntity() = 0;
		virtual void unk20() = 0;
		virtual void unk21() = 0;
		virtual void unk22() = 0;
		virtual void unk23() = 0;
		virtual void unk24() = 0;
		virtual void unk25() = 0;
		virtual void unk26() = 0;
		virtual void unk27() = 0;
		virtual void unk28() = 0;
		virtual void unk29() = 0;
		virtual void unk30() = 0;
		virtual void unk31() = 0;
		virtual void unk32() = 0;
		virtual void unk33() = 0;
		virtual void unk34() = 0;
		virtual void unk35() = 0;
		virtual void unk36() = 0;
		virtual uint32_t* getTypePtr() = 0;

		char _0x00[0x12 - 0xE];         // 0x0
		bool isremote;            // 0x12
		char _0x14[0x32];         // 0x14
		uint16_t objectType;      // 0x40
		uint16_t objectId;        // 0x42
		char _0x44[0x1];          // 0x44
		int8_t ownerID;           // 0x45
		char _0x46[0x1];          // 0x46
		int8_t nextOwnerID;       // 0x47
		char _0x004D[0xB];        // 0x4d
		uint64_t physical;        // 0x58
	};

	class modelInfo {
	public:
		char _0x0000[0xA3];          // 0x0000
		uint8_t modelType;           // 0x00A3
		char _0x00A4[0x33C];         // 0x00A4
		int32_t vehicleHandlingType; // 0x03E0

		inline int32_t getModelType() { return modelType & 7; }
		bool isModelObject() { return getModelType() == 1; };
		bool isModelVehicle() { return getModelType() == 5; };
		bool isModelPed() { return getModelType() == 6; }

	};

	struct pedFactory {
		char _0x0000[0x8];
		uint64_t ped;
	};

	struct networkPlayerManager {
		char _0x0000[0xE8];
		netGamePlayer* localNetGamePlayer;
		char _0x00F0[0x90];
		netGamePlayer* players[32];
	};

	struct netMsgPlayerInfo {
		char _0x0000[0x4];
		uint32_t rid;

		uint64_t getRid() {
			return (uint64_t)_byteswap_ulong(rid);
		}
	};

	struct netMsgPlayerInfoWrapper {
		char _0x0000[0x60];
		uint32_t ip;
		char _0x0064[0x1C];
		netMsgPlayerInfo* playerInfo;
	};

	struct netMsgPlayer {
		char _0x0000[0x40];
		netMsgPlayerInfoWrapper* playerInfoWrapper;
		uint32_t netMsgIdentifier;
	};
}
