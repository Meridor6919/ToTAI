#pragma once
#include <vector>
#include "Constants.h"
struct AIobject
{
	std::vector<int>car_modifiers;
	std::vector<std::string>tire_modifiers;

	AIobject(const std::vector<int> &car_modifiers, const std::vector<std::string> &tire_modifiers) : car_modifiers(car_modifiers), tire_modifiers(tire_modifiers) {};
	static double GetProbability(double number_of_tests, double wanted_number);
	float EvaluateChance(std::string field, const float speed, const bool drift);
	float CalculateBurning(float value);
	float EvaluateSpeed(std::string field, const float chance, const bool drift);
	float TireEffectivness(const std::string &field);
	float EvaluateSpeed(const std::string &current_field, const float current_speed, const int acceleration_value);
};