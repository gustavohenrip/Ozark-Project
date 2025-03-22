#pragma once

template<class T>
class singleton {
public:
	static T* getInstance() {
		static T instance;
		return &instance;
	}
};