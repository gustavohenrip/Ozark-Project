#include "menu/base/submenu.hpp"

class NetworkCasinoMenu : public Menu::Submenu {
public:
	static NetworkCasinoMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkCasinoMenu()
		: Menu::Submenu() {}

	~NetworkCasinoMenu();
};

namespace NetworkCasinoMenuVars {
	struct Vars_ {};

	extern Vars_ m_Vars;
}