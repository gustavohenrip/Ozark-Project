#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"

namespace Instructionals {
	class Instructionals {
	public:
		void Setup();
		void AddInstructional(std::string Text, eControls Control);
		void AddInstructional(std::string Text, eScaleformButtons Button);
		void AddInstructional(std::string Text, int VK);
		void Close();
	private:
		int m_InstructionalCount = 0;
		int m_Handle = 0;
	}; Instructionals* GetInstructionals();

	void MainMenu();
	void Standard(bool Scroller = false, bool Keyboard = false, bool Hotkey = false);
	void Rainbow();
	void Drone();
}