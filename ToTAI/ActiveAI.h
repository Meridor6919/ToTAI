#pragma once
#include "AIobject.h"
#include "Constants.h"
#include <array>


class ActiveAI : public AIobject
{
protected:
	GameValues::Behaviour behaviour;
	double best_tires_score = 0.0;
	double best_car_score = 0.0;
	double risk = 0.0;

	//tires selection methods
	std::array<int, GameValues::TerrainTypes> CountTerrainTypes(const std::vector<std::string>& tour);
	double AvarageProbabilityOfTires(double number_of_tests, double wanted_number);
	double TireEffectivness(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour);
	//car selection methods
	double CarParameterScore(double value, double increasing_bound, bool decrease_after);
	int OptimumMaximumSpeed(const std::vector<std::string>& tour);
	//attack methods
	int SelectTarget(int id, std::vector<double> local_score, double lower_bound, double upper_bound);
	//action method
	double EvaluateChance(std::string field, const double speed, const bool drift);
	double EvaluateChanceInversed(std::string field, const double chance_to_fail, const bool drift);
	double CalculateBurning(double value);
	double CalculateBurningInversed(double burning);
	double CalculateSpeed(const std::string &current_field, const double speed, const int acceleration_value);

public:
	ActiveAI();
	std::string GetName() override;
	void TryCar(const std::vector<int>& car_attributes, const std::vector<std::string>& tour, std::string car_path) override;
	void TryTires(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour, std::string tire_path) override;
	std::pair<int, int> GetAction(int global_id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour) override;
	int GetAttack(int global_id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour) override;

};

