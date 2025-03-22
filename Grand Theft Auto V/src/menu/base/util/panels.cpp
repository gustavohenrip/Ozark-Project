#include "panels.hpp"
#include "menu/base/renderer.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/player_manager.hpp"
#include "menu/base/base.hpp"
#include "utils/va.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/submenus/main/network/players/teleport/network_teleport_apartment.hpp"
#include "menu/base/submenus/main/settings/videos.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_ip.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_name.hpp"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_rid.hpp"
#include "menu/base/submenus/main/network/network_spoofing.hpp"
#include "rage/engine.hpp"
#include"utils/utils.hpp"
#include <algorithm>

namespace Menu::Panels {
	TranslationString T_Cherax("Cherax", true, true);
	TranslationString T_Impulse("Impulse", true, true);
	TranslationString T_ImpulseVIP("Impulse VIP", true, true);
	TranslationString T_ImpulseStaff("Impulse Staff", true, true);
	TranslationString T_ImpulseBlock("Impulse Block", true, true);
	TranslationString T_Name("Name", true, true);
	TranslationString T_Rank("Rank", true, true);
	TranslationString T_Health("Health", true, true);
	TranslationString T_Invalid("Invalid", true, true);
	TranslationString T_Dead("Dead", true, true);
	TranslationString T_Full("Full", true, true);
	TranslationString T_Cash("Cash", true, true);
	TranslationString T_Armor("Armor", true, true);
	TranslationString T_Bank("Bank", true, true);
	TranslationString T_WantedLevel("Wanted Level", true, true);
	TranslationString T_KDRatio("K/D Ratio", true, true);
	TranslationString T_Ammo("Ammo", true, true);
	TranslationString T_None("None", true, true);
	TranslationString T_Weapon("Weapon", true, true);
	TranslationString T_Unarmed("Unarmed", true, true);
	TranslationString T_Unknown("Unknown", true, true);
	TranslationString T_Zone("Zone", true, true);
	TranslationString T_Street("Street", true, true);
	TranslationString T_Coords("Coords", true, true);
	TranslationString T_Heading("Heading", true, true);
	TranslationString T_North("North", true, true);
	TranslationString T_East("East", true, true);
	TranslationString T_South("South", true, true);
	TranslationString T_West("West", true, true);
	TranslationString T_Distance("Distance", true, true);
	TranslationString T_Speed("Speed", true, true);
	TranslationString T_Passive("Passive", true, true);
	TranslationString T_Invisible("Invisible", true, true);
	TranslationString T_Yes("Yes", true, true);
	TranslationString T_No("No", true, true);
	TranslationString T_RID("RID", true, true);
	TranslationString T_Hidden("Hidden", true, true);
	TranslationString T_IP("IP", true, true);
	TranslationString T_Vehicle("Vehicle", true, true);
	TranslationString T_Crew("Crew", true, true);
	TranslationString T_CrewTag("Crew Tag", true, true);
	TranslationString T_CrewID("Crew ID", true, true);
	TranslationString T_CrewRank("Crew Rank", true, true);
	TranslationString T_UsingController("Using Controller", true, true);
	TranslationString T_UsingKeyboard("Using Keyboard", true, true);
	TranslationString T_Badsport("Badsport", true, true);
	TranslationString T_Cheater("Cheater", true, true);
	TranslationString T_Toxic("Toxic", true, true);
	TranslationString T_MutedMe("Muted Me", true, true);
	TranslationString T_OffTheRadar("Off the Radar", true, true);
	TranslationString T_RevealPlayers("Reveal Players", true, true);
	TranslationString T_RDeveloper("R* Developer", true, true);
	TranslationString T_RQADeveloper("R* QA Tester", true, true);
	TranslationString T_InvalidModel("Invalid Model", true, true);
	TranslationString T_SpoofedName("Spoofed Name", true, true);
	TranslationString T_SpoofedPeer("Spoofed Peer", true, true);
	TranslationString T_SpoofedRID("Spoofed RID", true, true);
	TranslationString T_SpoofedIP("Spoofed IP", true, true);
	TranslationString T_CRCFlood("CRC Flood", true, true);
	TranslationString T_CRCCompromised("CRC Compromised", true, true);
	TranslationString T_CRCNotReplied("CRC Not Replied", true, true);
	TranslationString T_InfoChange1("Info Change 1", true, true);
	TranslationString T_InfoChange2("Info Change 2", true, true);
	TranslationString T_InfoChange4("Info Change 4", true, true);
	TranslationString T_InfoChange5("Info Change 5", true, true);
	TranslationString T_InfoChange6("Info Change 6", true, true);
	TranslationString T_InfoChange7("Info Change 7", true, true);
	TranslationString T_Arxan("Arxan", true, true);
	TranslationString T_SpoofedChat("Spoofed Chat", true, true);
	TranslationString T_SpoofedText("Spoofed Text", true, true);
	TranslationString T_CrashAttempt("Crash Attempt", true, true);
	TranslationString T_WeatherChange("Weather Change", true, true);
	TranslationString T_Spectating("Spectating", true, true);
	TranslationString T_ExplosiveAmmo("Explosive Ammo", true, true);
	TranslationString T_ExplosiveMelee("Explosive Melee", true, true);
	TranslationString T_FireAmmo("Fire Ammo", true, true);
	TranslationString T_SuperJump("Super Jump", true, true);
	TranslationString T_MoneyDrop("Money Drop", true, true);
	TranslationString T_RPDrop("RP Drop", true, true);
	TranslationString T_QuantumKick("Quantum Kick", true, true);
	TranslationString T_RAntiCheat("R* Anti Cheat", true, true);
	TranslationString T_ScriptMigration("Script Migration", true, true);
	TranslationString T_Profanity("Profanity", true, true);
	TranslationString T_JoinTimeout("Join Timeout", true, true);
	TranslationString T_InfoMismatch("Info Mismatch", true, true);
	TranslationString T_Property("Property", true, true);
	TranslationString T_Properties("Properties", true, true);
	TranslationString T_OfficeGarage("Office Garage", true, true);
	TranslationString T_Clubhouse("Clubhouse", true, true);
	TranslationString T_Office("Office", true, true);
	TranslationString T_RacesWon("Races Won", true, true);
	TranslationString T_DeathmatchesWon("Deathmatches Won", true, true);
	TranslationString T_DartsWon("Darts Won", true, true);
	TranslationString T_ArmWrestlingWon("Arm Wrestling Won", true, true);
	TranslationString T_TennisWon("Tennis Won", true, true);
	TranslationString T_GolfWon("Golf Won", true, true);
	TranslationString T_ShootingRangeWon("Shooting Range Won", true, true);
	TranslationString T_Deaths("Deaths", true, true);
	TranslationString T_MissionsCreated("Missions Created", true, true);
	TranslationString T_Zip("Zip", true, true);
	TranslationString T_Region("Region", true, true);
	TranslationString T_City("City", true, true);
	TranslationString T_Country("Country", true, true);
	TranslationString T_Timezone("Timezone", true, true);
	TranslationString T_ISP("ISP", true, true);
	TranslationString T_Org("Org", true, true);
	TranslationString T_Station("Station", true, true);
	TranslationString T_Song("Song", true, true);
	TranslationString T_Artist("Artist", true, true);
	TranslationString T_Placeholder("Placeholder", true, true);
	TranslationString T_SomeTimeAgo("Some Time Ago", true, true);
	TranslationString T_Online("Online", true, true);
	TranslationString T_InSession("In Session", true, true);
	TranslationString T_FirstEncountered("First Encountered", true, true);
	TranslationString T_LastEncountered("Last Encountered", true, true);
	TranslationString T_Preview("Preview", true, true);
	TranslationString T_Enabled("Enabled", true, true);
	TranslationString T_Manufacturer("Manufacturer", true, true);
	TranslationString T_MaxSpeed("Max Speed", true, true);
	TranslationString T_MaxBraking("Max Braking", true, true);
	TranslationString T_MaxTraction("Max Traction", true, true);
	TranslationString T_MaxSeats("Max Seats", true, true);
	TranslationString T_BlacklistedPlayers("Blacklisted Players", true, true);
	TranslationString T_NextSessionHostQueue("Next Session Host Queue", true, true);
	TranslationString T_Peer("Peer", true, true);
	TranslationString T_Second("Second", true, true);
	TranslationString T_Minute("Minute", true, true);
	TranslationString T_Hour("Hour", true, true);
	TranslationString T_Ago("Ago", true, true);
	TranslationString T_Added("Added", true, true);
	TranslationString T_Generic("Generic", true, true);
	TranslationString T_Geo("Geo", true, true);
	TranslationString T_Flags("Flags", true, true);
	TranslationString T_KickQueue("Kick Queue", true, true);
	TranslationString T_Stats("Stats", true, true);
	TranslationString T_Radio("Radio", true, true);
	TranslationString T_StationInfo("Station Info", true, true);
	TranslationString T_RecentPlayer("Recent Player", true, true);
	TranslationString T_PlayerInfo("Player Info", true, true);
	TranslationString T_Friend("Friend", true, true);
	TranslationString T_FriendInfo("Friend Info", true, true);
	TranslationString T_Overseer("Overseer", true, true);
	TranslationString T_OverseerInfo("Overseer Info", true, true);
	TranslationString T_VehiclePreview("Vehicle Preview", true, true);
	TranslationString T_VehiclePreviewInfo("Vehicle Preview Info", true, true);
	TranslationString T_Session("Session", true, true);
	TranslationString T_HostQueueInfo("Host Queue Info", true, true);
	TranslationString T_BlacklistInfo("Blacklist Info", true, true);
	TranslationString T_SessionBrowser("Session Browser", true, true);
	TranslationString T_SessionBrowserInfo("Session Browser Info", true, true);
	TranslationString T_BlacklistedPlayersInfo("Blacklisted Players Info", true, true);

