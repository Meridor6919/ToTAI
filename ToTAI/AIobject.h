#include <vector>
#include <string>

class AIobject {

public:
	AIobject();
	virtual void TryCar(const std::vector<int> &car_attributes, const std::vector<std::string> &tour);
	virtual void TryTires(const std::vector<std::string> &tire_attributes, const std::vector<std::string> &tour);
	virtual std::string GetBestCar();
	virtual std::string GetBestTires();
	virtual std::string GetName();
	virtual std::pair<int, int> GetAction(int id, const std::vector<std::string> &all_attributes, const std::vector<std::string> &tour);
	virtual int GetAttack(int id, const std::vector<std::string> &all_attributes, const std::vector<std::string> &tour);
};