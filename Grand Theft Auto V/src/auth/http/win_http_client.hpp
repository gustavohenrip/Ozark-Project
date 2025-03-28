/**
*  Copyright 2008-2010 Cheng Shi.  All rights reserved.
*  Email: shicheng107@hotmail.com
*/

#ifndef WINHTTPCLIENT_H
#define WINHTTPCLIENT_H

#include "stdafx.hpp"
#include "regex.hpp"
#include "string_process.hpp"
#include <Winhttp.h>
#pragma comment(lib, "Winhttp.lib")

typedef bool(*PROGRESSPROC)(double);

static const unsigned int INT_RETRYTIMES = 3;
static wchar_t* SZ_AGENT = L"";
static const int INT_BUFFERSIZE = 10240;    // Initial 10 KB temporary buffer, double if it is not enough.

class HttpClient {
public:
	inline HttpClient(const std::wstring& url, PROGRESSPROC progressProc = NULL);
	inline ~HttpClient(void);

	// It is a synchronized method and may take a long time to finish.
	inline bool SendHttpRequest(const std::wstring& httpVerb = L"GET", bool disableAutoRedirect = false);
	inline std::wstring GetResponseHeader(void);
	inline std::wstring GetResponseContent(void);
	inline std::wstring GetResponseCharset(void);
	inline std::wstring GetResponseStatusCode(void);
	inline std::wstring GetResponseLocation(void);
	inline std::wstring GetRequestHost(void);
	inline std::wstring GetRequestURL(void);
	inline const BYTE* GetRawResponseContent(void);
	inline unsigned int GetRawResponseContentLength(void);
	inline unsigned int GetRawResponseReceivedContentLength(void);
	inline bool SaveResponseToFile(const std::wstring& filePath);
	inline std::wstring GetResponseCookies(void);
	inline bool SetAdditionalRequestCookies(const std::wstring& cookies);
	inline bool SetAdditionalDataToSend(BYTE* data, unsigned int dataSize);
	inline bool UpdateUrl(const std::wstring& url);
	inline bool ResetAdditionalDataToSend(void);
	inline bool SetAdditionalRequestHeaders(const std::wstring& additionalRequestHeaders);
	inline bool SetRequireValidSslCertificates(bool require);
	inline bool SetProxy(const std::wstring& proxy);
	inline DWORD GetLastError(void);
	inline bool SetUserAgent(const std::wstring& userAgent);
	inline bool SetForceCharset(const std::wstring& charset);
	inline bool SetProxyUsername(const std::wstring& username);
	inline bool SetProxyPassword(const std::wstring& password);
	inline bool SetTimeouts(unsigned int resolveTimeout = 0,
		unsigned int connectTimeout = 60000,
		unsigned int sendTimeout = 30000,
		unsigned int receiveTimeout = 30000);

private:
	inline HttpClient(const HttpClient& other);
	inline bool SetProgress(unsigned int byteCountReceived);

	HINTERNET m_sessionHandle;
	bool m_requireValidSsl;
	std::wstring m_requestURL;
	std::wstring m_requestHost;
	std::wstring m_responseHeader;
	std::wstring m_responseContent;
	std::wstring m_responseCharset;
	BYTE* m_pResponse;
	unsigned int m_responseByteCountReceived;   // Up to 4GB.
	PROGRESSPROC m_pfProcessProc;
	unsigned int m_responseByteCount;
	std::wstring m_responseCookies;
	std::wstring m_additionalRequestCookies;
	BYTE* m_pDataToSend;
	unsigned int m_dataToSendSize;
	std::wstring m_additionalRequestHeaders;
	std::wstring m_proxy;
	DWORD m_dwLastError;
	std::wstring m_statusCode;
	std::wstring m_userAgent;
	bool m_bForceCharset;
	std::wstring m_proxyUsername;
	std::wstring m_proxyPassword;
	std::wstring m_location;
	unsigned int m_resolveTimeout;
	unsigned int m_connectTimeout;
	unsigned int m_sendTimeout;
	unsigned int m_receiveTimeout;
};

