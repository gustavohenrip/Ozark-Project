#pragma once
#include "stdafx.hpp"
#include "global/ui_vars.hpp"

namespace utils {
	class config {
	public:
		void initialize();
		bool doesFileExist(std::string file);
		std::vector<std::string> getFilesInDirectory(std::string folder, std::string extension = ".txt");
		std::string createColorString(color c);
		color parseColorString(std::string str);

		const char* readString(const char* section, const char* key, const char* defaultString = "");
		int readInt(const char* section, const char* key, int defaultInt = 0);
		uint64_t readUInt64(const char* section, const char* key, int defaultInt = 0);
		float readFloat(const char* section, const char* key, float defaultFloat = 0.f);
		bool readBool(const char* section, const char* key, bool defaultBool = false);
		void readColor(const char* section, const char* key, color* pcol);

		void writeString(const char* section, const char* key, const char* value);
		void writeInt(const char* section, const char* key, int value);
		void writeUInt64(const char* section, const char* key, uint64_t value);
		void writeFloat(const char* section, const char* key, float value);
		void writeBool(const char* section, const char* key, bool value);
		void writeColor(const char* section, const char* key, color value);

		const char* getDocumentsPath() { return documentsPath; }
		const char* getAppDataPath() { return appdataPath; }
		const char* getTempPath() { return tmpPath; }
		const char* getConfigPath() { return configPath; }
		const char* getLogPath() { return logPath; }
		const char* getAuthPath() { return authPath; }
		const char* getThemePath() { return themePath; }
		const char* getTranslationPath() { return translationPath; }
		const char* getScriptHookPath() { return scriptHookPath; }
		const char* getOverseerPath() { return overseerPath; }
		const char* getOutfitsPath() { return ozarkOutfitsPath; }
		const char* getTeleportSavePath() { return teleportSavePath; }
		const char* getRecentPlayersPath() { return recentPlayersSavePath; }
		const char* getTextMessageSavePath() { return textMessageSavePath; }
		const char* getWindowsFilePath() { return windowsFilePath; }
	private:
		char documentsPath[MAX_PATH];
		char windowsFilePath[MAX_PATH];
		char appdataPath[MAX_PATH];
		char tmpPath[MAX_PATH];
		char configPath[MAX_PATH];
		char logPath[MAX_PATH];
		char authPath[MAX_PATH];
		char themePath[MAX_PATH];
		char translationPath[MAX_PATH];
		char scriptHookPath[MAX_PATH];
		char overseerPath[MAX_PATH];
		char ozarkOutfitsPath[MAX_PATH];
		char teleportSavePath[MAX_PATH];
		char recentPlayersSavePath[MAX_PATH];
		char textMessageSavePath[MAX_PATH];
	}; config* getConfig();
}