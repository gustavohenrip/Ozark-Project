#include "helpers.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/player_manager.hpp"
#include "utils/utils.hpp"
#include "utils/log.hpp"
#include "utils/fiber_pool.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/caller.hpp"
#include <mutex>

namespace Menu::Helpers {
	bool IsRockstarIP(uint32_t ExternalIP) {
		return (ExternalIP >= 0x68FF6800 && ExternalIP <= 0x68FF6BFF)
			|| (ExternalIP >= 0xC051F000 && ExternalIP <= 0xC051F7FF)
			|| (ExternalIP >= 0x8B8AE000 && ExternalIP <= 0x8B8AE5FF)
			|| (ExternalIP >= 0x8B8AE700 && ExternalIP <= 0x8B8AF6FF)
			|| (ExternalIP >= 0x8B8AFF00 && ExternalIP <= 0x8B8AFFFF)
			|| (ExternalIP >= 0xC7A83D00 && ExternalIP <= 0xC7A83EFF)
			|| (ExternalIP >= 0xC7E5E000 && ExternalIP <= 0xC7E5E0FF)
			|| (ExternalIP >= 0xD1CCF000 && ExternalIP <= 0xD1CCF6FF)
			|| (ExternalIP >= 0xD1CCF800 && ExternalIP <= 0xD1CCFAFF)
			|| (ExternalIP >= 0xD1CCFC00 && ExternalIP <= 0xD1CCFCFF)
			|| (ExternalIP >= 0xB9384000 && ExternalIP <= 0xB93843FF);
	};

