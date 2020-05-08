#include <vector>
#include <string>

class AIobject {

	std::string best_car;
	std::string best_tires;
	std::vector<int> car_attributes;
	std::vector<std::string> tire_attributes;

public:
	AIobject();
	virtual void TryCar(const std::vector<int> &car_attributes, const std::vector<std::string> &tour, std::string car_path);
	virtual void TryTires(const std::vector<std::string> &tire_attributes, const std::vector<std::string> &tour, std::string tire_path);
	virtual std::string GetBestCar();
	virtual std::string GetBestTires();
	virtual std::string GetName();
	virtual std::pair<int, int> GetAction(int id, const std::vector<std::string> &all_attributes, const std::vector<std::string> &tour);
	virtual int GetAttack(int id, const std::vector<std::string> &all_attributes, const std::vector<std::string> &tour);
};