	Math::Vector2<float> Panel::GetRenderingPosition() {
		Math::Vector2<float> Return;

		if (m_Child->m_Column == 0) {
			// Base
			if (::Global::UIVars::g_MenuX >= 0.40f) {
				Return.m_X = ::Global::UIVars::g_MenuX - 0.283f;

				if (!m_Child->m_DoubleSided) {
					Return.m_X += 0.14f; // size of half panel
				}
			} else {
				Return.m_X = ::Global::UIVars::g_MenuX + 0.234f;
			}
		} else {
			// Calculate X offsets
			float CalculatedOffset = 0.f;

			for (int i = 0; i < m_Child->m_Column; i++) {
				if (m_Child->m_Parent->m_ColumnOffset.find(i) != m_Child->m_Parent->m_ColumnOffset.end()) {
					CalculatedOffset += m_Child->m_Parent->m_ColumnOffset[i].m_X;
				}
			}

			if (::Global::UIVars::g_MenuX >= 0.40f) {
				Return.m_X = ::Global::UIVars::g_MenuX - 0.283f;
				Return.m_X -= CalculatedOffset;

				if (m_Child->m_DoubleSided && m_Child->m_Parent->m_ColumnOffset[m_Child->m_Column - 1].m_X == 0.14f) {
					Return.m_X -= 0.14f;
				}

				if (!m_Child->m_DoubleSided) {
					Return.m_X += 0.14f; // size of half panel
				}
			} else {
				Return.m_X = ::Global::UIVars::g_MenuX + 0.234f;
				Return.m_X += CalculatedOffset;
			}
		}

		if (m_Child->m_Column != 0) {
			if (::Global::UIVars::g_MenuX >= 0.40f) Return.m_X -= 0.002f;
			else Return.m_X += 0.002f;
		}

		return Return;
	}

	Panel::Panel(PanelChild& Child, Color HeaderCol) {
		m_Child = &Child;

		int MaxCount = Child.m_PanelOptionCountLeft;
		if (Child.m_PanelOptionCountRight > MaxCount) {
			MaxCount = Child.m_PanelOptionCountRight;
		}

		Child.m_PanelOptionCountLeft = Child.m_PanelTickLeft - 1;
		Child.m_PanelOptionCountRight = Child.m_PanelOptionCountRight - 1;

		if (m_Child->m_Parent->m_ColumnOffset.find(Child.m_Column) == m_Child->m_Parent->m_ColumnOffset.end()) {
			m_ColumnOffset = 0.f;
		} else {
			m_ColumnOffset = (m_Child->m_Parent->m_ColumnOffset[Child.m_Column].m_Y);
		}

		// render bar & bg
		m_ColumnAdjustment = GetRenderingPosition();

		float X = m_ColumnAdjustment.m_X;
		float Width = Child.m_DoubleSided ? 0.28f : 0.14f;
		
		// header
		Menu::GetRenderer()->DrawRect({ X, (::Global::UIVars::g_MenuY - .127f) + m_ColumnOffset }, { Width, (0.007875f / 2.f) }, HeaderCol);

		// bg
		Menu::GetRenderer()->DrawRect({ X, (::Global::UIVars::g_MenuY - .127f) + (0.007875f / 2.f) + m_ColumnOffset }, { Width, (0.03f * MaxCount) }, { 0, 0, 0, 180 });

		m_Width = Width;
		m_Height = (0.03f * MaxCount) + (0.007875f / 2.f) + 0.004f;

		Child.m_PanelTickLeft = 1;
		Child.m_PanelTickRight = 1;
	}

	void Panel::Item(std::string Name, std::string Value, int Font, float FontScale, Color Col) {
		if (!m_Child->m_DoubleSided) {
			PanelLeftItem(Name, Value, Font, FontScale, Col);
			return;
		}

		if (m_Child->m_PanelTickLeft > m_Child->m_PanelTickRight) {
			PanelRightItem(Name, Value, Font, FontScale, Col);
		} else {
			PanelLeftItem(Name, Value, Font, FontScale, Col);
		}
	}

	void Panel::Item(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font, float FontScale, Color Col, Math::Vector2<float> Scale) {
		if (!m_Child->m_DoubleSided) {
			PanelLeftSpriteItem(Name, SpriteLeft, SpriteRight, Font, FontScale, Col, Scale);
			return;
		}

		if (m_Child->m_PanelTickLeft > m_Child->m_PanelTickRight) {
			PanelRightSpriteItem(Name, SpriteLeft, SpriteRight, Font, FontScale, Col, Scale);
		} else {
			PanelLeftSpriteItem(Name, SpriteLeft, SpriteRight, Font, FontScale, Col, Scale);
		}
	}

