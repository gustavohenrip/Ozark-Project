#pragma once
#include "stdafx.hpp"

class translationString {
public:
	translationString(std::string original, bool hasTranslation = false) {
		set(original.c_str());
		hasTranslation_ = hasTranslation;
	}

	translationString(const translationString& src) {
		*this = src;
	}

	void reset() { setMapped(original); }
	void set(const char* s) { strcpy(original, s); setMapped(original); }
	void setMapped(const char* s) { strcpy(mapped, s); }
	void setHasTranslation(bool has) { hasTranslation_ = has; }
	std::string getOriginal() { return original; }
	std::string get() { return hasTranslation_ ? mapped : original; }
	bool hasTranslation() { return hasTranslation_; }
private:
	char original[100];
	char mapped[100];
	bool hasTranslation_;
};