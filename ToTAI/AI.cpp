#include "AI.h"

void AI::CarSelection()
{
	std::vector<std::string> car_names = pipe_connection->GetCarNames();
	for (int i = 0; i < static_cast<int>(car_names.size()); ++i)
	{
		std::vector<int> car_atributes = pipe_connection->GetCarParams(car_names[i]);
		for (int j = 0; j < number_of_ais; ++j)
		{
			ai_object[j].TryCar(car_atributes, tour, car_names[i]);
		}
	}
	for (int i = 0; i < number_of_ais; ++i)
	{
		pipe_connection->SetCar(i, ai_object[i].GetBestCar());
	}
}

void AI::TiresSelection()
{
	std::vector<std::string> tire_names = pipe_connection->GetTireNames();
	for (int i = 0; i < static_cast<int>(tire_names.size()); ++i)
	{
		std::vector<std::string> tire_atributes = pipe_connection->GetTireParams(tire_names[i]);
		for (int j = 0; j < number_of_ais; ++j)
		{
			ai_object[j].TryTires(tire_atributes, tour, tire_names[i]);
		}
	}
	for (int i = 0; i < number_of_ais; ++i)
	{
		pipe_connection->SetTires(i, ai_object[i].GetBestTires());
	}
}
void AI::InitializationPhase()
{
	pipe_connection = std::make_unique<PipeConnection>();
	pipe_connection->Start();

	const std::pair<int, int> initialization_data = pipe_connection->GetInit();
	number_of_ais = initialization_data.first;
	number_of_participants = initialization_data.second;

	tour = pipe_connection->GetTour();

	for (int i = 0; i < number_of_ais; ++i)
	{
		ai_object.emplace_back(ActiveAI());
		pipe_connection->SetName(i, ai_object[i].GetName());
	}
	CarSelection();
	TiresSelection();
	while (true)
	{
		std::vector<std::string> current_attributes = pipe_connection->GetAllAttributes(number_of_participants);
		if (atof(current_attributes[(number_of_participants - number_of_ais) * 3 + 1].c_str()) > 0.0)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void AI::GamePhase()
{
	bool first_turn = true;
	while (static_cast<int>(tour.size()) > 0)
	{
		std::vector<std::string> current_attributes = pipe_connection->GetAllAttributes(number_of_participants);
		std::vector<double> durability = {};
		int alive_participant = -1;
		for (int i = 0; i < number_of_ais; ++i)
		{
			durability.emplace_back(atof(current_attributes[(number_of_participants - number_of_ais + i) * 3 + 1].c_str()));
			if (durability[i] > 0.0)
			{
				alive_participant = i;
			}
		}
		if (alive_participant < 0)
		{
			break;
		}
		while (!pipe_connection->NewTurn(alive_participant))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		for (int i = 0; i < number_of_ais; ++i)
		{
			if (durability[i] > 0.0)
			{
				if (!first_turn)
				{
					int target = ai_object[i].GetAttack(i, current_attributes, tour);
					pipe_connection->SetAttack(i, target);
				}
				std::pair<int, int> action = ai_object[i].GetAction(i, current_attributes, tour);
				pipe_connection->SetAction(i, action.first, action.second);
			}
		}
		tour.erase(tour.begin());
	}
	pipe_connection->Exit();
}
