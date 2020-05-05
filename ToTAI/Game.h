#pragma once
#include "AIobject.h"
#include "PipeConnection.h"
#include <vector>
#include <time.h>
#include <thread>

class Game
{
	int number_of_instances;
	int number_of_all_participants;
	std::vector<AIobject> ai;
	std::unique_ptr<PipeConnection> pipe_connection;
	std::vector<std::string> tour;

	void GetNames();
	void GetCars();
	void GetTires();

	int GetMaxSpeed(const std::vector<std::string>&tour);

public:
	Game();
	virtual void LobbyPhase();
	virtual void RacePhase();
	~Game();
};