HttpClient::HttpClient(const std::wstring& url, PROGRESSPROC progressProc)
	: m_requestURL(url),
	m_sessionHandle(NULL),
	m_requireValidSsl(false),
	m_responseHeader(L""),
	m_responseContent(L""),
	m_responseCharset(L""),
	m_requestHost(L""),
	m_pResponse(NULL),
	m_responseByteCountReceived(0),
	m_pfProcessProc(progressProc),
	m_responseByteCount(0),
	m_responseCookies(L""),
	m_additionalRequestCookies(L""),
	m_pDataToSend(NULL),
	m_dataToSendSize(0),
	m_proxy(L""),
	m_dwLastError(0),
	m_statusCode(L""),
	m_userAgent(SZ_AGENT),
	m_bForceCharset(false),
	m_proxyUsername(L""),
	m_proxyPassword(L""),
	m_location(L""),
	m_resolveTimeout(0),
	m_connectTimeout(60000),
	m_sendTimeout(30000),
	m_receiveTimeout(30000) {}

HttpClient::~HttpClient(void) {
	if (m_pResponse != NULL) {
		delete[] m_pResponse;
	}
	if (m_pDataToSend != NULL) {
		delete[] m_pDataToSend;
	}

	if (m_sessionHandle != NULL) {
		::WinHttpCloseHandle(m_sessionHandle);
	}
}

