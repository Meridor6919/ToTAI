#include "ActiveAI.h"

std::array<int, GameValues::TerrainTypes> ActiveAI::CountTerrainTypes(const std::vector<std::string>& tour)
{
	std::array<int, GameValues::TerrainTypes> ret;
	ret.fill(0);
	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		++ret[tour[i][0] - 48];
	}
	return ret;
}
double ActiveAI::AvarageProbabilityOfTires(double number_of_tests, double wanted_number)
{
	double result = 1;
	double extra = (number_of_tests - wanted_number) < 1 ? 1.0 : 0.0;
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
double ActiveAI::TireEffectivness(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour)
{
	double score = 0.0;
	double x;
	double y;

	for (int i = 0; i < static_cast<int>(GameValues::TerrainTypes); ++i)
	{
		for (int j = 0; j < static_cast<int>(tire_attributes[i].size()); ++j)
		{
			if (tire_attributes[i][j] == 'x')
			{
				x = atof(tire_attributes[i].substr(0, j).c_str());
				y = atof(tire_attributes[i].substr(j + 1, static_cast<int>(tire_attributes[i].size()) - j - 1).c_str());
				break;
			}
		}
		std::array<int, GameValues::TerrainTypes> terrain = CountTerrainTypes(tour);
		for (int j = 0; j < static_cast<int>(y - x + 1); ++j)
		{
			score += AvarageProbabilityOfTires(y, j) * terrain[i];
		}
	}
	return score;
}
double ActiveAI::CarParameterScore(double value, double increasing_bound, bool decrease_after)
{
	const bool after_the_peak = value > increasing_bound;
	if (after_the_peak && decrease_after)
	{
		value = 2 * increasing_bound - value;
		if (value < 0)
		{
			value = 0;
		}
	}
	else
	{
		if (after_the_peak)
		{
			value = increasing_bound;
		}
	}
	return (value / increasing_bound)*(value / increasing_bound);
}
int ActiveAI::OptimumMaximumSpeed(const std::vector<std::string>& tour)
{
	double global_max = 0.0;
	double local_max = 0.0;
	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		local_max += 75.0;
		const int segment_size = static_cast<int>(tour[i].size());
		if (segment_size > 1)
		{
			double safe_speed = atof(tour[i].substr(1, segment_size - 1).c_str());
			if (safe_speed < local_max)
			{
				local_max = safe_speed;
			}
		}
		if (local_max >= 900)
		{
			local_max = 900;
		}
		if (local_max > global_max)
		{
			global_max = local_max;
		}
	}
	return static_cast<int>(global_max);
}
int ActiveAI::SelectTarget(int id, std::vector<double> local_score, double lower_bound, double upper_bound)
{
	int selected_id = 10;
	double high_score = lower_bound;
	for (int i = 0; i < static_cast<int>(local_score.size()); ++i)
	{
		if (id != i)
		{
			if (local_score[id] <= local_score[i] + upper_bound && local_score[id] >= local_score[i] + lower_bound && high_score >= local_score[i])
			{
				selected_id = i;
				high_score = local_score[i];
			}
		}
	}
	return selected_id;
}
double ActiveAI::EvaluateChance(std::string field, const double speed, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0;
	}
	field.erase(0, 1);
	float base = (speed / atof(field.c_str()) - 1) * 100.0 + speed - atof(field.c_str());

	if (base < 0.0)
	{
		base = 0.0;
	}
	if (drift)
	{
		base /= static_cast<double>(car_attributes[CarAttributes::drift_mod]) / 100.0;
		if (base > 100.0)
		{
			base = 100.0;
		}
		float result = (speed + base) / 2.0;
		if (base < speed)
		{
			result = (speed + base * 2.0) / 3.0;
		}
		if (result > 100.0)
		{
			result = 100.0;
		}
		return result;
	}
	else
	{
		base /= static_cast<double>(car_attributes[CarAttributes::turn_mod]) / 100.0;
		if (base > 100.0)
		{
			base = 100.0;
		}
		return (sqrt(base * -base + 200.0 * base) + 2.0 * base) / 3.0;
	}
}
double ActiveAI::CalculateBurning(double value)
{
	double raw = value / static_cast<double>(car_attributes[CarAttributes::max_speed]);
	double result = 0.0;

	if (raw < 0.0)
	{
		return 0.0;
	}
	if (raw > 0.25)
	{
		raw = 0.25;
		value = static_cast<double>(car_attributes[CarAttributes::max_speed]) * 0.25;
	}
	int level = static_cast<int>(raw*20.0) + 10;
	result = value * static_cast<double>(level + level * level) / 2.0;
	return result / 50.0;
}
double ActiveAI::MaximumSpeedOnTurn(std::string field, const double chance_to_fail, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}
	field.erase(0, 1);

	if (drift)
	{
		double result = (2.0 * chance_to_fail*static_cast<double>(car_attributes[CarAttributes::drift_mod]) + 10000.0 + 100.0 * atof(field.c_str())) /
			(10000.0 / atof(field.c_str()) + 100.0 + static_cast<double>(car_attributes[CarAttributes::drift_mod]));
		double secondary_result = (3.0 * chance_to_fail*static_cast<double>(car_attributes[CarAttributes::drift_mod]) + 20000.0 + 200.0 * atof(field.c_str())) /
			(20000.0 / atof(field.c_str()) + 200.0 + static_cast<double>(car_attributes[CarAttributes::drift_mod]));

		if (secondary_result > result)
		{
			result = secondary_result;
		}
		float base = ((result / atof(field.c_str()) - 1.0) * 100.0 + result - atof(field.c_str())) /
			(static_cast<double>(car_attributes[CarAttributes::drift_mod]) / 100.0);

		if (base > 100.0)
		{
			result = 2.0 * chance_to_fail - 100.0;
		}
		else if (base < 0.0)
		{
			result = 3.0 * chance_to_fail;
		}
		return result;
	}
	else
	{
		double delta = (200.0 + 12.0 * chance_to_fail)*(200.0 + 12.0 * chance_to_fail) - 180.0 * chance_to_fail*chance_to_fail;
		double base = (-(200.0 + 12.0 * chance_to_fail) + sqrt(delta)) / -10.0*((static_cast<double>(car_attributes[CarAttributes::turn_mod])) / 100.0);
		return atof(field.c_str()) + base / (100.0 / atof(field.c_str()) + 1.0);
	}
}
double ActiveAI::CalculateSpeed(const std::string &current_field, const double speed, const int acceleration_value)
{
	const int terrain_type = current_field[0] - 48;
	double x, y;
	double tire_modifier = 0.0;
	for (int i = 0; i < static_cast<int>(tire_attributes[terrain_type].size()); ++i)
	{
		if (tire_attributes[terrain_type][i] == 'x')
		{
			x = atof(tire_attributes[terrain_type].substr(0, i).c_str());
			y = atof(tire_attributes[terrain_type].substr(i + 1, static_cast<int>(tire_attributes[terrain_type].size()) - i - 1).c_str());
			break;
		}
	}
	for (int j = 0; j < static_cast<int>(y - x + 1); ++j)
	{
		tire_modifier += AvarageProbabilityOfTires(y, j);
	}
	return (speed + static_cast<double>(acceleration_value) * (0.9 + 0.2*tire_modifier)) * 0.9;
}
ActiveAI::ActiveAI()
{
	behaviour = static_cast<GameValues::Behaviour>(rand() % GameValues::Behaviour::last);
}
std::string ActiveAI::GetName()
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

	return ret;
}
void ActiveAI::TryCar(const std::vector<int>& car_attributes, const std::vector<std::string>& tour, std::string car_path)
{
	double local_score = 1.0;
	const static double optimum_max_speed = OptimumMaximumSpeed(tour);
	std::array<double, CarAttributes::last> maximum_value;
	std::array<double, CarAttributes::last> value_weight;

	//setting peak values for attributes
	maximum_value[CarAttributes::max_speed] = optimum_max_speed;
	maximum_value[CarAttributes::max_accelerating] = car_attributes[CarAttributes::max_speed];
	maximum_value[CarAttributes::max_braking] = car_attributes[CarAttributes::max_speed] * 0.9;
	maximum_value[CarAttributes::hand_brake_value] = 30.0;
	maximum_value[CarAttributes::durability] = car_attributes[CarAttributes::max_speed] * 10;
	maximum_value[CarAttributes::visibility] = 7;
	maximum_value[CarAttributes::turn_mod] = 300;
	maximum_value[CarAttributes::drift_mod] = 300;

	//setting weights for diffrent ai behaviour in order:
	//max speed, max_accelerating, max_braking, hand_brake_value, durability, visibility, turn_mod, drift_mod
	switch (behaviour)
	{
		case GameValues::Drifter:
		{
			
			value_weight = { 3.0, 6.0, 0.5, 9.9, 5.0, 2.0, 0.0, 10.0 };
			break;
		}
		case GameValues::Aggressive:
		{
			value_weight = { 6.0, 9.0, 0.5, 0.5, 8.0, 0.0, 10.0, 10.0 };
			break;
		}
		case GameValues::Balanced:
		{
			value_weight = { 5.0, 6.0, 3.0, 3.5, 7.0, 3.0, 9.9, 9.9 };
			break;
		}
	}
	//Calculating total score for a car and comparing it to the best option
	double max_speed_modifier = CarParameterScore(car_attributes[CarAttributes::max_speed], maximum_value[CarAttributes::max_speed], false);
	for (int i = 0; i < CarAttributes::last; ++i)
	{
		double local_modifier = CarParameterScore(car_attributes[i], maximum_value[i], i == CarAttributes::hand_brake_value);
		if (i != CarAttributes::visibility && max_speed_modifier*2.0 < local_modifier)
		{
			local_modifier = max_speed_modifier * 2.0;
		}
		local_score *= 10.0 - value_weight[i] + local_modifier* value_weight[i];
	}
	if (local_score > best_car_score)
	{
		best_car_score = local_score;
		best_car = car_path;
		this->car_attributes = car_attributes;

	}
}
void ActiveAI::TryTires(const std::vector<std::string>& tire_attributes, const std::vector<std::string>& tour, std::string tire_path)
{
	double local_score = TireEffectivness(tire_attributes, tour);
	if (local_score > best_tires_score)
	{
		best_tires_score = local_score;
		best_tires = tire_path;
		this->tire_attributes = tire_attributes;
	}
}
std::pair<int, int> ActiveAI::GetAction(int id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	//placeholder
	return std::pair<int, int>(4, 0);
}
int ActiveAI::GetAttack(int id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	std::vector<double> score_vector = {};
	for (int i = 0; i < static_cast<int>(all_attributes.size()) / 3; ++i)
	{
		score_vector.emplace_back(atof(all_attributes[i * 3].c_str()));
	}

	switch (behaviour)
	{
		//always attacking opponents on the front as well as on the back 
		case GameValues::Behaviour::Aggressive:
		{
			return SelectTarget(id, score_vector, -GameValues::attack_backward_distance, GameValues::attack_forward_distance); 
		}
		//only attacking opponents on the front when the current segment is not a turn
		case GameValues::Behaviour::Drifter:
		{
			if (static_cast<int>(tour[0].size()) > 1 || static_cast<int>(tour[1].size()) > 1)
			{
				return SelectTarget(id, score_vector, 0, GameValues::attack_forward_distance);
			}
			break;
		}
		//only attacking opponents on the front when durability is above 33%
		case GameValues::Behaviour::Balanced:
		{
			if (atof(all_attributes[id*3+1].c_str()) < static_cast<double>(car_attributes[CarAttributes::durability])*0.33)
			{
				return SelectTarget(id, score_vector, 0, GameValues::attack_forward_distance); //both ways attacking attacking
			}
			break;
		}
	}
	return 10;
}
