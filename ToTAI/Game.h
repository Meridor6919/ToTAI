#pragma once
#include "AIhandler.h"
#include "PipeConnection.h"
#include <vector>
#include <time.h>

class Game
{
	int number_of_instances;
	int number_of_all_participants;
	std::vector<AIhandler> ai;
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

