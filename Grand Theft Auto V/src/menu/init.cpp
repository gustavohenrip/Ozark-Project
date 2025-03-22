#include "init.hpp"
#include "auth/http/win_http_client.hpp"
#include "utils/config.hpp"
#include "utils/log.hpp"
#include "global/variables.hpp"
#include "utils/memory/pattern.hpp"
#include "utils/memory/memory.hpp"
#include "utils/hooking/hooking.hpp"
#include "rage/invoker/native_invoker.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/hooks/hooks.hpp"
#include "menu/base/util/input.hpp"
#include "utils/fibers.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "auth.hpp"
#include "utils/thread_pool.hpp"
#include "utils/fiber_pool.hpp"
#include "menu/base/submenu_handler.hpp"
#include "utils/caller.hpp"
#include "rage/api/social.hpp"
#include "utils/json.hpp"
#include "menu/base/util/panels.hpp"
#include "utils/va.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_game_events.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_notifications.hpp"
#include "menu/base/submenus/main.hpp"
#include "utils/utils.hpp"
#include "menu/base/submenus/main/network/protection/network_protection_event_reactions.hpp"
#include <random>

namespace Menu {
	LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfoPtr) {
		DWORD ExceptionCode = ExceptionInfoPtr->ExceptionRecord->ExceptionCode;
		ULONG_PTR ExceptionInfo = ExceptionInfoPtr->ExceptionRecord->ExceptionInformation[0];
		ULONG_PTR ExceptionInfo1 = ExceptionInfoPtr->ExceptionRecord->ExceptionInformation[1];
		ULONG_PTR ExceptionInfo2 = ExceptionInfoPtr->ExceptionRecord->ExceptionInformation[2];

		uint64_t ExceptionAddress = (uint64_t)ExceptionInfoPtr->ExceptionRecord->ExceptionAddress;

		LOG_INFO("Game crashed with code 0x%X", ExceptionCode);

		if ((ExceptionAddress >= Global::Vars::g_MenuAddress && ExceptionAddress <= (Global::Vars::g_MenuAddress + Global::Vars::g_MenuSize))
			|| (ExceptionAddress - 0x180000000) < Global::Vars::g_MenuSize) {
			if ((ExceptionAddress - 0x180000000) < Global::Vars::g_MenuSize) Global::Vars::g_MenuAddress = 0x180000000;
			LOG_INFO("Crashed inside our module @ 0x%llx (%s)", ExceptionAddress - Global::Vars::g_MenuAddress, Utils::ConvertBytesToString((uint8_t*)ExceptionAddress, 0x20, true).c_str());
		} else {
			LOG_INFO("Crashed @ 0x%llx (game=%llx)", ExceptionAddress, Global::Vars::g_GameAddress);
		}

		switch (ExceptionCode) {
			case EXCEPTION_ACCESS_VIOLATION:
				LOG_INFO("Cause: EXCEPTION_ACCESS_VIOLATION");
				if (ExceptionInfo == 0) {
					LOG_INFO("Attempted to read from: 0x%llx", ExceptionInfo1);
				} else if (ExceptionInfo == 1) {
					LOG_INFO("Attempted to write to: 0x%llx", ExceptionInfo1);
				} else if (ExceptionInfo == 8) {
					LOG_INFO("Data Execution Prevention (DEP) at: 0x%llx", ExceptionInfo1);
				} else {
					LOG_INFO("Unknown access violation at: 0x%llx", ExceptionInfo1);
				}
				break;

			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: LOG_INFO("Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED"); break;
			case EXCEPTION_BREAKPOINT: LOG_INFO("Cause: EXCEPTION_BREAKPOINT"); break;
			case EXCEPTION_DATATYPE_MISALIGNMENT: LOG_INFO("Cause: EXCEPTION_DATATYPE_MISALIGNMENT"); break;
			case EXCEPTION_FLT_DENORMAL_OPERAND: LOG_INFO("Cause: EXCEPTION_FLT_DENORMAL_OPERAND"); break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO: LOG_INFO("Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO"); break;
			case EXCEPTION_FLT_INEXACT_RESULT: LOG_INFO("Cause: EXCEPTION_FLT_INEXACT_RESULT"); break;
			case EXCEPTION_FLT_INVALID_OPERATION: LOG_INFO("Cause: EXCEPTION_FLT_INVALID_OPERATION"); break;
			case EXCEPTION_FLT_OVERFLOW: LOG_INFO("Cause: EXCEPTION_FLT_OVERFLOW"); break;
			case EXCEPTION_FLT_STACK_CHECK: LOG_INFO("Cause: EXCEPTION_FLT_STACK_CHECK"); break;
			case EXCEPTION_FLT_UNDERFLOW: LOG_INFO("Cause: EXCEPTION_FLT_UNDERFLOW"); break;
			case EXCEPTION_ILLEGAL_INSTRUCTION: LOG_INFO("Cause: EXCEPTION_ILLEGAL_INSTRUCTION"); break;
			case EXCEPTION_IN_PAGE_ERROR:
				LOG_INFO("Cause: EXCEPTION_IN_PAGE_ERROR");
				if (ExceptionInfo == 0) {
					LOG_INFO("Attempted to read from: 0x%llx", ExceptionInfo1);
				} else if (ExceptionInfo == 1) {
					LOG_INFO("Attempted to write to: 0x%llx", ExceptionInfo1);
				} else if (ExceptionInfo == 8) {
					LOG_INFO("Data Execution Prevention (DEP) at: 0x%llx", ExceptionInfo1);
				} else {
					LOG_INFO("Unknown access violation at: 0x%llx", ExceptionInfo1);
				}

				LOG_INFO("NTSTATUS: 0x%llx", ExceptionInfo2);
				break;

			case EXCEPTION_INT_DIVIDE_BY_ZERO: LOG_INFO("Cause: EXCEPTION_INT_DIVIDE_BY_ZERO"); break;
			case EXCEPTION_INT_OVERFLOW: LOG_INFO("Cause: EXCEPTION_INT_OVERFLOW"); break;
			case EXCEPTION_INVALID_DISPOSITION: LOG_INFO("Cause: EXCEPTION_INVALID_DISPOSITION"); break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION: LOG_INFO("Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION"); break;
			case EXCEPTION_PRIV_INSTRUCTION: LOG_INFO("Cause: EXCEPTION_PRIV_INSTRUCTION"); break;
			case EXCEPTION_SINGLE_STEP: LOG_INFO("Cause: EXCEPTION_SINGLE_STEP"); break;
			case EXCEPTION_STACK_OVERFLOW: LOG_INFO("Cause: EXCEPTION_STACK_OVERFLOW"); break;
			case DBG_CONTROL_C: LOG_INFO("Cause: DBG_CONTROL_C"); break;
			default: LOG_INFO("Cause: 0x%08x", ExceptionCode);
		}

		LOG_INFO("Dumping ASM registers:");
		LOG_INFO("RAX: 0x%llx || RSI: 0x%llx", ExceptionInfoPtr->ContextRecord->Rax, ExceptionInfoPtr->ContextRecord->Rsi);
		LOG_INFO("RBX: 0x%llx || RDI: 0x%llx", ExceptionInfoPtr->ContextRecord->Rbx, ExceptionInfoPtr->ContextRecord->Rdi);
		LOG_INFO("RCX: 0x%llx || RBP: 0x%llx", ExceptionInfoPtr->ContextRecord->Rcx, ExceptionInfoPtr->ContextRecord->Rbp);
		LOG_INFO("RDX: 0x%llx || RSP: 0x%llx", ExceptionInfoPtr->ContextRecord->Rdx, ExceptionInfoPtr->ContextRecord->Rsp);
		LOG_INFO("R8: 0x%llx || R9: 0x%llx", ExceptionInfoPtr->ContextRecord->R8, ExceptionInfoPtr->ContextRecord->R9);
		LOG_INFO("R10: 0x%llx || R11: 0x%llx", ExceptionInfoPtr->ContextRecord->R10, ExceptionInfoPtr->ContextRecord->R11);
		LOG_INFO("R12: 0x%llx || R13: 0x%llx", ExceptionInfoPtr->ContextRecord->R12, ExceptionInfoPtr->ContextRecord->R13);
		LOG_INFO("R14: 0x%llx || R15: 0x%llx", ExceptionInfoPtr->ContextRecord->R14, ExceptionInfoPtr->ContextRecord->R15);

		LOG_INFO(Utils::GetStackTrace().c_str());

		return EXCEPTION_CONTINUE_SEARCH;
	}

	void Init::ExternalUnload() {
		LOG_WARN("Unloading from external sources! If you're not hot reloading, please send the menus you have loaded to a staff member :)"_Protect);
		GetInit()->Unload();
	}

	void Init::UnloadThread() {
#ifndef DEV
		Security::Relink();
#endif

		for (auto& Patch : Global::Vars::g_FunctionPatches) {
			Memory::WriteVector(Patch.m_Address, Patch.m_Original);
		}

		// free((void*)Global::Vars::g_TextureBypass);

		Menu::Panels::GetPanelManager()->Unload();
		Utils::GetThreadPool()->Unload();
		Hook::GetHooking()->Unload();
		Utils::GetFiberManager()->Unload();
		Utils::GetScriptHookFiberManager()->Unload();
		Util::GetInput()->Unload();
		GetSubmenuHandler()->Unload();
		Utils::VA::Cleanup();
		Logger::GetLog()->Cleanup();

		FreeConsole();
		PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);

