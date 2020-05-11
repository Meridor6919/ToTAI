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

	std::array<int, GameValues::TerrainTypes> CountTerrainTypes(const std::vector<std::string>& tour);
	double AvarageProbabilityOfTires(double number_of_tests, double wanted_number);
	double TireEffectivness(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour);
	double CarParameterScore(double value, double increasing_bound, bool decrease_after);

public:
	ActiveAI();
	std::string GetName() override;
	void TryCar(const std::vector<int>& car_attributes, const std::vector<std::string>& tour, std::string car_path) override;
	void TryTires(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour, std::string tire_path) override;

};