bool HttpClient::SendHttpRequest(const std::wstring& httpVerb, bool disableAutoRedirect) {
	if (m_requestURL.size() <= 0) {
		m_dwLastError = ERROR_PATH_NOT_FOUND;
		return false;
	}
	// Make verb uppercase.
	std::wstring verb = httpVerb;
	if (_wcsicmp(verb.c_str(), L"GET") == 0) {
		verb = L"GET";
	} else if (_wcsicmp(verb.c_str(), L"POST") == 0) {
		verb = L"POST";
	} else {
		m_dwLastError = ERROR_INVALID_PARAMETER;
		return false;
	}
	bool bRetVal = true;

	if (m_sessionHandle == NULL) {
		m_sessionHandle = ::WinHttpOpen(m_userAgent.c_str(),
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS,
			0);
		if (m_sessionHandle == NULL) {
			m_dwLastError = ::GetLastError();
			return false;
		}
	}

	::WinHttpSetTimeouts(m_sessionHandle,
		m_resolveTimeout,
		m_connectTimeout,
		m_sendTimeout,
		m_receiveTimeout);

	wchar_t szHostName[MAX_PATH] = L"";
	wchar_t szURLPath[MAX_PATH * 5] = L"";
	URL_COMPONENTS urlComp;
	memset(&urlComp, 0, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);
	urlComp.lpszHostName = szHostName;
	urlComp.dwHostNameLength = MAX_PATH;
	urlComp.lpszUrlPath = szURLPath;
	urlComp.dwUrlPathLength = MAX_PATH * 5;
	urlComp.dwSchemeLength = 1; // None zero

	if (::WinHttpCrackUrl(m_requestURL.c_str(), (DWORD)m_requestURL.size(), 0, &urlComp)) {
		m_requestHost = szHostName;
		HINTERNET hConnect = NULL;
		hConnect = ::WinHttpConnect(m_sessionHandle, szHostName, urlComp.nPort, 0);
		if (hConnect != NULL) {
			DWORD dwOpenRequestFlag = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
			HINTERNET hRequest = NULL;
			hRequest = ::WinHttpOpenRequest(hConnect,
				verb.c_str(),
				urlComp.lpszUrlPath,
				NULL,
				WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				dwOpenRequestFlag);
			if (hRequest != NULL) {
				// If HTTPS, then client is very susceptable to invalid certificates
				// Easiest to accept anything for now
				if (!m_requireValidSsl && urlComp.nScheme == INTERNET_SCHEME_HTTPS) {
					DWORD options = SECURITY_FLAG_IGNORE_CERT_CN_INVALID
						| SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
						| SECURITY_FLAG_IGNORE_UNKNOWN_CA;
					::WinHttpSetOption(hRequest,
						WINHTTP_OPTION_SECURITY_FLAGS,
						(LPVOID)&options,
						sizeof(DWORD));
				}

				bool bGetReponseSucceed = false;
				unsigned int iRetryTimes = 0;

				// Retry for several times if fails.
				while (!bGetReponseSucceed && iRetryTimes++ < INT_RETRYTIMES) {
					if (m_additionalRequestHeaders.size() > 0) {
						if (!::WinHttpAddRequestHeaders(hRequest, m_additionalRequestHeaders.c_str(), (DWORD)m_additionalRequestHeaders.size(), WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON)) {
							m_dwLastError = ::GetLastError();
						}
					}
					if (m_additionalRequestCookies.size() > 0) {
						std::wstring cookies = L"Cookie: ";
						cookies += m_additionalRequestCookies;
						if (!::WinHttpAddRequestHeaders(hRequest, cookies.c_str(), (DWORD)cookies.size(), WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON)) {
							m_dwLastError = ::GetLastError();
						}
					}
					if (m_proxy.size() > 0) {
						WINHTTP_PROXY_INFO proxyInfo;
						memset(&proxyInfo, 0, sizeof(proxyInfo));
						proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
						wchar_t szProxy[MAX_PATH] = L"";
						wcscpy_s(szProxy, MAX_PATH, m_proxy.c_str());
						proxyInfo.lpszProxy = szProxy;

						if (!::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo))) {
							m_dwLastError = ::GetLastError();
						}

						if (m_proxyUsername.size() > 0) {
							if (!::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY_USERNAME, (LPVOID)m_proxyUsername.c_str(), (DWORD)m_proxyUsername.size() * sizeof(wchar_t))) {
								m_dwLastError = ::GetLastError();
							}
							if (m_proxyPassword.size() > 0) {
								if (!::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY_PASSWORD, (LPVOID)m_proxyPassword.c_str(), (DWORD)(m_proxyPassword.size() * sizeof(wchar_t)))) {
									m_dwLastError = ::GetLastError();
								}
							}
						}
					}

					if (disableAutoRedirect) {
						DWORD dwDisableFeature = WINHTTP_DISABLE_REDIRECTS;
						if (!::WinHttpSetOption(hRequest, WINHTTP_OPTION_DISABLE_FEATURE, &dwDisableFeature, sizeof(dwDisableFeature))) {
							m_dwLastError = ::GetLastError();
						}
					}
					bool bSendRequestSucceed = false;
					if (::WinHttpSendRequest(hRequest,
						WINHTTP_NO_ADDITIONAL_HEADERS,
						0,
						WINHTTP_NO_REQUEST_DATA,
						0,
						m_dataToSendSize,
						NULL)) {
						bSendRequestSucceed = true;
					} else {
						// Query the proxy information from IE setting and set the proxy if any.
						WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxyConfig;
						memset(&proxyConfig, 0, sizeof(proxyConfig));
						if (::WinHttpGetIEProxyConfigForCurrentUser(&proxyConfig)) {
							if (proxyConfig.lpszAutoConfigUrl != NULL) {
								WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions;
								memset(&autoProxyOptions, 0, sizeof(autoProxyOptions));
								autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT | WINHTTP_AUTOPROXY_CONFIG_URL;
								autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP;
								autoProxyOptions.lpszAutoConfigUrl = proxyConfig.lpszAutoConfigUrl;
								autoProxyOptions.fAutoLogonIfChallenged = TRUE;
								autoProxyOptions.dwReserved = 0;
								autoProxyOptions.lpvReserved = NULL;

								WINHTTP_PROXY_INFO proxyInfo;
								memset(&proxyInfo, 0, sizeof(proxyInfo));

								if (::WinHttpGetProxyForUrl(m_sessionHandle, m_requestURL.c_str(), &autoProxyOptions, &proxyInfo)) {
									if (::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo))) {
										if (::WinHttpSendRequest(hRequest,
											WINHTTP_NO_ADDITIONAL_HEADERS,
											0,
											WINHTTP_NO_REQUEST_DATA,
											0,
											m_dataToSendSize,
											NULL)) {
											bSendRequestSucceed = true;
										}
									}
									if (proxyInfo.lpszProxy != NULL) {
										::GlobalFree(proxyInfo.lpszProxy);
									}
									if (proxyInfo.lpszProxyBypass != NULL) {
										::GlobalFree(proxyInfo.lpszProxyBypass);
									}
								} else {
									m_dwLastError = ::GetLastError();
								}
							} else if (proxyConfig.lpszProxy != NULL) {
								WINHTTP_PROXY_INFO proxyInfo;
								memset(&proxyInfo, 0, sizeof(proxyInfo));
								proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
								wchar_t szProxy[MAX_PATH] = L"";
								wcscpy_s(szProxy, MAX_PATH, proxyConfig.lpszProxy);
								proxyInfo.lpszProxy = szProxy;

								if (proxyConfig.lpszProxyBypass != NULL) {
									wchar_t szProxyBypass[MAX_PATH] = L"";
									wcscpy_s(szProxyBypass, MAX_PATH, proxyConfig.lpszProxyBypass);
									proxyInfo.lpszProxyBypass = szProxyBypass;
								}

								if (!::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo))) {
									m_dwLastError = ::GetLastError();
								}
							}

							if (proxyConfig.lpszAutoConfigUrl != NULL) {
								::GlobalFree(proxyConfig.lpszAutoConfigUrl);
							}
							if (proxyConfig.lpszProxy != NULL) {
								::GlobalFree(proxyConfig.lpszProxy);
							}
							if (proxyConfig.lpszProxyBypass != NULL) {
								::GlobalFree(proxyConfig.lpszProxyBypass);
							}
						} else {
							m_dwLastError = ::GetLastError();
						}
					}

					if (bSendRequestSucceed) {
						if (m_pDataToSend != NULL) {
							DWORD dwWritten = 0;
							if (!::WinHttpWriteData(hRequest,
								m_pDataToSend,
								m_dataToSendSize,
								&dwWritten)) {
								m_dwLastError = ::GetLastError();
							}
						}
						if (::WinHttpReceiveResponse(hRequest, NULL)) {
							DWORD dwSize = 0;
							BOOL bResult = FALSE;
							bResult = ::WinHttpQueryHeaders(hRequest,
								WINHTTP_QUERY_RAW_HEADERS_CRLF,
								WINHTTP_HEADER_NAME_BY_INDEX,
								NULL,
								&dwSize,
								WINHTTP_NO_HEADER_INDEX);
							if (bResult || (!bResult && (::GetLastError() == ERROR_INSUFFICIENT_BUFFER))) {
								wchar_t* szHeader = new wchar_t[dwSize];
								if (szHeader != NULL) {
									memset(szHeader, 0, dwSize * sizeof(wchar_t));
									if (::WinHttpQueryHeaders(hRequest,
										WINHTTP_QUERY_RAW_HEADERS_CRLF,
										WINHTTP_HEADER_NAME_BY_INDEX,
										szHeader,
										&dwSize,
										WINHTTP_NO_HEADER_INDEX)) {
										m_responseHeader.assign(szHeader);
										std::vector<std::wstring> result;
										std::wstring regExp = L"";
										if (!m_bForceCharset) {
											regExp = L"charset={[A-Za-z0-9\\-_]+}";
											if (ParseRegExp(regExp, false, 1, m_responseHeader, result) && result.size() > 0) {
												m_responseCharset = result[0];
											}
										}
										regExp = L"Content-Length: {[0-9]+}";
										if (ParseRegExp(regExp, false, 1, m_responseHeader, result) && result.size() > 0) {
											m_responseByteCount = (unsigned int)_wtoi(result[0].c_str());
										}
										regExp = L"Location: {[0-9]+}";
										if (ParseRegExp(regExp, false, 1, m_responseHeader, result) && result.size() > 0) {
											m_location = result[0];
										}
										regExp = L"Set-Cookie:\\b*{.+?}\\n";
										if (ParseRegExp(regExp, false, 1, m_responseHeader, result) && result.size() > 0) {
											for (std::vector<std::wstring>::size_type i = 0; i < result.size(); i++) {
												m_responseCookies += result[i];
												if (i != result.size() - 1) {
													m_responseCookies += L"; ";
												}
											}
											m_responseCookies = Trim(m_responseCookies, L" ");
											if (m_responseCookies.size() > 0 && m_responseCookies[m_responseCookies.size() - 1] != L';') {
												m_responseCookies += L";";
											}
										}
									}
									delete[] szHeader;
								}
							}

							dwSize = 0;
							bResult = ::WinHttpQueryHeaders(hRequest,
								WINHTTP_QUERY_STATUS_CODE,
								WINHTTP_HEADER_NAME_BY_INDEX,
								NULL,
								&dwSize,
								WINHTTP_NO_HEADER_INDEX);
							if (bResult || (!bResult && (::GetLastError() == ERROR_INSUFFICIENT_BUFFER))) {
								wchar_t* szStatusCode = new wchar_t[dwSize];
								if (szStatusCode != NULL) {
									memset(szStatusCode, 0, dwSize * sizeof(wchar_t));
									if (::WinHttpQueryHeaders(hRequest,
										WINHTTP_QUERY_STATUS_CODE,
										WINHTTP_HEADER_NAME_BY_INDEX,
										szStatusCode,
										&dwSize,
										WINHTTP_NO_HEADER_INDEX)) {
										m_statusCode = szStatusCode;
									}
									delete[] szStatusCode;
								}
							}

							unsigned int iMaxBufferSize = INT_BUFFERSIZE;
							unsigned int iCurrentBufferSize = 0;
							if (m_pResponse != NULL) {
								delete[] m_pResponse;
								m_pResponse = NULL;
							}
							m_pResponse = new BYTE[iMaxBufferSize];
							if (m_pResponse == NULL) {
								bRetVal = false;
								break;
							}
							memset(m_pResponse, 0, iMaxBufferSize);
							do {
								dwSize = 0;
								if (::WinHttpQueryDataAvailable(hRequest, &dwSize)) {
									SetProgress(iCurrentBufferSize);
									BYTE* pResponse = new BYTE[dwSize + 1];
									if (pResponse != NULL) {
										memset(pResponse, 0, (dwSize + 1) * sizeof(BYTE));
										DWORD dwRead = 0;
										if (::WinHttpReadData(hRequest,
											pResponse,
											dwSize,
											&dwRead)) {
											if (dwRead + iCurrentBufferSize > iMaxBufferSize) {
												BYTE* pOldBuffer = m_pResponse;
												m_pResponse = new BYTE[iMaxBufferSize * 2];
												if (m_pResponse == NULL) {
													m_pResponse = pOldBuffer;
													bRetVal = false;
													break;
												}
												iMaxBufferSize *= 2;
												memset(m_pResponse, 0, iMaxBufferSize);
												memcpy(m_pResponse, pOldBuffer, iCurrentBufferSize);
												delete[] pOldBuffer;
											}
											memcpy(m_pResponse + iCurrentBufferSize, pResponse, dwRead);
											iCurrentBufferSize += dwRead;
										}
										delete[] pResponse;
									}
								} else {
									m_dwLastError = ::GetLastError();
								}
							} while (dwSize > 0);
							SetProgress(iCurrentBufferSize);
							m_responseByteCountReceived = iCurrentBufferSize;

							UINT codePage = CP_ACP;
							DWORD dwFlag = MB_PRECOMPOSED;
							if (_wcsnicmp(m_responseCharset.c_str(), L"utf-8", 5) == 0) {
								codePage = CP_UTF8;
								dwFlag = 0;
							}
							int iLength = ::MultiByteToWideChar(codePage,
								dwFlag,
								(LPCSTR)m_pResponse,
								m_responseByteCountReceived + 1,
								NULL,
								0);
							if (iLength <= 0) {
								// Use CP_ACP if UTF-8 fail
								codePage = CP_ACP;
								dwFlag = MB_PRECOMPOSED;
								iLength = ::MultiByteToWideChar(codePage,
									dwFlag,
									(LPCSTR)m_pResponse,
									m_responseByteCountReceived + 1,
									NULL,
									0);
							}
							if (iLength > 0) {
								wchar_t* wideChar = new wchar_t[iLength];
								if (wideChar != NULL) {
									memset(wideChar, 0, iLength * sizeof(wchar_t));
									iLength = ::MultiByteToWideChar(codePage,
										dwFlag,
										(LPCSTR)m_pResponse,
										m_responseByteCountReceived + 1,
										wideChar,
										iLength);
									if (iLength > 0) {
										m_responseContent = wideChar;
									}
									delete[] wideChar;
								}
							}
							bGetReponseSucceed = true;

							// If the resposne html web page size is less than 200, retry.
							if (verb == L"GET" && !disableAutoRedirect) {
								std::wstring regExp = L"{<html>}";
								std::vector<std::wstring> result;
								if (ParseRegExp(regExp, false, 1, m_responseContent, result) && result.size() > 0) {
									regExp = L"{</html>}";
									if (!ParseRegExp(regExp, false, 1, m_responseContent, result) || result.size() <= 0) {
										m_dwLastError = ERROR_INVALID_DATA;
										bGetReponseSucceed = false;
									}
								}
							}
						} else {
							m_dwLastError = ::GetLastError();
						}
					}
				} // while
				if (!bGetReponseSucceed) {
					bRetVal = false;
				}

				::WinHttpCloseHandle(hRequest);
			}
			::WinHttpCloseHandle(hConnect);
		}

	}

	return bRetVal;
}