	void Panel::ItemFull(std::string Name, std::string Value, int Font, float FontScale, Color Col) {
		if (m_Child->m_PanelTickLeft != m_Child->m_PanelTickRight) {
			if (m_Child->m_PanelTickLeft > m_Child->m_PanelTickRight) {
				m_Child->m_PanelTickRight = m_Child->m_PanelTickLeft;
			} else {
				m_Child->m_PanelTickLeft = m_Child->m_PanelTickRight;
			}
		}

		float Adjustment = m_ColumnAdjustment.m_X;
		float Wrap = (Adjustment + 0.28f - 0.004f);;
		float X = Adjustment + 0.003f;

		// draw name
		Menu::GetRenderer()->DrawText(Name.c_str(), { X, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col);

		// draw value
		Menu::GetRenderer()->DrawText(Value.c_str(), { 0.f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col, JUSTIFY_RIGHT, false, { 0.f, Wrap });

		m_Child->m_PanelTickLeft++;
		m_Child->m_PanelTickRight++;
	}

	void Panel::ItemFull(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font, float FontScale, Color Col, Math::Vector2<float> Scale) {
		if (m_Child->m_PanelTickLeft != m_Child->m_PanelTickRight) {
			if (m_Child->m_PanelTickLeft > m_Child->m_PanelTickRight) {
				m_Child->m_PanelTickRight = m_Child->m_PanelTickLeft;
			} else {
				m_Child->m_PanelTickLeft = m_Child->m_PanelTickRight;
			}
		}

		float Adjustment = m_ColumnAdjustment.m_X;
		float Wrap = (Adjustment + 0.28f - 0.004f);;
		float X = Adjustment + 0.003f;

		// sprite
		Menu::GetRenderer()->DrawSprite(SpriteLeft.c_str(), SpriteRight.c_str(), { X + 0.006f, (::Global::UIVars::g_MenuY - .127f) + 0.017f + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, Scale, 0.f, Col);

		// draw name
		Menu::GetRenderer()->DrawText(Name.c_str(), { X + 0.014f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col);

		m_Child->m_PanelTickLeft++;
		m_Child->m_PanelTickRight++;
	}

	void Panel::PanelLeftItem(std::string Name, std::string Value, int Font, float FontScale, Color Col) {
		float X = m_ColumnAdjustment.m_X;
		float X2 = X + 0.003f;
		float Wrap = (X + 0.14f - 0.004f);

		// draw name
		Menu::GetRenderer()->DrawText(Name.c_str(), { X2, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col);

		// draw value
		Menu::GetRenderer()->DrawText(Value.c_str(), { 0.f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col, JUSTIFY_RIGHT, false, { 0.f, Wrap });

		m_Child->m_PanelTickLeft++;
	}

	void Panel::PanelRightItem(std::string Name, std::string Value, int Font, float FontScale, Color Col) {
		float Adjustment = m_ColumnAdjustment.m_X;
		float X = m_ColumnAdjustment.m_X;
		float Wrap = (X + 0.28f - 0.004f);

		// draw divider
		Menu::GetRenderer()->DrawRect({ X + 0.14f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickRight - 1) * 0.03f) + (0.007875f / 2.f) + m_ColumnOffset + 0.00525f }, { 0.001f, 0.021f }, { 255, 255, 255, 255 });

		// draw name
		Menu::GetRenderer()->DrawText(Name.c_str(), { X + 0.14f + 0.004f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickRight - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col);

		// draw value
		Menu::GetRenderer()->DrawText(Value.c_str(), { 0.f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickRight - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col, JUSTIFY_RIGHT, false, { 0.f, Wrap });

		m_Child->m_PanelTickRight++;
	}

	void Panel::PanelLeftSpriteItem(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font, float FontScale, Color Col, Math::Vector2<float> Scale) {
		float X = m_ColumnAdjustment.m_X;
		float X2 = X + 0.003f;

		if (m_Child->m_DoubleSided) {
			// draw divider
			Menu::GetRenderer()->DrawRect({ X + 0.14f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + (0.007875f / 2.f) + m_ColumnOffset + 0.00525f }, { 0.001f, 0.021f }, { 255, 255, 255, 255 });
		}

		// sprite
		Menu::GetRenderer()->DrawSprite(SpriteLeft.c_str(), SpriteRight.c_str(), { X2 + 0.006f, (::Global::UIVars::g_MenuY - .127f) + 0.017f + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + ((0.007875f / 2.f) - 0.0017875f) +  m_ColumnOffset }, Scale, 0.f, Col);

		// draw name
		Menu::GetRenderer()->DrawText(Name.c_str(), { X2 + 0.014f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickLeft - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col);

		m_Child->m_PanelTickLeft++;
	}

	void Panel::PanelRightSpriteItem(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font, float FontScale, Color Col, Math::Vector2<float> Scale) {
		float Adjustment = m_ColumnAdjustment.m_X;
		float X = m_ColumnAdjustment.m_X;
		float X2 = X + 0.14f + 0.003f;

		// sprite
		Menu::GetRenderer()->DrawSprite(SpriteLeft.c_str(), SpriteRight.c_str(), { X2 + 0.009f, (::Global::UIVars::g_MenuY - .127f) + 0.017f + ((m_Child->m_PanelTickRight - 1) * 0.03f) + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, Scale, 0.f, Col);

		// draw name
		Menu::GetRenderer()->DrawText(Name.c_str(), { X + 0.14f + 0.004f + 0.018f, (::Global::UIVars::g_MenuY - .127f) + ((m_Child->m_PanelTickRight - 1) * 0.03f) + 0.006f + ((0.007875f / 2.f) - 0.0017875f) + m_ColumnOffset }, FontScale, Font, Col);

		m_Child->m_PanelTickRight++;
	}

	void PanelManager::Update() {
		if (::Global::Vars::g_Unloading || m_Rearranging || !Menu::GetBase()->m_Open) return;

		for (auto Panel : m_Panels) {
			if (::Global::Vars::g_Unloading || m_Rearranging) return;
			for (auto& E : Panel->m_ColumnOffset) E.second = Math::Vector2<float>(0.f, 0.f);

			if (Panel->m_Render) {
				if (!Panel->m_ChildrenPanels.empty()) {
					for (int j = 0; j < 20; j++) {
						for (std::size_t i = 0; i < Panel->m_ChildrenPanels.size(); i++) {
							if (::Global::Vars::g_Unloading || m_Rearranging) return;

							auto& Child = Panel->m_ChildrenPanels[i];
							if (Child.m_Column == j) {
								if (Child.m_Render) {
									Math::Vector2<float> Scale = Child.m_Update(Child);

									if (Panel->m_ColumnOffset[Child.m_Column].m_X < Scale.m_X) {
										Panel->m_ColumnOffset[Child.m_Column].m_X = Scale.m_X;
									}

									Panel->m_ColumnOffset[Child.m_Column].m_Y += Scale.m_Y;
								}
							}
						}
					}
				}
			}
		}
	}

	void PanelManager::Unload() {
		for (auto Panel : m_Panels) {
			if (Panel) delete Panel;
		}
	}

	PanelParent* PanelManager::GetParent(std::string ID) {
		auto Search = std::find_if(m_Panels.begin(), m_Panels.end(), [=] (PanelParent* Element) { return Element->m_ID == ID; });
		if (Search != m_Panels.end()) {
			return *(Search);
		}

		return nullptr;
	}

	PanelChild& PanelManager::GetChild(PanelParent* Parent, std::string ID) {
		static PanelChild Static;
		if (!Parent) return Static;

		auto Search = std::find_if(Parent->m_ChildrenPanels.begin(), Parent->m_ChildrenPanels.end(), [=] (PanelChild& Element) { return Element.m_ID == ID; });
		if (Search != Parent->m_ChildrenPanels.end()) {
			return *(Search);
		}

		return Static;
	}

	void PanelManager::TogglePanelRender(std::string ParentID, bool Toggle) {
		PanelParent* Parent = GetParent(ParentID);
		if (Parent) {
			Parent->m_Render = Toggle;
		}
	}

	void PanelManager::Rearrange(PanelParent* Parent, std::string ID, int Column, int Position) {
		if (!Parent) return;
		m_Rearranging = true;

		PanelChild& Child = GetChild(Parent, ID);

		if (Child.m_Index != Position) {
			Child.m_Index = Position;
		}

		if (Column != Child.m_Column) {
			int Index = 0;
			for (auto& Elem : Parent->m_ChildrenPanels) {
				if (Elem.m_Column == Column) {
					if (Elem.m_Index > Index) {
						Index = Elem.m_Index;
					}
				}
			}

			Child.m_Index = Index + 1;
			Child.m_Column = Column;
		}

		std::sort(Parent->m_ChildrenPanels.begin(), Parent->m_ChildrenPanels.end(), [=] (PanelChild& Left, PanelChild& Right) {
			return Left.m_Index < Right.m_Index;
		});
		
		m_Rearranging = false;
	}

	void PanelManager::NullStructure(PanelChild& This) {
		memset(This.m_CustomPtr, 0, sizeof(This.m_CustomPtr));
	}

	void PanelManager::SetStructure(PanelChild& This, void* Data, int Size) {
		// memset(This.m_CustomPtr, 0, sizeof(This.m_CustomPtr));
		memcpy(This.m_CustomPtr, Data, Size);
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoName(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);
		if (!Menu::GetSelectedPlayer().m_Name) return Math::Vector2<float>(0.f, 0.f);

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

		const char* Name = Menu::GetSelectedPlayer().m_Name;
		if (Menu::GetLocalPlayer().m_ID == Menu::GetSelectedPlayer().m_ID) {
			if (!NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName.empty()) {
				Name = NetworkSpoofingNameMenuVars::m_Vars.m_SpoofedName.c_str();
			}
		}

		if (Native::IsPedheadshotValid(Menu::GetSelectedPlayer().m_PedHeadshot)) {
			auto Texture = Native::GetPedheadshotTxdString(Menu::GetSelectedPlayer().m_PedHeadshot);
			Panel.ItemFull(std::string(Utils::VA::VA("%s", Name)), Texture, Texture, 1, 0.41f, { 255,255,255,255 }, { 0.0192f * 0.8f, 0.0336f * 0.8f });
		} else Panel.ItemFull(std::string(Utils::VA::VA("%s", Name)), "", 1, 0.41f);

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoGeneric(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);

		static char m_Buffers[11][50];
		memset(m_Buffers, 0, sizeof(m_Buffers));

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);
		auto& Selected = Menu::GetSelectedPlayer();

		Panel.Item(TRANSLATE(T_Rank), std::string(Utils::VA::VAP(m_Buffers[0], "%i", Global(1590682).At(Selected.m_ID, 883).At(211).At(6).As<int>())));
		
		int Health = Native::GetEntityHealth(Selected.m_Ped);
		int MaxHealth = Native::GetEntityMaxHealth(Selected.m_Ped);

		if (Health == 0 && MaxHealth == 0) {
			Panel.Item(TRANSLATE(T_Health), "~m~" + std::string(TRANSLATE(T_Invalid)));
		} else if (Health == 0 && MaxHealth != 0) {
			Panel.Item(TRANSLATE(T_Health), "~r~" + std::string(TRANSLATE(T_Dead)));
		} else if (Health == MaxHealth) {
			Panel.Item(TRANSLATE(T_Health), "~g~" + std::string(TRANSLATE(T_Full)));
		} else {
			Panel.Item(TRANSLATE(T_Health), std::string(Utils::VA::VAP(m_Buffers[1], "~o~%i%%", ((Health * 100) / MaxHealth))).c_str());
		}

		Panel.Item(TRANSLATE(T_Cash), std::string(Utils::VA::VAP(m_Buffers[2], "%i", Global(1590682).At(Selected.m_ID, 883).At(211).At(3).As<int>())));
		Panel.Item(TRANSLATE(T_Armor), std::string(Utils::VA::VAP(m_Buffers[3], "%i", Native::GetPedArmour(Selected.m_Ped))));
		Panel.Item(TRANSLATE(T_Bank), std::string(Utils::VA::VAP(m_Buffers[4], "%i", Global(1590682).At(Selected.m_ID, 883).At(211).At(56).As<int>() - Global(1590682).At(Selected.m_ID, 883).At(211).At(3).As<int>())));
		Panel.Item(TRANSLATE(T_WantedLevel), std::string(Utils::VA::VAP(m_Buffers[5], "%i/5", Native::GetPlayerWantedLevel(Selected.m_ID))));
		Panel.Item(TRANSLATE(T_KDRatio), std::string(Utils::VA::VAP(m_Buffers[6], "%.2f", Global(1590682).At(Selected.m_ID, 883).At(211).At(26).As<float>())));
		
		int Ammo = Native::GetAmmoInPedWeapon(Selected.m_Ped, Selected.m_WeaponHash);
		Panel.Item(TRANSLATE(T_Ammo), Ammo > 0 ? std::to_string(Ammo) : "~m~" + std::string(TRANSLATE(T_None)));

		bool Rendered = false;
		for (auto& Weapon : ::Global::Arrays::g_Weapons) {
			if (Selected.m_WeaponHash == Weapon.m_Result) {
				Rendered = true;
				Panel.ItemFull(TRANSLATE(T_Weapon), Weapon.m_Name.Get());
				break;
			}
		}

		if (!Rendered) {
			Panel.ItemFull(TRANSLATE(T_Weapon), "~m~" + std::string(TRANSLATE(T_Unarmed)));
		}

		Panel.ItemFull(TRANSLATE(T_Zone), Native::_GetLabelText(Native::GetNameOfZone(Selected.m_Coords.m_X, Selected.m_Coords.m_Y, Selected.m_Coords.m_Z)));

		uint32_t Street, Crossing;
		Native::GetStreetNameAtCoord(Selected.m_Coords.m_X, Selected.m_Coords.m_Y, Selected.m_Coords.m_Z, &Street, &Crossing);

		const char* StreetLabel = Native::GetStreetNameFromHashKey(Street);
		const char* CrossingLabel = Native::GetStreetNameFromHashKey(Crossing);

		const char* StreetOutput = "";
		if (strlen(StreetLabel) > 0) {
			StreetOutput = StreetLabel;
		} else if (strlen(CrossingLabel) > 0) {
			StreetOutput = CrossingLabel;
		} else StreetOutput = ("~m~" + std::string(TRANSLATE(T_Unknown))).c_str();

		Panel.ItemFull(TRANSLATE(T_Street), StreetOutput);
		Panel.Item(TRANSLATE(T_Coords), std::string(Utils::VA::VAP(m_Buffers[7], "[%.0f,%.0f,%.0f]", Selected.m_Coords.m_X, Selected.m_Coords.m_Y, Selected.m_Coords.m_Z)));

		float Heading = Selected.m_Heading;
		if (Heading >= 0.f && Heading <= 90.f) {
			Panel.Item(TRANSLATE(T_Heading), std::string(Utils::VA::VAP(m_Buffers[8], "%s (%.0f)", TRANSLATE(T_North), Heading)).c_str());
		} else if (Heading >= 90.f && Heading <= 180.f) {
			Panel.Item(TRANSLATE(T_Heading), std::string(Utils::VA::VAP(m_Buffers[8], "%s (%.0f)", TRANSLATE(T_East), Heading)).c_str());
		} else if (Heading >= 180.f && Heading <= 270.f) {
			Panel.Item(TRANSLATE(T_Heading), std::string(Utils::VA::VAP(m_Buffers[8], "%s (%.0f)", TRANSLATE(T_South), Heading)).c_str());
		} else if (Heading >= 270.f && Heading <= 360.f) {
			Panel.Item(TRANSLATE(T_Heading), std::string(Utils::VA::VAP(m_Buffers[8], "%s (%.0f)", TRANSLATE(T_West), Heading)).c_str());
		} else Panel.Item(TRANSLATE(T_Heading), "~m~" + std::string(TRANSLATE(T_Invalid)));

		Panel.Item(TRANSLATE(T_Distance), std::string(Utils::VA::VAP(m_Buffers[9], "%.2f", Selected.m_Coords.GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords))));
		Panel.Item(TRANSLATE(T_Speed), std::string(Utils::VA::VAP(m_Buffers[10], "%.0f %s", Native::GetEntitySpeed(Selected.m_Entity) * ::Global::Vars::m_SelectedMetricSystem.first, ::Global::Vars::m_SelectedMetricSystem.second.c_str())));
		Panel.Item(TRANSLATE(T_Passive), Native::GetBlipSprite(Native::GetBlipFromEntity(Selected.m_Ped)) == 163 ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));
		Panel.Item(TRANSLATE(T_Invisible), !Native::IsEntityVisibleToScript(Selected.m_Ped) ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));

		bool NetGameIsValid = ((uint64_t)Selected.m_NetGamePlayer & 0xFFFFFFFF) != 0;

		Rendered = false;
		if (NetGameIsValid) {
			if (Selected.m_NetGamePlayer->m_PlayerInfo) {
				if (!VideosMenuVars::m_Vars.m_HideRockstarIDs) {
					uint64_t RockstarID = Selected.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID;
					
					if (Selected.m_ID == Menu::GetLocalPlayer().m_ID) {
						if (NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID != RockstarID) {
							RockstarID = NetworkSpoofingRockstarIDMenuVars::m_Vars.m_SpoofingRockstarID;
						}
					}

					Panel.Item(TRANSLATE(T_RID), std::to_string(RockstarID));
				} else {
					Panel.Item(TRANSLATE(T_RID), "~m~" + std::string(TRANSLATE(T_Hidden)));
				}

				Rendered = true;
			}
		}

		if (!Rendered) {
			Panel.Item(TRANSLATE(T_RID), "~m~" + std::string(TRANSLATE(T_Unknown)));
		}

		Rendered = false;
		if (NetGameIsValid) {
			if (Selected.m_NetGamePlayer->m_PlayerInfo) {
				uint32_t IP = Selected.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP;
				
				auto Vit = std::find_if(::Global::Vars::g_RelayUsage.begin(), ::Global::Vars::g_RelayUsage.end(), [=] (RelayUsage& Usage) {
					return !strcmp(Usage.m_Name, Selected.m_Name) && Usage.m_RockstarID == Selected.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID;
				});

				if (Vit != ::Global::Vars::g_RelayUsage.end()) {
					IP = Vit->m_ExternalIP;
				}

				char Buffer[16];
				sprintf(Buffer, "%i.%i.%i.%i", ((IP & 0xFF000000) >> 24), ((IP & 0xFF0000) >> 16), ((IP & 0xFF00) >> 8), (IP & 0xFF));

				if (Selected.m_ID == Menu::GetLocalPlayer().m_ID) {
					if (!NetworkSpoofingIPMenuVars::m_Vars.m_SpoofingIP.empty() && NetworkSpoofingIPMenuVars::m_Vars.m_SpoofingIP.compare(Buffer)) {
						strcpy(Buffer, NetworkSpoofingIPMenuVars::m_Vars.m_SpoofingIP.c_str());
					}
				}

				if (!VideosMenuVars::m_Vars.m_HideIps) {
					Panel.Item(TRANSLATE(T_IP), Buffer);
				} else {
					Panel.Item(TRANSLATE(T_IP), "~m~" + std::string(TRANSLATE(T_Hidden)));
				}

				Rendered = true;
			}
		}

		if (!Rendered) {
			Panel.Item("IP", "~m~" + std::string(TRANSLATE(T_Unknown)));
		}

		if (Selected.m_InVehicle) {
			Panel.ItemFull(TRANSLATE(T_Vehicle), Native::_GetLabelText(Native::GetDisplayNameFromVehicleModel(Native::GetEntityModel(Selected.m_Vehicle))));
		}

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoCrew(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);

		auto& Selected = Menu::GetSelectedPlayer();
		if (Selected.m_NetGamePlayer && strlen(Selected.m_NetGamePlayer->GetCrewName()) > 0) {
			Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

			Panel.ItemFull(TRANSLATE(T_Crew), Selected.m_NetGamePlayer->GetCrewName());
			Panel.Item(TRANSLATE(T_CrewTag), Selected.m_NetGamePlayer->GetCrewTag());
			Panel.Item(TRANSLATE(T_CrewID), std::to_string(Selected.m_NetGamePlayer->GetCrewID()));
			Panel.ItemFull(TRANSLATE(T_CrewRank), Selected.m_NetGamePlayer->GetCrewRank());

			return Panel.GetRenderScale();
		}

		return Math::Vector2<float>(0.f, 0.f);
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoFlags(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);

		This.m_DoubleSided = false;
		auto& Selected = Menu::GetSelectedPlayer();

		bool RockstarDeveloper = false;
		bool RockstarQATester = false;

		bool NetGameIsValid = ((uint64_t)Selected.m_NetGamePlayer & 0xFFFFFFFF) != 0;
		if (NetGameIsValid) {
			if (Selected.m_NetGamePlayer) {
				RockstarDeveloper = Selected.m_NetGamePlayer->m_IsRockstarDeveloper;
				RockstarQATester = Selected.m_NetGamePlayer->m_IsRockstarQA;
			}
		}

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

		if (Global(1590682).At(Selected.m_ID, 883).At(99).At(29).As<int>() == 2) {
			Panel.Item(TRANSLATE(T_UsingController), "mpinventory", "mp_specitem_remote", 0, 0.3f, { 255,255,255,255 }, { 0.0192f * 0.8f, 0.0336f * 0.8f });
		} else {
			Panel.Item(TRANSLATE(T_UsingKeyboard), "mpinventory", "mp_specitem_keycard", 0, 0.3f, { 255,255,255,255 }, { 0.0192f * 0.8f, 0.0336f * 0.8f });
		}

		if (Native::IsBitSet(Global(1590682).At(Selected.m_ID, 883).At(211).At(51).As<int>(), 1)) {
			Panel.Item(TRANSLATE(T_Badsport), "mpleaderboard", "leaderboard_deaths_icon");
		}

		if (Native::IsBitSet(Global(1590682).At(Selected.m_ID, 883).At(211).At(51).As<int>(), 4)) {
			Panel.Item(TRANSLATE(T_Cheater), "mpleaderboard", "leaderboard_globe_icon");
		}

		if (Native::IsBitSet(Global(1590682).At(Selected.m_ID, 883).At(211).At(51).As<int>(), 3)) {
			Panel.Item(TRANSLATE(T_Toxic), "mpleaderboard", "leaderboard_globe_icon");
		}

		if (Native::NetworkAmIMutedByPlayer(Selected.m_ID)) {
			Panel.Item(TRANSLATE(T_MutedMe), "mpleaderboard", "leaderboard_audio_3");
		}

		if (Global(2426097).At(Selected.m_ID, 443).At(200).As<int>() == 1) {
			Panel.Item(TRANSLATE(T_OffTheRadar), "mpleaderboard", "leaderboard_kd_icon");
		}

		if (Global(2426097).At(Selected.m_ID, 443).At(203).As<int>() == 1) {
			Panel.Item(TRANSLATE(T_RevealPlayers), "mpleaderboard", "leaderboard_kd_icon");
		}

		if (RockstarDeveloper) {
			Panel.Item(TRANSLATE(T_RDeveloper), "mpleaderboard", "leaderboard_star_icon");
		}

		if (RockstarQATester) {
			Panel.Item(TRANSLATE(T_RQADeveloper), "mpleaderboard", "leaderboard_star_icon");
		}

		std::string ReportReasons[] = {
			TRANSLATE(T_InvalidModel),
			TRANSLATE(T_SpoofedName),
			TRANSLATE(T_SpoofedPeer),
			TRANSLATE(T_SpoofedRID),
			TRANSLATE(T_SpoofedIP),
			TRANSLATE(T_CRCFlood),
			TRANSLATE(T_CRCCompromised),
			TRANSLATE(T_CRCNotReplied),
			TRANSLATE(T_InfoChange1),
			TRANSLATE(T_InfoChange2),
			TRANSLATE(T_InfoChange4),
			TRANSLATE(T_InfoChange5),
			TRANSLATE(T_InfoChange6),
			TRANSLATE(T_InfoChange7),
			TRANSLATE(T_Arxan),
			TRANSLATE(T_SpoofedChat),
			TRANSLATE(T_SpoofedText),
			TRANSLATE(T_CrashAttempt),
			TRANSLATE(T_WeatherChange),
			TRANSLATE(T_Spectating),
			TRANSLATE(T_ExplosiveAmmo),
			TRANSLATE(T_ExplosiveMelee),
			TRANSLATE(T_FireAmmo),
			TRANSLATE(T_SuperJump),
			TRANSLATE(T_MoneyDrop),
			TRANSLATE(T_RPDrop),
			TRANSLATE(T_QuantumKick),
			TRANSLATE(T_RAntiCheat),
			TRANSLATE(T_ScriptMigration),
			TRANSLATE(T_Profanity),
			TRANSLATE(T_JoinTimeout),
			TRANSLATE(T_InfoMismatch),
			TRANSLATE(T_Cherax),
			TRANSLATE(T_Impulse),
			TRANSLATE(T_ImpulseVIP),
			TRANSLATE(T_ImpulseStaff),
			TRANSLATE(T_ImpulseBlock),
		};

#ifdef DEV
		for (std::size_t i = 0; i < NUMOF(ReportReasons); i++) {
			if (Selected.m_ModderFlags[i]) {
				Panel.Item("~r~" + ReportReasons[i], "mpleaderboard", "leaderboard_players_icon");
			}
		}
#else
		for (std::size_t i = 0; i < NUMOF(ReportReasons) - 5; i++) {
			if (Selected.m_ModderFlags[i]) {
				Panel.Item("~r~" + ReportReasons[i], "mpleaderboard", "leaderboard_players_icon");
			}
		}
#endif

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoVoteKickQueue(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);

		auto& Selected = Menu::GetSelectedPlayer();

		std::vector<std::string> VotingToKick;
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			if (Menu::Global(1619452).At(Player.m_ID, 100).At(Selected.m_ID + 1).As<int>() == 1) {
				VotingToKick.push_back(Player.m_Name);
			}
		}, true);

		if (VotingToKick.size()) {
			This.m_DoubleSided = false;
			Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

			for (auto& Client : VotingToKick) {
				Panel.Item(Client, "mpleaderboard", "leaderboard_kick_icon");
			}

			return Panel.GetRenderScale();
		}

		return Math::Vector2<float>(0.f, 0.f);
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoProperties(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);

		auto& Selected = Menu::GetSelectedPlayer();

		std::vector<std::pair<int, bool>> Apartments;

		for (int i = 0; i < 4; i++) {
			int Val = Global(1590682).At(Selected.m_ID, 883).At(267 + 7 + i).As<int>();
			Apartments.push_back({ Val, Val != 0 });
		}

		std::vector<std::pair<int, bool>> Garages;
		for (int i = 0; i < 3; i++) {
			int Val = Global(1590682).At(Selected.m_ID, 883).At(275 + 7 + i).As<int>();
			Garages.push_back({ Val, Val != 0 });
		}

		std::pair<int, bool> Office;
		Office.first = Global(1590682).At(Selected.m_ID, 883).At(272 + 7).As<int>();
		Office.second = Office.first != 0;

		std::pair<int, bool> Clubhouse;
		Clubhouse.first = Global(1590682).At(Selected.m_ID, 883).At(273 + 7).As<int>();
		Clubhouse.second = Clubhouse.first != 0;

		bool OwnsProperty =
			Apartments[0].second || Apartments[1].second || Apartments[2].second || Apartments[3].second
			|| Office.second || Clubhouse.second;

		int PropertyCount =
			(int)Apartments[0].second + (int)Apartments[1].second + (int)Apartments[2].second + (int)Apartments[3].second
			+ (int)Clubhouse.second + (int)Clubhouse.second;

		if (OwnsProperty) {
			static char m_Buffers[4][50];

			Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

			int Count = 0;
			for (auto& E : Garages) {
				if (E.second) Count++;
			}

			if (PropertyCount == 1) {
				if (Office.second && Count > 0) {
					Panel.Item("1 " + std::string(TRANSLATE(T_Property)), "mpleaderboard", "leaderboard_rankshield_icon");
				} else {
					Panel.ItemFull("1 " + std::string(TRANSLATE(T_Property)), "mpleaderboard", "leaderboard_rankshield_icon");
				}
			} else {
				if (Office.second && Count > 0) {
					Panel.Item(std::string(Utils::VA::VAP(m_Buffers[0], "%i %s", PropertyCount, TRANSLATE(T_Properties))), "mpleaderboard", "leaderboard_rankshield_icon");
				} else {
					Panel.ItemFull(std::string(Utils::VA::VAP(m_Buffers[1], "%i %s", PropertyCount, TRANSLATE(T_Properties))), "mpleaderboard", "leaderboard_rankshield_icon");
				}
			}

			if (Office.second && Count > 0) {
				Panel.Item(std::string(Utils::VA::VAP(m_Buffers[2], "%i %S%c", Count, TRANSLATE(T_OfficeGarage), Count > 1 ? 's' : ' ')), "mpinventory", "mp_specitem_car", 0, 0.3f, {255,255,255,255}, { 0.0192f * 0.8f, 0.0336f * 0.8f });
			}

			for (int i = 0; i < 4; i++) {
				if (Apartments[i].second) {
					Panel.ItemFull(std::string(Utils::VA::VAP(m_Buffers[3], "%s #%i", TRANSLATE(T_Property), i + 1)), NetworkTeleportApartmentMenuVars::GetLocationFromID(Apartments[i].first));
				}
			}

			if (Clubhouse.second) {
				Panel.ItemFull(TRANSLATE(T_Clubhouse), NetworkTeleportApartmentMenuVars::GetLocationFromID(Clubhouse.first));
			}

			if (Office.second) {
				Panel.ItemFull(TRANSLATE(T_Office), NetworkTeleportApartmentMenuVars::GetLocationFromID(Office.first));
			}

			return Panel.GetRenderScale();
		}

		return Math::Vector2<float>(0.f, 0.f);
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoStats(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);

		This.m_DoubleSided = false;

		auto& Selected = Menu::GetSelectedPlayer();
		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

		int RacesWon = Global(1590682).At(Selected.m_ID, 883).At(211).At(15).As<int>();
		int RacesLost = Global(1590682).At(Selected.m_ID, 883).At(211).At(16).As<int>();

		int DeathmatchesWon = Global(1590682).At(Selected.m_ID, 883).At(211).At(20).As<int>();
		int DeathmatchesLost = Global(1590682).At(Selected.m_ID, 883).At(211).At(21).As<int>();

		int DartsWon = Global(1590682).At(Selected.m_ID, 883).At(211).At(32).As<int>();
		int DartsTotal = Global(1590682).At(Selected.m_ID, 883).At(211).At(33).As<int>();

		int ArmWrestlingWon = Global(1590682).At(Selected.m_ID, 883).At(211).At(34).As<int>();
		int ArmWrestlingTotal = Global(1590682).At(Selected.m_ID, 883).At(211).At(35).As<int>();

		int TennisWon = Global(1590682).At(Selected.m_ID, 883).At(211).At(36).As<int>();
		int TennisLost = Global(1590682).At(Selected.m_ID, 883).At(211).At(37).As<int>();

		int GolfWon = Global(1590682).At(Selected.m_ID, 883).At(211).At(40).As<int>();
		int GolfLost = Global(1590682).At(Selected.m_ID, 883).At(211).At(41).As<int>();

		int ShootingRangeWon = Global(1590682).At(Selected.m_ID, 883).At(211).At(42).As<int>();
		int ShootingRangeLost = Global(1590682).At(Selected.m_ID, 883).At(211).At(43).As<int>();

		int Deaths = Global(1590682).At(Selected.m_ID, 883).At(211).At(29).As<int>();
		int MissionsCreated = Global(1590682).At(Selected.m_ID, 883).At(211).At(50).As<int>();

		static char m_Buffers[9][50];

		Panel.Item(TRANSLATE(T_RacesWon), std::string(Utils::VA::VAP(m_Buffers[0], "%i/%i", RacesWon, RacesWon + RacesLost)));
		Panel.Item(TRANSLATE(T_DeathmatchesWon), std::string(Utils::VA::VAP(m_Buffers[1], "%i/%i", DeathmatchesWon, DeathmatchesWon + DeathmatchesLost)));
		Panel.Item(TRANSLATE(T_DartsWon), std::string(Utils::VA::VAP(m_Buffers[2], "%i/%i", DartsWon, DartsTotal)));
		Panel.Item(TRANSLATE(T_ArmWrestlingWon), std::string(Utils::VA::VAP(m_Buffers[3], "%i/%i", ArmWrestlingWon, ArmWrestlingTotal)));
		Panel.Item(TRANSLATE(T_TennisWon), std::string(Utils::VA::VAP(m_Buffers[4], "%i/%i", TennisWon, TennisWon + TennisLost)));
		Panel.Item(TRANSLATE(T_GolfWon), std::string(Utils::VA::VAP(m_Buffers[5], "%i/%i", GolfWon, GolfWon + GolfLost)));
		Panel.Item(TRANSLATE(T_ShootingRangeWon), std::string(Utils::VA::VAP(m_Buffers[6], "%i/%i", ShootingRangeWon, ShootingRangeWon + ShootingRangeLost)));
		Panel.Item(TRANSLATE(T_Deaths), std::string(Utils::VA::VAP(m_Buffers[7], "%i", Deaths)));
		Panel.Item(TRANSLATE(T_MissionsCreated), std::string(Utils::VA::VAP(m_Buffers[8], "%i", MissionsCreated)));

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelPlayerInfoGeo(PanelChild& This) {
		if (!Menu::GetSelectedPlayer().m_Connected) return Math::Vector2<float>(0.f, 0.f);
		auto& Selected = Menu::GetSelectedPlayer();

		bool NetGameIsValid = ((uint64_t)Selected.m_NetGamePlayer & 0xFFFFFFFF) != 0;
		if (NetGameIsValid) {
			if (Selected.m_NetGamePlayer && Selected.m_NetGamePlayer->m_PlayerInfo) {
				auto Vit = ::Global::Vars::g_Geo.find(Selected.m_NetGamePlayer->m_PlayerInfo->m_ExternalIP);
				if (Vit != ::Global::Vars::g_Geo.end()) {
					Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

					if (!Vit->second.m_Zip.empty()) Panel.Item(TRANSLATE(T_Zip), Vit->second.m_Zip);
					if (!Vit->second.m_RegionName.empty()) Panel.Item(TRANSLATE(T_Region), Vit->second.m_RegionName);
					if (!Vit->second.m_City.empty()) Panel.Item(TRANSLATE(T_City), Vit->second.m_City);
					if (!Vit->second.m_Country.empty()) Panel.Item(TRANSLATE(T_Country), Vit->second.m_Country);
					if (!Vit->second.m_Timezone.empty()) Panel.ItemFull(TRANSLATE(T_Timezone), Vit->second.m_Timezone);
					if (!Vit->second.m_ISP.empty()) Panel.ItemFull(TRANSLATE(T_ISP), Vit->second.m_ISP);
					if (!Vit->second.m_ORG.empty()) Panel.ItemFull(TRANSLATE(T_Org), Vit->second.m_ORG);

					return Panel.GetRenderScale();
				}
			}
		}

		return Math::Vector2<float>(0.f, 0.f);
	}

	Math::Vector2<float> PanelManager::RenderPanelRadio(PanelChild& This) {
		if (Native::GetPlayerRadioStationIndex() != 255) {
			Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

			char Local_20[500];
			int iLocal_26 = Native::GetAudibleMusicTrackTextId();

			strncpy(Local_20, "", 64);
			strncat(Local_20, std::to_string(iLocal_26).c_str(), 64);
			strncat(Local_20, "S", 64);
			strncpy(&(Local_20[16]), "", 32);
			strncat(&(Local_20[16]), std::to_string(iLocal_26).c_str(), 32);
			strncat(&(Local_20[16]), "A", 32);
			strncpy(&(Local_20[25]), Native::GetPlayerRadioStationName(), 24);

			Panel.ItemFull(TRANSLATE(T_Station), Native::_GetLabelText(Native::GetPlayerRadioStationName()));
			if (Native::GetHashKey(Native::_GetLabelText(Local_20)) != joaat("NULL")) {
				Panel.ItemFull(TRANSLATE(T_Song), Native::_GetLabelText(Local_20));
				Panel.ItemFull(TRANSLATE(T_Artist), Native::_GetLabelText(&Local_20[16]));
			}
			
			return Panel.GetRenderScale();
		}

		return Math::Vector2<float>(0.f, 0.f);
	}

	Math::Vector2<float> PanelManager::RenderPanelRecentPlayer(PanelChild& This) {
		PanelCustomRecentPlayer* Data = (PanelCustomRecentPlayer*)This.m_CustomPtr;

		if (*(uint8_t*)(&Data->m_Name) == 0 || Data->m_Name[0] == 0) {
			// Preview
			Data->m_RockstarID = 1337;
			strcpy(Data->m_Name, TRANSLATE(T_Placeholder));
			strcpy(Data->m_IP, "1.1.1.1");
			strcpy(Data->m_FirstEncountered, TRANSLATE(T_SomeTimeAgo));
			strcpy(Data->m_LastEncountered, TRANSLATE(T_SomeTimeAgo));
		}

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);
		
		Panel.Item(TRANSLATE(T_Name), Data->m_Name);
		Panel.Item(TRANSLATE(T_RID), std::string(Utils::VA::VA("%i", Data->m_RockstarID)));
		Panel.Item(TRANSLATE(T_IP), Data->m_IP);

		if (Data->m_RockstarID == 1337) {
			Panel.Item(TRANSLATE(T_Online), "~r~" + std::string(TRANSLATE(T_No)));
		} else Panel.Item(TRANSLATE(T_Online), ::Global::Vars::g_OnlineRockstarIds[Data->m_RockstarID] ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));

		bool InSession = false;
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& PlayerI) {
			if (IsValidPtr(PlayerI.m_NetGamePlayer)) {
				if (IsValidPtr(PlayerI.m_NetGamePlayer->m_PlayerInfo)) {
					if (PlayerI.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID == Data->m_RockstarID) {
						InSession = true;
					}
				}
			}
		}, true);

