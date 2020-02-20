#pragma once
#include "AIInstance.h"
#include "ToTAIFunctions.h"
#include <vector>
#include <time.h>

class Game
{
	int number_of_instances;
	int number_of_all_participants;
	std::vector<AIInstance> ai;
	std::vector<std::string> tour;

	void GetNames();
	void GetCars();
	void GetTires();


public:
	Game();
	virtual void LobbyPhase();
	virtual void RacePhase();
	~Game();
};

