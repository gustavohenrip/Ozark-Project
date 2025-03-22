#include "hooks.hpp"
#include "utils/log.hpp"

bool Menu::Hooks::PrepareRequestUriHook(uint64_t Rcx, uint64_t Rdx) {
	bool Return = OriginalPrepareRequestUri(Rcx, Rdx);

	char* Request = *(char**)(Rcx);
	if (Request != nullptr) {
		LOG_INFO("%s", Request);

		/*std::string RequestUri = Request;
		size_t Pos = RequestUri.rfind("/global/");

		if (Pos != std::string::npos) {
			if (RequestUri.length() > Pos + 8) {
				auto v = std::find_if(Global::Vars::g_NewsContexts.begin(), Global::Vars::g_NewsContexts.end(), [=] (std::pair<std::string, CustomNewsContext> Elem) {
					return Elem.first == RequestUri.substr(Pos + 8);
				});

				if (v != Global::Vars::g_NewsContexts.end()) {
					printf("[news] replacing %s with %s\n", Request, v->second.m_ImageUri.c_str());
					strcpy(Request, v->second.m_ImageUri.c_str());
				}
			}
		}*/
	}

	return Return;
}