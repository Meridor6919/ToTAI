#pragma once
#include <string>
namespace ConnectionCodes{
	enum Codes{
		Start = 10,
		NewTurn,
		GetInit,
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

