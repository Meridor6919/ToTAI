#pragma once
#include <vector>
#include <string>
#include "Constants.h"

class AIobject
{
	std::vector<int>car_params;
	std::vector<std::string>tire_params;
	int behaviour;

	double GetParameterScore(double parameter_value, const double max, const double maximum_score);
	double GetProbability(double number_of_tests, double wanted_number);
	float EvaluateChance(std::string field, const float speed, const bool drift);
	float CalculateBurning(float value);
	//from 0 to 100
	float EvaluateSpeed(std::string field, const float chance_to_fail, const bool drift);
	float TireEffectivness(const std::string &field);
	float GetSpeed(const std::string &current_field, const float current_speed, const int acceleration_value);

	std::string TakeActionDrifterAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score);
	std::string TakeActionAggressiveAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score);
	std::string TakeActionBalancedAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score);
	double NormalizeScore(double score, double max_local_score, double max_global_score = 10.0);


public:
	AIobject();
	std::string GetName();
	int GetCarScore(const int optimum_max_speed, const std::vector<int>&car_params);
	int GetTireScore(const std::vector<int>&terrain, const std::vector<std::string>&tire_params);
	void SetCarAttributes(const std::vector<int>&car_params);
	void SetTireAttributes(const std::vector<std::string>&tire_params);
	std::string TakeAction(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score);
};

