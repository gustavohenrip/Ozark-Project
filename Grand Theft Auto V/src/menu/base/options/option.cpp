#include "option.hpp"

void Option::Render(int pos) {}
void Option::InvokeSave(std::stack<std::string> MenuStack) {}
void Option::RenderSelected(int pos, std::stack<std::string> MenuStack) {}
void Option::InvokeHotkey() {}
void Option::WriteTranslation(nlohmann::json& JsobObject) {}
void Option::ReadTranslation(nlohmann::json& JsobObject) {}
void Option::ResetTranslation() {}