#include "config.hpp"
#include "va.hpp"
#include "utils.hpp"
#include <shlobj.h>
#include <shlwapi.h>
#include <direct.h>

namespace utils {
	void config::initialize() {
		char localTmp[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documentsPath);
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appdataPath);
		SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, localTmp);

		// create folders that don't alread exist
		_mkdir(utils::va::va("%s\\Ozark\\Red Dead Redemption 2"_Protect, documentsPath));
		_mkdir(utils::va::va("%s\\Ozark\\Red Dead Redemption 2\\ScriptHook"_Protect, documentsPath));
		_mkdir(utils::va::va("%s\\Ozark\\Red Dead Redemption 2\\Themes"_Protect, documentsPath));
		_mkdir(utils::va::va("%s\\Ozark\\Red Dead Redemption 2\\Translations"_Protect, documentsPath));
		_mkdir(utils::va::va("%s\\Ozark\\Red Dead Redemption 2\\Outfits"_Protect, documentsPath));

		// setup the paths
		snprintf(tmpPath, MAX_PATH, "%s\\"_Protect, localTmp);
		snprintf(configPath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Config.ini"_Protect, documentsPath);
		snprintf(logPath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Log.txt"_Protect, documentsPath);
		snprintf(authPath, MAX_PATH, "%s\\Ozark\\Ozark.auth"_Protect, documentsPath);
		snprintf(themePath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Themes\\"_Protect, documentsPath);
		snprintf(translationPath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Translations\\"_Protect, documentsPath);
		snprintf(scriptHookPath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\ScriptHook\\"_Protect, documentsPath);
		snprintf(overseerPath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Overseer.json", documentsPath);
		snprintf(ozarkOutfitsPath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Outfits\\"_Protect, documentsPath);
		snprintf(teleportSavePath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Teleports.json", documentsPath);
		snprintf(recentPlayersSavePath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Recent Players.json", documentsPath);
		snprintf(textMessageSavePath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Text Messages.json", documentsPath);
		snprintf(windowsFilePath, MAX_PATH, "%s\\Ozark\\Red Dead Redemption 2\\Windows7", documentsPath);
	}

	bool config::doesFileExist(std::string file) {
		std::ifstream stream(file);
		if (stream.good()) {
			stream.close();
			return true;
		}

		return false;
	}

	std::vector<std::string> config::getFilesInDirectory(std::string folder, std::string extension) {
		std::vector<std::string> buffer;

		std::string loc = folder + "*" + extension;
		WIN32_FIND_DATAA fd;
		auto vit = FindFirstFileA(loc.c_str(), &fd);
		if (vit != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					std::string orig = fd.cFileName;
					buffer.push_back(orig.substr(0, orig.length() - extension.length()));
				}
			} while (FindNextFileA(vit, &fd));
			FindClose(vit);
		}

		return buffer;
	}

	std::string config::createColorString(color c) {
		return std::to_string(c.r) + ";" + std::to_string(c.g) + ";" + std::to_string(c.b) + ";" + std::to_string(c.a) + ";";
	}

	color config::parseColorString(std::string str) {
		std::vector<std::string> sc = utils::split(str, ";");
		if (sc.size() == 4) return { stoi(sc.at(0)), stoi(sc.at(1)), stoi(sc.at(2)), stoi(sc.at(3)) };
		else return { 0, 0, 0, 0 };
	}

	template<typename Type>
	Type readNumber(const char* section, const char* key, const char* format, Type defaultVal, Type(*func)(const char* text)) {
		char resultBuffer[255];
		char defaultBuffer[255];
		sprintf_s(defaultBuffer, format, defaultVal);
		GetPrivateProfileStringA(section, key, defaultBuffer, resultBuffer, 255, getConfig()->getConfigPath());
		return func(resultBuffer);
	}

	const char* config::readString(const char* section, const char* key, const char* defaultString) {
		static char currentOptionBuffer[255];
		GetPrivateProfileStringA(section, key, defaultString, currentOptionBuffer, 255, getConfigPath());
		return currentOptionBuffer;
	}

	int config::readInt(const char* section, const char* key, int defaultInt) {
		return readNumber<int>(section, key, "%d", defaultInt, [] (const char* text) { return atoi(text); });
	}

	uint64_t config::readUInt64(const char* section, const char* key, int defaultInt) {
		return readNumber<uint64_t>(section, key, "%I64d", defaultInt, [] (const char* text) { return (uint64_t)atoll(text); });
	}

	float config::readFloat(const char* section, const char* key, float defaultFloat) {
		return readNumber<float>(section, key, "%f", defaultFloat, [] (const char* text) { return (float)atof(text); });
	}

	bool config::readBool(const char* section, const char* key, bool defaultBool) {
		char currentOptionBuffer[255];
		GetPrivateProfileStringA(section, key, defaultBool ? "true" : "false", currentOptionBuffer, 255, getConfigPath());
		return strcmp(currentOptionBuffer, "true") == 0;
	}

	void config::readColor(const char* section, const char* key, color* pcol) {
		std::string temp = readString(section, key, "null");
		if (temp != "null") {
			*pcol = parseColorString(temp);
		}
	}

	template<typename Type>
	void writeNumber(const char* section, const char* key, const char* format, Type value) {
		char buffer[255];
		sprintf_s(buffer, format, value);
		WritePrivateProfileStringA(section, key, buffer, getConfig()->getConfigPath());
	}

	void config::writeString(const char* section, const char* key, const char* value) {
		WritePrivateProfileStringA(section, key, value, getConfigPath());
	}

	void config::writeInt(const char* section, const char* key, int value) {
		writeNumber<int>(section, key, "%d", value);
	}

	void config::writeUInt64(const char* section, const char* key, uint64_t value) {
		writeNumber<uint64_t>(section, key, "%I64d", value);
	}

	void config::writeFloat(const char* section, const char* key, float value) {
		writeNumber<float>(section, key, "%f", value);
	}

	void config::writeBool(const char* section, const char* key, bool value) {
		WritePrivateProfileStringA(section, key, value ? "true" : "false", getConfigPath());
	}

	void config::writeColor(const char* section, const char* key, color value) {
		WritePrivateProfileStringA(section, key, createColorString(value).c_str(), getConfigPath());
	}

	config* getConfig() {
		static config instance;
		return &instance;
	}
}