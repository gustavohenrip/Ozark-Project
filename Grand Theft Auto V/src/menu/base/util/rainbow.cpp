#include "rainbow.hpp"

namespace Menu::Rainbow {
	Rainbow::Rainbow(int Min, int Max, int Steps) {
		m_Min = Min;
		m_Max = Max;
		m_Steps = Steps;
	}

	void Rainbow::Add(Color* Col) {
		m_Colors.push_back(Col);
		m_DefaultColors.push_back(std::make_pair(Col, *Col));
	}

	Color Rainbow::GetDefault(Color* Col) {
		for (auto& Def : m_DefaultColors) {
			if (Def.first == Col) {
				return Def.second;
			}
		}

		return Color();
	}

	void Rainbow::Remove(Color* Col) {
		auto Vit = std::find(m_Colors.begin(), m_Colors.end(), Col);
		if (Vit != m_Colors.end()) {
			*Col = GetDefault(Col);
			m_Colors.erase(Vit);

			for (std::size_t i = 0; i < m_DefaultColors.size(); i++) {
				if (m_DefaultColors[i].first == Col) {
					m_DefaultColors.erase(m_DefaultColors.begin() + i);
					break;
				}
			}
		}
	}

	void Rainbow::Run() {
		if (m_Enabled) {
			int Step = (m_Max - m_Min) / m_Steps;

			switch (m_Increment) {
				case 0:
					m_Color.G += Step;
					if (m_Color.G >= m_Max) {
						m_Color.G = m_Max;
						m_Increment++;
					}

					break;
				case 1:
					m_Color.R -= Step;
					if (m_Color.R <= m_Min) {
						m_Color.R = m_Min;
						m_Increment++;
					}

					break;
				case 2:
					m_Color.B += Step;
					if (m_Color.B >= m_Max) {
						m_Color.B = m_Max;
						m_Increment++;
					}

					break;
				case 3:
					m_Color.G -= Step;
					if (m_Color.G <= m_Min) {
						m_Color.G = m_Min;
						m_Increment++;
					}

					break;
				case 4:
					m_Color.R += Step;
					if (m_Color.R >= m_Max) {
						m_Color.R = m_Max;
						m_Increment++;
					}

					break;
				case 5:
					m_Color.B -= Step;
					if (m_Color.B <= m_Min) {
						m_Color.B = m_Min;
						m_Increment = 0;
					}

					break;
			}

			m_Color.A = 255;

			for (auto C : m_Colors) {
				*C = m_Color;
			}
		}
	}

	void Rainbow::Start() {
		m_Enabled = true;
	}

	void Rainbow::Stop() {
		m_Enabled = false;

		for (auto C : m_Colors) {
			*C = GetDefault(C);
		}
	}

	void Rainbow::Toggle(bool Toggle) {
		if (Toggle) {
			Start();
		} else {
			Stop();
		}
	}
}