std::wstring HttpClient::GetResponseHeader(void) {
	return m_responseHeader;
}

std::wstring HttpClient::GetResponseContent(void) {
	return m_responseContent;
}

std::wstring HttpClient::GetResponseCharset(void) {
	return m_responseCharset;
}

std::wstring HttpClient::GetRequestHost(void) {
	return m_requestHost;
}

std::wstring HttpClient::GetRequestURL(void) {
	return m_requestURL;
}

bool HttpClient::SaveResponseToFile(const std::wstring& filePath) {
	if (m_pResponse == NULL || m_responseByteCountReceived <= 0) {
		return false;
	}
	FILE* f = NULL;
	int iResult = _wfopen_s(&f, filePath.c_str(), L"wb");
	if (iResult == 0 && f != NULL) {
		fwrite(m_pResponse, m_responseByteCountReceived, 1, f);
		fclose(f);
		return true;
	}

	return false;
}

bool HttpClient::SetProgress(unsigned int byteCountReceived) {
	bool bReturn = false;
	if (m_pfProcessProc != NULL && m_responseByteCount > 0) {
		double dProgress = (double)byteCountReceived * 100 / m_responseByteCount;
		m_pfProcessProc(dProgress);
		bReturn = true;
	}

	return bReturn;
}

std::wstring HttpClient::GetResponseCookies(void) {
	return m_responseCookies;
}

