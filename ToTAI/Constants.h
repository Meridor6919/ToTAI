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
namespace GameValues {
	enum Values {
		BehaviourDrifter,
		BehaviourAggressive,
		BehaviourBalanced,
		TerrainTypes = 6,
	};
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