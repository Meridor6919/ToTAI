#include "Game.h"

void Game::GetNames()
{
	for (int i = 0; i < number_of_instances; ++i)
	{
		PipeConnection::SetName(i, ai[i].GetName());
	}
}
void Game::GetCars()
{
	const std::vector<std::string> car_names = PipeConnection::GetCarNames();
	int *best_score = new int[number_of_instances];
	int *best_index = new int[number_of_instances];
	for (int i = 0; i < number_of_instances; ++i)
	{
		best_score[i] = 0;
		best_index[i] = 0;
	}
	const int max_speed = GetMaxSpeed(tour);
	for (int i = 0; i < static_cast<int>(car_names.size()); ++i)
	{
		const std::vector<int> car_params = PipeConnection::GetCarParams(car_names[i]);
		for (int j = 0; j < number_of_instances; ++j)
		{
			int score = ai[j].GetCarScore(max_speed, car_params);
			if (score > best_score[j])
			{
				best_score[j] = score;
				best_index[j] = i;
			}
		}
	}
	for (int i = 0; i < number_of_instances; ++i)
	{
		ai[i].SetCarAttributes(PipeConnection::GetCarParams(car_names[best_index[i]]));
		PipeConnection::SetCar(i, car_names[best_index[i]]);
	}
	delete best_score;
	delete best_index;
}
void Game::GetTires()
{
	const std::vector<std::string> tire_names = PipeConnection::GetTireNames();
	int *best_score = new int[number_of_instances];
	int *best_index = new int[number_of_instances];
	for (int i = 0; i < number_of_instances; ++i)
	{
		best_score[i] = 0;
		best_index[i] = 0;
	}

	std::vector<int> terrain = { 0,0,0,0,0,0 };
	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		terrain[tour[i][0] - 48] += 1 + 9 * (static_cast<int>(tour[i].size()) > 1);
	}

	for (int i = 0; i < static_cast<int>(tire_names.size()); ++i)
	{
		const std::vector<std::string> car_params = PipeConnection::GetTireParams(tire_names[i]);
		for (int j = 0; j < number_of_instances; ++j)
		{
			int score = ai[j].GetTireScore(terrain, car_params);
			if (score > best_score[j])
			{
				best_score[j] = score;
				best_index[j] = i;
			}
		}
	}
	for (int i = 0; i < number_of_instances; ++i)
	{
		ai[i].SetTireAttributes(PipeConnection::GetTireParams(tire_names[best_index[i]]));
		PipeConnection::SetTires(i, tire_names[best_index[i]]);
	}
	delete best_score;
	delete best_index;
}

int Game::GetMaxSpeed(const std::vector<std::string>& tour)
{
	double max_speed = 0.0;
	int count = 0;
	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		const int tour_size = static_cast<int>(tour[i].size());
		if (tour_size > 1)
		{
			double required = atof(tour[i].substr(1, tour_size - 1).c_str());
			if (required*(count / 2 + 1) > max_speed)
			{
				max_speed = required * (count / 2 + 1);
			}
			count = 0;
		}
		else
		{
			++count;
		}
	}
	return static_cast<int>(max_speed);
	
}

Game::Game()
{
	srand(static_cast<unsigned int>(time(0)));
	PipeConnection::Start();
	const std::pair<int, int> init = PipeConnection::GetInit();
	number_of_instances = init.first;
	number_of_all_participants = init.second;
	tour = PipeConnection::GetTour();
	for (int i = 0; i < number_of_instances; ++i)
	{
		ai.push_back(AIobject());
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
	PipeConnection::Exit();
}
