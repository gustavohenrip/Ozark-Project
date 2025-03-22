#pragma once
#include "stdafx.hpp"
#include "global/ui_vars.hpp"
#include "rage/types/base_types.hpp"

namespace Menu::Panels {
	class PanelManager;
	struct PanelChild;

	struct PanelCustomRecentPlayer {
		char m_Name[0x20];
		char m_IP[16];
		char m_FirstEncountered[0x50];
		char m_LastEncountered[0x50];
		uint64_t m_RockstarID;
	};

	struct PanelCustomFriend {
		char m_Name[32];
		uint64_t m_RockstarID;
		int m_State;
	};

	struct PanelCustomOverseer {
		std::string m_Name;
		std::string m_Reason;
		uint64_t m_Rid;
		int m_Flags;
	};

	struct PanelCustomVehiclePreview {
		uint32_t m_Model;
		char m_Dict[50];
		char m_Name[50];
		char m_Manufacturer[12];
		float m_MaxSpeed;
		float m_MaxBraking;
		float m_MaxTraction;
		int m_Seats;
	};

	struct PanelCustomSessionBrowser {
		uint64_t m_RockstarID;
		uint64_t m_PeerToken;
		char m_Name[0x20];
	};

	struct PanelCustomBlacklistedPlayer {
		char m_Name[0x20];
		uint64_t m_RockstarID;
		uint32_t m_BlacklistedTime;
	};

	struct PanelParent {
		bool m_Render; // needed to render *any* children
		std::string m_ID; // identifier
		std::string m_Name;
		std::unordered_map<int, Math::Vector2<float>> m_ColumnOffset; // index'd by column
		std::vector<PanelChild> m_ChildrenPanels;
	};

	struct PanelChild {
		PanelParent* m_Parent;
		bool m_Render; // render this child
		std::string m_ID; // identifier
		std::string m_Name;

		bool m_DoubleSided = true;
		int m_Index = 0; // render order
		int m_Column = 0; // 0 = next to menu, 1 = next to that, etc
		int m_PanelTickLeft = 0; // used for drawing
		int m_PanelTickRight = 0; // used for drawing
		int m_PanelOptionCountLeft = 0; // used for drawing
		int m_PanelOptionCountRight = 0; // used for drawing

		uint8_t m_CustomPtr[0x150]; // struct addresses for passing data
		Math::Vector2<float>(*m_Update)(PanelChild&); // callback for rendering, returns total height of rendered element
	};

	class Panel {
	public:
		Panel(PanelChild& Child, Color HeaderCol);
		
		void Item(std::string Name, std::string Value, int Font = 0, float FontScale = 0.3f, Color Col = { 255, 255, 255, 255 });
		void Item(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font = 0, float FontScale = 0.3f, Color Col = { 255, 255, 255, 255 }, Math::Vector2<float> Scale = { 0.0192f, 0.0336f });
		void ItemFull(std::string Name, std::string Value, int Font = 0, float FontScale = 0.3f, Color Col = { 255, 255, 255, 255 });
		void ItemFull(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font = 0, float FontScale = 0.3f, Color Col = { 255, 255, 255, 255 }, Math::Vector2<float> Scale = { 0.0192f, 0.0336f });

		Math::Vector2<float> GetRenderScale() { return { m_Width, m_Height }; }
		Math::Vector2<float> GetColumnAdjustment() { return m_ColumnAdjustment; }
		Math::Vector2<float> GetRenderingPosition();
		PanelChild* GetPanelChild() { return m_Child; }
		float GetColumnOffset() { return m_ColumnOffset; }
	private:
		void PanelLeftItem(std::string Name, std::string Value, int Font, float FontScale, Color Col);
		void PanelRightItem(std::string Name, std::string Value, int Font, float FontScale, Color Col);
		void PanelLeftSpriteItem(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font, float FontScale, Color Col = { 255, 255, 255, 255 }, Math::Vector2<float> Scale = { 0.0192f, 0.0336f });
		void PanelRightSpriteItem(std::string Name, std::string SpriteLeft, std::string SpriteRight, int Font, float FontScale, Color Col = { 255, 255, 255, 255 }, Math::Vector2<float> Scale = { 0.0192f, 0.0336f });
		
		PanelChild* m_Child;
		float m_Width = 0.f;
		float m_Height = 0.f;
		float m_ColumnOffset = 0.f;
		Math::Vector2<float> m_ColumnAdjustment;
	};

	class PanelManager {
	public:
		void Initialize();
		void Unload();
		void Update();

		void NullStructure(PanelChild& This);
		void SetStructure(PanelChild& This, void* Data, int Size);

		// Panels
		static Math::Vector2<float> RenderPanelPlayerInfoName(PanelChild& This);
		static Math::Vector2<float> RenderPanelPlayerInfoGeneric(PanelChild& This);
		static Math::Vector2<float> RenderPanelPlayerInfoFlags(PanelChild& This);
		static Math::Vector2<float> RenderPanelPlayerInfoVoteKickQueue(PanelChild& This);
		static Math::Vector2<float> RenderPanelPlayerInfoCrew(PanelChild& This);
		static Math::Vector2<float> RenderPanelPlayerInfoProperties(PanelChild& This);
		static Math::Vector2<float> RenderPanelPlayerInfoStats(PanelChild& This);
		static Math::Vector2<float> RenderPanelPlayerInfoGeo(PanelChild& This);

		static Math::Vector2<float> RenderPanelRadio(PanelChild& This);
		static Math::Vector2<float> RenderPanelRecentPlayer(PanelChild& This);
		static Math::Vector2<float> RenderPanelFriend(PanelChild& This);
		static Math::Vector2<float> RenderPanelOverseer(PanelChild& This);
		static Math::Vector2<float> RenderPanelVehiclePreview(PanelChild& This);
		static Math::Vector2<float> RenderPanelHostQueue(PanelChild& This);
		static Math::Vector2<float> RenderPanelBlacklist(PanelChild& This);
		static Math::Vector2<float> RenderPanelSessionBrowser(PanelChild& This);
		static Math::Vector2<float> RenderPanelBlacklistedPlayer(PanelChild& This);

		PanelParent* GetParent(std::string ID);
		PanelChild& GetChild(PanelParent* Parent, std::string ID);
		void Rearrange(PanelParent* Parent, std::string ID, int NewColumn, int NewPosition);
		void TogglePanelRender(std::string ParentID, bool Toggle);

		std::vector<PanelParent*>& GetPanels() {
			return m_Panels;
		}
	private:
		std::vector<PanelParent*> m_Panels;
		bool m_Rearranging = false;
	}; PanelManager* GetPanelManager();
}