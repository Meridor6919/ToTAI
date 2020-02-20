#include "Game.h"

void Game::GetNames()
{
	for (int i = 0; i < number_of_instances; ++i)
	{
		ToTAIFunctions::SetName(i, ai[i].GetName());
	}
}
void Game::GetCars()
{
	const std::vector<std::string> car_names = ToTAIFunctions::GetCarNames();
	int *best_score = new int[number_of_instances];
	int *best_index = new int[number_of_instances];
	memset(best_score, 0, number_of_instances);
	memset(best_index, 0, number_of_instances);

	for (int i = 0; i < static_cast<int>(car_names.size()); ++i)
	{
		const std::vector<int> car_params = ToTAIFunctions::GetCarParams(car_names[i]);
		for (int j = 0; j < number_of_instances; ++j)
		{
			int score = ai[j].GetCarScore(tour, car_params);
			if (score > best_score[j])
			{
				best_score[j] = score;
				best_index[j] = i;
			}
		}
	}
	for (int i = 0; i < number_of_instances; ++i)
	{
		ai[i].SetCarAttributes(ToTAIFunctions::GetCarParams(car_names[best_index[i]]));
		ToTAIFunctions::SetCar(i, car_names[best_index[i]]);
	}
	delete best_score;
	delete best_index;
}
void Game::GetTires()
{
	const std::vector<std::string> tire_names = ToTAIFunctions::GetTireNames();
	int *best_score = new int[number_of_instances];
	int *best_index = new int[number_of_instances];
	memset(best_score, 0, number_of_instances);
	memset(best_index, 0, number_of_instances);

	for (int i = 0; i < static_cast<int>(tire_names.size()); ++i)
	{
		const std::vector<std::string> car_params = ToTAIFunctions::GetTireParams(tire_names[i]);
		for (int j = 0; j < number_of_instances; ++j)
		{
			int score = ai[j].GetTireScore(tour, car_params);
			if (score > best_score[j])
			{
				best_score[j] = score;
				best_index[j] = i;
			}
		}
	}
	for (int i = 0; i < number_of_instances; ++i)
	{
		ai[i].SetTireAttributes(ToTAIFunctions::GetTireParams(tire_names[best_index[i]]));
		ToTAIFunctions::SetTires(i, tire_names[best_index[i]]);
	}
	delete best_score;
	delete best_index;
}

Game::Game()
{
	srand(time(0));
	ToTAIFunctions::Start();
	const std::pair<int, int> init = ToTAIFunctions::GetInit();
	number_of_instances = init.first;
	number_of_all_participants = init.second;
	tour = ToTAIFunctions::GetTour();
	for (int i = 0; i < number_of_instances; ++i)
	{
		ai.push_back(AIInstance());
	}
}

void Game::LobbyPhase()
{
	GetNames();
	GetCars();
	GetTires();
}

void Game::RacePhase()
{

}

Game::~Game()
{
	ToTAIFunctions::Exit();
}
