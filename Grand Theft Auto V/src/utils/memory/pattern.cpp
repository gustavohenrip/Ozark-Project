#include "pattern.hpp"
#include "global/auth_vars.hpp"
#include "menu/init.hpp"
#include "utils/log.hpp"

namespace Memory {
	uint64_t Scan(std::string Pattern, const char* Name) {
		MODULEINFO module = {};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(Name), &module, sizeof(MODULEINFO));

		std::vector<PatternByte> p;
		std::istringstream iss(Pattern);
		std::string w;

		while (iss >> w) {
			if (w.data()[0] == '?') {
				p.push_back(PatternByte());
			} else if (w.length() == 2 && isxdigit(w.data()[0]) && isxdigit(w.data()[1])) { // Hex
				p.push_back(PatternByte(w));
			} else {
				return 0;
			}
		}

		for (uint32_t i = 0; i < module.SizeOfImage; i++) {
			uint8_t* currentByte = (uint8_t*)((uint64_t)module.lpBaseOfDll + i);
			bool found = true;

			for (size_t ps = 0; ps < p.size(); ps++) {
				if (p[ps].m_Ignore == false && currentByte[ps] != p[ps].m_Data) {
					found = false;
					break;
				}
			}

			if (found) {
				return (uint64_t)currentByte;
			}
		}

