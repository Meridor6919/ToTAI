#pragma once
#include <vector>
#include <string>
#include "Constants.h"

class AIInstance
{
	double GetParameterScore(double parameter_value, const double max, const double maximum_score);
	int GetMaxSpeed(const std::vector<std::string>&tour);
	int behaviour;
public:
	AIInstance();
	std::string GetName();
	int GetCarScore(const std::vector<std::string>&tour, const std::vector<int>&car_params);
	int GetTireScore(const std::vector<std::string>&tour, const std::vector<std::string>&tire_params);
	void SetCarAttributes(const std::vector<int>&car_params);
	void SetTireAttributes(const std::vector<std::string>&tire_params);

};

