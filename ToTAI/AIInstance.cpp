#include "AIInstance.h"

void AIInstance::GetName()
{
}

int AIInstance::GetCarScore(const std::vector<std::string>& tour, const std::vector<int>& car_params)
{
	return 0;
}

int AIInstance::GetTireScore(const std::vector<std::string>& tour, const std::vector<std::string>& tire_params)
{
	auto Probability = [](double number_of_tests, double wanted_number) {

		double result = 1;
		int extra = number_of_tests - wanted_number;
		extra < 1 ? extra = 1 : 0;
		wanted_number < 1 ? wanted_number = 1 : 0;
		while (number_of_tests > 1)
		{
			result *= number_of_tests / wanted_number / extra / 2;
			--number_of_tests;
			--wanted_number < 1 ? wanted_number = 1: 0;
			--extra < 1 ? extra = 1: 0;
		}
		return result;
	};

	int terrain[GameValues::TerrainTypes] = { 0,0,0,0,0,0 };
	double points = 0;
	double x;
	double y;
	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		terrain[tour[i][0] - 48] += 1 + 9 * (static_cast<int>(tour[i].size()) > 1);
	}
	for (int i = 0; i < static_cast<int>(GameValues::TerrainTypes); ++i)
	{
		for (int j = 0; j < static_cast<int>(tire_params[i].size()); ++j)
		{
			if (tire_params[i][j] == 'x')
			{
				x = atof(tire_params[i].substr(0, j).c_str());
				y = atof(tire_params[i].substr(j + 1, static_cast<int>(tire_params[i].size()) - j - 1).c_str());
			}
		}
		for (int j = 0; j < static_cast<int>(y - x + 1); ++j)
		{
			points += Probability(y, j) * terrain[i];
		}
	}
	return static_cast<int>(points*100.0f);
}

void AIInstance::SetCarAttributes(const std::vector<int>& car_params)
{
}

void AIInstance::SetTireAttributes(const std::vector<std::string>& tire_params)
{
}
