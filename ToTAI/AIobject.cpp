#include "AIobject.h"

AIobject::AIobject()
{
}

void AIobject::TryCar(const std::vector<int>& car_attributes, const std::vector<std::string>& tour, std::string car_path)
{
	best_car = car_path;
	this->car_attributes = car_attributes;
}

void AIobject::TryTires(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour, std::string tire_path)
{
	best_tires = tire_path;
	this->tire_attributes = tire_attributes;
}

std::string AIobject::GetBestCar()
{
	return best_car;
}

std::string AIobject::GetBestTires()
{
	return best_tires;
}

std::string AIobject::GetName()
{
	return std::string("TestObjectAI");
}

std::pair<int, int> AIobject::GetAction(int id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	return std::pair<int, int>(4, 0);
}

int AIobject::GetAttack(int global_id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	return 10;
}
