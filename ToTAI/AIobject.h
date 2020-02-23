#pragma once
#include <vector>
#include <string>
#include "Constants.h"

class AIobject
{
	std::vector<int>car_modifiers;
	std::vector<std::string>tire_modifiers;
	int behaviour;

	double GetParameterScore(double parameter_value, const double max, const double maximum_score);
	double GetProbability(double number_of_tests, double wanted_number);
	float EvaluateChance(std::string field, const float speed, const bool drift);
	float CalculateBurning(float value);
	float EvaluateSpeed(std::string field, const float chance, const bool drift);
	float TireEffectivness(const std::string &field);
	float EvaluateSpeed(const std::string &current_field, const float current_speed, const int acceleration_value);
public:
	AIobject();
	std::string GetName();
	int GetCarScore(const int optimum_max_speed, const std::vector<int>&car_params);
	int GetTireScore(const std::vector<int>&terrain, const std::vector<std::string>&tire_params);
	void SetCarAttributes(const std::vector<int>&car_params);
	void SetTireAttributes(const std::vector<std::string>&tire_params);
};