		Panel.Item(TRANSLATE(T_InSession), InSession ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));
		Panel.ItemFull(TRANSLATE(T_FirstEncountered), Data->m_FirstEncountered);
		Panel.ItemFull(TRANSLATE(T_LastEncountered), Data->m_LastEncountered);
		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelFriend(PanelChild& This) {
		PanelCustomFriend* Data = (PanelCustomFriend*)This.m_CustomPtr;

		if (*(uint8_t*)(&Data->m_Name) == 0 || Data->m_Name[0] == 0) {
			// Preview
			strcpy(Data->m_Name, TRANSLATE(T_Preview));
			Data->m_RockstarID = 1337;
			Data->m_State = 0;
		}

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

		std::string State = "~r~" + std::string(TRANSLATE(T_No));
		if (abs(Data->m_State) == 3) State = "~g~" + std::string(TRANSLATE(T_Yes));

		Panel.Item(TRANSLATE(T_Name), Data->m_Name);
		Panel.Item(TRANSLATE(T_Online), State);
		Panel.Item(TRANSLATE(T_RID), std::string(Utils::VA::VA("%i", Data->m_RockstarID)));

		bool InSession = false;
		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& PlayerI) {
			if (IsValidPtr(PlayerI.m_NetGamePlayer)) {
				if (IsValidPtr(PlayerI.m_NetGamePlayer->m_PlayerInfo)) {
					if (PlayerI.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID == Data->m_RockstarID) {
						InSession = true;
					}
				}
			}
		}, true);

		Panel.Item(TRANSLATE(T_InSession), InSession ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelOverseer(PanelChild& This) {
		PanelCustomOverseer* Data = (PanelCustomOverseer*)This.m_CustomPtr;

		if (*(uint8_t*)(&Data->m_Name) == 0 || Data->m_Name[0] == 0) {
			Data->m_Name = TRANSLATE(T_Preview);
			Data->m_Rid = 1337;
			Data->m_Flags = 0;
		}

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);
		
		Panel.Item(TRANSLATE(T_Name), Data->m_Name);

		if (::Global::Vars::g_OnlineRockstarIds.find(Data->m_Rid) == ::Global::Vars::g_OnlineRockstarIds.end()) {
			Panel.Item(TRANSLATE(T_Online), "~r~" + std::string(TRANSLATE(T_No)));
		} else Panel.Item(TRANSLATE(T_Online), ::Global::Vars::g_OnlineRockstarIds[Data->m_Rid] ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));

		Panel.Item(TRANSLATE(T_RID), std::string(Utils::VA::VA("%i", Data->m_Rid)));

		bool InSession = false;
		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& PlayerI) {
			if (IsValidPtr(PlayerI.m_NetGamePlayer)) {
				if (IsValidPtr(PlayerI.m_NetGamePlayer->m_PlayerInfo)) {
					if (PlayerI.m_NetGamePlayer->m_PlayerInfo->m_Identifier.m_RockstarID == Data->m_Rid) {
						InSession = true;
					}
				}
			}
		}, true);

		Panel.Item(TRANSLATE(T_InSession), InSession ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));
		Panel.Item(TRANSLATE(T_Enabled), (Data->m_Flags & 1) ? "~g~" + std::string(TRANSLATE(T_Yes)) : "~r~" + std::string(TRANSLATE(T_No)));

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelVehiclePreview(PanelChild& This) {
		PanelCustomVehiclePreview* Data = (PanelCustomVehiclePreview*)This.m_CustomPtr;

		if (*(uint8_t*)(&Data->m_Name) == 0 || Data->m_Name[0] == 0) {
			strcpy(Data->m_Name, TRANSLATE(T_Preview));
			strcpy(Data->m_Dict, "");
			strcpy(Data->m_Name, "");

			strcpy(Data->m_Manufacturer, TRANSLATE(T_Preview));
			Data->m_MaxSpeed = 0.f;
			Data->m_MaxBraking = 0.f;
			Data->m_MaxTraction = 0.f;
			Data->m_Seats = 0;
		}

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

		float Adjustment = Panel.GetColumnAdjustment().m_X;
		float Wrap = (Adjustment + 0.28f - 0.004f);;
		float X = Adjustment + 0.003f;

		Math::Vector2<float> S = { 0.2625f, 0.185f };
		float _X = X + 0.006f + (S.m_X * 0.5f);
		float _Y = (::Global::UIVars::g_MenuY - .127f) + 0.017f + ((Panel.GetPanelChild()->m_PanelTickLeft - 1) * 0.03f) + ((0.007875f / 2.f) - 0.0017875f) + Panel.GetColumnOffset() + (S.m_Y * 0.5f);

		Menu::GetRenderer()->DrawSprite(Data->m_Dict, Data->m_Name, { _X, _Y }, S, 0.f, { 255, 255, 255, 255 });

		Panel.GetPanelChild()->m_PanelTickLeft = 8;
		Panel.GetPanelChild()->m_PanelTickRight = 8;

		static char m_Buffers[3][50];
		memset(m_Buffers, 0, sizeof(m_Buffers));

		Panel.ItemFull(TRANSLATE(T_Manufacturer), Data->m_Manufacturer);
		Panel.Item(TRANSLATE(T_MaxSpeed), Utils::VA::VAP(m_Buffers[0], "%.0f %s", Data->m_MaxSpeed, ::Global::Vars::m_SelectedMetricSystem.second.c_str()));
		Panel.Item(TRANSLATE(T_MaxBraking), Utils::VA::VAP(m_Buffers[1], "%.2f", Data->m_MaxBraking));
		Panel.Item(TRANSLATE(T_MaxTraction), Utils::VA::VAP(m_Buffers[2], "%.2f", Data->m_MaxTraction));
		Panel.Item(TRANSLATE(T_MaxSeats), std::to_string(Data->m_Seats));

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelBlacklist(PanelChild& This) {
		This.m_DoubleSided = false;

		std::vector<Rage::Network::BlacklistedPlayer> Players;

		if (::Global::Vars::g_NetworkBaseConfig) {
			Rage::Network::NetworkBaseConfig* RealConfig = (*(Rage::Network::NetworkBaseConfig**)((uint64_t)::Global::Vars::g_NetworkBaseConfig));
			if (::Global::Vars::g_Steam) {
				RealConfig = (Rage::Network::NetworkBaseConfig*)(*(uint64_t*)((uint64_t)::Global::Vars::g_NetworkBaseConfig) - 0x50);
			}

			Rage::Network::BlacklistedPlayer* BlacklistedPlayers = RealConfig->m_FirstBlacklistedPlayer;
			if (IsValidPtr(BlacklistedPlayers)) {
				for (; BlacklistedPlayers; BlacklistedPlayers = BlacklistedPlayers->m_Next) {
					Players.push_back(*BlacklistedPlayers);
				}
			}
		}

		if (Players.size()) {
			Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);
			Panel.Item(TRANSLATE(T_BlacklistedPlayers), "", 1, 0.41f);

			if (Players.size() > 20) {
				std::sort(Players.begin(), Players.end(), [] (Rage::Network::BlacklistedPlayer& Left, Rage::Network::BlacklistedPlayer& Right) {
					return Left.m_Timestamp > Right.m_Timestamp;
				});
			}

			for (auto& Player : Players) {
				Panel.Item(std::to_string(Player.m_Player.m_RockstarID), "");
			}

			return Panel.GetRenderScale();
		}

		return Math::Vector2<float>(0.f, 0.f);
	}

	Math::Vector2<float> PanelManager::RenderPanelHostQueue(PanelChild& This) {
		This.m_DoubleSided = false;

		std::vector<std::pair<uint64_t, int>> HostQueue;

		Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
			if (Rage::Engine::GetHostNetGamePlayer() == Player.m_NetGamePlayer) return;

			if (IsValidPtr(Player.m_NetGamePlayer)) {
				if (IsValidPtr(Player.m_NetGamePlayer->m_PlayerInfo)) {
					HostQueue.push_back({ Player.m_NetGamePlayer->m_PlayerInfo->m_HostToken, Player.m_ID });
				}
			}
		}, true, true);

		if (!HostQueue.size()) {
			return Math::Vector2<float>(0.f, 0.f);
		}

		std::sort(HostQueue.begin(), HostQueue.end(), [] (std::pair<uint64_t, int> a, std::pair<uint64_t, int> b) { return a.first < b.first; });

		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);
		Panel.Item(TRANSLATE(T_NextSessionHostQueue), "", 1, 0.41f);

		for (auto& User : HostQueue) {
			if (Menu::GetPlayer(User.second).IsValidName()) {
				Panel.Item(Menu::GetPlayer(User.second).m_Name, "");
			} else Panel.Item("~m~" + std::string(TRANSLATE(T_Unknown)), "");
		}

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelSessionBrowser(PanelChild& This) {
		PanelCustomSessionBrowser* Data = (PanelCustomSessionBrowser*)This.m_CustomPtr;

		if (Data->m_Name[0] == 1) {
			strcpy(Data->m_Name, TRANSLATE(T_Preview));
		}

		This.m_DoubleSided = false;
		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

		if (Data->m_Name[0]) {
			Panel.Item(TRANSLATE(T_Name), Data->m_Name);
		}

		std::stringstream Stream;
		Stream << std::hex << ((Data->m_PeerToken >> 32) & 0xFFFFFFFF);

		Panel.Item(TRANSLATE(T_RID), std::to_string(Data->m_RockstarID));
		Panel.Item(TRANSLATE(T_Peer), Stream.str());

		return Panel.GetRenderScale();
	}

	Math::Vector2<float> PanelManager::RenderPanelBlacklistedPlayer(PanelChild& This) {
		PanelCustomBlacklistedPlayer* Data = (PanelCustomBlacklistedPlayer*)This.m_CustomPtr;

		if (*(uint8_t*)(&Data->m_Name) == 0 || Data->m_Name[0] == 0) {
			strcpy(Data->m_Name, TRANSLATE(T_Placeholder));
			Data->m_BlacklistedTime = timeGetTime();
			Data->m_RockstarID = 0;
		}

		This.m_DoubleSided = false;
		Panel Panel(This, ::Global::UIVars::g_TitleHeaderColor);

		Panel.Item(TRANSLATE(T_RID), std::to_string(Data->m_RockstarID));
		Panel.Item(TRANSLATE(T_Name), Data->m_Name);

		std::string Added = TRANSLATE(T_SomeTimeAgo);

		uint32_t Elapsed = (GetTickCount() - Data->m_BlacklistedTime) / 1000;
		if (Elapsed > 0) {
			uint32_t Minutes = Elapsed / 60;
			uint32_t Hours = Minutes / 60;
			uint32_t Days = Hours / 24;
			uint32_t Weeks = Days / 7;
			uint32_t Months = Weeks / 4;
			uint32_t Years = Months / 12;

			if (Elapsed < 60) {
				Added = std::string(Utils::VA::VA("%u %s%s Ago", Elapsed, TRANSLATE(T_Second), Elapsed == 1 ? "" : "s"));
			} else if (Minutes < 60) {
				Added = std::string(Utils::VA::VA("%u %s%s Ago", Minutes, TRANSLATE(T_Minute), Minutes == 1 ? "" : "s"));
			} else if (Hours < 24) {
				Added = std::string(Utils::VA::VA("%u %s%s Ago", Hours, TRANSLATE(T_Hour), Hours == 1 ? "" : "s"));
			}
		}

		Panel.Item(TRANSLATE(T_Added), Added);

		return Panel.GetRenderScale();
	}

	void PanelManager::Initialize() {
#pragma region PlayerInfo
		PanelParent* PlayerInfo = new PanelParent();
		PlayerInfo->m_Render = false;
		PlayerInfo->m_ID = "PANEL_PLAYER_INFO";
		PlayerInfo->m_Name = TRANSLATE(T_PlayerInfo);

		std::unordered_map<int, int> Index;

		PanelChild PlayerInfoName;
		PlayerInfoName.m_Parent = PlayerInfo;
		PlayerInfoName.m_ID = "PANEL_PLAYER_INFO_NAME";
		PlayerInfoName.m_Name = TRANSLATE(T_Name);
		PlayerInfoName.m_Column = 0;
		PlayerInfoName.m_Index = Index[PlayerInfoName.m_Column]++;
		PlayerInfoName.m_Render = true; // render this child by default
		PlayerInfoName.m_Update = RenderPanelPlayerInfoName;

		PanelChild PlayerInfoGeneric;
		PlayerInfoGeneric.m_Parent = PlayerInfo;
		PlayerInfoGeneric.m_ID = "PANEL_PLAYER_INFO_GENERIC";
		PlayerInfoGeneric.m_Name = TRANSLATE(T_Generic);
		PlayerInfoGeneric.m_Column = 0;
		PlayerInfoGeneric.m_Index = Index[PlayerInfoGeneric.m_Column]++;
		PlayerInfoGeneric.m_Render = true; // render this child by default
		PlayerInfoGeneric.m_Update = RenderPanelPlayerInfoGeneric;

		PanelChild PlayerInfoCrew;
		PlayerInfoCrew.m_Parent = PlayerInfo;
		PlayerInfoCrew.m_ID = "PANEL_PLAYER_INFO_CREW";
		PlayerInfoCrew.m_Name = TRANSLATE(T_Crew);
		PlayerInfoCrew.m_Column = 0;
		PlayerInfoCrew.m_Index = Index[PlayerInfoCrew.m_Column]++;
		PlayerInfoCrew.m_Render = true; // render this child by default
		PlayerInfoCrew.m_Update = RenderPanelPlayerInfoCrew;

		PanelChild PlayerInfoProperties;
		PlayerInfoProperties.m_Parent = PlayerInfo;
		PlayerInfoProperties.m_ID = "PANEL_PLAYER_INFO_PROPERTIES";
		PlayerInfoProperties.m_Name = TRANSLATE(T_Properties);
		PlayerInfoProperties.m_Column = 0;
		PlayerInfoProperties.m_Index = Index[PlayerInfoProperties.m_Column]++;
		PlayerInfoProperties.m_Render = true; // render this child by default
		PlayerInfoProperties.m_Update = RenderPanelPlayerInfoProperties;

		PanelChild PlayerInfoGeo;
		PlayerInfoGeo.m_Parent = PlayerInfo;
		PlayerInfoGeo.m_ID = "PANEL_PLAYER_INFO_GEO";
		PlayerInfoGeo.m_Name = TRANSLATE(T_Geo);
		PlayerInfoGeo.m_Column = 1;
		PlayerInfoGeo.m_Index = Index[PlayerInfoGeo.m_Column]++;
		PlayerInfoGeo.m_Render = true;
		PlayerInfoGeo.m_Update = RenderPanelPlayerInfoGeo;

		PanelChild PlayerInfoFlags;
		PlayerInfoFlags.m_Parent = PlayerInfo;
		PlayerInfoFlags.m_ID = "PANEL_PLAYER_INFO_FLAGS";
		PlayerInfoFlags.m_Name = TRANSLATE(T_Flags);
		PlayerInfoFlags.m_Column = 1;
		PlayerInfoFlags.m_Index = Index[PlayerInfoFlags.m_Column]++;
		PlayerInfoFlags.m_Render = true;
		PlayerInfoFlags.m_Update = RenderPanelPlayerInfoFlags;

		PanelChild PlayerInfoVoteKickQueue;
		PlayerInfoVoteKickQueue.m_Parent = PlayerInfo;
		PlayerInfoVoteKickQueue.m_ID = "PANEL_PLAYER_INFO_VOTE_KICK_QUEUE";
		PlayerInfoVoteKickQueue.m_Name = TRANSLATE(T_KickQueue);
		PlayerInfoVoteKickQueue.m_Column = 1;
		PlayerInfoVoteKickQueue.m_Index = Index[PlayerInfoVoteKickQueue.m_Column]++;
		PlayerInfoVoteKickQueue.m_Render = true;
		PlayerInfoVoteKickQueue.m_Update = RenderPanelPlayerInfoVoteKickQueue;

		PanelChild PlayerInfoStats;
		PlayerInfoStats.m_Parent = PlayerInfo;
		PlayerInfoStats.m_ID = "PANEL_PLAYER_INFO_STATS";
		PlayerInfoStats.m_Name = TRANSLATE(T_Stats);
		PlayerInfoStats.m_Column = 1;
		PlayerInfoStats.m_Index = Index[PlayerInfoStats.m_Column]++;
		PlayerInfoStats.m_Render = true;
		PlayerInfoStats.m_Update = RenderPanelPlayerInfoStats;

		PlayerInfo->m_ChildrenPanels.reserve(10);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoName);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoGeneric);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoCrew);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoProperties);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoGeo);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoFlags);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoVoteKickQueue);
		PlayerInfo->m_ChildrenPanels.push_back(PlayerInfoStats);

		m_Panels.push_back(PlayerInfo);
