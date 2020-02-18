#pragma once
#include "Constants.h"
#include <iostream>
#include <vector>
#include <string>

namespace ToTAIFunctions
{
	//Waits for parent process to initialize
	void Start();
	//Waits for exit signal
	void Exit();
	//Returns number of ais and number of participants wrapped into pair
	std::pair<int, int> GetInit();
	//Returns wheter participant can perform an action in this turn
	bool NewTurn(int id);
	//Returns possible cars
	std::vector<std::string> GetCarNames();
	//Returns possible tires
	std::vector<std::string> GetTireNames();
	//Returns car params
	std::vector<int> GetCarParams(std::string car_path);
	//Returns tire params
	std::vector<std::string> GetTireParams(std::string tire_path);
	//Returns atributes of all participants starting with current speed of all participants, then current durablity and then score.
	std::vector<std::string> GetAllAtributes(int number_of_participants);
	//Returns all segments of tour
	std::vector<std::string> GetTour();

	void SetAction(int id, int action_type, int value);
	void SetAttack(int id, int target);
	void SetName(int id, std::string name);
	void SetTires(int id, std::string tire_path);
	void SetCar(int id, std::string car_path);

}