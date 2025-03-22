#include "wmi.hpp"
#include "utils/utils.hpp"
#include "utils/log.hpp"
#include "utils/va.hpp"

namespace wmi {
	IWbemLocator* locator;
	IWbemServices* services;
	IEnumWbemClassObject* enumerator;

	bool initialize() {
		HRESULT result;

		result = CoInitializeEx(0, COINIT_MULTITHREADED);
		if (FAILED(result)) {
			error("Failed to initialize service (#1)"_Protect);
			return false;
		}

		CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
		result = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&locator);

		if (FAILED(result)) {
			error("Failed to initialize service (#2)"_Protect);
			CoUninitialize();
			return false;
		}

		result = locator->ConnectServer(_bstr_t(utils::va::vaw("ROOT\\CIMV2"_Protect)), NULL, NULL, 0, NULL, 0, 0, &services);
		if (FAILED(result)) {
			error("Failed to initialize service (#3)"_Protect);
			locator->Release();
			CoUninitialize();
			return false;
		}

		result = CoSetProxyBlanket(services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
		if (FAILED(result)) {
			error("Failed to initialize service (#4)"_Protect);
			services->Release();
			locator->Release();
			CoUninitialize();
			return false;
		}

		return true;
	}

	std::string getData(std::string part, std::string property) {
		HRESULT result;
		std::string output = "Error."_Protect;

		result = services->ExecQuery(bstr_t("WQL"_Protect), bstr_t((std::string("SELECT * FROM "_Protect) + part).c_str()), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator);

		if (FAILED(result)) {
			return output;
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG ret = 0;

		while (enumerator) {
			HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &pclsObj, &ret);
			if (FAILED(hr) || !ret) {
				break;
			}

			VARIANT vtProp;

			std::wstring stemp = std::wstring(property.begin(), property.end());
			LPCWSTR sw = stemp.c_str();
			hr = pclsObj->Get(sw, 0, &vtProp, 0, 0);
			if (SUCCEEDED(hr)) {
				if (vtProp.vt == VT_BSTR) {
					output = _bstr_t(vtProp.bstrVal);
				} else if (vtProp.vt == VT_I4) {
					output = std::to_string(vtProp.iVal);
				}

				VariantClear(&vtProp);
				pclsObj->Release();
				enumerator->Release();
			} else {
				enumerator->Release();
			}

			break;
		}

		return output;
	}

	void cleanup() {
		if (services) services->Release();
		if (locator) locator->Release();
		CoUninitialize();
	}
}