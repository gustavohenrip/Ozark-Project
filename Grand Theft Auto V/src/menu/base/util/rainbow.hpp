#pragma once
#include "stdafx.hpp"
#include "global/ui_vars.hpp"

namespace Menu::Rainbow {
	class Rainbow {
	public:
		Rainbow(int Min = 25, int Max = 250, int Steps = 25);

		void Add(Color* Col);
		void Remove(Color* Col);
		void Run();
		void Start();
		void Stop();
		void Toggle(bool Toggle);
		Color GetDefault(Color* Col);

		int m_Min;
		int m_Max;
		int m_Steps;
		Color m_Color;
	private:
		int m_Increment;

		bool m_Enabled;
		std::vector<Color*> m_Colors;
		std::vector<std::pair<Color*, Color>> m_DefaultColors;
	};
}