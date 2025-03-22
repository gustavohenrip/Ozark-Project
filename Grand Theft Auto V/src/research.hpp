#pragma once

// NOT USED, FOR RESEARCH

/*

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

	bool IsNodeValid(Rage::Network::DataNode* Node) {
		uint64_t N = (uint64_t)Node;
		if (N > 0xFFFFFFFF) {
			if ((N & 0xFFFFFFFF) != 0xFFFFFFFF) {
				return !IsBadReadPtr(Node, 8);
			}
		}

		return false;
	}

	bool ProcessDataNode(Rage::Network::DataNode* Node) {
		if (Node != nullptr && (uint64_t)Node > 0xFFFFFFFF) {
			Rage::Network::DataNode* Current = Node;
			while (IsNodeValid(Current) && IsNodeValid((Rage::Network::DataNode*)Current->m_Vtable)) {
				// printf("Processing node %s : %llx\n", GetVtableName(Current->m_Vtable).c_str(), Current->m_Vtable);

				if (!GetVtableName(Current->m_Vtable).compare("CPhysicalAttachDataNode")) {
					return false;
				}

				if (Current->m_ChildCount > 0) {
					Rage::Network::DataNode* Children = Current->m_FirstChild;
					if (IsNodeValid(Children)) {
						if (!ProcessDataNode(Children)) return false;
					}
				}

				Current = Current->m_Next;
			}
		}

		return true;
	}

		addOption(ButtonOption("[DEV] Load Session Info")
		.addOnClick([] {
		Rage::Network::NetMsgIdentifier* Loser = new Rage::Network::NetMsgIdentifier();
		memset(Loser, 0, sizeof(Rage::Network::NetMsgIdentifier));

		Loser->m_RockstarID = 96668400;
		Loser->m_Type = 3;

		Native::ClearGamerTaskData();
		Caller::Call<int>(Global::Vars::g_GameAddress + 0x1041574, Loser, 1);
		delete Loser;
	}));

	addOption(ButtonOption("[DEV] Load Into Invite Only")
		.addOnClick([] {
			// Big hax attempt
			if (!m_Vars.m_GsInfo.empty()) {
				uint8_t yesyes[0x500];
				memset(yesyes, 0, sizeof(yesyes));

				const char* session_key = m_Vars.m_GsInfo.c_str();
				char handle[32];
				sprintf(handle, "SC %i", Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID);

				printf("processed session info: %i\n", Caller::Call<bool>(Global::Vars::g_GameAddress + 0x13D2388, &yesyes[8], session_key, 0, 0));
				printf("processed sc info: %i\n", Caller::Call<int>(Global::Vars::g_GameAddress + 0x13D21A0, &yesyes[0x58], handle));

				printf("using key %s\n", session_key);
				printf("using handle %s\n", handle);

				*(uint8_t*)&yesyes[0x68] = 1;
				*(uint8_t*)&yesyes[0x69] = 1;

				Memory::Nop(Global::Vars::g_GameAddress + 0x1094C09, 6);
				Memory::Nop(Global::Vars::g_GameAddress + 0x1094C1A, 6);
				Caller::Call<int>(Global::Vars::g_GameAddress + 0x1094B88, yesyes);

				//uint8_t sdsdsds[0x40];
				//Caller::Call<int>(Global::Vars::g_GameAddress + 0x10CABB8, &yesyes[0x58], 1, &yesyes[8], Global::Vars::g_GameAddress + 0x18D2AFB, sdsdsds, Global::Vars::g_GameAddress + 0x18D2AFB, -1, -1, 0, 0, 0xA000);
			}
		}));


			addOption(ButtonOption("[DEV] Force Into Session")
		.addOnClick([] {
		Rage::Network::NetMsgIdentifier* Loser = new Rage::Network::NetMsgIdentifier();
		memset(Loser, 0, sizeof(Rage::Network::NetMsgIdentifier));

		Loser->m_RockstarID = 152804745;
		Loser->m_Type = 3;

		Native::ClearGamerTaskData();
		Caller::Call<int>(Global::Vars::g_GameAddress + 0x1041574, Loser, 1);

		// Big hax attempt
		// Memory::WriteVector(Global::Vars::g_GameAddress + 0x1094BBB, { 0xEB, 0x63 });
		Memory::Nop(Global::Vars::g_GameAddress + 0x1094C09, 6);
		Memory::Nop(Global::Vars::g_GameAddress + 0x1094C1A, 6);

		uint8_t yesyes[0x500];
		memset(yesyes, 0, sizeof(yesyes));

		const char* session_key = "wKgACBoQwFH1fvAQAPQhWw9Z/////wAAQJG52JAAAAAAAiUVAIpflrgoCeIlil+WuSgJ4iU=";
		char handle[32];
		sprintf(handle, "SC %i", Menu::GetLocalPlayer().m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID);

		printf("processed session info: %i\n", Caller::Call<bool>(Global::Vars::g_GameAddress + 0x13D2388, &yesyes[8], session_key, 0, 0));
		printf("processed sc info: %i\n", Caller::Call<int>(Global::Vars::g_GameAddress + 0x13D21A0, &yesyes[0x58], handle));

		Caller::Call<int>(Global::Vars::g_GameAddress + 0x1094B88, yesyes);

		delete Loser;
	}));

		addOption(ButtonOption("[DEV] Yes")
		.addOnClick([] {
			char buf[512];
			sprintf_s(buf, R"({"gm.evt":{"e":"ainv","d":{"s":"%s","h":"SC %i","t":1,"ad":0}}})", "wKgALBoQuThBoPAQz9Xds2pC/////wAAQFwwrwAAAAAAyLaT4P/2hsuG5NQe//aGzIbk1B4=", Global::Vars::g_RsInfo->m_RockstarID);

			Rage::Network::RockstarIdentifier Identifier;
			Identifier.m_RockstarID = 96668400;
			Identifier.m_Type = 3;

			auto payloadPtr = std::make_unique<BYTE[]>(1536);
			SecureZeroMemory(payloadPtr.get(), 1536);
			memcpy(payloadPtr.get(), buf, strlen(buf));
			memcpy(payloadPtr.get() + 1024, "self", 4);

			Caller::Call<int>(Global::Vars::g_GameAddress + 0x1087790, 0x0FFFFFFFF, &Identifier, 1, __int64(payloadPtr.get()), 1);

			printf("%s\n", Rage::Engine::SocialClubGetGameInfoString("gsinfo"));
		}));

		Rage::Network::NetworkHandle EmptyHandle;
				Caller::Call<int>(Global::Vars::g_GameAddress + 0x10CABB8, &Request.m_Identifier, 1, Request.m_SessionInfo, "", &EmptyHandle, "", -1, -1, 0, 0, Types[m_Vars.m_JoinThemType].m_Result);



				addOption(ButtonOption("[DEV] CPed")
		.addOnClick([] { printf("%llx\n", Menu::GetLocalPlayer().m_PedPointer); }));

	addOption(ButtonOption("[DEV] Dump Global")
		.addOnClick([] {
			char Buffer[0x100];
			int Global = 2425662;
			int Size = 421;
			int PlayerIndex = Menu::GetLocalPlayer().m_ID;

			std::ofstream Output("D://Global_2425662.txt");
			if (Output.good()) {
				for (int i = 0; i < Size; i++) {
					int Value = *Menu::Global(Global).At(PlayerIndex, Size).At(i).Get<int>();
					snprintf(Buffer, 0x100, "Global_%i[%i <%i>].f_%i = %X", Global, PlayerIndex, Size, i, Value);
					Output << Buffer << std::endl;
				}

				Output.close();
			}
		}));

	addOption(ButtonOption("[DEV] Sky")
		.addOnClick([] {
			// 0000000140E1AC09 - updates the color, nop
			Rage::Network::DictionaryItem* Mine = (Rage::Network::DictionaryItem*)Rage::Engine::GetSpriteTexture("OzarkSkydome", "moon-new");

			Rage::Network::TxdStore* Txd = (Rage::Network::TxdStore*)(Global::Vars::g_GameAddress + 0x2E30A70);
			if (Txd->m_Pool && Txd->m_Mask) {
				Rage::Network::DictionaryPool* Pool = Txd->m_Pool;
				for (uint32_t i = 0; i < Txd->m_Count; i++) {
					if (~(Txd->m_Mask[i] >> 7) & 1) {
						Rage::Network::DictionaryPool Element = Pool[i];
						if (Element.m_Pool > 0) {
							for (short j = 0; j < Element.m_Pool->m_Count; j++) {
								Rage::Network::DictionaryItem* Item = Element.m_Pool->m_Items[j];
								if (Item > 0) {
									if (Item->m_Name) {
										printf("checking %s\n", Item->m_Name);
										if (!strcmp(Item->m_Name, "moon-new")) {
											Memory::WriteMemcpy(Item, Mine, sizeof(Rage::Network::DictionaryItem));
											printf("found in %X\n", Element.m_DictionaryHash);
											return;
										}
									}
								}
							}
						}
					}
				}

				printf("done!\n");
			}
		}));

	addOption(ButtonOption("[DEV] Replace All Textures")
		.addOnClick([] {
		Rage::Network::DictionaryItem* Mine = (Rage::Network::DictionaryItem*)Rage::Engine::GetSpriteTexture("OzarkSkydome", "moon-new");

		Rage::Network::TxdStore* Txd = (Rage::Network::TxdStore*)(Global::Vars::g_GameAddress + 0x2E30A70);
		if (Txd->m_Pool && Txd->m_Mask) {
			Rage::Network::DictionaryPool* Pool = Txd->m_Pool;
			for (uint32_t i = 0; i < Txd->m_Count; i++) {
				if (~(Txd->m_Mask[i] >> 7) & 1) {
					Rage::Network::DictionaryPool Element = Pool[i];
					if (Element.m_Pool > 0) {
						for (short j = 0; j < Element.m_Pool->m_Count; j++) {
							Rage::Network::DictionaryItem* Item = Element.m_Pool->m_Items[j];
							if (Item > 0) {
								Memory::WriteMemcpy(Item, Mine, sizeof(Rage::Network::DictionaryItem));
								// printf("[TxdHack] Editing dict %X item %i\n", Element.m_DictionaryHash, j);
							}
						}
					}
				}
			}

			printf("done!\n");
		}
	}));


*/