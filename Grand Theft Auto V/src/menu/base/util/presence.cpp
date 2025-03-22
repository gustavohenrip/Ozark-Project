#include "presence.hpp"
#include "rage/types/network_types.hpp"
#include "rage/engine.hpp"
#include "rage/invoker/natives.hpp"

namespace Menu::Presence {
	bool SendPresenceEvent(uint64_t RockstarID, const char* Payload) {
		Rage::Network::RockstarIdentifier Identifier;
		Identifier.m_RockstarID = RockstarID;
		Identifier.m_Type = 3;
		Identifier.m_SubType = 0;

		std::unique_ptr<uint8_t[]> PayloadPtr = std::make_unique<BYTE[]>(1536);
		SecureZeroMemory(PayloadPtr.get(), 1536);
		memcpy(PayloadPtr.get(), Payload, strlen(Payload));
		memcpy(PayloadPtr.get() + 1024, "self", 4);

		return Rage::Engine::SendPresenceEvent(&Identifier, PayloadPtr.get());
	}

    bool InvitePlayer(uint64_t To) {
        char Payload[512];
        sprintf_s(Payload, "\"ros.mp.invite\":{\"h\":\"SC %lli\",\"n\":\"%s\",\"s\":\"%s\"}", Global::Vars::g_RsInfo->m_RockstarID, Global::Vars::g_RsInfo->m_Name, Global::Vars::g_RealGsInfo.c_str());
        return SendPresenceEvent(To, Payload);
    }
}