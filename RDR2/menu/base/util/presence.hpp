#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"

namespace menu {
	class presence {
	public:
		presenceInfo decodeHandle(const char* in);
		std::string buildHandleString();
		std::string buildHandleString(uint64_t rid);
		bool sendRockstarMessage(uint64_t id, const char* msg);
		bool sendCrewMessage(uint64_t id, const char* msg, const char* crewTag, int crewId);
		bool joinMe(uint64_t id);
		bool forceRidIntoSession(uint64_t rid, uint64_t toRid);
		bool kickPlayer(uint64_t rid);
		void joinThem(uint64_t id);
		void gameTriggerEvent(uint64_t id);
		bool invitePlayer(uint64_t to, uint64_t from);
		bool socialclubMessageNotify(uint64_t to, std::string name);
		bool refreshSession(std::function<void()> callback);
	}; presence* getPresence();
}