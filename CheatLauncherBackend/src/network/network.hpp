#pragma once
#include "stdafx.hpp"
#include "win_http_client.hpp"

namespace Network {
	class Request {
	public:
		Request() {}
		Request(std::string URL, PROGRESSPROC Prog = 0);

		Request& addParam(std::string ParamName, std::string ParamValue);
		Request& addHeader(std::string HeaderName, std::string HeaderValue);
		Request& sendRequest();
		Request& saveResponseToFile(bool* Result, std::string SaveFile);

		bool getResponse(std::string& Resp);
		HttpClient& GetHttpClient() { return m_HttpClient; }
	private:
		HttpClient m_HttpClient;
		std::vector<std::pair<std::string, std::string>> m_Params;
		std::vector<std::pair<std::string, std::string>> m_Headers;
	};
}