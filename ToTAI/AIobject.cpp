#include "AIobject.h"

AIobject::AIobject()
{
}

void AIobject::TryCar(const std::vector<int>& car_attributes, const std::vector<std::string>& tour, std::string car_path)
{
}

void AIobject::TryTires(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour, std::string tire_path)
{
}

std::string AIobject::GetBestCar()
{
	return std::string("mikrus.car");
}

std::string AIobject::GetBestTires()
{
	return std::string("A1.tire");
}

std::string AIobject::GetName()
{
	return std::string("LUL");
}

std::pair<int, int> AIobject::GetAction(int id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	return std::pair<int, int>(4, 0);
}

int AIobject::GetAttack(int id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	return 10;
}
