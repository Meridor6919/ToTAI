#pragma once
#include <vector>
#include <string>
#include "Constants.h"

class AIobject
{
	double score;
	double durability;
	double current_speed;
	std::vector<int>car_params;
	std::vector<std::string>tire_params;
	int behaviour;

	double GetParameterScore(double parameter_value, const double max, const double maximum_score);
	double GetProbability(double number_of_tests, double wanted_number);
	double EvaluateChance(std::string field, const double speed, const bool drift);
	double CalculateBurning(double value);
	//from 0 to 100
	double EvaluateSpeed(std::string field, const double chance_to_fail, const bool drift);
	double TireEffectivness(const std::string &field);
	double CalculateSpeed(const std::string &current_field, const double speed, const int acceleration_value);

	int AttackAggressiveAI(const std::vector<std::string> & tour, int id, const std::vector<std::string> &data);
	int AttackDrifterAI(const std::vector<std::string> & tour, int id, const std::vector<std::string> &data);
	int AttackBalancedAI(const std::vector<std::string> & tour, int id, const std::vector<std::string> &data);
	std::string TakeActionDrifterAI(const std::vector<std::string> & tour);
	std::string TakeActionAggressiveAI(const std::vector<std::string> & tour);
	std::string TakeActionBalancedAI(const std::vector<std::string> & tour);
	double NormalizeScore(double value, double max_local_score, double max_global_score = 10.0);
	double SafeSpeed(const std::vector<std::string> &tour, char turn_behaviour);
public:
	AIobject();

	std::string GenerateName();
	int GetCarScore(const int optimum_max_speed, const std::vector<int>&car_params);
	int GetTireScore(const std::vector<int>&terrain, const std::vector<std::string>&tire_params);
	double GetScore();

	void SetCarAttributes(const std::vector<int>&car_params);
	void SetTireAttributes(const std::vector<std::string>&tire_params);
	void SetRaceAttributes(const int id, const std::vector<std::string> &data);

	std::string TakeAction(const std::vector<std::string> & tour);
	int Attack(const std::vector<std::string> & tour, int id, const std::vector<std::string> &data);
};

