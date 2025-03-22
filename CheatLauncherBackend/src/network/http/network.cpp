#include "network.hpp"

namespace Network {
	std::wstring Convert(std::string in) {
		std::wstring wsTmp(in.begin(), in.end());
		return wsTmp;
	}

	Request::Request(std::string URL, PROGRESSPROC Prog) {
		m_HttpClient = HttpClient(Convert(URL), Prog);
	}

	Request& Request::addParam(std::string ParamName, std::string ParamValue) {
		m_Params.push_back({ ParamName, ParamValue });
		return *this;
	}

	Request& Request::addHeader(std::string HeaderName, std::string HeaderValue) {
		m_Headers.push_back({ HeaderName, HeaderValue });
		return *this;
	}

	Request& Request::sendRequest() {
		m_HttpClient.SetRequireValidSslCertificates(true);

		std::string Param = "";
		for (auto& P : m_Params) {
			if (Param.empty()) {
				Param = P.first + "="_Protect + P.second;
			} else {
				Param += "&"_Protect + P.first + "="_Protect + P.second;
			}
		}

		m_HttpClient.SetAdditionalDataToSend((BYTE*)Param.c_str(), (unsigned int)Param.length());

		std::string Headers = "Content-Length: "_Protect;
		Headers += std::to_string(Param.length());
		Headers += "\r\nContent-Type: application/x-www-form-urlencoded\r\n"_Protect;

		for (auto& H : m_Headers) {
			Headers += H.first + ": " + H.second + "\r\n"_Protect;
		}

		m_HttpClient.SetAdditionalRequestHeaders(Convert(Headers));
		m_HttpClient.SendHttpRequest(Convert("POST"_Protect));

		return *this;
	}

	bool Request::getResponse(std::string& Resp) {
		auto Response = m_HttpClient.GetResponseContent();
		if (Response.empty()) return false;
		Resp = std::string(Response.begin(), Response.end());;
		return true;
	}

	Request& Request::saveResponseToFile(bool* Result, std::string SaveFile) {
		*Result = m_HttpClient.SaveResponseToFile(Convert(SaveFile));
		return *this;
	}
}