bool HttpClient::SetAdditionalRequestCookies(const std::wstring& cookies) {
	m_additionalRequestCookies = cookies;

	return true;
}

bool HttpClient::SetAdditionalDataToSend(BYTE* data, unsigned int dataSize) {
	if (data == NULL || dataSize < 0) {
		return false;
	}

	if (m_pDataToSend != NULL) {
		delete[] m_pDataToSend;
	}
	m_pDataToSend = NULL;
	m_pDataToSend = new BYTE[dataSize];
	if (m_pDataToSend != NULL) {
		memcpy(m_pDataToSend, data, dataSize);
		m_dataToSendSize = dataSize;
		return true;
	}

	return false;
}

// Reset additional data fields
bool HttpClient::ResetAdditionalDataToSend(void) {
	if (m_pDataToSend != NULL) {
		delete[] m_pDataToSend;
	}

	m_pDataToSend = NULL;
	m_dataToSendSize = 0;

	return true;
}

// Allow us to reset the url on subsequent requests
bool HttpClient::UpdateUrl(const std::wstring& url) {
	m_requestURL = url;
	ResetAdditionalDataToSend();

	return true;
}

bool HttpClient::SetAdditionalRequestHeaders(const std::wstring& additionalRequestHeaders) {
	m_additionalRequestHeaders = additionalRequestHeaders;

	return true;
}