#pragma endregion

#pragma region Radio
		PanelParent* Radio = new PanelParent();
		Radio->m_Render = false;
		Radio->m_ID = "PANEL_RADIO";
		Radio->m_Name = TRANSLATE(T_Radio);

		PanelChild PlayerRadio;
		PlayerRadio.m_Parent = Radio;
		PlayerRadio.m_ID = "PANEL_RADIO_INFO";
		PlayerRadio.m_Name = TRANSLATE(T_StationInfo);
		PlayerRadio.m_Column = 0;
		PlayerRadio.m_Index = 0;
		PlayerRadio.m_Render = true; // render this child by default
		PlayerRadio.m_Update = RenderPanelRadio;

		Radio->m_ChildrenPanels.reserve(10);
		Radio->m_ChildrenPanels.push_back(PlayerRadio);

		m_Panels.push_back(Radio);
#pragma endregion

#pragma region Recent
		PanelParent* RecentPlayer = new PanelParent();
		RecentPlayer->m_Render = false;
		RecentPlayer->m_ID = "PANEL_RECENT_PLAYER";
		RecentPlayer->m_Name = TRANSLATE(T_RecentPlayer);

		PanelChild RecentPlayerInfo;
		RecentPlayerInfo.m_Parent = RecentPlayer;
		RecentPlayerInfo.m_ID = "PANEL_RECENT_PLAYER_INFO";
		RecentPlayerInfo.m_Name = TRANSLATE(T_PlayerInfo);
		RecentPlayerInfo.m_Column = 0;
		RecentPlayerInfo.m_Index = 0;
		RecentPlayerInfo.m_Render = true; // render this child by default
		RecentPlayerInfo.m_Update = RenderPanelRecentPlayer;

		RecentPlayer->m_ChildrenPanels.reserve(10);
		RecentPlayer->m_ChildrenPanels.push_back(RecentPlayerInfo);

		m_Panels.push_back(RecentPlayer);
