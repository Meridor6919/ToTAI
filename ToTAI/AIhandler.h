#pragma once
#include <vector>
#include <string>
#include "Constants.h"
#include "AIobject.h"

class AIhandler
{
	double GetParameterScore(double parameter_value, const double max, const double maximum_score);
	int behaviour;
public:
	AIhandler();
	std::string GetName();
	int GetCarScore(const int optimum_max_speed, const std::vector<int>&car_params);
	int GetTireScore(const std::vector<int>&terrain, const std::vector<std::string>&tire_params);
	void SetCarAttributes(const std::vector<int>&car_params);
	void SetTireAttributes(const std::vector<std::string>&tire_params);

};

