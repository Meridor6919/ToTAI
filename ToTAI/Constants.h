#pragma once
#include <string>
namespace ConnectionCodes{
	enum Codes{
		Start = 10,
		GetInit,
		NewTurn,
		GetCarNames,
		GetTireNames,
		GetCarParams,
		GetTireParams,
		GetAllAttributes,
		GetTour,
		SetName,
		SetCar,
		SetTires,
		SetAction,
		SetAttack,
		last
	};
}
namespace Report {
	enum StreamType {
		Output,
		Input
	};
}
namespace GameValues {
	enum Behaviour {
		Drifter,
		Aggressive,
		Balanced,
		last
	};
	const int TerrainTypes = 6;
	const int attack_forward_distance = 4;
	const int attack_backward_distance = 6;
}
namespace CarAttributes
{
	enum
	{
		max_speed,
		max_accelerating,
		max_braking,
		hand_brake_value,
		durability,
		visibility,
		turn_mod,
		drift_mod,
		last
	};
}