#pragma endregion

#pragma region Friend
		PanelParent* Friend = new PanelParent();
		Friend->m_Render = false;
		Friend->m_ID = "PANEL_FRIEND";
		Friend->m_Name = TRANSLATE(T_Friend);

		PanelChild RecentFriendInfo;
		RecentFriendInfo.m_Parent = Friend;
		RecentFriendInfo.m_ID = "PANEL_FRIEND_INFO";
		RecentFriendInfo.m_Name = TRANSLATE(T_FriendInfo);
		RecentFriendInfo.m_Column = 0;
		RecentFriendInfo.m_Index = 0;
		RecentFriendInfo.m_Render = true; // render this child by default
		RecentFriendInfo.m_Update = RenderPanelFriend;

		Friend->m_ChildrenPanels.reserve(10);
		Friend->m_ChildrenPanels.push_back(RecentFriendInfo);

		m_Panels.push_back(Friend);
#pragma endregion

#pragma region Overseer
		PanelParent* Overseer = new PanelParent();
		Overseer->m_Render = false;
		Overseer->m_ID = "PANEL_OVERSEER";
		Overseer->m_Name = TRANSLATE(T_Overseer);

		PanelChild OverseerInfo;
		OverseerInfo.m_Parent = Overseer;
		OverseerInfo.m_ID = "PANEL_OVERSEER_INFO";
		OverseerInfo.m_Name = TRANSLATE(T_OverseerInfo);
		OverseerInfo.m_Column = 0;
		OverseerInfo.m_Index = 0;
		OverseerInfo.m_Render = true; // render this child by default
		OverseerInfo.m_Update = RenderPanelOverseer;

		Overseer->m_ChildrenPanels.reserve(10);
		Overseer->m_ChildrenPanels.push_back(OverseerInfo);

		m_Panels.push_back(Overseer);
