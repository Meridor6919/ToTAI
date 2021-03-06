﻿#include "ActiveAI.h"

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
	double extra = (number_of_tests - wanted_number) < 1 ? 1.0 : (number_of_tests - wanted_number);
	wanted_number < 1 ? wanted_number = 1 : wanted_number;
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
	double high_score = local_score[id] - lower_bound;
	for (int i = 0; i < static_cast<int>(local_score.size()); ++i)
	{
		if (id != i)
		{
			if (local_score[id] < local_score[i] + upper_bound && local_score[id] > local_score[i] + lower_bound && high_score >= local_score[i])
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
	double base = (speed / atof(field.c_str()) - 1) * 100.0 + speed - atof(field.c_str());

	if (base < 0.0)
	{
		base = 0.0;
	}
	if (drift)
	{
		base /= static_cast<double>(car_attributes[CarAttributes::Types::drift_mod]) / 100.0;
		if (base > 100.0)
		{
			base = 100.0;
		}
		double result = (speed + base) / 2.0;
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
		base /= static_cast<double>(car_attributes[CarAttributes::Types::turn_mod]) / 100.0;
		if (base > 100.0)
		{
			base = 100.0;
		}
		return (sqrt(base * -base + 200.0 * base) + 2.0 * base) / 3.0;
	}
}
double ActiveAI::CalculateBurning(double value)
{
	double raw = value / static_cast<double>(car_attributes[CarAttributes::Types::max_speed]);
	double result = 0.0;

	if (raw < 0.0)
	{
		return 0.0;
	}
	if (raw > 0.25)
	{
		raw = 0.25;
		value = static_cast<double>(car_attributes[CarAttributes::Types::max_speed]) * 0.25;
	}
	int level = static_cast<int>(raw*20.0) + 10;
	result = value * static_cast<double>(level + level * level) / 2.0;
	return result / 50.0;
}
double ActiveAI::CalculateBurningInversed(double burning)
{
	double lower_bound = 0;
	double higher_bound = static_cast<double>(car_attributes[CarAttributes::Types::max_speed]) * 1.25 - static_cast<double>(car_attributes[CarAttributes::Types::max_speed]);
	int max_number_of_iterations = 0;

	for (int i = 0; i < 10000; ++i)
	{
		double temp = burning - CalculateBurning((lower_bound + higher_bound) / 2);
		if (temp < 0.01 && temp > -0.01)
		{
			return (lower_bound + higher_bound) / 2;
		}
		else if (temp < 0)
		{
			higher_bound = (lower_bound + higher_bound) / 2;
		}
		else
		{
			lower_bound = (lower_bound + higher_bound) / 2;
		}
	}
	return burning;
}
double ActiveAI::EvaluateChanceInversed(std::string field, const double chance_to_fail, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}

	const double required_speed = atof(field.c_str());
	const double turn_modifier = static_cast<double>(car_attributes[CarAttributes::Types::turn_mod + drift]);
	field.erase(0, 1);

	if (drift)
	{
		double result = (2.0 * chance_to_fail*turn_modifier + 10000.0 + 100.0 * required_speed) / (10000.0 / required_speed + 100.0 + turn_modifier);
		double secondary_result = (3.0 * chance_to_fail*turn_modifier + 20000.0 + 200.0 *required_speed) / (20000.0 / required_speed + 200.0 + turn_modifier);

		if (secondary_result > result)
		{
			result = secondary_result;
		}
		double base = ((result / required_speed - 1.0) * 100.0 + result - required_speed) / (turn_modifier / 100.0);

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
		double base = (-(200.0 + 12.0 * chance_to_fail) + sqrt(delta)) / -10.0*(turn_modifier / 100.0);
		return required_speed + base / (100.0 / required_speed + 1.0);
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
	risk = static_cast<double>(rand() % 300 + 5 * (1 + (behaviour == GameValues::Behaviour::Aggressive))) / 10.0;
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
	std::array<double, CarAttributes::Types::last> maximum_value;
	std::array<double, CarAttributes::Types::last> value_weight;

	//setting peak values for attributes
	maximum_value[CarAttributes::Types::max_speed] = optimum_max_speed;
	maximum_value[CarAttributes::Types::max_accelerating] = car_attributes[CarAttributes::max_speed];
	maximum_value[CarAttributes::Types::max_braking] = car_attributes[CarAttributes::max_speed] * 0.9;
	maximum_value[CarAttributes::Types::hand_brake_value] = 30.0;
	maximum_value[CarAttributes::Types::durability] = car_attributes[CarAttributes::max_speed] * 10;
	maximum_value[CarAttributes::Types::visibility] = 7;
	maximum_value[CarAttributes::Types::turn_mod] = 300;
	maximum_value[CarAttributes::Types::drift_mod] = 300;

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
	double max_speed_modifier = CarParameterScore(car_attributes[CarAttributes::Types::max_speed], maximum_value[CarAttributes::Types::max_speed], false);
	for (int i = 0; i < CarAttributes::Types::last; ++i)
	{
		double local_modifier = CarParameterScore(car_attributes[i], maximum_value[i], i == CarAttributes::Types::hand_brake_value);
		if (i != CarAttributes::Types::visibility && max_speed_modifier*2.0 < local_modifier)
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
std::pair<int, int> ActiveAI::GetAction(int global_id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	double current_speed = atof(all_attributes[global_id * 3 + CarAttributes::RecvData::current_speed].c_str());
	double current_durability = atof(all_attributes[global_id * 3 + CarAttributes::RecvData::current_durability].c_str());
	bool hand_brake_braking = car_attributes[CarAttributes::Types::hand_brake_value] > car_attributes[CarAttributes::Types::max_braking];
	double safe_speed = car_attributes[CarAttributes::Types::max_speed]*1.25;

	switch (behaviour)
	{
		case GameValues::Behaviour::Drifter:
		{
			//always drift when possible
			if (tour[0].size() > 1 && current_speed > GameValues::drift_value)
			{
				return std::pair<int, int>(Actions::hand_braking, -car_attributes[CarAttributes::Types::hand_brake_value]);
			}

			//calculating safe_speed
			/*
					TO DO Evaluate safe speed taking into consideration:
						1. hand_braking in turns
						2. friction
						3. speed modulation depending on terrain type
						4. risk
						5. possible attacks
				*/
			for (int i = 0; i < car_attributes[CarAttributes::Types::visibility]; ++i)
			{
				
			}

			//durability burning check
			if (safe_speed > car_attributes[CarAttributes::Types::max_speed])
			{
				double allowed_burning = current_durability * (0.65 + risk / 100.0) / static_cast<double>(tour.size());
				safe_speed = car_attributes[CarAttributes::Types::max_speed] + CalculateBurningInversed(allowed_burning);
			}
			//adjusting to car attributes
			if (safe_speed - current_speed > car_attributes[CarAttributes::Types::max_accelerating])
			{
				safe_speed = current_speed + car_attributes[CarAttributes::Types::max_accelerating];
			}
			else if (current_speed - safe_speed > car_attributes[CarAttributes::Types::max_braking + hand_brake_braking])
			{
				safe_speed = current_speed - car_attributes[CarAttributes::Types::max_braking + hand_brake_braking];
			}

			//selecting action
			double difference = safe_speed - current_speed;
			if (difference < 0)
			{
				return std::pair<int, int>(Actions::braking + hand_brake_braking, (hand_brake_braking ? -car_attributes[CarAttributes::Types::hand_brake_value] : difference));
			}
			else if (difference == 0)
			{
				return std::pair<int, int>(Actions::pass, difference);
			}
			else
			{
				return std::pair<int, int>(Actions::acceleration, difference);
			}
		}
		case GameValues::Behaviour::Aggressive:
		{
			return std::pair<int, int>(Actions::acceleration, 10);//placeholder
		}
		case GameValues::Behaviour::Balanced:
		{
			return std::pair<int, int>(Actions::acceleration, 10);//placeholder
		}	
	}	
}
int ActiveAI::GetAttack(int global_id, const std::vector<std::string>& all_attributes, const std::vector<std::string>& tour)
{
	std::vector<double> score_vector = {};
	for (int i = 0; i < static_cast<int>(all_attributes.size()) / 3; ++i)
	{
		if (atof(all_attributes[i * 3 + CarAttributes::RecvData::current_durability].c_str()) > 0.0)
		{
			score_vector.emplace_back(atof(all_attributes[i * 3 + CarAttributes::RecvData::current_score].c_str()));
		}
		else
		{
			score_vector.emplace_back(-GameValues::attack_forward_distance-1);
		}
	}

	switch (behaviour)
	{
		//always attacking opponents on the front as well as on the back 
		case GameValues::Behaviour::Aggressive:
		{
			return SelectTarget(global_id, score_vector, -GameValues::attack_backward_distance, GameValues::attack_forward_distance);
		}
		//only attacking opponents on the front when the current segment is not a turn
		case GameValues::Behaviour::Drifter:
		{
			if (static_cast<int>(tour[0].size()) > 1 || static_cast<int>(tour[1].size()) > 1)
			{
				return SelectTarget(global_id, score_vector, 0, GameValues::attack_forward_distance);
			}
			break;
		}
		//only attacking opponents on the front when durability is above 33%
		case GameValues::Behaviour::Balanced:
		{
			if (atof(all_attributes[global_id * 3 + CarAttributes::RecvData::current_durability].c_str()) < static_cast<double>(car_attributes[CarAttributes::durability])*0.33)
			{
				return SelectTarget(global_id, score_vector, 0, GameValues::attack_forward_distance); //both ways attacking attacking
			}
			break;
		}
	}
	return 10;
}
