#include "hooks.hpp"
#include "utils/json.hpp"

bool Menu::Hooks::ParseNewsStoryFromJsonHook(uint64_t This) {
	char* ResponseData = *(char**)(This + 0x10);
	if (ResponseData != nullptr) {
		nlohmann::json Json = nlohmann::json::parse(ResponseData);

		if (Global::Vars::g_CurrentNewsContext >= Global::Vars::g_NewsContexts.size()) {
			Global::Vars::g_CurrentNewsContext = 0;
		}

		Json["title"] = Global::Vars::g_NewsContexts.at(Global::Vars::g_CurrentNewsContext).second.m_Title;
		Json["headline"] = Global::Vars::g_NewsContexts.at(Global::Vars::g_CurrentNewsContext).second.m_Headline;
		Json["subtitle"] = Global::Vars::g_NewsContexts.at(Global::Vars::g_CurrentNewsContext).second.m_Subtitle;
		Json["content"] = Global::Vars::g_NewsContexts.at(Global::Vars::g_CurrentNewsContext).second.m_Content;
		Json["image"]["path"] = Global::Vars::g_NewsContexts.at(Global::Vars::g_CurrentNewsContext).first;

		std::string New = Json.dump();
		strcpy(ResponseData, New.c_str());

		*(uint32_t*)(This + 0x18) = std::numeric_limits<uint32_t>::max(); // Max
		*(uint32_t*)(This + 0x1C) = (uint32_t)New.length(); // Current

		Global::Vars::g_CurrentNewsContext++;
	}

	return OriginalParseNewsStoryFromJson(This);
}