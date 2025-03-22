#include "translation_string.hpp"
#include "global/variables.hpp"

void TranslationString::RegisterTranslation() {
	Global::Vars::g_TranslationTable.push_back(this);
}