	Menu::PlayerVars& GetPlayerFromPeerMac(uint32_t Peer) {
		int ID = -1;
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			if (IsValidPtr(Player.m_NetGamePlayer)) {
				if (IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) {
					if ((uint32_t)((Player.m_NetGamePlayer->m_PlayerInfo->m_HostToken >> 32) & 0xFFFFFFFF) == Peer) {
						ID = Player.m_ID;
					}
				}
			}
		}, true, true);

		static PlayerVars Static;
		if (ID == -1) return Static;
		return Menu::GetPlayer(ID);
	}

	Menu::PlayerVars& GetPlayerFromIP(uint32_t IP) {
		int ID = -1;
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			if (IsValidPtr(Player.m_NetGamePlayer)) {
				if (IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) {
					if (Player.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP == IP) {
						ID = Player.m_ID;
					}
				}
			}
		}, true, true);

		static PlayerVars Static;
		if (ID == -1) return Static;
		return Menu::GetPlayer(ID);
	}

	Menu::PlayerVars& GetPlayerFromPeer(uint64_t Peer) {
		int ID = -1;
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			if (IsValidPtr(Player.m_NetGamePlayer)) {
				if (IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) {
					if (Player.m_NetGamePlayer->m_PlayerInfo->m_HostToken == Peer) {
						ID = Player.m_ID;
					}
				}
			}
			}, true, true);

		static PlayerVars Static;
		if (ID == -1) return Static;
		return Menu::GetPlayer(ID);
	}

	Menu::PlayerVars& GetPlayerFromRID(uint64_t RID) {
		int ID = -1;
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			if (IsValidPtr(Player.m_NetGamePlayer)) {
				if (IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) {
					if (Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID == RID) {
						ID = Player.m_ID;
					}
				}
			}
		}, true, true);

		static PlayerVars Static;
		if (ID == -1) return Static;
		return Menu::GetPlayer(ID);
	}

	std::string GetVtableName(uint64_t Table) {
		uint64_t ObjectLocator = *(uint64_t*)(Table - 8);
		if (ObjectLocator) {
			uint32_t Offset = *(uint32_t*)(ObjectLocator + 0xC);
			if (Offset) {
				const char* String = (const char*)(Global::Vars::g_GameAddress + Offset + 0x10);
				if (String) {
					std::string Str(String);
					Str = Str.substr(4, Str.length());
					std::size_t At = Str.find_first_of('@');

					if (At > 0 && At < 0xFFFF) {
						Str = Str.substr(0, At);
					}

					return Str;
				}

				return String;
			} else {
				return "no offset";
			}
		} else {
			return "no locator";
		}

		return "null";
	}

	void GetNodeCount(Rage::Network::DataNode* Node, int* TotalNodes, int* TotalDataNodes) {
		if (IsValidPtr(Node)) {
			Rage::Network::DataNode* Current = Node;
			while (IsValidPtr(Current) && IsValidPtr((*(uint64_t*)((uint64_t)Current)))) {
				if (Current->IsDataNode()) {
					if (Current->m_DataNodeCommonOperations->m_Data) {
						*TotalDataNodes = *TotalDataNodes + 1;
					}
				}

				*TotalNodes = *TotalNodes + 1;

				if (Current->m_ChildCount > 0) {
					Rage::Network::DataNode* Children = Current->m_FirstChild;
					if (IsValidPtr(Children)) {
						GetNodeCount(Children, TotalNodes, TotalDataNodes);
					}
				}

				Current = Current->m_Next;
			}
		}
	}

	void GetModelHashFromNode(Rage::Network::DataNode* Node, Rage::BitBuffer* BitBuffer, uint32_t* OutModel, std::string* OutNameDebugOnly) {
		if (IsValidPtr(Node)) {
			Rage::Network::DataNode* Current = Node;
			while (IsValidPtr(Current) && IsValidPtr((*(uint64_t*)((uint64_t)Current)))) {
				if (Current->IsDataNode()) {
					if (Current->m_DataNodeCommonOperations->m_Data) {
						std::string ClassName = GetVtableName(*(uint64_t*)((uint64_t)Current));
						if (!ClassName.empty()) {
							if (strstr(ClassName.c_str(), "CreationDataNode") && !strstr(ClassName.c_str(), "CAutomobileCreationDataNode")) {
								*OutNameDebugOnly = ClassName;
								uint32_t HashedName = Native::GetHashKey(ClassName.c_str());
								
								switch (HashedName) {
									// CDoorCreationDataNode
									case 0xc06ec0f2: {
										BitBuffer->ReadUint(OutModel, 0x20);
										return;
									}

									// CObjectCreationDataNode
									case 0x39862f6a: {
										uint32_t Unk = 0;
										BitBuffer->ReadUint(&Unk, 5);

										if ((Unk & 0xFFFFFFFD) != 0) {
											BitBuffer->ReadUint(OutModel, 0x20);
										}

										return;
									}

									// CPedCreationDataNode
									case 0x6105c796: {
										bool Temp = false;
										BitBuffer->ReadBool(&Temp);
										BitBuffer->ReadBool(&Temp);
										BitBuffer->ReadUint(OutModel, 4);
										BitBuffer->ReadUint(OutModel, 0x20);
										return;
									}

									// CPickupCreationDataNode
									case 0x37150d85: {
										// whole lotta ass

										bool Unk = false;
										BitBuffer->ReadBool(&Unk);

										uint32_t Hash1 = 0;
										uint32_t Hash2 = 0;
										uint8_t UnkData[0x200];

										if (Unk) {
											Caller::Call<int>(Global::Vars::g_ReadPickupFromBitBuffer, UnkData, BitBuffer);
										} else {
											BitBuffer->ReadUint(&Hash1, 0x20);
										}

										Unk = *(uint32_t*)(&UnkData[84]) > 0;
										BitBuffer->ReadBool(&Unk);

										if (Unk) {
											BitBuffer->ReadUint(&Hash2, 0x20);
										}

										*OutModel = *(uint32_t*)(&UnkData[88]);

										Unk = *(uint32_t*)(&UnkData[88]) != 0;
										BitBuffer->ReadBool(&Unk);

										if (Unk) {
											BitBuffer->ReadUint(OutModel, 0x20);
										}

										return;
									}

									// CPickupPlacementCreationDataNode
									case 0xcdbcdf40: {
										// whole lotta ass

										bool Unk = false;
										BitBuffer->ReadBool(&Unk);

										Math::Vector3_<float> UnkVec3;
										uint32_t UnkUint = 0;

										if (!Unk) {
											auto Instance = BitBuffer->CreateSyncReaderInstance();

											BitBuffer->ReadVector3(&Instance, &UnkVec3, 0, 0x13);
											BitBuffer->ReadVector3(&Instance, &UnkVec3, 0xC, 0);
											BitBuffer->ReadUint(&UnkUint, 0x20);
											BitBuffer->ReadUint(&UnkUint, 0x13);

											Unk = false;
											BitBuffer->ReadBool(&Unk);

											if (Unk) {
												BitBuffer->ReadUint(OutModel, 0x20);
											}
										}

										return;
									}

									// CPlayerCreationDataNode
									case 0x216d2585: {
										BitBuffer->ReadUint(OutModel, 0x20);
										return;
									}

									// CVehicleCreationDataNode
									case 0xb2642b55: {
										BitBuffer->ReadUint(OutModel, 0x20);
										return;
									}
								}
							}
						}
					}
				}

				if (Current->IsParentNode()) {
					if (Current->m_ChildCount > 0) {
						Rage::Network::DataNode* Children = Current->m_FirstChild;
						if (IsValidPtr(Children)) {
							GetModelHashFromNode(Children, BitBuffer, OutModel, OutNameDebugOnly);
						}
					}
				}

				Current = Current->m_Next;
			}
		}
	}

	bool GetPlayerModelFromPlayerAppearanceDataNode(Rage::Network::DataNode* Node, uint32_t* OutModel, uint64_t* OutNode) {
		if (IsValidPtr(Node)) {
			Rage::Network::DataNode* Current = Node;
			while (IsValidPtr(Current) && IsValidPtr((*(uint64_t*)((uint64_t)Current)))) {
				if (Current->IsDataNode()) {
					if (Current->m_DataNodeCommonOperations->m_Data) {
						std::string ClassName = GetVtableName(*(uint64_t*)((uint64_t)Current));
						if (!ClassName.empty()) {
							if (!ClassName.compare("CPlayerAppearanceDataNode")) {
								*OutModel = *(uint32_t*)(Current->m_DataNodeCommonOperations->m_Data + 0x410);
								*OutNode = Current->m_DataNodeCommonOperations->m_Data;
								return true;
							}
						}
					}
				}

				if (Current->m_ChildCount > 0) {
					Rage::Network::DataNode* Children = Current->m_FirstChild;
					if (IsValidPtr(Children)) {
						if (!GetPlayerModelFromPlayerAppearanceDataNode(Children, OutModel, OutNode)) return false;
					}
				}

				Current = Current->m_Next;
			}
		}

		return true;
	}

	void DrawESP(Entity Ent, Color Col, eESPTypes Type, bool InnerLinesBox) {
		if (Native::DoesEntityExist(Ent)) {
			Math::Vector3<float> MinDimensions;
			Math::Vector3<float> MaxDimensions;

			Native::GetModelDimensions(Native::GetEntityModel(Ent), &MinDimensions, &MaxDimensions);

			Math::Vector3<float> Diameter = { MaxDimensions.m_X - MinDimensions.m_X, MaxDimensions.m_Y - MinDimensions.m_Y, MaxDimensions.m_Z - MinDimensions.m_Z };
			Math::Vector3<float> Radius = { Diameter.m_X / 2, Diameter.m_Y / 2, Diameter.m_Z / 2 };

			switch (Type) {
				case ESP_BOX:
				case ESP_BOX_NO_INNER: {
					Math::Vector3<float> FACE_UL = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X * -1, Radius.m_Y, Radius.m_Z);
					Math::Vector3<float> FACE_UR = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X, Radius.m_Y, Radius.m_Z);
					Math::Vector3<float> FACE_BL = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X * -1, Radius.m_Y, Radius.m_Z * -1);
					Math::Vector3<float> FACE_BR = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X, Radius.m_Y, Radius.m_Z * -1);

					Math::Vector3<float> BACK_UL = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X * -1, Radius.m_Y * -1, Radius.m_Z);
					Math::Vector3<float> BACK_UR = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X, Radius.m_Y * -1, Radius.m_Z);
					Math::Vector3<float> BACK_BL = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X * -1, Radius.m_Y * -1, Radius.m_Z * -1);
					Math::Vector3<float> BACK_BR = Native::GetOffsetFromEntityInWorldCoords(Ent, Radius.m_X, Radius.m_Y * -1, Radius.m_Z * -1);

					// Poles
					Menu::GetRenderer()->DrawLine(FACE_BL, FACE_UL, Col);
					Menu::GetRenderer()->DrawLine(FACE_BR, FACE_UR, Col);
					Menu::GetRenderer()->DrawLine(BACK_BL, BACK_UL, Col);
					Menu::GetRenderer()->DrawLine(BACK_BR, BACK_UR, Col);
					Menu::GetRenderer()->DrawLine(FACE_UL, FACE_UR, Col);
					Menu::GetRenderer()->DrawLine(FACE_BL, FACE_BR, Col);
					Menu::GetRenderer()->DrawLine(BACK_UL, BACK_UR, Col);
					Menu::GetRenderer()->DrawLine(BACK_BL, BACK_BR, Col);

					// Bridges
					Menu::GetRenderer()->DrawLine(FACE_UL, BACK_UL, Col);
					Menu::GetRenderer()->DrawLine(FACE_UR, BACK_UR, Col);
					Menu::GetRenderer()->DrawLine(FACE_BL, BACK_BL, Col);
					Menu::GetRenderer()->DrawLine(FACE_BR, BACK_BR, Col);

					// Inner Bridges
					if (Type == ESP_BOX) {
						auto Coords = Native::GetEntityCoords(Ent, false);
						Menu::GetRenderer()->DrawLine(Coords, FACE_UL, Col);
						Menu::GetRenderer()->DrawLine(Coords, FACE_UR, Col);
						Menu::GetRenderer()->DrawLine(Coords, FACE_BL, Col);
						Menu::GetRenderer()->DrawLine(Coords, FACE_BR, Col);
						Menu::GetRenderer()->DrawLine(Coords, BACK_UL, Col);
						Menu::GetRenderer()->DrawLine(Coords, BACK_UR, Col);
						Menu::GetRenderer()->DrawLine(Coords, BACK_BL, Col);
						Menu::GetRenderer()->DrawLine(Coords, BACK_BR, Col);
					}

					break;
				}

				case ESP_AXIS: {
					Math::Vector3<float> X_L = Native::GetOffsetFromEntityInWorldCoords(Ent, Diameter.m_X * -1, 0, 0);
					Math::Vector3<float> X_R = Native::GetOffsetFromEntityInWorldCoords(Ent, Diameter.m_X, 0, 0);
					Math::Vector3<float> Y_F = Native::GetOffsetFromEntityInWorldCoords(Ent, 0, Diameter.m_Y, 0);
					Math::Vector3<float> Y_B = Native::GetOffsetFromEntityInWorldCoords(Ent, 0, Diameter.m_Y * -1, 0);
					Math::Vector3<float> Z_U = Native::GetOffsetFromEntityInWorldCoords(Ent, 0, 0, 35);
					Math::Vector3<float> Z_D = Native::GetOffsetFromEntityInWorldCoords(Ent, 0, 0, -35);

					Menu::GetRenderer()->DrawLine(X_L, X_R, { 255, 0, 0, 255 });
					Menu::GetRenderer()->DrawLine(Y_F, Y_B, { 0, 255, 0, 255 });
					Menu::GetRenderer()->DrawLine(Z_U, Z_D, { 0, 0, 255, 255 });
					break;
				}
			}
		}
	}

	void DrawSnapline(Entity From, Entity To, Color Col) {
		if (Native::DoesEntityExist(From) && Native::DoesEntityExist(To)) {
			if (Native::IsEntityDead(To, 0)) Col.A = 125;
			Menu::GetRenderer()->DrawLine(Native::GetEntityCoords(From, true), Native::GetEntityCoords(To, false), Col);
		}
	}

	void TriggerScriptEvent(std::vector<int> Args, int Count, Player Bitset) {
		int Bitset_ = 0;
		Native::SetBit(&Bitset_, Bitset);

		if (Bitset_ != 0) {
			int* Args_ = new int[Count];
			for (int i = 0; i < Count; i++) {
				Args_[i] = Args[i];
			}

			Args[1] = Bitset;
			Rage::Engine::TriggerScriptEvent(1, Args_, Count, Bitset_);
			delete[] Args_;
		}
	}

	void TriggerScriptEvent(int* Args, int Count, Player Bitset) {
		int Bitset_ = 0;
		Native::SetBit(&Bitset_, Bitset);
		if (Bitset_ != 0) {
			Args[1] = Bitset;
			Rage::Engine::TriggerScriptEvent(1, Args, Count, Bitset_);
		}
	}

	Color GetColorFromBlip(int ColorID) {
		switch (ColorID) {
			case 1: return { 0xe13b3bff };
			case 2: return { 0x79ce79ff };
			case 3: return { 0x65b9e7ff };
			case 4: return { 0xf1f1f1ff };
			case 5: return { 0xf0cb58ff };
			case 6: return { 0xc65859ff };
			case 7: return { 0xa175b4ff };
			case 8: return { 0xff81c8ff };
			case 9: return { 0xf7a580ff };
			case 10: return { 0xb6968bff };
			case 11: return { 0x92d0abff };
			case 12: return { 0x78adb3ff };
			case 13: return { 0xd5d3e8ff };
			case 14: return { 0x95859fff };
			case 15: return { 0x71c8c2ff };
			case 16: return { 0xd8c69eff };
			case 17: return { 0xec9359ff };
			case 18: return { 0x9ecdebff };
			case 19: return { 0xb6698dff };
			case 20: return { 0x95927fff };
			case 21: return { 0xaa7b67ff };
			case 22: return { 0xb4abacff };
			case 23: return { 0xe993a0ff };
			case 24: return { 0xbfd863ff };
			case 25: return { 0x17815dff };
			case 26: return { 0x80c7ffff };
			case 27: return { 0xaf45e7ff };
			case 28: return { 0xd0ac18ff };
			case 29: return { 0x4f6ab1ff };
			case 30: return { 0x35aabcff };
			case 31: return { 0xbda284ff };
			case 32: return { 0xcde2ffff };
			case 33: return { 0xf1f19bff };
			case 34: return { 0xee91a4ff };
			case 35: return { 0xf98f8fff };
			case 36: return { 0xfdf0aaff };
			case 37: return { 0xf1f1f1ff };
			case 38: return { 0x3776bdff };
			case 39: return { 0x9f9f9fff };
			case 40: return { 0x555555ff };
			case 41: return { 0xf29e9eff };
			case 42: return { 0x6db8d7ff };
			case 43: return { 0xb0eeafff };
			case 44: return { 0xffa75fff };
			case 45: return { 0xf1f1f1ff };
			case 46: return { 0xecf029ff };
			case 47: return { 0xff9a18ff };
			case 48: return { 0xf745a5ff };
			case 49: return { 0xe13b3bff };
			case 50: return { 0x8a6de3ff };
			case 51: return { 0xff8b5cff };
			case 52: return { 0x426d42ff };
			case 53: return { 0xb3ddf3ff };
			case 54: return { 0x3a647aff };
			case 55: return { 0xa0a0a0ff };
			case 56: return { 0x847232ff };
			case 57: return { 0x65b9e7ff };
			case 58: return { 0x4c4276ff };
			case 59: return { 0xe13b3bff };
			case 60: return { 0xf0cb58ff };
			case 61: return { 0xce3f99ff };
			case 62: return { 0xcfcfcfff };
			case 63: return { 0x286b9fff };
			case 64: return { 0xd87b1bff };
			case 65: return { 0x8e8393ff };
			case 66: return { 0xf0cb58ff };
			case 67: return { 0x65b9e7ff };
			case 68: return { 0x64b7e4ff };
			case 69: return { 0x79ce79ff };
			case 70: return { 0xf0cb58ff };
			case 71: return { 0xf0cb58ff };
			case 73: return { 0xf0cb58ff };
			case 74: return { 0x65b9e7ff };
			case 75: return { 0xe13b3bff };
			case 76: return { 0x782424ff };
			case 77: return { 0x65b9e7ff };
			case 78: return { 0x3a647aff };
			case 81: return { 0xf2a40cff };
			case 82: return { 0xa4ccaaff };
			case 83: return { 0xa854f2ff };
		}

		return { 255, 255, 255 };
	}

	void SendTextMessage(Player ID, Player Sender, const char* Message) {
		Rage::Network::TextMessageNetMsg Identifier;
		memset(&Identifier.m_Sender, 0, sizeof(Identifier.m_Sender));

		auto& Player = Menu::GetPlayer(Sender);
		if (!IsValidPtr(Player.m_NetGamePlayer)) return;
		if (!IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) return;

		Identifier.m_Sender.m_RockstarID = Player.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID; // spoofed sender
		Identifier.m_Sender.m_Type = 3;

		Rage::Network::NetMsgIdentifier Target;
		memset(&Target, 0, sizeof(Target));

		auto& SenderPlayer = Menu::GetPlayer(ID);
		if (!IsValidPtr(SenderPlayer.m_NetGamePlayer)) return;
		if (!IsValidPtr(SenderPlayer.m_NetGamePlayer->m_PlayerInfo)) return;

		Target.m_RockstarID = SenderPlayer.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID; // recipient
		Target.m_Type = 3;

		strncpy(Identifier.m_Message, Message, 0x100);
		Rage::Engine::SendTextMessageNetMsg(&Target, &Identifier);
	}

	std::string CleanPlayerName(std::string Name) {
		std::string Fixed = Name;
		if (!Fixed.empty()) {
			if (strstr(Fixed.c_str(), "~")) {
				std::string Alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

				for (char C : Alphabet) {
					char b[2];
					snprintf(b, 2, "%c", C);
					Utils::ReplaceString(Fixed, (std::string("~") + b + std::string("~")), "");
				}

				Utils::ReplaceString(Fixed, "~bold~", "");
				Utils::ReplaceString(Fixed, "~italic~", "");
			}
		}

		return Fixed;
	}
}