#pragma endregion

#pragma region Vehicle Preview
		PanelParent* VehiclePreview = new PanelParent();
		VehiclePreview->m_Render = false;
		VehiclePreview->m_ID = "PANEL_VEHICLE_PREVIEW";
		VehiclePreview->m_Name = TRANSLATE(T_VehiclePreview);

		PanelChild VehiclePreviewInfo;
		VehiclePreviewInfo.m_Parent = VehiclePreview;
		VehiclePreviewInfo.m_ID = "PANEL_VEHICLE_PREVIEW_INFO";
		VehiclePreviewInfo.m_Name = TRANSLATE(T_VehiclePreviewInfo);
		VehiclePreviewInfo.m_Column = 0;
		VehiclePreviewInfo.m_Index = 0;
		VehiclePreviewInfo.m_Render = true; // render this child by default
		VehiclePreviewInfo.m_Update = RenderPanelVehiclePreview;

		VehiclePreview->m_ChildrenPanels.reserve(10);
		VehiclePreview->m_ChildrenPanels.push_back(VehiclePreviewInfo);

		m_Panels.push_back(VehiclePreview);
#pragma endregion

#pragma region Host Queue
		PanelParent* Session = new PanelParent();
		Session->m_Render = false;
		Session->m_ID = "PANEL_SESSION";
		Session->m_Name = TRANSLATE(T_Session);

		PanelChild HostQueueInfo;
		HostQueueInfo.m_Parent = Session;
		HostQueueInfo.m_ID = "PANEL_HOST_QUEUE_INFO";
		HostQueueInfo.m_Name = TRANSLATE(T_HostQueueInfo);
		HostQueueInfo.m_Column = 0;
		HostQueueInfo.m_Index = 0;
		HostQueueInfo.m_Render = true; // render this child by default
		HostQueueInfo.m_Update = RenderPanelHostQueue;

		PanelChild BlacklistInfo;
		BlacklistInfo.m_Parent = Session;
		BlacklistInfo.m_ID = "PANEL_BLACKLIST_INFO";
		BlacklistInfo.m_Name = TRANSLATE(T_BlacklistInfo);
		BlacklistInfo.m_Column = 1;
		BlacklistInfo.m_Index = 0;
		BlacklistInfo.m_Render = true; // render this child by default
		BlacklistInfo.m_Update = RenderPanelBlacklist;

		Session->m_ChildrenPanels.reserve(10);
		Session->m_ChildrenPanels.push_back(HostQueueInfo);
		Session->m_ChildrenPanels.push_back(BlacklistInfo);

		m_Panels.push_back(Session);
