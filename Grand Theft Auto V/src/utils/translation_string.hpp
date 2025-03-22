#pragma once
#include "stdafx.hpp"

class TranslationString {
public:
	TranslationString() {}

	TranslationString(std::string Original, bool HasTranslation = false, bool Register = false) {
		if (Register) RegisterTranslation();
		Set(Original.c_str());
		m_HasTranslation = HasTranslation;
	}

	void Reset() {
		SetMapped(m_OriginalString);
	}

	void Set(std::string S) {
		strcpy(m_OriginalString, S.c_str());
		SetMapped(m_OriginalString);
	}

	void SetMapped(const char* S) {
		strcpy(m_MappedString, S);
	}

	void SetHasTranslation(bool Has) {
		m_HasTranslation = Has;
	}

	std::string GetOriginal() {
		return m_OriginalString;
	}

	std::string Get() {
		return m_HasTranslation ? m_MappedString : m_OriginalString;
	}

	bool HasTranslation() {
		return m_HasTranslation;
	}

	void RegisterTranslation();
private:
	char m_OriginalString[300];
	char m_MappedString[300];
	bool m_HasTranslation;
};