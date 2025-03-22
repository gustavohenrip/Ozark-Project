#include "wmi.hpp"
#include <comdef.h>
#include <Wbemidl.h>
#include "utils/log.hpp"
#include "utils/va.hpp"
#pragma comment(lib, "wbemuuid.lib")

namespace WMI {
	IWbemLocator* g_Locator;
	IWbemServices* g_Services;
	IEnumWbemClassObject* g_Enumerator;

	bool Initialize() {
		HRESULT Result;

		Result = CoInitializeEx(0, COINIT_MULTITHREADED);
		if (FAILED(Result)) {
			LOG_ERROR("Failed to initialize service (#1)"_Protect);
			return false;
		}

		CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
		Result = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&g_Locator);

		if (FAILED(Result)) {
			LOG_ERROR("Failed to initialize service (#2)"_Protect);
			CoUninitialize();
			return false;
		}

		Result = g_Locator->ConnectServer(_bstr_t(Utils::VA::VAW("ROOT\\CIMV2"_Protect)), NULL, NULL, 0, NULL, 0, 0, &g_Services);
		if (FAILED(Result)) {
			LOG_ERROR("Failed to initialize service (#3)"_Protect);
			g_Locator->Release();
			CoUninitialize();
			return false;
		}

		Result = CoSetProxyBlanket(g_Services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
		if (FAILED(Result)) {
			LOG_ERROR("Failed to initialize service (#4)"_Protect);
			g_Services->Release();
			g_Locator->Release();
			CoUninitialize();
			return false;
		}

		return true;
	}

	std::string Read(std::string Part, std::string Property) {
		std::string Output = "Error."_Protect;

		HRESULT Result = g_Services->ExecQuery(bstr_t("WQL"_Protect), bstr_t((std::string("SELECT * FROM "_Protect) + Part).c_str()), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &g_Enumerator);

		if (FAILED(Result)) {
			return Output;
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG ret = 0;

		while (g_Enumerator) {
			HRESULT hr = g_Enumerator->Next(WBEM_INFINITE, 1, &pclsObj, &ret);
			if (FAILED(hr) || !ret) {
				break;
			}

			VARIANT vtProp;

			std::wstring stemp = std::wstring(Property.begin(), Property.end());
			LPCWSTR sw = stemp.c_str();
			hr = pclsObj->Get(sw, 0, &vtProp, 0, 0);
			if (SUCCEEDED(hr)) {
				if (vtProp.vt == VT_BSTR) {
					Output = _bstr_t(vtProp.bstrVal);
				} else if (vtProp.vt == VT_I4) {
					Output = std::to_string(vtProp.iVal);
				}

				VariantClear(&vtProp);
				pclsObj->Release();
				g_Enumerator->Release();
			} else {
				g_Enumerator->Release();
			}

			break;
		}

		return Output;
	}

	void Cleanup() {
		if (g_Services) g_Services->Release();
		if (g_Locator) g_Locator->Release();
		CoUninitialize();
	}
}