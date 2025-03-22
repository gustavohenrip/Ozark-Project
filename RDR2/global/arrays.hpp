#pragma once
#include "stdafx.hpp"
#include "rage/types/invoker_types.hpp"
#include "menu/base/options/scroll_option.hpp"

namespace global::arrays {
	struct particle {
		const char* first;
		const char* second;
		float scale;
		bool looped;
		const char* evolution;
	};

	extern scrollStruct<Hash> g_weaponList[56];
	extern scrollStruct<Hash> g_ammoList[63];

	extern scrollStruct<Hash> g_pedModelOnline[2];
	extern scrollStruct<Hash> g_pedModelAlligators[2];
	extern scrollStruct<Hash> g_pedModelPanthers[3];
	extern scrollStruct<Hash> g_pedModelDogs[15];
	extern scrollStruct<variants> g_pedLegendary[43];
	extern scrollStruct<Hash> g_pedModelSparrows[7];
	extern scrollStruct<Hash> g_pedModelSnake[12];
	extern scrollStruct<Hash> g_pedModelMales[38];
	extern scrollStruct<Hash> g_pedModelFemales[50];
	extern scrollStruct<Hash> g_pedModelHorses[105];

	extern scrollStruct<Hash> g_vehicleModelTrain[19];
	extern scrollStruct<Hash> g_vehicleModelWagon[22];
	extern scrollStruct<Hash> g_vehicleModelBoat[15];
	extern scrollStruct<Hash> g_vehicleModelStagecoach[13];
	extern scrollStruct<Hash> g_vehicleModelCart[6];
	extern scrollStruct<Hash> g_vehicleModelSpecial[1];
	extern scrollStruct<Hash> g_vehicleModelTurrets[4];

	extern scrollStruct<particle> g_particles[5];
}