bool HttpClient::SetProxy(const std::wstring& proxy) {
	m_proxy = proxy;

	return true;
}

// If we don't require valid SSL Certs then accept any
// certificate on an SSL connection
bool HttpClient::SetRequireValidSslCertificates(bool require) {
	m_requireValidSsl = require;

	return true;
}

const BYTE* HttpClient::GetRawResponseContent(void) {
	return m_pResponse;
}

unsigned int HttpClient::GetRawResponseContentLength(void) {
	return m_responseByteCount;
}

unsigned int HttpClient::GetRawResponseReceivedContentLength(void) {
	return m_responseByteCountReceived;
}

DWORD HttpClient::GetLastError(void) {
	return m_dwLastError;
}

std::wstring HttpClient::GetResponseStatusCode(void) {
	return m_statusCode;
}

bool HttpClient::SetUserAgent(const std::wstring& userAgent) {
	m_userAgent = userAgent;

	return true;
}

bool HttpClient::SetForceCharset(const std::wstring& charset) {
	m_responseCharset = charset;

	return true;
}

bool HttpClient::SetProxyUsername(const std::wstring& username) {
	m_proxyUsername = username;

	return true;
}

bool HttpClient::SetProxyPassword(const std::wstring& password) {
	m_proxyPassword = password;

	return true;
}

std::wstring HttpClient::GetResponseLocation(void) {
	return m_location;
}

bool HttpClient::SetTimeouts(unsigned int resolveTimeout,
	unsigned int connectTimeout,
	unsigned int sendTimeout,
	unsigned int receiveTimeout) {
	m_resolveTimeout = resolveTimeout;
	m_connectTimeout = connectTimeout;
	m_sendTimeout = sendTimeout;
	m_receiveTimeout = receiveTimeout;

	return true;
}

#endif // WINHTTPCLIENT_H