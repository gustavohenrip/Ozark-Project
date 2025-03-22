#include "pattern.hpp"
#include "utils/log.hpp"
#include "menu/init.hpp"

namespace memory {
	bool pattern::scanProcess(patternInfo pattern, std::function<void(uint64_t)> callback) {
		auto address = scan(nullptr, pattern.pattern);
		if (address) {
			custom(consoleForeground::MAGENTA, "Signature"_Protect, "Found %s"_Protect, pattern.name.c_str());
			callback(address + pattern.offset);
			pattern.reset();
			return true;
		}

		if (pattern.important) {
			customError("Signature"_Protect, "Failed to find %s"_Protect, pattern.name.c_str());
			menu::getInit()->unload();
		} else {
			customWarn("Signature"_Protect, "Failed to find %s"_Protect, pattern.name.c_str());
			pattern.reset();
			return true;
		}

		pattern.reset();
		return false;
	}

	bool pattern::serverScanProcess(const char* name, std::function<void(uint64_t)> callback) {
		patternInfo& pattern = global::auth::g_signatures[name];
		if (pattern.pattern.empty()) return false;

		auto address = scan(nullptr, pattern.pattern);
		if (address) {
#ifdef DEV
			custom(consoleForeground::MAGENTA, "Signature"_Protect, "Found %s @ 0x%p"_Protect, pattern.name.c_str(), address);
#else
			custom(consoleForeground::MAGENTA, "Signature"_Protect, "Found %s"_Protect, pattern.name.c_str());
#endif
			callback(address + pattern.offset);
			pattern.reset();
			return true;
		}

		if (pattern.important) {
			customError("Signature"_Protect, "Failed to find %s"_Protect, pattern.name.c_str());
			menu::getInit()->unload();
		} else {
			customWarn("Signature"_Protect, "Failed to find %s"_Protect, pattern.name.c_str());
			pattern.reset();
			return true;
		}

		pattern.reset();
		return false;
	}

	void pattern::scanModule(const char* moduleName, patternInfo pattern, std::function<void(uint64_t)> callback) {
		auto address = scan(moduleName, pattern.pattern);
		if (address) {
			custom(consoleForeground::MAGENTA, "Signature"_Protect, "Found %s"_Protect, pattern.name.c_str());
			callback(address + pattern.offset);
			return;
		}

		if (pattern.important) {
			customError("Signature"_Protect, "Failed to find %s"_Protect, pattern.name.c_str());
		} else {
			customWarn("Signature"_Protect, "Failed to find %s"_Protect, pattern.name.c_str());
		}

		callback(0);
	}

	uint64_t pattern::scan(const char* moduleName, std::string pattern) {
		MODULEINFO module = {};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(moduleName), &module, sizeof(MODULEINFO));

		std::vector<patternByte> p;
		std::istringstream iss(pattern);
		std::string w;

		while (iss >> w) {
			if (w.data()[0] == '?') {
				p.push_back(patternByte());
			} else if (w.length() == 2 && isxdigit(w.data()[0]) && isxdigit(w.data()[1])) { // Hex
				p.push_back(patternByte(w));
			} else {
				return 0;
			}
		}

		for (DWORD i = 0; i < module.SizeOfImage; i++) {
			uint8_t* currentByte = (uint8_t*)((uint64_t)module.lpBaseOfDll + i);
			bool found = true;

			for (size_t ps = 0; ps < p.size(); ps++) {
				if (p[ps].ignore == false && currentByte[ps] != p[ps].data) {
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

	pattern* getPattern() {
		static pattern instance;
		return &instance;
	}
}