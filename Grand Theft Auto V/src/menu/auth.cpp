#include "auth.hpp"
#include "utils/wmi.hpp"
#include "auth/crypto/crypto.hpp"
#include "global/auth_vars.hpp"
#include "utils/log.hpp"
#include "utils/utils.hpp"
#include "utils/va.hpp"
#include "utils/config.hpp"
#include "utils/json.hpp"
#include "auth/server/server_packets.hpp"
#include <filesystem>

namespace Auth {
	LONG GetStringRegKey(HKEY hKey, const std::string& strValueName, std::string& strValue, const std::string& strDefaultValue) {
		strValue = strDefaultValue;
		CHAR szBuffer[512];
		DWORD dwBufferSize = sizeof(szBuffer);
		ULONG nError;
		nError = RegQueryValueExA(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);

		if (ERROR_SUCCESS == nError) {
			strValue = szBuffer;
		}

		return nError;
	}

	bool Run() {
		if (WMI::Initialize()) {
			std::string Str = WMI::Read("Win32_OperatingSystem"_Protect, "Manufacturer"_Protect);
			Str += WMI::Read("Win32_OperatingSystem"_Protect, "OSArchitecture"_Protect);
			Str += WMI::Read("Win32_OperatingSystem"_Protect, "SerialNumber"_Protect);
			Str += WMI::Read("Win32_OperatingSystem"_Protect, "SystemDrive"_Protect);
			Str += WMI::Read("Win32_OperatingSystem"_Protect, "WindowsDirectory"_Protect);
			Str += WMI::Read("Win32_ComputerSystem"_Protect, "Model"_Protect);
			Str += WMI::Read("Win32_ComputerSystem"_Protect, "Manufacturer"_Protect);
			Str += WMI::Read("Win32_BIOS"_Protect, "Manufacturer"_Protect);
			Str += WMI::Read("Win32_Processor"_Protect, "Name"_Protect);

			Crypto::XECRYPT_SHA512_STATE State;
			Crypto::Sha512Init(&State);
			Crypto::Sha512Update(&State, (uint8_t*)Str.data(), (uint32_t)Str.length());
			Crypto::Sha512Final(&State, Global::Auth::g_HardwareID, 64);

			Str.clear();
			WMI::Cleanup();
		} else {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize hardware"_Protect);
			return false;
		}

		if (std::filesystem::exists(Utils::GetConfig()->GetAuthPath())) {
			std::ifstream InputAuthConfig(Utils::GetConfig()->GetAuthPath());
			if (InputAuthConfig.good()) {
				nlohmann::json Temp;

				try {
					InputAuthConfig >> Temp;
				} catch (std::exception& E) {
					LOG_WARN("[JSON] %s", E.what());
				}

				InputAuthConfig.close();

				Global::Auth::g_Username = Temp["username"_Protect].get<std::string>();
				Global::Auth::g_Password = Temp["password"_Protect].get<std::string>();

				if (Global::Auth::g_Password.length() == 128) {
					// Old password from old format
					LOG_CUSTOM_ERROR("Auth"_Protect, "Detected Ozark.auth having old password format, checking registry..."_Protect);
					goto UseReg;
				}

				uint8_t Hash[64];

				Crypto::XECRYPT_SHA512_STATE State;
				Crypto::Sha512Init(&State);
				Crypto::Sha512Update(&State, (uint8_t*)Global::Auth::g_Password.data(), (uint32_t)Global::Auth::g_Password.length());
				Crypto::Sha512Final(&State, Hash, 64);

				Global::Auth::g_Password = Utils::Rot13(Utils::Rot13Number(Utils::ConvertBytesToString(Hash, 64)));
			} else {
				LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to open Ozark.auth"_Protect);
			}
		} else {
		UseReg:
			HKEY hKey;
			LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Ozark", 0, KEY_READ, &hKey);
			if (lRes == ERROR_SUCCESS) {
				if (GetStringRegKey(hKey, "Username", Global::Auth::g_Username, "") == ERROR_SUCCESS) {
					std::string Password = "";
					if (GetStringRegKey(hKey, "Password", Password, "") == ERROR_SUCCESS) {
						Global::Auth::g_Username = Crypto::Rotate(Crypto::DecryptAES(Crypto::Base64Decode(Global::Auth::g_Username), "ahfgcbfhghghtydh"_Protect, 0));
						Password = Crypto::Rotate(Crypto::DecryptAES(Crypto::Base64Decode(Password), "ahfgcbfhghghtydh"_Protect, 0));

						uint8_t Hash[64];

						Crypto::XECRYPT_SHA512_STATE State;
						Crypto::Sha512Init(&State);
						Crypto::Sha512Update(&State, (uint8_t*)Password.data(), (uint32_t)Password.length());
						Crypto::Sha512Final(&State, Hash, 64);

						Global::Auth::g_Password = Utils::Rot13(Utils::Rot13Number(Utils::ConvertBytesToString(Hash, 64)));
						goto Auth;
					}
				}

				LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to extract credentials from registry"_Protect);
				return false;
			}

			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to find credentials in registry"_Protect);
			return false;
		}

	Auth:
		if (Auth::Server::PacketPing()) {
			if (Auth::Server::PacketWelcome()) {
				if (Auth::Server::PacketGetTime()) {
					if (Auth::Server::PacketGetSignatures()) {
						LOG_CUSTOM_SUCCESS("Auth"_Protect, "Successfully passed all authentication stages"_Protect);
						return true;
					}
				}
			}
		}

		return false;
	}

