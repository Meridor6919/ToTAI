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
float AIobject::EvaluateSpeed(std::string field, const float chance_to_fail, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}
	field.erase(0, 1);

	if (drift)
	{
		float result = (2.0f * chance_to_fail*static_cast<float>(car_params[CarAttributes::drift_mod]) + 10000.0f + 100.0f * static_cast<float>(atof(field.c_str()))) /
			(10000.0f / static_cast<float>(atof(field.c_str())) + 100.0f + static_cast<float>(car_params[CarAttributes::drift_mod]));
		float secondary_result = (3.0f * chance_to_fail*static_cast<float>(car_params[CarAttributes::drift_mod]) + 20000.0f + 200.0f * static_cast<float>(atof(field.c_str()))) /
			(20000.0f / static_cast<float>(atof(field.c_str())) + 200.0f + static_cast<float>(car_params[CarAttributes::drift_mod]));

		if (secondary_result > result)
		{
			result = secondary_result;
		}
		float base = ((result / static_cast<float>(atof(field.c_str())) - 1.0f) * 100.0f + result - static_cast<float>(atof(field.c_str()))) /
			(static_cast<float>(car_params[CarAttributes::drift_mod]) / 100.0f);

		if (base > 100.0f)
		{
			result = 2.0f * chance_to_fail - 100.0f;
		}
		else if (base < 0.0f)
		{
			result = 3.0f * chance_to_fail;
		}
		return result;
	}
	else
	{
		float delta = (200.0f + 12.0f * chance_to_fail)*(200.0f + 12.0f * chance_to_fail) - 180.0f * chance_to_fail*chance_to_fail;
		float base = (-(200.0f + 12.0f * chance_to_fail) + sqrt(delta)) / -10.0f*((static_cast<float>(car_params[CarAttributes::turn_mod])) / 100.0f);
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
float AIobject::GetSpeed(const std::string &current_field, const float current_speed, const int acceleration_value)
{
	return (current_speed + static_cast<float>(acceleration_value) * (0.9f + 0.2f*TireEffectivness(current_field))) * 0.9f;
}
float AIobject::GetScore(const std::vector<std::string> &raw_data, int id)
{
	return atof(raw_data[id*3 + 2].c_str());
}
float AIobject::GetDurability(const std::vector<std::string>& raw_data, int id)
{
	return atof(raw_data[id * 3 + 1].c_str());
}
int AIobject::AttackAggressiveAI(const std::vector<std::string>& tour, int id, const std::vector<std::string>& data)
{
	const float this_score = GetScore(data, id);
	int selected_id = 10;
	float hi_score = this_score + GameValues::attack_backward_distance;
	for (int i = 0; i < static_cast<int>(data.size()); ++i)
	{
		if (id != i)
		{
			const float local_score = GetScore(data, i);
			if (this_score - GameValues::attack_forward_distance < local_score && this_score + GameValues::attack_backward_distance > local_score && local_score < hi_score)
			{
				selected_id = i;
				hi_score = local_score;
			}
		}
	}
	return selected_id;
}
int AIobject::AttackDrifterAI(const std::vector<std::string>& tour, int id, const std::vector<std::string>& data)
{
	if (static_cast<int>(tour[0].size()) > 1 || static_cast<int>(tour[1].size()) > 1)
	{
		return 10;
	}
	else
	{
		return AttackAggressiveAI(tour, id, data);
	}
}
int AIobject::AttackBalancedAI(const std::vector<std::string>& tour, int id, const std::vector<std::string>& data)
{
	if (GetDurability(data, id) < static_cast<float>(car_params[CarAttributes::durability])*0.33333f)
	{
		return 10;
	}
	else
	{
		return AttackDrifterAI(tour, id, data);
	}
}
std::string AIobject::TakeActionDrifterAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score)
{
	return std::string("010");
}
std::string AIobject::TakeActionAggressiveAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score)
{
	/*
	int max_speed = INT_MAX;
	for (int i = 0; i < car_params[CarAttributes::visibility]; ++i)
	{
		if (static_cast<int>(tour[i].size()) > 1)
		{
			bool drift = rand() % 2;
			float estimated_speed = EvaluateSpeed(tour[i], 5.0f, drift);
		}
	}
	*/
	return std::string("040");
}
std::string AIobject::TakeActionBalancedAI(const std::vector<std::string> & tour, const float current_speed, const float current_durablity, const float current_score)
{
	return std::string("015");
}
double AIobject::NormalizeScore(double score, double max_local_score, double max_global_score)
{
	return (max_global_score - max_local_score + score) / max_global_score;
}
double AIobject::SafeSpeed(const std::vector<std::string>& tour, char turn_behaviour)
{
	double return_value = car_params[CarAttributes::max_speed] * 1.25;
	const double max_braking_value = car_params[CarAttributes::max_braking] > car_params[CarAttributes::hand_brake_value] ? car_params[CarAttributes::max_braking] : car_params[CarAttributes::hand_brake_value];
	double max_brake = 0.0f;
	for (int i = 1; i < car_params[CarAttributes::visibility] && i < static_cast<int>(tour.size()); ++i)
	{
		if (static_cast<int>(tour[i].size()) > 1)//turn
		{
			const double local_braking_value = (turn_behaviour & 1) ? car_params[CarAttributes::max_braking] : car_params[CarAttributes::hand_brake_value];
			max_brake = (max_brake + local_braking_value* (0.9f + 0.2f*TireEffectivness(tour[i]))) / 0.9;
			if (return_value > atof(tour[i].substr(1, static_cast<int>(tour[i].size()) - 1).c_str()))
			{
				return_value = atof(tour[i].substr(1, static_cast<int>(tour[i].size()) - 1).c_str());
			}
		}
		else
		{
			max_brake = (max_brake + max_braking_value * (0.9f + 0.2f*TireEffectivness(tour[i]))) / 0.9;
		}
	}
	return return_value;
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
	//max value - 100000000.0
	double final_score = 1;
	switch (behaviour)
	{
		case GameValues::Behaviour::Drifter:
		{
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 6.0), 6.0);//acceleration
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 3.0), 3.0);//max_speed
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed]/10*9, 0.5), 0.5);//max_braking
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_speed] - car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 9.9),9.9);//hand-brake-value
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed]*10, 5.0),5.0);//durability
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 10.0), 10.0);//drift_mod
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::visibility], 10, 2.0),2.0);//visibility
			break;
		}
		case GameValues::Behaviour::Aggressive:
		{
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 10.0), 10.0);//acceleration
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 6.0), 6.0);//max_speed
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed] / 10 * 9, 0.5), 0.5);//max_braking
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_speed] - car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 0.7), 0.7);//hand-brake-value
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed] * 10, 8.0), 8.0);//durability
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::turn_mod], 1000, 10.0), 10.0);//turn_mod
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 10.0), 10.0);//drift_mod
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::visibility], 10, 2.0), 1.0);//visibility
			break;
		}
		case GameValues::Behaviour::Balanced:
		{
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 6.0), 6.0);//acceleration
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 5.0), 5.0);//max_speed
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed] / 10 * 9, 3.0), 3.0);//max_braking
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::max_speed] - car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 3.0), 3.0);//hand-brake-value
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed] * 10, 8.0), 7.0);//durability
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::turn_mod], 1000, 10.0), 10.0);//turn_mod
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 10.0), 10.0);//drift_mod
			final_score *= NormalizeScore(GetParameterScore(car_params[CarAttributes::visibility], 10, 2.0), 2.5);//visibility
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
		case GameValues::Behaviour::Aggressive:
		{
			return TakeActionAggressiveAI(tour, current_speed, current_durablity, current_score);
			break;
		}
		case GameValues::Behaviour::Drifter:
		{
			return TakeActionDrifterAI(tour, current_speed, current_durablity, current_score);
			break;
		}
		case GameValues::Behaviour::Balanced:
		{
			return TakeActionBalancedAI(tour, current_speed, current_durablity, current_score);
			break;
		}
	}
	return "5";
}
int AIobject::Attack(const std::vector<std::string>& tour, int id, const std::vector<std::string>& data)
{
	switch (behaviour)
	{
	case GameValues::Behaviour::Aggressive:
	{
		return AttackAggressiveAI(tour, id, data);
		break;
	}
	case GameValues::Behaviour::Drifter:
	{
		return AttackDrifterAI(tour, id, data);
		break;
	}
	case GameValues::Behaviour::Balanced:
	{
		return AttackBalancedAI(tour, id, data);
		break;
	}
	}
	return 10;
}


