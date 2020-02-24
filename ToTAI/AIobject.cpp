#include "AIobject.h"

double AIobject::GetParameterScore(double parameter_value, const double max, const double maximum_score)
{
	if (parameter_value > max)
	{
		parameter_value = max;
	}
	if (parameter_value < 0)
	{
		parameter_value = 0;
	}
	parameter_value = max - parameter_value;
	return sqrt(max*max - parameter_value * parameter_value) * maximum_score/max;
}
double AIobject::GetProbability(double number_of_tests, double wanted_number)
{
	double result = 1;
	double extra = number_of_tests - wanted_number;
	extra < 1 ? extra = 1 : 0;
	wanted_number < 1 ? wanted_number = 1 : 0;
	while (number_of_tests > 1)
	{
		result *= number_of_tests / wanted_number / extra / 2;
		--number_of_tests;
		--wanted_number < 1 ? wanted_number = 1 : 0;
		--extra < 1 ? extra = 1 : 0;
	}
	return result / 2;
}
float AIobject::EvaluateChance(std::string field, const float speed, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}
	field.erase(0, 1);
	float base = (speed / static_cast<float>(atof(field.c_str())) - 1) * 100.0f + speed - static_cast<float>(atof(field.c_str()));

	if (base < 0.0f)
	{
		base = 0.0f;
	}
	if (drift)
	{
		base /= static_cast<float>(car_params[CarAttributes::drift_mod]) / 100.0f;
		if (base > 100.0f)
		{
			base = 100.0f;
		}
		float result = (speed + base) / 2.0f;
		if (base < speed)
		{
			result = (speed + base * 2.0f) / 3.0f;
		}
		if (result > 100.0f)
		{
			result = 100.0f;
		}
		return result;
	}
	else
	{
		base /= static_cast<float>(car_params[CarAttributes::turn_mod]) / 100.0f;
		if (base > 100.0f)
		{
			base = 100.0f;
		}
		return (sqrt(base * -base + 200.0f * base) + 2.0f * base) / 3.0f;
	}
}
float AIobject::CalculateBurning(float value)
{
	float raw = value / static_cast<float>(car_params[CarAttributes::max_speed]);
	float result = 0.0f;

	if (raw < 0.0f)
	{
		return 0.0f;
	}
	if (raw > 0.25f)
	{
		raw = 0.25f;
		value = static_cast<float>(car_params[CarAttributes::max_speed]) * 0.25f;
	}
	int level = static_cast<int>(raw*20.0f) + 10;
	result = value * static_cast<float>(level + level * level) / 2.0f;
	return result / 50.0f;
}
float AIobject::EvaluateSpeed(std::string field, const float chance, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}
	field.erase(0, 1);

	if (drift)
	{
		float result = (2.0f * chance*static_cast<float>(car_params[CarAttributes::drift_mod]) + 10000.0f + 100.0f * static_cast<float>(atof(field.c_str()))) /
			(10000.0f / static_cast<float>(atof(field.c_str())) + 100.0f + static_cast<float>(car_params[CarAttributes::drift_mod]));
		float secondary_result = (3.0f * chance*static_cast<float>(car_params[CarAttributes::drift_mod]) + 20000.0f + 200.0f * static_cast<float>(atof(field.c_str()))) /
			(20000.0f / static_cast<float>(atof(field.c_str())) + 200.0f + static_cast<float>(car_params[CarAttributes::drift_mod]));

		if (secondary_result > result)
		{
			result = secondary_result;
		}
		float base = ((result / static_cast<float>(atof(field.c_str())) - 1.0f) * 100.0f + result - static_cast<float>(atof(field.c_str()))) /
			(static_cast<float>(car_params[CarAttributes::drift_mod]) / 100.0f);

		if (base > 100.0f)
		{
			result = 2.0f * chance - 100.0f;
		}
		else if (base < 0.0f)
		{
			result = 3.0f * chance;
		}
		return result;
	}
	else
	{
		float delta = (200.0f + 12.0f * chance)*(200.0f + 12.0f * chance) - 180.0f * chance*chance;
		float base = (-(200.0f + 12.0f * chance) + sqrt(delta)) / -10.0f*((static_cast<float>(car_params[CarAttributes::turn_mod])) / 100.0f);
		return static_cast<float>(atof(field.c_str())) + base / (100.0f / static_cast<float>(atof(field.c_str())) + 1.0f);
	}
}
float AIobject::TireEffectivness(const std::string &field)
{
	const int terrain = field[0] - 48;
	double x, y;
	double result = 0.0;
	for (int i = 0; i < static_cast<int>(tire_params[terrain].size()); ++i)
	{
		if (tire_params[terrain][i] == 'x')
		{
			x = atof(tire_params[terrain].substr(0, i).c_str());
			y = atof(tire_params[terrain].substr(i + 1, static_cast<int>(tire_params[terrain].size()) - i - 1).c_str());
			break;
		}
	}
	for (int j = 0; j < static_cast<int>(y - x + 1); ++j)
	{
		result += GetProbability(y, j);
	}
	return static_cast<float>(result);
}
float AIobject::EvaluateSpeed(const std::string &current_field, const float current_speed, const int acceleration_value)
{
	return (current_speed + static_cast<float>(acceleration_value) * (0.9f + 0.2f*TireEffectivness(current_field))) * 0.9f;
}
std::string AIobject::TakeActionDrifterAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score)
{
	return std::string("010");
}
std::string AIobject::TakeActionAggressiveAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score)
{
	return std::string("040");
}
std::string AIobject::TakeActionBalancedAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score)
{
	return std::string("015");
}
AIobject::AIobject()
{
	behaviour = rand() % 3;
}
std::string AIobject::GetName()
{
	static std::vector<std::vector<const char*>> names = { { "Mark Driver", "Isao Fujimoto", "Miguela Aguela", "Hans Ufner", "Igor Belov", "Andrew Anderson", "Jane Turning", "Sam Samson", "Ed Thompson", "Barbara Hudson",
													"Frank Sharpe", "Alan Robinson", "Paul Reynolds", "Armand Buchard", "Leon Guerin", "Franz Geisler", "Rudi Schultz", "Schultz Vogel", "Tatsuo Okabe", "Yasuaki Tanikawa",
													"Masaharu Shoji", "Jungo Ishii", "Yoko Akira", "Kenji Morita", "Yuzo Yamanaka", "Maciek Kowalczyk", "Nikolai Yakubik", "Gennadi Likhachev", "Akinori Koyama", "Yataka Tanida" },
													{ "Ivan Atakovic", "Leszek Murzyn", "Sui Cide", "Quality Racer", "Helga Dick", "Krug Dudeski", "Bloodwyn Goethe", "Drew McNeil", "Gaston Reveneu", "Vladimir Zhdanovich",
													"Yuri Voronin", "Victor Shalimov", "Tatyana Petrov", "Leonid Ragulin", "Dmitriy Korkia", "Boris Chukarin", "Olga Gorokhova", "Anatoly Samusenko", "Sergei Kolotov", "Nikita Maleev",
													"Calvin Dodge", "Oscar Homburger", "Carl Crossett", "Patricia Bradley", "Brian Kazimierczyk", "Jeff Miller", "Louis Luget", "Jacques Bouissou", "Pierre Dreyfus", "Naohiro Matsumara" },
													{ "Paul Harackovy", "Max McDonald", "Gordon Goodman", "Blu Apostrof", "John Hill", "Roman Torbon", "Lester King", "Austin Bryant", "Siegfried Krause", "Neel Hartley",
													"Brendon Jani", "Timour Bernhard", "Steven Ricer", "Conrad Payne", "Chester Young", "John Parker", "Angelo Goodwin", "Jerrod Presley", "Zachery Polanco", "Quintin Herzog",
													"Bobbie Schaeffer", "Omer Seaton", "Mohammad Lundberg", "Juan Campbell", "Harold Cox", "Matthew Henderson", "John Schneider", "Angel Fitzgerald", "Marlon Torres", "Chad Sutton" } };

	const char* ret;
	const int size_of_used_vector = static_cast<int>(names[behaviour].size());
	int id = rand() % size_of_used_vector;
	ret = names[behaviour][id];

	names[behaviour][id] = names[behaviour][size_of_used_vector - 1];
	names[behaviour].erase(names[behaviour].begin() + (size_of_used_vector - 1));

	int x = 9;
	return ret;
}
int AIobject::GetCarScore(const int optimum_max_speed, const std::vector<int>& car_params)
{
	double final_score = 1;
	switch (behaviour)
	{
	case GameValues::BehaviourDrifter:
		{
			//max value - 5 000 00
			final_score *= GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 10.0);//acceleration
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 5.0);//max_speed
			final_score *= GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed]/10*9, 2.0);//max_braking
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed] - car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 10.0) + 0.01;//hand-brake-value
			final_score *= GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed]*10, 10.0);//durability
			final_score *= GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 10.0);//drift_mod
			final_score *= GetParameterScore(car_params[CarAttributes::visibility], 10, 5.0);//visibility
			break;
		}
		case GameValues::BehaviourAggressive:
		{
			//max value - 3 037 5
			final_score *= GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 150.0);//acceleration
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 15.0);//max_speed
			final_score *= GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed] / 10 * 9, 1.0);//max_braking
			final_score *= GetParameterScore(car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 1.0) + 0.01;//hand-brake-value
			final_score *= GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed] * 10, 15.0);//durability
			final_score *= GetParameterScore(car_params[CarAttributes::turn_mod], 1000, 3.0);//turn_mod
			final_score *= GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 3.0);//drift_mod
			final_score *= GetParameterScore(car_params[CarAttributes::visibility], 10, 1.0);//visibility
			break;
		}
		case GameValues::BehaviourBalanced:
		{
			//max value - 281 250 0
			final_score *= GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 15.0);//acceleration
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 10.0);//max_speed
			final_score *= GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed] / 10 * 9, 5.0);//max_braking
			final_score *= GetParameterScore(car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 5.0);//hand-brake-value
			final_score *= GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed] * 10, 10.0);//durability
			final_score *= GetParameterScore(car_params[CarAttributes::turn_mod], 1000, 5.0);//turn_mod
			final_score *= GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 3.0);//drift_mod
			final_score *= GetParameterScore(car_params[CarAttributes::visibility], 10, 5.0);//visibility
			break;
		}
	}
	return static_cast<int>(final_score);
}
int AIobject::GetTireScore(const std::vector<int>& terrain, const std::vector<std::string>& tire_params)
{
	double points = 0;
	double x;
	double y;
	
	for (int i = 0; i < static_cast<int>(GameValues::TerrainTypes); ++i)
	{
		for (int j = 0; j < static_cast<int>(tire_params[i].size()); ++j)
		{
			if (tire_params[i][j] == 'x')
			{
				x = atof(tire_params[i].substr(0, j).c_str());
				y = atof(tire_params[i].substr(j + 1, static_cast<int>(tire_params[i].size()) - j - 1).c_str());
				break;
			}
		}
		for (int j = 0; j < static_cast<int>(y - x + 1); ++j)
		{
			points += GetProbability(y, j) * terrain[i];
		}
	}
	return static_cast<int>(points*100.0f);
}
void AIobject::SetCarAttributes(const std::vector<int>& car_params)
{
	this->car_params = car_params;
}
void AIobject::SetTireAttributes(const std::vector<std::string>& tire_params)
{
	this->tire_params = tire_params;
}
std::string AIobject::TakeAction(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score)
{
	switch (behaviour)
	{
		case GameValues::BehaviourAggressive:
		{
			return TakeActionAggressiveAI(tour, current_speed, current_durablity, current_score);
			break;
		}
		case GameValues::BehaviourDrifter:
		{
			return TakeActionDrifterAI(tour, current_speed, current_durablity, current_score);
			break;
		}
		case GameValues::BehaviourBalanced:
		{
			return TakeActionBalancedAI(tour, current_speed, current_durablity, current_score);
			break;
		}
	}
	return "5";
}


