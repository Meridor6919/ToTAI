#pragma once
#include "ActiveAI.h"
#include "PipeConnection.h"
#include <vector>
#include <time.h>
#include <thread>

class AI
{
	std::unique_ptr<PipeConnection> pipe_connection;
	std::vector<std::string> tour;
	std::vector<ActiveAI> ai_object;
	std::vector<bool> alive;
	int number_of_participants;
	int number_of_ais;

	void CarSelection();
	void TiresSelection();

public:
	void InitializationPhase();
	void GamePhase();
};

