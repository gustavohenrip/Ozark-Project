#pragma once
#include "utils/singleton.hpp"
#include "rage/types/base_types.hpp"
#include "menu/base/renderer.hpp"
#include "utils/json.hpp"

namespace menu {
	class panelManager;

	class panel {
	public:
		panel(const char* title, color color = { 255, 255, 255, 255 });
		void item(const char* name, const char* value = NULL);
		void item(const char* name, const char* spriteLeft, const char* spriteRight, color rgba);
		void itemFull(const char* name, const char* value = NULL);
	private:
		void panelLeftItem(const char* name, const char* value);
		void panelRightItem(const char* name, const char* value);
		void panelLeftSpriteItem(const char* name, const char* spriteLeft, const char* spriteRight, color rgba);
		void panelRightSpriteItem(const char* name, const char* spriteLeft, const char* spriteRight, color rgba);
	};

	class panelManager {
	public:
		int panelTickLeft;
		int panelTickRight;
		int panelOptionCountLeft;
		int panelOptionCountRight;

		void panelEntity(const char* title, Entity handle, int creationTime, int type);
		void panelPool(const char* title, entityPool* pool, int type);
		void panelPlayer(Player id);
		void panelOverseer(std::string rid, std::string name, std::string reason, int flags);
		void panelFile(std::string name, struct stat st);
		void panelRecentPlayer(std::string name, std::string first, std::string last, std::string ip, std::string rid);
		void panelFriend(std::string name, std::string rid, networkHandle netHandle, bool inRdr2);
		void panelPresence(std::string name, std::string rid);
	}; panelManager* getPanelManager();
}