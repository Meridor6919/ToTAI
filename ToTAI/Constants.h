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
	constexpr int TerrainTypes = 6;
	constexpr int attack_forward_distance = 4;
	constexpr int attack_backward_distance = 6;
	constexpr double drift_value = 30.0;
}
namespace Actions {
	enum ActionType {
		acceleration,
		braking,
		hand_braking,
		pass,
		resign,
		last
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