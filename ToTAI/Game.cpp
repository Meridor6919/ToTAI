#include "Game.h"

void Game::GetNames()
{
	for (int i = 0; i < number_of_instances; ++i)
	{
		pipe_connection->SetName(i, ai[i].GenerateName());
	}
}
void Game::GetCars()
{
	const std::vector<std::string> car_names = pipe_connection->GetCarNames();
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
		const std::vector<int> car_params = pipe_connection->GetCarParams(car_names[i]);
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
		ai[i].SetCarAttributes(pipe_connection->GetCarParams(car_names[best_index[i]]));
		pipe_connection->SetCar(i, car_names[best_index[i]]);
	}
	delete best_score;
	delete best_index;
}
void Game::GetTires()
{
	const std::vector<std::string> tire_names = pipe_connection->GetTireNames();
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
		const std::vector<std::string> car_params = pipe_connection->GetTireParams(tire_names[i]);
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
		ai[i].SetTireAttributes(pipe_connection->GetTireParams(tire_names[best_index[i]]));
		pipe_connection->SetTires(i, tire_names[best_index[i]]);
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
	pipe_connection = std::make_unique<PipeConnection>();
	pipe_connection->Start();
	const std::pair<int, int> init = pipe_connection->GetInit();
	number_of_instances = init.first;
	number_of_all_participants = init.second;
	tour = pipe_connection->GetTour();
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
	while (static_cast<int>(tour.size()))
	{
		const int ai_index = number_of_all_participants - number_of_instances;
		for (int i = 0; i < number_of_instances; ++i)
		{
			while (!pipe_connection->NewTurn(i))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		}
		const std::vector<std::string> raw_data = pipe_connection->GetAllAtributes(number_of_all_participants);
		for (int i = 0; i < number_of_instances; ++i)
		{
			const double durablity = atof(raw_data[(i + ai_index)*3 + 1].c_str());
			if (durablity > 0)
			{
				ai[i].SetRaceAttributes(i + ai_index, raw_data);
				const std::string action = ai[i].TakeAction(tour);
				pipe_connection->SetAction(i, action[0] - 48, atoi(action.substr(1, static_cast<int>(action.size()) - 1).c_str()));
				if(i)
				{
					pipe_connection->SetAttack(i, ai[i].Attack(tour, i + ai_index, raw_data));
				}
			}
		}
		tour.erase(tour.begin());
	}
}
Game::~Game()
{
	pipe_connection->Exit();
}