#pragma endregion

#pragma region Session Browser
		PanelParent* SessionBrowser = new PanelParent();
		SessionBrowser->m_Render = false;
		SessionBrowser->m_ID = "PANEL_SESSION_BROWSER";
		SessionBrowser->m_Name = TRANSLATE(T_SessionBrowser);

		PanelChild SessionBrowserInfo;
		SessionBrowserInfo.m_Parent = SessionBrowser;
		SessionBrowserInfo.m_ID = "PANEL_SESSION_BROWSER_INFO";
		SessionBrowserInfo.m_Name = TRANSLATE(T_SessionBrowserInfo);
		SessionBrowserInfo.m_Column = 0;
		SessionBrowserInfo.m_Index = 0;
		SessionBrowserInfo.m_Render = true; // render this child by default
		SessionBrowserInfo.m_Update = RenderPanelSessionBrowser;

		SessionBrowser->m_ChildrenPanels.reserve(10);
		SessionBrowser->m_ChildrenPanels.push_back(SessionBrowserInfo);

		m_Panels.push_back(SessionBrowser);
#pragma endregion

#pragma region Blacklisted Players
		PanelParent* HostBlacklist = new PanelParent();
		HostBlacklist->m_Render = false;
		HostBlacklist->m_ID = "PANEL_BLACKLISTED_PLAYERS";
		HostBlacklist->m_Name = TRANSLATE(T_BlacklistedPlayers);

		PanelChild HostBlacklistInfo;
		HostBlacklistInfo.m_Parent = HostBlacklist;
		HostBlacklistInfo.m_ID = "PANEL_BLACKLISTED_PLAYERS_INFO";
		HostBlacklistInfo.m_Name = TRANSLATE(T_BlacklistedPlayersInfo);
		HostBlacklistInfo.m_Column = 0;
		HostBlacklistInfo.m_Index = 0;
		HostBlacklistInfo.m_Render = true; // render this child by default
		HostBlacklistInfo.m_Update = RenderPanelBlacklistedPlayer;

		HostBlacklist->m_ChildrenPanels.reserve(10);
		HostBlacklist->m_ChildrenPanels.push_back(HostBlacklistInfo);

		m_Panels.push_back(HostBlacklist);
#pragma endregion
	}

	PanelManager* GetPanelManager() {
		static PanelManager Instance;
		return &Instance;
	}
}