#ifdef DEV
		Sleep(1000);
		FreeLibraryAndExitThread(Global::Vars::g_ModuleHandle, 0);
#endif
	}

	void Init::Unload() {
		if (!Global::Vars::g_Unloading) {
			Global::Vars::g_Unloading = true;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UnloadThread, 0, 0, 0);
		}
	}

	namespace AntiCheat {
		uint64_t g_NullSub = 0;
		std::vector<std::string> BlockedMetrics = {
			"DIG",
			"XP_LOSS",
			"AWARD_XP",
			"CF",
			"CC",
			"CNR",
			"SCRIPT",
			"CHEAT",
			"AUX_DEUX",
			"WEATHER",
			"HARDWARE_OS",
			"HARDWARE_GPU",
			"HARDWARE_MOBO",
			"HARDWARE_MEM",
			"HARDWARE_CPU",
			"PCSETTINGS",
			"CASH_CREATED",
			"DR_PS",
			"UVC",
			"W_L",
			"ESVCS",
			"IDLEKICK",
			"GSCB",
			"GSINV",
			"GSCW",
			"GSINT",
			"EARN"
		};

		struct Metric {
			virtual ~Metric();
			virtual int GetType();
			virtual int GetSubType();
			virtual const char* GetName();
			virtual bool ConstructPayload(uint64_t Json);
			virtual int GetSize();
			virtual uint32_t GetHashedName();
		};

		struct CashCreatedMetric : public Metric {
			char _0x0008[0x14];
			uint32_t m_PeerTokenMac;
			char m_G[0x20];
			uint32_t m_Amount;
			uint32_t m_Hash;
		};

		TranslationString T_IsDroppingSomething("is dropping something", true, true);
		TranslationString T_IsDropping$("is dropping ~g~$~w~", true, true);
		TranslationString T_IsDroppingRP("is dropping RP", true, true);

		void(*OriginalAddMetricToJsonArray)(uint32_t, uint64_t, uint64_t, Metric*);
		void AddMetricToJsonArrayHook(uint32_t Ecx, uint64_t Rdx, uint64_t R8, Metric* Metric) {
			if (Metric) {
				LOG_DEV("METRIC: %s", Metric->GetName());
				if (Global::Vars::g_NativeRegistration && Menu::GetLocalPlayer().m_Connected) {
					if (!strcmp(Metric->GetName(), "CASH_CREATED")) {
						CashCreatedMetric* CashMetric = (CashCreatedMetric*)Metric;

						if (CashMetric->m_Amount > 500 && CashMetric->m_Amount < 3000) {
							uint64_t RockstarID = std::stoull(std::string(CashMetric->m_G).substr(3));

							Menu::PlayerVars& Player = Menu::Helpers::GetPlayerFromRID(RockstarID);
							if (Player.m_Alive) {
								auto& ProtectVars = NetworkProtectionNotificationsMenuVars::m_Vars;

								if ((GetTickCount() - ProtectVars.m_MoneyDropMap[RockstarID]) > 5000) {
									ProtectVars.m_MoneyDropMap[RockstarID] = GetTickCount();

									std::string Reason = TRANSLATE(T_IsDroppingSomething);
									Player.m_IsModder = true;

									switch (CashMetric->m_Hash) {
										case 0xCE6FDD6B: // Money
										case 0x5DE0AD3E: // Money
										case 0x1E9A99F8: // Money
										case 0x20893292: // Money
										case 0x14568F28: // Money
										case 0x711D02A4: // Money
										case 0xDE78F17E: // Money
										case 0xFE18F3AF: // Money
											Reason = TRANSLATE(T_IsDropping$) + std::to_string(CashMetric->m_Amount);
											Player.m_ModderFlags[Menu::REASON_MONEY_DROP] = true;
											NetworkProtectionEventReactionsMenuVars::ProcessEvent(NetworkProtectionEventReactionsMenuVars::EVENT_MONEY_DROP, Player.m_ID);
											break;

										case 0x2C014CA6: // RP
											Reason = TRANSLATE(T_IsDroppingRP);
											Player.m_ModderFlags[Menu::REASON_RP_DROP] = true;
											break;
									}

									if (ProtectVars.m_MoneyDrop) {
										Menu::GetNotify()->NotifyStacked(Menu::Helpers::CleanPlayerName(Player.m_Name) + " " + Reason);
									}
								}
							}
						}
					}
				}

				if (!strcmp(Metric->GetName(), "SPAWN")) {
					Utils::GetFiberPool()->Push([] {
						Utils::WaitFiber(2000);
						Global::Vars::g_ReapplyWeapons = true;
					});
				}

				if (std::find(BlockedMetrics.begin(), BlockedMetrics.end(), Metric->GetName()) != BlockedMetrics.end()) {
					LOG_CUSTOM_DEV("AC", "Blocking metric %s", Metric->GetName());
					return;
				}
			}

			return OriginalAddMetricToJsonArray(Ecx, Rdx, R8, Metric);
		}

		void(*OriginalIntegrityCheckRockstar)(uint64_t);
		void IntegrityCheckRockstarHook(uint64_t Rcx) {
			*(uint64_t*)(Rcx + 0x10) = g_NullSub;
			return OriginalIntegrityCheckRockstar(Rcx);
		}

		bool(*OriginalProcessTransactionQueue)(uint64_t, int, uint64_t);
		bool ProcessTransactionQueueHook(uint64_t Manager, int TransactionID, uint64_t Table) {
			struct TransactionNode {
				char _0x0000[0x8];
				NetShopTransactionBasket* m_Transaction;
				TransactionNode* m_Next;
			};

			if (Manager) {
				TransactionNode* Node = *(TransactionNode**)(Manager + 0x20);
				TransactionNode* LastNode = nullptr;

#ifdef DEV
				FILE* fp = fopen(("D://Dumps//Transactions//" + std::to_string(TransactionID) + ".txt").c_str(), "w");
#endif
				while (Node) {
					if (Node->m_Transaction->m_TransactionID == TransactionID) {
						if (Node->m_Transaction->m_Category == (int)0xAE04310C) {
							LOG_CUSTOM_DEV("AC", "Blocked arxan report adding to queue");
							return false;
						}

#ifdef DEV
						if (fp) {
							fprintf(fp, "Rage::Engine::ProcessBasketTransaction(");
							fprintf(fp, "%i, %i, %i, {", Node->m_Transaction->m_Category, Node->m_Transaction->m_Action, Node->m_Transaction->m_Target);

							for (int i = 0; i < Node->m_Transaction->m_TransactionCount; i++) {
								fprintf(fp, "{ %i, %i, %i, %i, %i }, ",
									Node->m_Transaction->m_Transactions[i].m_ID, Node->m_Transaction->m_Transactions[i].m_Unk1,
									Node->m_Transaction->m_Transactions[i].m_Price, Node->m_Transaction->m_Transactions[i].m_Multiplier,
									Node->m_Transaction->m_Transactions[i].m_Value);
							}

							fprintf(fp, "});\n");
						}
#endif
					}

					LastNode = Node;
					Node = Node->m_Next;
				}

#ifdef DEV
				if (fp) {
					fclose(fp);
				}
#endif
			}

			return OriginalProcessTransactionQueue(Manager, TransactionID, Table);
		}

		void(*OriginalSendNetworkEvent)(uint64_t NetTable, uint64_t Event);
		void SendNetworkEventHook(uint64_t NetTable, uint64_t Event) {
			if (Event) {
				short Type = *(short*)(Event + 8);

				if (Type == REPORT_MYSELF_EVENT
					|| Type == REPORT_CASH_SPAWN_EVENT
					|| Type == NETWORK_CHECK_CODE_CRCS_EVENT) {
					LOG_CUSTOM_DEV("AC", "Blocking network event - %i", Type);

					uint64_t Table = *(uint64_t*)Event;
					Caller::Call<int>(*(uint64_t*)Table, Event, 1); // Deallocate event
					return;
				}
			} else LOG_DEV("SendNetworkEvent bad event from 0x%llx", ((uint64_t)_ReturnAddress() - Global::Vars::g_GameAddress) + 0x140000000);

			return OriginalSendNetworkEvent(NetTable, Event);
		}
	}

	void Init::AntiCheat() {
		Memory::ScanProcess({ "48 0F 45 CA 48 89 4C 24 ? 5A 59 E9"_Protect, 0xB, "NSUB"_Protect, true }, [] (uint64_t Address) {
			AntiCheat::g_NullSub = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ScanProcess({ "48 8B 01 48 8B 69 10 E9"_Protect, 0, "RINT"_Protect, true }, [] (uint64_t Address) {
			Hook::GetHooking()->Create("RINT"_Protect, Address, &AntiCheat::IntegrityCheckRockstarHook, &AntiCheat::OriginalIntegrityCheckRockstar);
		});

		Memory::ScanProcess({ "48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 70 20 55 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 83"_Protect, 0, "PME"_Protect, true }, [] (uint64_t Address) {
			Hook::GetHooking()->Create("PME"_Protect, Address, &AntiCheat::AddMetricToJsonArrayHook, &AntiCheat::OriginalAddMetricToJsonArray);
		});

		Memory::ScanProcess({ "48 8B 5F 08 48 8B 7F 10 49 8B D6 48 8B 03 48 8B CB FF 90 ? ? ? ? 84 C0 0F 85"_Protect, -0x34, "SNE"_Protect, true }, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SNE"_Protect, Address, &AntiCheat::SendNetworkEventHook, &AntiCheat::OriginalSendNetworkEvent);
		});

		Memory::ScanProcess({ "48 89 5C 24 ? 57 48 83 EC 20 48 8B 59 20 45 32 C0 8B FA 48 85 DB 74 73 48 8B 43 10 48 8B 5B 08 3B 7B"_Protect, 0, "ATTQ"_Protect, true }, [] (uint64_t Address) {
			Hook::GetHooking()->Create("ATTQ"_Protect, Address, &AntiCheat::ProcessTransactionQueueHook, &AntiCheat::OriginalProcessTransactionQueue);
			Global::Vars::g_ProcessTransactionQueue = Address;
		});
	}

	void Init::HeartbeatThread() {
		if (!Auth::Heartbeat()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Heartbeat failed"_Protect);
			GetInit()->Unload();
		}

		// 5 min heartbeat
		Sleep(300000);
	}

	void Init::GeoIPThread() {
		Sleep(100);

		if (Global::Vars::g_GeoQueue.size()) {
			uint32_t Item = Global::Vars::g_GeoQueue.front();

			if (Global::Vars::g_Geo.find(Item) == Global::Vars::g_Geo.end()) {
				char IPStr[16];
				sprintf(IPStr, "%i.%i.%i.%i", ((Item & 0xFF000000) >> 24), ((Item & 0xFF0000) >> 16), ((Item & 0xFF00) >> 8), (Item & 0xFF));

				LOG_INFO("Sending GEO request for %s", IPStr);
				HttpClient Client(std::wstring(Utils::VA::VAW("https://ozarkengine.cc/geo.php?ip=%s"_Protect, IPStr)));

				if (Client.SendHttpRequest(std::wstring(Utils::VA::VAW("GET"_Protect)))) {
					try {
						nlohmann::json Json = nlohmann::json::parse(Client.GetResponseContent());
						if (!Json.empty()) {
							if (!Json["status"].is_null()) {
								if (!strcmp(Json["status"].get<std::string>().c_str(), "success")) {
									GeoIP geo;
									if (!Json["country"].is_null()) geo.m_Country = Json["country"].get<std::string>();
									if (!Json["countryCode"].is_null()) geo.m_CountryCode = Json["countryCode"].get<std::string>();
									if (!Json["regionName"].is_null()) geo.m_RegionName = Json["regionName"].get<std::string>();
									if (!Json["city"].is_null()) geo.m_City = Json["city"].get<std::string>();
									if (!Json["zip"].is_null()) geo.m_Zip = Json["zip"].get<std::string>();
									if (!Json["timezone"].is_null()) geo.m_Timezone = Json["timezone"].get<std::string>();
									if (!Json["isp"].is_null()) geo.m_ISP = Json["isp"].get<std::string>();
									if (!Json["org"].is_null()) geo.m_ORG = Json["org"].get<std::string>();

									Global::Vars::g_Geo[Item] = geo;
								}
							}
						}
					} catch (...) {
						LOG_WARN("Failed to parse GeoIP request for %s"_Protect, IPStr);
					}
				} else {
					LOG_WARN("Failed to send GeoIP request for %s"_Protect, IPStr);
				}
			}

			Global::Vars::g_GeoQueue.pop();
			Sleep(1000);
		}
	}

	void Init::Globals() {
		if (!Memory::ScanProcessModule("socialclub.dll"_Protect, { "48 8D 05 ? ? ? ? 48 03 F8 44 8B 47 14 48 8D 57 20 E8 ? ? ? ? 85"_Protect, 0, "SCGIT"_Protect, false }, [] (uint64_t Address) {
			Global::Vars::g_SocialClubGameInfoTable = Memory::GetAddressFromInstruction(Address);
		})) { Unload(); return; }

		if (!Memory::ScanProcessModule("socialclub.dll"_Protect, { "E8 ? ? ? ? 85 C0 78 27 3B 47 14 7D 1E 48 98 48 8B D6 48 69 C8"_Protect, 0, "SCGGIT"_Protect, false }, [] (uint64_t Address) {
			Global::Vars::g_SocialClubGetGameInfoTable = Memory::GetAddressFromInstruction(Address, 1, 5);
		})) { Unload(); return; }

		if (!Memory::ScanProcessModule("socialclub.dll"_Protect, { "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B 89 ? ? ? ? 49 8B D9 49 8B F8 8B F2 48 85 C9 74 16 48 8B 01 48 8B 5C 24 ? 48 8B 74 24 ? 48 83 C4 20 5F 48 FF 60 18 E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 84 C0 74 1F 41 B1 01 4C 8B C3 48 8B D7 8B CE 48 8B"_Protect, 0, "GSCIS"_Protect, false }, [] (uint64_t Address) {
			Hook::GetHooking()->Create("GSCIS"_Protect, Address, Hooks::GetScItemStringHook, &Hooks::OriginalGetScItemString);
		})) { Unload(); return; }

		Memory::ServerScanProcess("NRT"_Protect, [](uint64_t Address) {
			Global::Vars::g_NativeRegistration = (Rage::Invoker::NativeRegistration**)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("ST"_Protect, [](uint64_t Address) {
			Global::Vars::g_ScriptTable = (Rage::Script::ScriptTable*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ScanProcess({ "48 8D 0D ? ? ? ? E8 ? ? ? ? 83 38 FF 74 2D"_Protect, 0, "GSS"_Protect, true }, [](uint64_t Address) {
			Global::Vars::g_GameStreamedScripts = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ScanProcess({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B EA 48 8B F1 49 8B D0 B9 ? ? ? ? 49 8B F8 E8"_Protect, 0, "GSS_G"_Protect, true }, [](uint64_t Address) {
			Global::Vars::g_GetStreamedScript = Address;
		});

		Memory::ServerScanProcess("GS"_Protect, [](uint64_t Address) {
			Global::Vars::g_GameState = (GameState*)Memory::GetAddressFromInstruction(Address, 2, 7);
		});

		Memory::ServerScanProcess("CPF"_Protect, [](uint64_t Address) {
			Global::Vars::g_PedFactory = *(Rage::Types::PedFactory**)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("GCP"_Protect, [](uint64_t Address) {
			Global::Vars::g_GlobalCache = (uint64_t**)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("WCHL"_Protect, [](uint64_t Address) {
			Global::Vars::g_WeaponComponentHashListCount = Memory::GetAddressFromInstruction(Address, 2, 6);
			Global::Vars::g_WeaponComponentHashList = Memory::GetAddressFromInstruction(Address + 0x14);
		});

		Memory::ServerScanProcess("WIHL"_Protect, [] (uint64_t Address) {
			Global::Vars::g_WeaponInfoListCount = Memory::GetAddressFromInstruction(Address, 4, 8);
			Global::Vars::g_WeaponInfoList = Memory::GetAddressFromInstruction(Address + 0x10);
		});

		Memory::ServerScanProcess("GCA"_Protect, [](uint64_t Address) {
			Global::Vars::g_GameCameraAngles = (Rage::Network::GameCameraAngles*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("GTPC"_Protect, [](uint64_t Address) {
			Global::Vars::g_GetThirdPersonCamera = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("TPCP"_Protect, [](uint64_t Address) {
			Global::Vars::g_ThirdPersonCamera = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("GEA"_Protect, [](uint64_t Address) {
			Global::Vars::g_GetEntityAddress = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("GEH"_Protect, [](uint64_t Address) {
			Global::Vars::g_GetEntityHandleFromAddress = Address;
		});

		Memory::ServerScanProcess("PMSP"_Protect, [] (uint64_t Address) {
			Memory::WriteVector(Address, { 0xEB, 0x02 });
		});

		Memory::ServerScanProcess("MSP"_Protect, [] (uint64_t Address) {
			Memory::Nop(Address, 24);
		});

		Memory::ServerScanProcess("RSTRB"_Protect, [] (uint64_t Address) {
			Memory::Nop(Address, 3);
		});

		Memory::ServerScanProcess("RSTEB"_Protect, [] (uint64_t Address) {
			Memory::Nop(Address, 3);
		});

		Memory::ServerScanProcess("GCNGPFI"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetNetGamePlayerFromIndex = Address;
		});

		Memory::ServerScanProcess("GMI"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetModelInfo = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("SVG"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SetVehicleGravity = Address;
		});

		Memory::ServerScanProcess("GVPA"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetVehiclePaintArray = Address;
		});

		Memory::ServerScanProcess("DOI"_Protect, [] (uint64_t Address) {
			Global::Vars::g_DrawOriginIndex = Memory::GetAddressFromInstruction(Address, 2, 6);
		});

		Memory::ServerScanProcess("ATCT"_Protect, [] (uint64_t Address) {
			Global::Vars::g_AddToClockTime = Address;
		});

		Memory::ServerScanProcess("DST"_Protect, [] (uint64_t Address) {
			Global::Vars::g_DispatchServiceTable = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("SCT"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SyncClockTime = Address;
		});

		Memory::ServerScanProcess("SCT2"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendClockTime = Address;
		});

		Memory::ServerScanProcess("SWU"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendWeatherUpdate = Address;
		});

		Memory::ServerScanProcess("GHNGP"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetHostNetGamePlayer = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("TSE"_Protect, [] (uint64_t Address) {
			Global::Vars::g_TriggerScriptEvent = Address;
		});

		Memory::ServerScanProcess("SISE"_Protect, [](uint64_t Address) {
			Global::Vars::g_SendIncrementStatEvent = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("AOEB"_Protect, [] (uint64_t Address) {
			Memory::Write(Memory::GetAddressFromInstruction(Address), (uint8_t)0x1);
		});

		Memory::ServerScanProcess("UNOO"_Protect, [] (uint64_t Address) {
			Global::Vars::g_UpdateNetObjectOwner = Address;
		});

		Memory::ServerScanProcess("NOM"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetworkObjectManager = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("NPM"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetworkPlayerManager = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("HROFV"_Protect, [] (uint64_t Address) {
			Global::Vars::g_HandleRotationValuesFromOrder = Address;
		});

		Memory::ServerScanProcess("GNGPFH"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetNetGamePlayerFromHandle = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("GST"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetSpriteTexture = Address;
		});

		Memory::ServerScanProcess("STMNM"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendTextMessageNetMsg = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("STL"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SetTrafficLights = Address;
		});

		Memory::ServerScanProcess("UTL"_Protect, [] (uint64_t Address) {
			Global::Vars::g_UpdateTrafficLights = Address;
		});

		Memory::ServerScanProcess("STLC"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SetTrafficLightColors = Address;
		});

		Memory::ServerScanProcess("SVLC"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SetVehicleLights = Address;
		});

		Memory::ServerScanProcess("GSTFCT"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetSyncTreeFromCloneType = Address;
		});

		Memory::ServerScanProcess("GNOFNI"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetNetworkObjectFromNetworkID = Address;
		});

		Memory::ServerScanProcess("RBFBB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ReadBoolFromBitBuffer = Address;
			Global::Vars::g_ReadUnsignedIntFromBitBuffer = Memory::GetAddressFromInstruction(Address + 0x1F, 1, 5);
		});

		Memory::ServerScanProcess("RIFBB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ReadIntFromBitBuffer = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("RV3FBB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ReadVector3FromBitBuffer = Address;
		});

		Memory::ServerScanProcess("RAFBB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ReadUintArrayFromBitBuffer = Address;
		});

		Memory::ServerScanProcess("RSFBB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ReadShortFromBitBuffer = Address;
		});

		Memory::ServerScanProcess("TLG"_Protect, [](uint64_t Address) {
			Global::Vars::g_TaskLeaveGamer = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("GPFP"_Protect, [](uint64_t Address) {
			Global::Vars::g_GetPeerFromNetGamePlayer = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("GCFP", [](uint64_t Address) {
			Global::Vars::g_GetNetworkConfigFromPeer = Address;
		});

		Memory::ServerScanProcess("NER"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetworkEventRegistration = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("GPI"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetParticleInfo = Address;
		});

		Memory::ServerScanProcess("NARRCF3"_Protect, [] (uint64_t Address) {
			Memory::WriteVector(Address, { 0xE9, 0x7C, 0x01, 0x00, 0x00 }); // cherax crash fix
		});

		Memory::ServerScanProcess("NB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetworkBail = Address;
		});

		/*Memory::ServerScanProcess("GAKB"_Protect, [] (uint64_t Address) {
			Memory::Nop(Address, 13);
		});*/

		Memory::ServerScanProcess("WAYP"_Protect, [] (uint64_t Address) {
			Global::Vars::g_WaypointData = (Rage::Types::WaypointData*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("NSQ"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetShopQueue = *(Rage::Network::NetShopQueue**)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("UUIV1"_Protect, [] (uint64_t Address) {
			Global::Vars::g_UpdateUIValues1 = Address;
		});

		Memory::ServerScanProcess("UUIV2"_Protect, [] (uint64_t Address) {
			Global::Vars::g_UpdateUIValues2 = Address;
		});

		Memory::ServerScanProcess("UIVS"_Protect, [] (uint64_t Address) {
			Global::Vars::g_UIVisualSettings = (Rage::Types::UIVisualSettings*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("SVNUI"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SetVehicleNeonUI = Address;
		});

		Memory::ServerScanProcess("VFXL"_Protect, [] (uint64_t Address) {
			Global::Vars::g_VfxLiquid = (Rage::Types::VfxLiquid*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("HVBO"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SetSeethrough = Address;
		});

		Memory::ServerScanProcess("NS_ABTQ"_Protect, [] (uint64_t Address) {
			Global::Vars::g_AddBasketToQueue = Address;
		});

		Memory::ServerScanProcess("NS_AITB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_AddItemToBasket = Address;
		});

		Memory::ServerScanProcess("SSIR"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendSessionInfoRequest = Address;
		});

		Memory::ServerScanProcess("RSINFO"_Protect, [] (uint64_t Address) {
			Global::Vars::g_RsInfo = (Rage::Network::RsInfo*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("GSIFGS"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetSessionInfoFromGs = Address;
		});

		Memory::ServerScanProcess("SNER"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendNetworkEventReply = Address;
		});

		Memory::ServerScanProcess("SNEA"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendNetworkEventAck = Address;
		});

		Memory::ServerScanProcess("SGCE"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SetupGiveControlEvent = Address;
		});

		Memory::ServerScanProcess("SFR"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendFriendRequest = Address;
		});

		Memory::ServerScanProcess("SGCM"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendGlobalChatMessage = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("GCHP"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GlobalChatPtr = *(uint64_t*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("GCPK"_Protect, [] (uint64_t Address) {
			Global::Vars::g_GetClientPlayerKey = Address;
		});

		Memory::ServerScanProcess("NODES_1"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCDynamicEntityGameStateDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_2"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCVehicleGadgetDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_3"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCVehicleHealthDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_4"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCVehicleComponentReservationDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_5"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPedGameStateDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_6"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPedComponentReservationDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_7"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPedScriptGameStateDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_8"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPedTaskTreeDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_9"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPedInventoryDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_10"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPedTaskSequenceDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_11"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPlayerGameStateDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_12"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCPickupCreationDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_13"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCVehicleControlDataNode = Address;
		});

		Memory::ServerScanProcess("NODES_14"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NodeFixCVehicleProximityMigrationDataNode = Address;
		});

		Memory::ServerScanProcess("RWOM"_Protect, [] (uint64_t Address) {
			Global::Vars::g_RenderWaterOnMapIntensity = Memory::GetAddressFromInstruction(Address, 4, 8);
		});

		Memory::ServerScanProcess("MFI"_Protect, [] (uint64_t Address) {
			Global::Vars::g_MapFogIntensity = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("VFXW"_Protect, [] (uint64_t Address) {
			Global::Vars::g_VfxWheel = (Rage::Types::VfxWheel*)Memory::GetAddressFromInstruction(Address);
			Global::Vars::g_VfxWheelCount = *(uint8_t*)(Address + 0xA);
		});

		Memory::ServerScanProcess("FL"_Protect, [] (uint64_t Address) {
			Global::Vars::g_Friends = *(Rage::Network::Friends**)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("SPE"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendPresenceEvent = Address;
		});

		Memory::ServerScanProcess("MSH"_Protect, [] (uint64_t Address) {
			Global::Vars::g_MigrateScriptHost = Address;
		});

		Memory::ServerScanProcess("HPBR"_Protect, [] (uint64_t Address) {
			Global::Vars::g_IsReportedForReason = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("GCRASH"_Protect, [] (uint64_t Address) {
			int ThreadID = *(int*)Memory::GetAddressFromInstruction(Address);
			if (ThreadID) {
				LOG_DEV("Suspending game crasher thread -> ID: %i", ThreadID);
				SuspendThread((HANDLE)ThreadID);
			}
		});

		Memory::ServerScanProcess("CHATINF"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ChatInfo = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("TEXS"_Protect, [] (uint64_t Address) {
			Global::Vars::g_TextureStore = (Rage::Types::TxdStore*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("SMMR"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SendMatchmakingRequest = Address;
		});

		Memory::ServerScanProcess("RXMLS"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ReadXmlString = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("RXMLNS"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ReadXmlNodeString = Memory::GetAddressFromInstruction(Address, 1, 5);
		});

		Memory::ServerScanProcess("CKL"_Protect, [](uint64_t Address) {
			Memory::WriteVector(Memory::GetAddressFromInstruction(Address, 1, 5), { 0xB0, 0x00, 0xC3 });
		});

		Memory::ServerScanProcess("CKK"_Protect, [](uint64_t Address) {
			Memory::WriteVector(Memory::GetAddressFromInstruction(Address, 1, 5), { 0xB0, 0x00, 0xC3 });
		});

		Memory::ServerScanProcess("RULLFBB"_Protect, [](uint64_t Address) {
			Global::Vars::g_ReadUnsignedLongLongBuffer = Address;
		});

		Memory::ServerScanProcess("RNMHFBB"_Protect, [](uint64_t Address) {
			Global::Vars::g_ReadNetMsgHeaderFromBitBuffer = Address;
		});

		Memory::ServerScanProcess("WUITBB"_Protect, [](uint64_t Address) {
			Global::Vars::g_WriteUintToBitBuffer = Address;
		});

		Memory::ServerScanProcess("RYTD"_Protect, [](uint64_t Address) {
			Global::Vars::g_RegisterTexture = Address;
		});

		Memory::ServerScanProcess("ASDFWD"_Protect, [](uint64_t Address) {
			Global::Vars::g_FunctionPatches[2].m_Address = Address;
			Global::Vars::g_FunctionPatches[2].m_Patch = { 0xE9, 0xDF, 0x01, 0x00, 0x00 };
			for (int i = 0; i < 5; i++) Global::Vars::g_FunctionPatches[2].m_Original.push_back(*(uint8_t*)(Global::Vars::g_FunctionPatches[2].m_Address + i));
		});

		Memory::ServerScanProcess("ASDFWE"_Protect, [](uint64_t Address) {
			Global::Vars::g_FunctionPatches[3].m_Address = Address;
			Global::Vars::g_FunctionPatches[3].m_Patch = { 0xC3 };
			Global::Vars::g_FunctionPatches[3].m_Original.push_back(*(uint8_t*)(Global::Vars::g_FunctionPatches[3].m_Address));
		});

		Memory::ServerScanProcess("CGIP"_Protect, [](uint64_t Address) {
			Global::Vars::g_CreateGameInvitePresence = Address;
		});

		Memory::ServerScanProcess("SGIP"_Protect, [](uint64_t Address) {
			Global::Vars::g_SendGameInvitePresence = Address;
		});

		Memory::ServerScanProcess("SGIPT"_Protect, [](uint64_t Address) {
			Global::Vars::g_SendGameInvitePresenceTable = Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("RPFBB"_Protect, [](uint64_t Address) {
			Global::Vars::g_ReadPickupFromBitBuffer = Address;
		});

		Memory::ServerScanProcess("SRE"_Protect, [](uint64_t Address) {
			Global::Vars::g_SendRagdollEvent = Address;
		});

		Menu::Pools::GetPoolManager()->Initialize();
	}

	void Init::GlobalsInGame() {
		Memory::ServerScanProcess("EXPD"_Protect, [](uint64_t Address) {
			Global::Vars::g_ExplosionData = *(Rage::Types::ExplosionData**)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("EXPFX"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ExplosionFX = *(Rage::Types::ExplosionFX**)(Memory::GetAddressFromInstruction(Address) + 0x10);
		});

		Memory::ServerScanProcess("BL"_Protect, [] (uint64_t Address) {
			Global::Vars::g_BlipList = (Rage::Types::BlipList*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("OQ"_Protect, [] (uint64_t Address) {
			Global::Vars::g_OceanQuads.m_Ocean = (Rage::Types::OceanQuadInfo*)Memory::GetAddressFromInstruction(Address);
			Global::Vars::g_OceanQuads.m_Calming = (Rage::Types::OceanQuadInfo*)Memory::GetAddressFromInstruction(Address + 0xC);
			Global::Vars::g_OceanQuads.m_Wave = (Rage::Types::OceanQuadInfo*)Memory::GetAddressFromInstruction(Address + 0x25);
		});

		Memory::ServerScanProcess("WT"_Protect, [] (uint64_t Address) {
			Global::Vars::g_WaterTune = (Rage::Types::WaterTune*)Memory::GetAddressFromInstruction(Address);
		});

		// different on steam
		Memory::ServerScanProcess("NBC"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetworkBaseConfig = (Rage::Network::NetworkBaseConfig*)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("VFX-S"_Protect, [] (uint64_t Address) {
			Global::Vars::g_UIWeather = (Rage::Types::UIWeather*)(Memory::GetAddressFromInstruction(Address) + 0x60);
		});

		Memory::ServerScanProcess("VFX-P"_Protect, [] (uint64_t Address) {
			Global::Vars::g_UIPuddle = *(Rage::Types::UIPuddle**)Memory::GetAddressFromInstruction(Address);
		});

		Memory::ServerScanProcess("SHP"_Protect, [] (uint64_t Address) {
			Global::Vars::g_ScriptHandlerPoolCount = Memory::GetAddressFromInstruction(Address);
			Global::Vars::g_ScriptHandlerPool = *(uint64_t*)(Memory::GetAddressFromInstruction(Address) - 8);
		});
	}

	void Init::ScriptHookThread() {
		Sleep(1500);

		Hook::GetHooking()->CreateScript("SCPW_S"_Protect, "am_mp_property_int"_Protect, 0xADF692B254977C0C, &Hooks::SetCurrentPedWeaponHook);
		Hook::GetHooking()->CreateScript("DCA_S"_Protect, "am_mp_property_int"_Protect, 0xFE99B66D079CF6BC, &Hooks::DisableControlActionHook);

		Hook::GetHooking()->CreateScript("SCPW_SC"_Protect, "am_mp_smpl_interior_int"_Protect, 0xADF692B254977C0C, &Hooks::SetCurrentPedWeaponHook);
		Hook::GetHooking()->CreateScript("DCA_SC"_Protect, "am_mp_smpl_interior_int"_Protect, 0xFE99B66D079CF6BC, &Hooks::DisableControlActionHook);

		Hook::GetHooking()->CreateScript("SCPW_SF"_Protect, "freemode"_Protect, 0xADF692B254977C0C, &Hooks::SetCurrentPedWeaponHook);
		Hook::GetHooking()->CreateScript("DCA_SF"_Protect, "freemode"_Protect, 0xFE99B66D079CF6BC, &Hooks::DisableControlActionHook);
		Hook::GetHooking()->CreateScript("TTT_S"_Protect, "freemode"_Protect, 0x1090044AD1DA76FA, &Hooks::TerminateThisThreadHook);

		Hook::GetHooking()->CreateScript("SWM2_S"_Protect, "shop_controller"_Protect, 0xDC38CC1E35B6A5D7, &Hooks::SetWarningMessage2Hook);
	}

	void Init::Entry(HMODULE hModule) {
		SetUnhandledExceptionFilter(ExceptionFilter);

		srand((unsigned int)time(0));
		Utils::GetConfig()->Initialize();
		Logger::GetLog()->Initialize();

		LOG_DEV("Menu Base: %llx", Global::Vars::g_MenuAddress);

		MODULEINFO ModuleInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(0), &ModuleInfo, sizeof(ModuleInfo));

		Global::Vars::g_GameAddress = (uint64_t)ModuleInfo.lpBaseOfDll;
		Global::Vars::g_GameSize = (uint64_t)ModuleInfo.SizeOfImage;

		std::mt19937 rng(__rdtsc());
		std::uniform_int_distribution<int> val(0, 100);
		Global::Vars::g_SpoofedPeer = val(rng);

		Global::Vars::g_Steam = (uint64_t)GetModuleHandleA("steam_api64.dll"_Protect) > 0;
		// Global::Vars::g_TextureBypass = (uint64_t)malloc(0x50 * 1000);

		if (!(Global::Vars::g_GameWindow = FindWindowA("grcWindow"_Protect, nullptr))) {
			int WindowTimeout = 0;
			while (!Global::Vars::g_GameWindow && !Global::Vars::g_Unloading) {
				if (WindowTimeout >= 20) {
					LOG_INFO("Window not found within timeout, exiting"_Protect);
					GetInit()->Unload();
					return;
				}

				Global::Vars::g_GameWindow = FindWindowA("grcWindow"_Protect, nullptr);
				Sleep(1000);
				WindowTimeout++;
			}
		}

		GetInit()->AntiCheat();

		if (!Auth::Run()) {
			GetInit()->Unload();
			return;
		}

		Auth::ProcessCachedMetrics();
		GetInit()->Globals();

		Utils::GetThreadPool()->Add("HBT"_Protect, HeartbeatThread);
		Hook::GetHooking()->Create("SGCMH"_Protect, Global::Vars::g_SendGlobalChatMessage, Hooks::SendGameMessageHook, &Hooks::OriginalSendGameMessage);
		
		Memory::ScanProcess({ "48 83 EC 28 45 33 C0 4C 63 D1 48 85 D2 75 19 44 38 05 ? ? ? ? 75 14 48 8D 15 ? ? ? ? 4A 8B 14 D2"_Protect, 0, "SDSDSD15"_Protect, true }, [](uint64_t Address) {
			Menu::Hooks::g_GraphicCrashTable = Memory::GetAddressFromInstruction(Address + 0x18);
			Hook::GetHooking()->Create("SDSDSD15"_Protect, Address, Hooks::sub_141358C18, &Hooks::Originalsub_141358C18);
		});

		Memory::ScanProcess({ "48 8B 01 4C 8B C1 48 3B D0 75 21 48 8B 40 18 48 89 01 33 C0 48 89 42 18 48 8B 09 48 85 C9 74 06 48 89 41 20 EB 4E 49"_Protect, 0, "HAHAHA"_Protect, true }, [](uint64_t Address) {
			Hook::GetHooking()->Create("HAHAHA"_Protect, Address, Hooks::sub_1415BBB74, &Hooks::Originalsub_1415BBB74);
		});

		Memory::ScanProcess({ "44 89 81 ? ? ? ? 89 91 ? ? ? ? C6 05 ? ? ? ? ? C3"_Protect, 0, "HAHAHA2"_Protect, true }, [](uint64_t Address) {
			Hook::GetHooking()->Create("HAHAHA2"_Protect, Address, Hooks::ScriptWorldStateCrashHook, &Hooks::OriginalScriptWorldStateCrash);
		});

		Memory::ServerScanProcess("UPSS"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("UPSS"_Protect, Address, Hooks::UpdatePlayerScriptStatusHook, &Hooks::OriginalUpdatePlayerScriptStatus);
		});

		Memory::ServerScanProcess("WATDWB"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->CreateVirtual("WATDWB"_Protect, Memory::GetAddressFromInstruction(Address), 23, &Hooks::WriteArrayToDataWriterBufferHook, &Hooks::OriginalWriteArrayToDataWriterBuffer);
		});

		Memory::ServerScanProcess("HHJR"_Protect, [](uint64_t Address) {
			Global::Vars::g_FunctionPatches[4].m_Address = Address + 0x70;

			if (Global::Vars::g_Steam) {
				Global::Vars::g_FunctionPatches[4].m_Patch = { 0x41, 0xBC, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x70, 0x06, 0x00, 0x00 };
			} else {
				Global::Vars::g_FunctionPatches[4].m_Patch = { 0x41, 0xBC, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x82, 0x06, 0x00, 0x00 };
			}
			
			for (int i = 0; i < Global::Vars::g_FunctionPatches[4].m_Patch.size(); i++) Global::Vars::g_FunctionPatches[4].m_Original.push_back(*(uint8_t*)(Global::Vars::g_FunctionPatches[4].m_Address + i));

			Hook::GetHooking()->Create("HHJR"_Protect, Address, Hooks::HostHandleJoinRequestHook, &Hooks::OriginalHostHandleJoinRequest);
		});

		Memory::ScanProcess({ "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 85 ? ? ? ? 8B 9D"_Protect, 0, "PRESFIX", true }, [](uint64_t Address) {
			Global::Vars::g_FunctionPatches[5].m_Address = Address;
			Global::Vars::g_FunctionPatches[5].m_Patch = { 0xB0, 0x01, 0x90, 0x90, 0x90 };
			for (int i = 0; i < Global::Vars::g_FunctionPatches[5].m_Patch.size(); i++) Global::Vars::g_FunctionPatches[5].m_Original.push_back(*(uint8_t*)(Global::Vars::g_FunctionPatches[5].m_Address + i));
		});

		Memory::ServerScanProcess("PDN1"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("PDN1"_Protect, Address, Hooks::ParseDataNodeHook, &Hooks::OriginalParseDataNode);
		});

		Memory::ServerScanProcess("PDN2"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("PDN2"_Protect, Address, Hooks::ParseDataNode2Hook, &Hooks::OriginalParseDataNode2);
		});

		Memory::ServerScanProcess("RID0"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("RID0"_Protect, Address, Hooks::RockstarIDWriterHook, &Hooks::OriginalRockstarIDWriter);
		});

		Memory::ServerScanProcess("SBULLET"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("SBULLET"_Protect, Address, Hooks::ShootBulletHook, &Hooks::OriginalShootBullet);
		});

		Memory::ServerScanProcess("WPHDN"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("WPHDN"_Protect, Address, Hooks::WritePedHealthDataNodeHook, &Hooks::OriginalWritePedHealthDataNode);
		});

		Memory::ServerScanProcess("WPGDN"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("WPGDN"_Protect, Address, Hooks::WritePlayerGameStateDataNodeHook, &Hooks::OriginalWritePlayerGameStateDataNode);
		});

		Memory::ServerScanProcess("RBLM"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("RBLM"_Protect, Address, Hooks::ReadBlacklistMessageHook, &Hooks::OriginalReadBlacklistMessage);
		});

		Memory::ServerScanProcess("RND"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("RND"_Protect, Memory::GetAddressFromInstruction(Address, 1, 5), Hooks::ReadNodeDataHook, &Hooks::OriginalReadNodeData);

			Global::Vars::g_FunctionPatches[0].m_Address = Address + 5;
			Global::Vars::g_FunctionPatches[0].m_Patch = { 0xE9, 0xD2, 0x01, 0x00, 0x00 };
			for (int i = 0; i < 5; i++) Global::Vars::g_FunctionPatches[0].m_Original.push_back(*(uint8_t*)(Global::Vars::g_FunctionPatches[0].m_Address + i));

			Global::Vars::g_FunctionPatches[1].m_Address = Address + 0x1EC;
			Global::Vars::g_FunctionPatches[1].m_Patch = { 0xC3 };
			Global::Vars::g_FunctionPatches[1].m_Original.push_back(*(uint8_t*)(Global::Vars::g_FunctionPatches[1].m_Address));
		});

		Memory::ServerScanProcess("SDSDSD6"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD6"_Protect, Address, Hooks::sub_140AFB59C, &Hooks::Originalsub_140AFB59C);
		});

		Memory::ServerScanProcess("SDSDSD7"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD7"_Protect, Address, Hooks::sub_140E8407C, &Hooks::Originalsub_140E8407C);
		});

		Memory::ServerScanProcess("SDSDSD2"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD2"_Protect, Address, Hooks::sub_140A1BDF4, &Hooks::Originalsub_140A1BDF4);
		});

		Memory::ServerScanProcess("SDSDSD3"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD3"_Protect, Address, Hooks::sub_140E72620, &Hooks::Originalsub_140E72620);
		});

		Memory::ServerScanProcess("SDSDSD4"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD4"_Protect, Address, Hooks::sub_140E606D8, &Hooks::Originalsub_140E606D8);
		});

		Memory::ServerScanProcess("SDSDSD5"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD5"_Protect, Address, Hooks::sub_140AEA62C, &Hooks::Originalsub_140AEA62C);
		});

		Memory::ServerScanProcess("SDSDSD8"_Protect, [] (uint64_t Address) {
			Memory::Nop(Address + 0x31, 5);
		});

		Memory::ServerScanProcess("SDSDSD9"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD9"_Protect, Address, Hooks::sub_140730894, &Hooks::Originalsub_140730894);
		});

		Memory::ServerScanProcess("SDSDSD13"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD13"_Protect, Address, Hooks::sub_140E7C4E8, &Hooks::Originalsub_140E7C4E8);
		});

		Memory::ServerScanProcess("SDSDSD14"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD14"_Protect, Address, Hooks::sub_141644B58, &Hooks::Originalsub_141644B58);
		});

		Memory::ServerScanProcess("SDSDSD12"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD12"_Protect, Address, Hooks::sub_140632DD4, &Hooks::Originalsub_140632DD4);
		});

		Memory::ServerScanProcess("SDSDSD11"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD11"_Protect, Address, Hooks::CutscenePedWriteHook, &Hooks::OriginalCutscenePedWrite);
		});

		Memory::ServerScanProcess("SDSDSD1"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("SDSDSD1"_Protect, Address, Hooks::sub_14113A75C, &Hooks::Originalsub_14113A75C);
		});

		Memory::ServerScanProcess("SDSDSD10"_Protect, [](uint64_t Address) {
			Hooks::g_2Take1CrashLocation = Address + 5;
			Hook::GetHooking()->Create("SDSDSD10"_Protect, Memory::GetAddressFromInstruction(Address, 1, 5), Hooks::AccessNetObjHook, &Hooks::OriginalAccessNetObj);
		});

		Memory::ServerScanProcess("MBT"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("MBT"_Protect, Address, Hooks::MessageBoxTerminationHook, &Hooks::OriginalMessageBoxTermination);
		});

		Memory::ServerScanProcess("PRU"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("PRU"_Protect, Address, Hooks::ProcessRelayUsageHook, &Hooks::OriginalProcessRelayUsage);
		});

		Memory::ServerScanProcess("PCC"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("PCC"_Protect, Address, Hooks::PackCloneCreateHook, &Hooks::OriginalPackCloneCreate);
		});

		Memory::ServerScanProcess("PCS"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("PCS"_Protect, Address, Hooks::PackCloneSyncHook, &Hooks::OriginalPackCloneSync);
		});

		Memory::ServerScanProcess("HDNM"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("HDNM"_Protect, Address, Hooks::HandleNegotiationMsgHook, &Hooks::OriginalHandleNegotiationMsg);
		});

		Memory::ServerScanProcess("HDPR"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("HDPR"_Protect, Address, Hooks::HostDesyncPlayerRequestHook, &Hooks::OriginalHostDesyncPlayerRequest);
		});

		Memory::ServerScanProcess("GPT"_Protect, [](uint64_t Address) {
			Global::Vars::g_GetPeerToken = Address;
			Hook::GetHooking()->Create("GPT"_Protect, Address, Hooks::GetPeerTokenHook, &Hooks::OriginalGetPeerToken);
		});

		Memory::ServerScanProcess("RMMR"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("RMMR"_Protect, Address, Hooks::ReadMatchmakingResponseHook, &Hooks::OriginalReadMatchmakingResponse);
		});

		Memory::ServerScanProcess("WMMR"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("WMMR"_Protect, Address, Hooks::WriteMatchmakingRequestHook, &Hooks::OriginalWriteMatchmakingRequest);
		});

		Memory::ServerScanProcess("WESTR"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("WESTR"_Protect, Memory::GetAddressFromInstruction(Address, 1, 5), Hooks::WriteEncryptedStringToRequestHook, &Hooks::OriginalWriteEncryptedStringToRequest);
		});

		Memory::ServerScanProcess("STT"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("STT"_Protect, Address, Hooks::ScriptThreadTickHook, &Hooks::OriginalScriptThreadTick);
		});

		Memory::ServerScanProcess("GGXTL"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("GGXTL"_Protect, Address, Hooks::GetGxtLabelHook, &Hooks::OriginalGetGxtLabel);
		});

		Memory::ServerScanProcess("PGDN"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("PGDN"_Protect, Address, Hooks::PlayerGamerDataNodeHook, &Hooks::OriginalPlayerGamerDataNode);
		});

		Memory::ServerScanProcess("HRP"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("HRP"_Protect, Address, Hooks::HasRosPrivilegeHook, &Hooks::OriginalHasRosPrivilege);
		});

		Memory::ServerScanProcess("SJRM"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SJRM"_Protect, Memory::GetAddressFromInstruction(Address, 1, 5), Hooks::SendJoinRequestHook, &Hooks::OriginalSendJoinRequest);
		});

		Memory::ServerScanProcess("RSIR"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("RSIR"_Protect, Address, Hooks::ReadSessionInfoResponseHook, &Hooks::OriginalReadSessionInfoResponse);
		});

		Memory::ServerScanProcess("SAGTSC"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SAGTSC"_Protect, Address, Hooks::SendAddGamerToSessionCmdHook, &Hooks::OriginalSendAddGamerToSessionCmd);
		});

		Memory::ServerScanProcess("RCC"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("RCC"_Protect, Address, Hooks::ReceiveCloneCreateHook, &Hooks::OriginalReceiveCloneCreate);
		});

		Memory::ServerScanProcess("RCS"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("RCS"_Protect, Address, Hooks::ReceiveCloneSyncHook, &Hooks::OriginalReceiveCloneSync);
		});

		Memory::ServerScanProcess("PADN"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("PADN"_Protect, Address, Hooks::PhysicalAttachDataNodeHook, &Hooks::OriginalPhysicalAttachDataNode);
		});

		Memory::ServerScanProcess("PEDADN"_Protect, [] (uint64_t Address) {
			uint64_t Class = Memory::GetAddressFromInstruction(Address);
			if (Class) {
				Hook::GetHooking()->CreateVirtual("PEDADN"_Protect, Class, 3, Hooks::PedAttachDataNodeHook, &Hooks::OriginalPedAttachDataNode);
			}
		});

		Memory::ServerScanProcess("SEM"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SEM"_Protect, Address, Hooks::SyncEntityMatrixHook, &Hooks::OriginalSyncEntityMatrix);
		});

		Memory::ServerScanProcess("RUTDRB"_Protect, [] (uint64_t Address) {
			Global::Vars::g_SyncDataReaderVtable = Memory::GetAddressFromInstruction(Address);

			Hook::GetHooking()->CreateVirtual("KEKW1"_Protect, Memory::GetAddressFromInstruction(Address), 1, &Hooks::ReadUnsignedIntFromBitBufferHook, &Hooks::OriginalReadUnsignedIntFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW2"_Protect, Memory::GetAddressFromInstruction(Address), 2, &Hooks::ReadUnsignedShortFromBitBufferHook, &Hooks::OriginalReadUnsignedShortFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW3"_Protect, Memory::GetAddressFromInstruction(Address), 3, &Hooks::ReadUnsignedCharFromBitBufferHook, &Hooks::OriginalReadUnsignedCharFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW4"_Protect, Memory::GetAddressFromInstruction(Address), 4, &Hooks::ReadIntFromBitBufferHook, &Hooks::OriginalReadIntFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW5"_Protect, Memory::GetAddressFromInstruction(Address), 5, &Hooks::ReadShortFromBitBufferHook, &Hooks::OriginalReadShortFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW6"_Protect, Memory::GetAddressFromInstruction(Address), 6, &Hooks::ReadCharFromBitBufferHook, &Hooks::OriginalReadCharFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW7"_Protect, Memory::GetAddressFromInstruction(Address), 7, &Hooks::ReadBoolFromBitBufferHook, &Hooks::OriginalReadBoolFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW8"_Protect, Memory::GetAddressFromInstruction(Address), 8, &Hooks::ReadLongLongFromBitBufferHook, &Hooks::OriginalReadLongLongFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW9"_Protect, Memory::GetAddressFromInstruction(Address), 9, &Hooks::ReadIntFromBitBufferHook, &Hooks::OriginalReadIntFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW10"_Protect, Memory::GetAddressFromInstruction(Address), 10, &Hooks::ReadShortFromBitBufferHook, &Hooks::OriginalReadShortFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW11"_Protect, Memory::GetAddressFromInstruction(Address), 11, &Hooks::ReadCharFromBitBufferHook, &Hooks::OriginalReadCharFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW12"_Protect, Memory::GetAddressFromInstruction(Address), 12, &Hooks::ReadUnsignedLongLongFromBitBufferHook, &Hooks::OriginalReadUnsignedLongLongFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW13"_Protect, Memory::GetAddressFromInstruction(Address), 13, &Hooks::ReadUnsignedIntFromBitBufferHook, &Hooks::OriginalReadUnsignedIntFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW14"_Protect, Memory::GetAddressFromInstruction(Address), 14, &Hooks::ReadUnsignedShortFromBitBufferHook, &Hooks::OriginalReadUnsignedShortFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW15"_Protect, Memory::GetAddressFromInstruction(Address), 15, &Hooks::ReadUnsignedCharFromBitBufferHook, &Hooks::OriginalReadUnsignedCharFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW16"_Protect, Memory::GetAddressFromInstruction(Address), 16, &Hooks::ReadFloatFromBitBufferHook, &Hooks::OriginalReadFloatFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW17"_Protect, Memory::GetAddressFromInstruction(Address), 17, &Hooks::ReadUnsignedFloatFromBitBufferHook, &Hooks::OriginalReadUnsignedFloatFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW18"_Protect, Memory::GetAddressFromInstruction(Address), 18, &Hooks::ReadNetworkIdFromBitBufferHook, &Hooks::OriginalReadNetworkIdFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW19"_Protect, Memory::GetAddressFromInstruction(Address), 23, &Hooks::ReadArrayFromBitBufferHook, &Hooks::OriginalReadArrayFromBitBuffer);
			Hook::GetHooking()->CreateVirtual("KEKW20"_Protect, Memory::GetAddressFromInstruction(Address), 24, &Hooks::ReadStringFromBitBufferHook, &Hooks::OriginalReadStringFromBitBuffer);
		});

		Memory::ServerScanProcess("HMNSH"_Protect, [](uint64_t Address) {
			Global::Vars::g_ReadMsgNewHost = Memory::GetAddressFromInstruction(Address, 1, 5);
			Hook::GetHooking()->Create("HMNSH"_Protect, Address - 0x6D, Hooks::HandleMsgNewScriptHostHook, &Hooks::OriginalHandleMsgNewScriptHost);
		});

		Memory::ServerScanProcess("RCR"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("RCR"_Protect, Address, Hooks::ReceiveCloneRemoveHook, &Hooks::OriginalReceiveCloneRemove);
		});

		Memory::ServerScanProcess("HMKP"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("HMKP"_Protect, Address, Hooks::HandleMsgKickPlayerHook, &Hooks::OriginalHandleMsgKickPlayer);
		});

		Memory::ServerScanProcess("HNE"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("HNE"_Protect, Address, Hooks::HandleNetworkEventHook, &Hooks::OriginalHandleNetworkEvent);
		});

		Memory::ServerScanProcess("RBV"_Protect, [] (uint64_t Address) {
			Global::Vars::g_NetStubLogginInterface = Memory::GetAddressFromInstruction(Address + 0x17);
			Hook::GetHooking()->Create("RBV"_Protect, Address, Hooks::ReceiveBroadcastVariablesHook, &Hooks::OriginalReceiveBroadcastVariables);
		});

		Memory::ServerScanProcess("WUTDWB"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("WUTDWB"_Protect, Address, Hooks::WriteUintToDataWriterBufferHook, &Hooks::OriginalWriteUintToDataWriterBuffer);
		});

		Memory::ServerScanProcess("RTMH"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("RTMH"_Protect, Address, Hooks::ReceiveTextMsgHook, &Hooks::OriginalReceiveTextMsg);
		});

		Memory::ServerScanProcess("RCMH"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("RCMH"_Protect, Address, Hooks::ReceiveChatMsgHook, &Hooks::OriginalReceiveChatMsg);
		});

		Memory::ServerScanProcess("RTMBB"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("RTMBB"_Protect, Memory::GetAddressFromInstruction(Address, 1, 5), Hooks::ReadTextMessageBitbufferHook, &Hooks::OriginalReadTextMessageBitbuffer);
		});

		Memory::ServerScanProcess("SPM"_Protect, [](uint64_t Address) {
			uint64_t Func = Memory::GetAddressFromInstruction(Address, 1, 5);
			if (Func) {
				uint64_t Class = Memory::GetAddressFromInstruction(Func + 0x43);
				if (Class) {
					Hook::GetHooking()->CreateVirtual("SPM"_Protect, Class, 19, Hooks::SyncPlayerModelHook, &Hooks::OriginalSyncPlayerModel);
				}
			}
		});

		Memory::ServerScanProcess("RCMBB"_Protect, [](uint64_t Address) {
			Hook::GetHooking()->Create("RCMBB"_Protect, Memory::GetAddressFromInstruction(Address, 1, 5), Hooks::ReadChatMessageBitbufferHook, &Hooks::OriginalReadChatMessageBitbuffer);
		});

		Memory::ServerScanProcess("HNGE"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("HNGE"_Protect, Address, Hooks::HandleNetworkGameEventHook, &Hooks::OriginalHandleNetworkGameEvent);
		});

		Memory::ServerScanProcess("APPI"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("APPI"_Protect, Address, Hooks::ApplyPlayerPhysicalIndexHook, &Hooks::OriginalApplyPlayerPhysicalIndex);
		});

		Memory::ServerScanProcess("HPE"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("HPE"_Protect, Memory::GetAddressFromInstruction(Address, 1, 5), Hooks::HandlePresenceEventHook, &Hooks::OriginalHandlePresenceEvent);
		});

		Memory::ServerScanProcess("AWM"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("AWM"_Protect, Address, Hooks::ApplyWeatherMetaHook, &Hooks::OriginalApplyWeatherMeta);
		});

		Memory::ServerScanProcess("SCPCS"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SCPCS"_Protect, Address, Hooks::ScProfanityCheckStringStatusHook, &Hooks::OriginalScProfanityCheckStringStatus);
		});

		Memory::ServerScanProcess("SCPCSC"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SCPCSC"_Protect, Address, Hooks::GetChatProfanityStatusHook, &Hooks::OriginalGetChatProfanityStatus);
		});

		Memory::ServerScanProcess("FGES"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("FGES"_Protect, Address, Hooks::FormatGameEngineStringHook, &Hooks::OriginalFormatGameEngineString);
		});

		Memory::ServerScanProcess("GGESLC"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("GGESLC"_Protect, Address, Hooks::GetGameEngineStringLineCountHook, &Hooks::OriginalGetGameEngineStringLineCount);
		});

		Memory::ServerScanProcess("SPH"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("SPH"_Protect, Address, Hooks::SyncPedOrientationHook, &Hooks::OriginalSyncPedOrientation);
		});

		Memory::ServerScanProcess("PMDN"_Protect, [] (uint64_t Address) {
			Hook::GetHooking()->Create("PMDN"_Protect, Address, Hooks::PedMovementDataNodeHook, &Hooks::OriginalPedMovementDataNode);
		});

		Memory::ServerScanProcess("SULL"_Protect, [](uint64_t Address) {
			Global::Vars::g_ArxanRestoreAddresses[0] = Address;
			memcpy(Global::Vars::g_ArxanRestoreBytes[0][0], (void*)Address, sizeof(Global::Vars::g_ArxanRestoreBytes[0][0]));
			Hook::GetHooking()->Create("SULL"_Protect, Address, Hooks::WriteNetMsgIdentifierHook, &Hooks::OriginalWriteNetMsgIdentifier);
			memcpy(Global::Vars::g_ArxanRestoreBytes[0][1], (void*)Address, sizeof(Global::Vars::g_ArxanRestoreBytes[0][1]));
		});

		Memory::ServerScanProcess("WDD"_Protect, [](uint64_t Address) {
			Global::Vars::g_ArxanRestoreAddresses[2] = Address;
			memcpy(Global::Vars::g_ArxanRestoreBytes[2][0], (void*)Address, sizeof(Global::Vars::g_ArxanRestoreBytes[2][0]));
			Hook::GetHooking()->Create("WDD"_Protect, Address, Hooks::WriteUnsignedLongLongToBitBufferHook, &Hooks::OriginalWriteUnsignedLongLongToBitBuffer);
			memcpy(Global::Vars::g_ArxanRestoreBytes[2][1], (void*)Address, sizeof(Global::Vars::g_ArxanRestoreBytes[2][1]));
		});

		if (Global::Vars::g_Steam) {
			Memory::ScanProcess({ "E9 69 AC 3A FF"_Protect, 0, "ARX1-S"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX1-S"_Protect, Address, Hooks::RockstarIntegrityCheckStartHook, &Hooks::OriginalRockstarIntegrityCheckStart);
			});

			Memory::ScanProcess({ "E9 01 10 1E 00"_Protect, 0, "ARX1-E"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX1-E"_Protect, Address, Hooks::RockstarIntegrityCheckEndHook, &Hooks::OriginalRockstarIntegrityCheckEnd);
			});

			Memory::ScanProcess({ "E9 D7 8D D9 FF"_Protect, 0, "ARX2-S"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX2-S"_Protect, Address, Hooks::RockstarIntegrityCheckStart2Hook, &Hooks::OriginalRockstarIntegrityCheckStart2);
			});

			Memory::ScanProcess({ "E9 1C 4C 18 00"_Protect, 0, "ARX2-E"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX2-E"_Protect, Address, Hooks::RockstarIntegrityCheckEnd2Hook, &Hooks::OriginalRockstarIntegrityCheckEnd2);
			});

			Memory::ScanProcess({ "E9 2E 5E 0A 00"_Protect, 0, "ARX3-S"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX3-S"_Protect, Address, Hooks::RockstarIntegrityCheckStart3Hook, &Hooks::OriginalRockstarIntegrityCheckStart3);
			});

			Memory::ScanProcess({ "E9 25 63 E1 FC"_Protect, 0, "ARX3-E"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX3-E"_Protect, Address, Hooks::RockstarIntegrityCheckEnd3Hook, &Hooks::OriginalRockstarIntegrityCheckEnd3);
			});

			Memory::ScanProcess({ "E9 1A 57 19 00"_Protect, 0, "ARX4-S"_Protect, true }, [](uint64_t Address) {
				Hook::GetHooking()->Create("ARX4-S"_Protect, Address, Hooks::RockstarIntegrityCheckStart4Hook, &Hooks::OriginalRockstarIntegrityCheckStart4);
			});

			Memory::ScanProcess({ "E9 4C 86 01 00"_Protect, 0, "ARX3-E"_Protect, true }, [](uint64_t Address) {
				Hook::GetHooking()->Create("ARX4-E"_Protect, Address, Hooks::RockstarIntegrityCheckEnd4Hook, &Hooks::OriginalRockstarIntegrityCheckEnd4);
			});
		} else {
			Memory::ScanProcess({ "E9 DD BB 1A FC"_Protect, 0, "ARX1-S"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX1-S"_Protect, Address, Hooks::RockstarIntegrityCheckStartHook, &Hooks::OriginalRockstarIntegrityCheckStart);
			});

			Memory::ScanProcess({ "E9 97 8D B9 00"_Protect, 0, "ARX1-E"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX1-E"_Protect, Address, Hooks::RockstarIntegrityCheckEndHook, &Hooks::OriginalRockstarIntegrityCheckEnd);
			});

			Memory::ScanProcess({ "E9 24 AD 21 03"_Protect, 0, "ARX2-S"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX2-S"_Protect, Address, Hooks::RockstarIntegrityCheckStart2Hook, &Hooks::OriginalRockstarIntegrityCheckStart2);
			});

			Memory::ScanProcess({ "E9 F7 FD 42 FF"_Protect, 0, "ARX2-E"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX2-E"_Protect, Address, Hooks::RockstarIntegrityCheckEnd2Hook, &Hooks::OriginalRockstarIntegrityCheckEnd2);
			});

			Memory::ScanProcess({ "E9 3B 9A A3 01"_Protect, 0, "ARX3-S"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX3-S"_Protect, Address, Hooks::RockstarIntegrityCheckStart3Hook, &Hooks::OriginalRockstarIntegrityCheckStart3);
			});

			Memory::ScanProcess({ "E9 DB CC A4 00"_Protect, 0, "ARX3-E"_Protect, true }, [] (uint64_t Address) {
				Hook::GetHooking()->Create("ARX3-E"_Protect, Address, Hooks::RockstarIntegrityCheckEnd3Hook, &Hooks::OriginalRockstarIntegrityCheckEnd3);
			});

			Memory::ScanProcess({ "E9 B4 45 97 FF"_Protect, 0, "ARX4-S"_Protect, true }, [](uint64_t Address) {
				Hook::GetHooking()->Create("ARX4-S"_Protect, Address, Hooks::RockstarIntegrityCheckStart4Hook, &Hooks::OriginalRockstarIntegrityCheckStart4);
			});

			Memory::ScanProcess({ "E9 AD 4B C7 00"_Protect, 0, "ARX4-E"_Protect, true }, [](uint64_t Address) {
				Hook::GetHooking()->Create("ARX4-E"_Protect, Address, Hooks::RockstarIntegrityCheckEnd4Hook, &Hooks::OriginalRockstarIntegrityCheckEnd4);
			});
		}

		while (*Global::Vars::g_GameState != GameStatePlaying) Sleep(500);

		GetInit()->GlobalsInGame();

		Hook::GetHooking()->Create("GPA"_Protect, (uint64_t)&GetProcAddress, &Hooks::GetProcAddressHook, &Hooks::OriginalGetProcAddress);
		
		int ScriptHookError = 0;
		if (!Hook::GetHooking()->CreateScript("SGT_S"_Protect, "main_persistent"_Protect, 0x767FBC2AC802EF3D, &Hooks::StatGetIntHook, &ScriptHookError)) {
			LOG_CUSTOM_INFO("Hook"_Protect, "Failed to hook SGT_S (Err=%i)"_Protect, ScriptHookError);
			GetInit()->Unload();
			return;
		}

		Utils::GetThreadPool()->Add("SCT"_Protect, [] { Rage::API::GetSocialClub()->RequestThread(); });
		Utils::GetThreadPool()->Add("GIPT"_Protect, GeoIPThread);
		Utils::GetThreadPool()->Add("SHT"_Protect, ScriptHookThread);

		for (auto& Sig : Global::Auth::g_Signatures) {
			Sig.second.Reset();
		}
	}

	Init* GetInit() {
		static Init Instance;
		return &Instance;
	}
}