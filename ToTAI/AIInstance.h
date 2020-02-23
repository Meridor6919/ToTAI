#pragma once
#include <vector>
#include <string>
#include "Constants.h"

class AIInstance
{
	double GetProbability(double number_of_tests, double wanted_number);
	double GetParameterScore(double parameter_value, const double max, const double maximum_score);
	int behaviour;
public:
	AIInstance();
	std::string GetName();
	int GetCarScore(const int optimum_max_speed, const std::vector<int>&car_params);
	int GetTireScore(const std::vector<int>&terrain, const std::vector<std::string>&tire_params);
	void SetCarAttributes(const std::vector<int>&car_params);
	void SetTireAttributes(const std::vector<std::string>&tire_params);

};