		return 0;
	}

	uint64_t ScanFromBase(std::string Pattern, uint32_t Offset) {
		MODULEINFO module = {};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(0), &module, sizeof(MODULEINFO));

		std::vector<PatternByte> p;
		std::istringstream iss(Pattern);
		std::string w;

		while (iss >> w) {
			if (w.data()[0] == '?') {
				p.push_back(PatternByte());
			} else if (w.length() == 2 && isxdigit(w.data()[0]) && isxdigit(w.data()[1])) { // Hex
				p.push_back(PatternByte(w));
			} else {
				return 0;
			}
		}

		for (uint32_t i = Offset; i < module.SizeOfImage; i++) {
			uint8_t* currentByte = (uint8_t*)((uint64_t)module.lpBaseOfDll + i);
			bool found = true;

			for (size_t ps = 0; ps < p.size(); ps++) {
				if (p[ps].m_Ignore == false && currentByte[ps] != p[ps].m_Data) {
					found = false;
					break;
				}
			}

			if (found) {
				return (uint64_t)currentByte;
			}
		}

		return 0;
	}

	bool ServerScanProcess(const char* Name, std::function<void(uint64_t)> Callback) {
		PatternInfo& Pattern = Global::Auth::g_Signatures[Name];
		if (Pattern.m_Pattern.empty()) {
			LOG_CUSTOM_WARN("Signature"_Protect, "Failed to find %s"_Protect, Name);
			return false;
		}

		uint64_t Address = Scan(Pattern.m_Pattern);
		if (Address) {
#ifdef DEV
			LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s @ 0x%p"_Protect, Pattern.m_Name.c_str(), Address);
#else
			LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s"_Protect, Pattern.m_Name.c_str());
#endif
			Callback(Address + Pattern.m_Offset);
			Pattern.Reset();
			return true;
		}

		if (Pattern.m_Important) {
			LOG_CUSTOM_ERROR("Signature"_Protect, "Failed to find %s"_Protect, Pattern.m_Name.c_str());
#ifdef DEV
			return true;
#else
			Menu::GetInit()->Unload();
#endif
		} else {
			LOG_CUSTOM_WARN("Signature"_Protect, "Failed to find %s"_Protect, Pattern.m_Name.c_str());
			Pattern.Reset();
			return true;
		}

		Pattern.Reset();
		return false;
	}

	bool ScanProcess(PatternInfo Pattern, std::function<void(uint64_t)> Callback) {
		uint64_t Address = Scan(Pattern.m_Pattern);
		if (Address) {
#ifdef DEV
			LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s @ 0x%p"_Protect, Pattern.m_Name.c_str(), Address);
#else
			LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s"_Protect, Pattern.m_Name.c_str());
#endif

			Callback(Address + Pattern.m_Offset);
			Pattern.Reset();
			return true;
		}

		if (Pattern.m_Important) {
			LOG_CUSTOM_ERROR("Signature"_Protect, "Failed to find %s"_Protect, Pattern.m_Name.c_str());
#ifdef DEV
			return true;
#else
			Menu::GetInit()->Unload();
#endif
		} else {
			LOG_CUSTOM_WARN("Signature"_Protect, "Failed to find %s"_Protect, Pattern.m_Name.c_str());
			Pattern.Reset();
			return true;
		}

		Pattern.Reset();
		return false;
	}

	bool ScanProcessModule(const char* Name, PatternInfo Pattern, std::function<void(uint64_t)> Callback) {
		uint64_t Address = Scan(Pattern.m_Pattern, Name);
		if (Address) {
#ifdef DEV
			LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s @ 0x%p"_Protect, Pattern.m_Name.c_str(), Address);
#else
			LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s"_Protect, Pattern.m_Name.c_str());
#endif

			Callback(Address + Pattern.m_Offset);
			Pattern.Reset();
			return true;
		}

		if (Pattern.m_Important) {
			LOG_CUSTOM_ERROR("Signature"_Protect, "Failed to find %s"_Protect, Pattern.m_Name.c_str());
#ifdef DEV
			return true;
#else
			Menu::GetInit()->Unload();
#endif
		} else {
			LOG_CUSTOM_WARN("Signature"_Protect, "Failed to find %s"_Protect, Pattern.m_Name.c_str());
			Pattern.Reset();
			return true;
		}

		Pattern.Reset();
		return false;
	}

	void PatternBatch::Submit(PatternInfo Pattern, std::function<void(uint64_t)> Callback) {
		std::vector<PatternByte> p;
		std::istringstream iss(Pattern.m_Pattern);
		std::string w;

		while (iss >> w) {
			if (w.data()[0] == '?') {
				p.push_back(PatternByte());
			} else if (w.length() == 2 && isxdigit(w.data()[0]) && isxdigit(w.data()[1])) { // Hex
				p.push_back(PatternByte(w));
			} else {
				return;
			}
		}

		m_Buffer.push_back({ Pattern, p, Callback, false, 0 });
	}

	void PatternBatch::Submit(const char* Name, std::function<void(uint64_t)> Callback) {
		PatternInfo& Pattern = Global::Auth::g_Signatures[Name];
		if (Pattern.m_Pattern.empty()) return;
		Submit(Pattern, Callback);
	}
	
	void PatternBatch::Flush() {
		MODULEINFO module = {};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(0), &module, sizeof(MODULEINFO));

		for (uint32_t i = 0; i < module.SizeOfImage; i++) {
			uint8_t* CurrentByte = (uint8_t*)((uint64_t)module.lpBaseOfDll + i);

			for (auto& Batch : m_Buffer) {
				if (Batch.m_Found) continue;

				bool Found = true;
				for (size_t ps = 0; ps < Batch.m_Bytes.size(); ps++) {
					if (Batch.m_Bytes[ps].m_Ignore == false && CurrentByte[ps] != Batch.m_Bytes[ps].m_Data) {
						Found = false;
						break;
					}
				}

				if (Found) {
					Batch.m_Found = true;
					Batch.m_Address = (uint64_t)CurrentByte;
				}
			}
		}

		for (auto& Batch : m_Buffer) {
			if (Batch.m_Found && Batch.m_Address) {
#ifdef DEV
				LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s @ 0x%p"_Protect, Batch.m_Pattern.m_Name.c_str(), Batch.m_Address);
#else
				LOG_CUSTOM(ConsoleForeground::MAGENTA, "Signature"_Protect, "Found %s"_Protect, Batch.m_Pattern.m_Name.c_str());
#endif

				Batch.m_Callback(Batch.m_Address + Batch.m_Pattern.m_Offset);
				Batch.m_Pattern.Reset();
				continue;
			}

			if (Batch.m_Pattern.m_Important) {
				LOG_CUSTOM_ERROR("Signature"_Protect, "Failed to find %s"_Protect, Batch.m_Pattern.m_Name.c_str());
				Menu::GetInit()->Unload();
				return;
			} else {
				LOG_CUSTOM_WARN("Signature"_Protect, "Failed to find %s"_Protect, Batch.m_Pattern.m_Name.c_str());
				Batch.m_Pattern.Reset();
			}
		}
	}
}