	bool Heartbeat() {
		return Auth::Server::PacketHeartbeat();
	}

	void ProcessCachedMetrics() {
		std::vector<std::string> Cached = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetAppDataPath(), ".flextape"_Protect);
		if (Cached.size()) {
			for (auto& Metric : Cached) {
				std::string FilePath = Utils::VA::VA("%s/%s.flextape"_Protect, Utils::GetConfig()->GetAppDataPath().c_str(), Metric.c_str());
				if (std::filesystem::file_size(FilePath) == 201) {
					uint8_t FileBytes[201];

					FILE* File = fopen(FilePath.c_str(), "rb"_Protect);
					if (File) {
						fread(FileBytes, 1, sizeof(FileBytes), File);
						fclose(File);
						remove(FilePath.c_str());

						for (int i = 0; i < sizeof(FileBytes); i++) {
							FileBytes[i] ^= 69;
						}

						char Info[200];
						memcpy(Info, FileBytes + 1, sizeof(Info));

						switch (FileBytes[0]) {
							case Security::Metrics::METRIC_CRASHED: // don't process
								break;

							case Security::Metrics::METRIC_MODULE_DIGEST_MISMATCH:
								ProcessMetric(new Security::Metrics::MetricModuleDigestMismatch(), Info);
								break;

							case Security::Metrics::METRIC_BLACKLISTED_PROCESS:
								ProcessMetric(new Security::Metrics::MetricBlacklistedProcess(), Info);
								break;

							case Security::Metrics::METRIC_FUNCTION_INTEGRITY_FAILED:
								ProcessMetric(new Security::Metrics::MetricFunctionIntegrityFailed(), Info);
								break;

							case Security::Metrics::METRIC_THREAD_BEING_DEBUGGED:
								ProcessMetric(new Security::Metrics::MetricThreadBeingDebugged(), Info);
								break;

							case Security::Metrics::METRIC_DEBUGGING:
								ProcessMetric(new Security::Metrics::MetricDebugging(), Info);
								break;

							case Security::Metrics::METRIC_THREAD_CREATED_IN_UNMAPPED_MEMORY:
								ProcessMetric(new Security::Metrics::MetricThreadCreatedInUnmappedMemory(), Info);
								break;
						}
					}
				}
			}
		}
	}

	void ProcessMetric(Security::Metrics::Metric* Metric, std::string Info) {
		return;

		if (Metric->GetMetricID() != Security::Metrics::METRIC_THREAD_CREATED_IN_UNMAPPED_MEMORY) {
			LOG_CUSTOM_ERROR("S"_Protect, "Processing %i"_Protect, Metric->GetMetricID());
		}

		if (!Auth::Server::PacketMetric(Metric, Info)) {
			// It failed, let's cache it Justin Case™ they disconnected their internetz
			uint8_t Data[201];
			memset(Data, 0, sizeof(Data));

			Data[0] = Metric->GetMetricID();
			memcpy(Data + 1, Info.c_str(), Info.length());

			for (int i = 0; i < sizeof(Data); i++) {
				Data[i] ^= 69;
			}

			FILE* File = fopen(Utils::VA::VA("%s/%s.flextape"_Protect, Utils::GetConfig()->GetAppDataPath().c_str(), Utils::CreateRandomString(20).c_str()), "wb"_Protect);
			if (File) {
				fwrite(Data, 1, sizeof(Data), File);
				fclose(File);
			}

			Security::CloseProcess();
		}

		Metric->OnEvent();
		delete Metric;
	}
}