#include "PipeConnection.h"

void PipeConnection::Start()
{
	std::string temp;
	std::cout << ConnectionCodes::Start << std::endl;
	std::cin >> temp;
}
void PipeConnection::Exit()
{
	std::string temp;
	std::cin >> temp;
}
std::pair<int, int> PipeConnection::GetInit()
{
	int x;
	std::string temp;
	std::cout << ConnectionCodes::GetInit << std::endl;
	std::cin >> temp;
	x = temp[0] - 48;
	std::cin >> temp;
	return { x, temp[0] - 48 };
}
bool PipeConnection::NewTurn(int id)
{
	std::string temp;
	std::cout << ConnectionCodes::NewTurn << id << std::endl;
	std::cin >> temp;
	if (temp == "0")
	{
		return true;
	}
	else
	{
		return false;
	}
}
std::vector<std::string> PipeConnection::GetCarNames()
{
	std::vector<std::string> ret;
	std::string temp;
	std::cout << ConnectionCodes::GetCarNames << std::endl;
	std::cin >> temp;
	const int number_of_cars = atoi(temp.c_str());
	for (int i = 0; i < number_of_cars; ++i)
	{
		std::cin >> temp;
		ret.emplace_back(temp);
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetTireNames()
{
	std::vector<std::string> ret;
	std::string temp;
	std::cout << ConnectionCodes::GetTireNames << std::endl;
	std::cin >> temp;
	const int number_of_tires = atoi(temp.c_str());
	for (int i = 0; i < number_of_tires; ++i)
	{
		std::cin >> temp;
		ret.emplace_back(temp);
	}
	return ret;
}
std::vector<int> PipeConnection::GetCarParams(std::string car_path)
{
	std::vector<int> ret;
	std::string temp;
	std::cout << ConnectionCodes::GetCarParams << car_path << std::endl;
	for (int i = 0; i < CarAttributes::last; ++i)
	{
		std::cin >> temp;
		ret.emplace_back(atoi(temp.c_str()));
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetTireParams(std::string tire_path)
{
	std::vector<std::string> ret;
	std::string temp;
	std::cout << ConnectionCodes::GetTireParams << tire_path << std::endl;
	for (int i = 0; i < GameValues::TerrainTypes; ++i)
	{
		std::cin >> temp;
		ret.emplace_back(temp);
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetAllAtributes(int number_of_participants)
{
	std::vector<std::string> ret;
	std::string temp;
	std::cout << ConnectionCodes::GetAllAttributes << std::endl;
	for (int i = 0; i < number_of_participants*3; ++i)
	{
		std::cin >> temp;
		ret.emplace_back(temp);
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetTour()
{
	std::vector<std::string> ret;
	std::string temp;
	std::cout << ConnectionCodes::GetTour << std::endl;
	int size;
	std::cin >> size;
	for (int i = 0; i < size; ++i)
	{
		std::cin >> temp;
		ret.emplace_back(temp);
	}
	return ret;
}
void PipeConnection::SetAction(int id, int action_type, int value)
{
	std::cout << ConnectionCodes::SetAction << id << action_type << value << std::endl;
}
void PipeConnection::SetAttack(int id, int target)
{
	std::cout << ConnectionCodes::SetAttack << id << target << std::endl;
}
void PipeConnection::SetName(int id, std::string name)
{
	std::cout << ConnectionCodes::SetName << id << name << std::endl;
}
void PipeConnection::SetTires(int id, std::string tire_path)
{
	std::cout << ConnectionCodes::SetTires << id << tire_path << std::endl;
}
void PipeConnection::SetCar(int id, std::string car_path)
{
	std::cout << ConnectionCodes::SetCar << id << car_path << std::endl;
}