#pragma once
#include "stdafx.hpp"
#include "utils/json.hpp"
#include "rage/types/network_types.hpp"

namespace rage::api {
	class socialclub {
	public:
		void requestThread();
		void send(std::string url, std::string data, std::function<void(std::wstring)> d);
		void getProfileFromRID(uint64_t rockstarId, std::function<void(bool, rage::network::rockstarAccount)> callback);
		void getProfileFromName(std::string name, std::function<void(bool, rage::network::rockstarAccount)> callback);
		void searchProfiles(std::string name, std::function<void(bool, std::vector<std::pair<std::string, uint64_t>>)> callback);
		void getProfileResponseFromName(std::string name, int pageIndex, std::function<void(bool, std::wstring)> callback);

		template<class T>
		static std::enable_if_t < std::is_same<T, std::string>::value || std::is_same<T, char*>::value || std::is_same<T, const char*>::value, T> safeRead(nlohmann::json json, const char* key) {
			return json[key].is_null() ? "null" : json[key].get<T>();
		}

		std::vector<std::pair<std::pair<std::string, std::string>, std::function<void(std::wstring)>>> queue;
	private:
	}; socialclub* getSocialclub();
}