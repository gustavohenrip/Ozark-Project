#include "hooks.hpp"
#include "utils/memory/memory.hpp"
#include "utils/hooking/hooking.hpp"
#include "utils/log.hpp"

void Menu::Hooks::RockstarIntegrityCheckStartHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][0], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][0], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][0], 25);
	Global::Vars::g_arxan_integ_calls[0] = true;
	return OriginalRockstarIntegrityCheckStart(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckEndHook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][1], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][1], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][1], 25);
	Global::Vars::g_arxan_integ_calls[1] = true;
	return OriginalRockstarIntegrityCheckEnd(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckStart2Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][0], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][0], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][0], 25);
	Global::Vars::g_arxan_integ_calls[2] = true;
	return OriginalRockstarIntegrityCheckStart2(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckEnd2Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][1], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][1], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][1], 25);
	Global::Vars::g_arxan_integ_calls[3] = true;
	return OriginalRockstarIntegrityCheckEnd2(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckStart3Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][0], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][0], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][0], 25);
	Global::Vars::g_arxan_integ_calls[4] = true;
	return OriginalRockstarIntegrityCheckStart3(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckEnd3Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][1], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][1], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][1], 25);
	Global::Vars::g_arxan_integ_calls[5] = true;
	return OriginalRockstarIntegrityCheckEnd3(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckStart4Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][0], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][0], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][0], 25);
	Global::Vars::g_arxan_integ_calls[6] = true;
	return OriginalRockstarIntegrityCheckStart4(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckEnd4Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][1], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][1], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][1], 25);
	Global::Vars::g_arxan_integ_calls[7] = true;
	return OriginalRockstarIntegrityCheckEnd4(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckStart5Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][0], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][0], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][0], 25);
	Global::Vars::g_arxan_integ_calls[8] = true;
	return OriginalRockstarIntegrityCheckStart5(Rcx, Rdx, R8, R9);
}

void Menu::Hooks::RockstarIntegrityCheckEnd5Hook(uint64_t Rcx, uint64_t Rdx, uint64_t R8, uint64_t R9) {
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[0], Global::Vars::g_ArxanRestoreBytes[0][1], 25);
	//memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[1], Global::Vars::g_ArxanRestoreBytes[1][1], 25);
	memcpy((void*)Global::Vars::g_ArxanRestoreAddresses[2], Global::Vars::g_ArxanRestoreBytes[2][1], 25);
	Global::Vars::g_arxan_integ_calls[9] = true;
	return OriginalRockstarIntegrityCheckEnd5(Rcx, Rdx, R8, R9);
}