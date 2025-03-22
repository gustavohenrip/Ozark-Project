#include "shop_locations.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"

using namespace shopLocationsMenuVars;

namespace shopLocationsMenuVars {
	vars_ vars;

	scrollStruct<math::vector3<float>> stDennis[17] = {
		{ "Gunsmith", { 2725.589111f, -1282.219360f, 49.374f} },
		{ "Photo Studio", { 2734.479004f, -1112.595825f, 48.899f } },
		{ "Saloon", { 2789.246338f, -1179.293091f, 47.975f } },
		{ "Frontier.st Saloon", { 2621.000000f, -1213.125000f, 53.420f } },
		{ "Fence", { 2845.356689f, -1205.543213f, 47.692f } },
		{ "Trapper", { 2829.402832f, -1226.295776f, 47.613f } },
		{ "General Store", { 2818.676758f, -1306.859741f, 46.648f } },
		{ "Butcher", { 2802.394043f, -1317.376465f, 46.457123f } },
		{ "Doctor", { 2720.251465f, -1239.176025f, 50.066f } },
		{ "Barber", { 2662.076904f, -1181.335938f, 53.227f } },
		{ "Movies", { 2684.273438f, -1365.074951f, 47.406f } },
		{ "Vaudeville Movies", { 2541.292480f, -1266.018188f, 49.204f } },
		{ "Post Office", { 2736.017578f, -1389.087891f, 46.196f } },
		{ "Stable", { 2516.078369f, -1433.861938f, 46.376f } },
		{ "Dominoes", { 2507.509521f, -1254.313965f, 49.189f } },
		{ "Clothing Store", { 2548.750000f, -1178.559204f, 53.312f } },
		{ "Boat Docs", { 2718.303f, -1517.282f, 44.174f } },
	};

	scrollStruct<math::vector3<float>> valentine[11] = {
		{ "Post Office", {-183.825241f, 616.845764f, 113.445f } },
		{ "Saloon", {-245.479156f, 767.946960f, 121.088f } },
		{ "Main Saloon", {-306.417511f, 796.599976f, 118.957f } },
		{ "Show", {-350.701691f, 707.343445f, 116.826f } },
		{ "Gunsmith", {-279.070343f, 787.236572f, 119.502f } },
		{ "Doctor", {-285.119629f, 800.075256f, 119.414f } },
		{ "Barber", {-302.725616f, 813.175354f, 118.411f } },
		{ "General Store", {-322.242706f, 794.659729f, 120.942f } },
		{ "Hotel", {-327.496552f, 779.233948f, 117.429f } },
		{ "Butcher", {-342.078857f, 767.437134f, 116.603f } },
		{ "Stable", {-368.238007f, 776.889282f, 116.319f } },
	};

	scrollStruct<math::vector3<float>> blackwater[5] = {
		{ "Saloon", {-805.957153f, -1330.157593f, 43.668f } },
		{ "Barber", { -804.144287f, -1363.770020f, 43.502f } },
		{ "Post Office", { -866.145264f, -1332.811890f, 43.375f } },
		{ "Tailor", { -754.167969f, -1294.644409f, 43.602f } },
		{ "Meat Market",{ -748.966125f, -1275.702637f, 43.220f } },
	};

	scrollStruct<math::vector3<float>> armadillo[3] = {
		{ "General Store", { -3683.319092f, -2620.153320f, -6.773f } },
		{ "Saloon", { -3702.736084f, -2604.262695f, -10.313f } },
		{ "Gunsmith", { -3673.684326f, -2606.047852f, -13.720f } }
	};

	scrollStruct<math::vector3<float>> tumbleweed[5] = {
		{ "Stable", { -5520.086914f, -3035.010986f, -2.355f } },
		{ "Gunsmith", { -5506.661621f, -2959.126221f, 2.300f } },
		{ "General Store", { -5492.754883f, -2943.141113f, -0.467f } },
		{ "Butcher", { -5507.164063f, -2945.906494f, -1.837f } },
		{ "Saloon", { -5517.656738f, -2921.533447f, -2.417f } },
	};
}

void shopLocationsMenu::init() {
	setName("Shops");
	setParentSubmenu<teleportMenu>();

	addOption(scrollOption<math::vector3<float>>(SCROLLSELECT, "Saint Dennis")
		.addTranslation()
		.addScroll(vars.saintDennis,0,NUMOF(stDennis),stDennis)
		.addOnClick([=] {teleportMenuVars::teleportToLocation(stDennis[vars.saintDennis].result); }));

	addOption(scrollOption<math::vector3<float>>(SCROLLSELECT, "Valentine")
		.addTranslation()
		.addScroll(vars.valentine, 0, NUMOF(valentine), valentine)
		.addOnClick([=] {teleportMenuVars::teleportToLocation(valentine[vars.valentine].result); }));

	addOption(scrollOption<math::vector3<float>>(SCROLLSELECT, "Blackwater")
		.addTranslation()
		.addScroll(vars.blackwater, 0, NUMOF(blackwater), blackwater)
		.addOnClick([=] {teleportMenuVars::teleportToLocation(blackwater[vars.blackwater].result); }));

	addOption(scrollOption<math::vector3<float>>(SCROLLSELECT, "Armadillo")
		.addTranslation()
		.addScroll(vars.armadillo, 0, NUMOF(armadillo), armadillo)
		.addOnClick([=] {teleportMenuVars::teleportToLocation(armadillo[vars.armadillo].result); }));

	addOption(scrollOption<math::vector3<float>>(SCROLLSELECT, "Tumbleweed")
		.addTranslation()
		.addScroll(vars.tumbleweed, 0, NUMOF(tumbleweed), tumbleweed)
		.addOnClick([=] {teleportMenuVars::teleportToLocation(tumbleweed[vars.tumbleweed].result); }));
}

/*Called while in submenu*/
void shopLocationsMenu::update() {}

/*Called once on submenu open*/
void shopLocationsMenu::updateOnce() {}

/*Called always*/
void shopLocationsMenu::featureUpdate() {}

shopLocationsMenu* _instance;
shopLocationsMenu* shopLocationsMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new shopLocationsMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

shopLocationsMenu::~shopLocationsMenu() { delete _instance; }