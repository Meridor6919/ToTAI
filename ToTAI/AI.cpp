#include "AI.h"

void AI::CarSelection()
{
	std::vector<std::string> car_names = pipe_connection->GetCarNames();
	for (int i = 0; i < static_cast<int>(car_names.size()); ++i)
	{
		std::vector<int> car_atributes = pipe_connection->GetCarParams(car_names[i]);
		for (int j = 0; j < number_of_ais; ++j)
		{
			ai_object[j].TryCar(car_atributes, tour);
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
			ai_object[j].TryTires(tire_atributes, tour);
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
		ai_object.emplace_back(AIobject());
		pipe_connection->SetName(i, ai_object[i].GetName());
	}
	CarSelection();
	TiresSelection();
}

void AI::GamePhase()
{
	bool first_turn = true;
	while (static_cast<int>(tour.size()) > 0)
	{
		std::vector<std::string> current_attributes = pipe_connection->GetAllAttributes(number_of_participants);
		while(!pipe_connection->NewTurn(0))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		for (int i = 0; i < number_of_ais; ++i)
		{
			if (!first_turn)
			{
				int target = ai_object[i].GetAttack(i, current_attributes, tour);
				pipe_connection->SetAttack(i, target);
			}
			std::pair<int, int> action = ai_object[i].GetAction(i, current_attributes, tour);
			pipe_connection->SetAction(i, action.first, action.second);
		}
		tour.erase(tour.begin());
	}
	pipe_connection->Exit();
}
