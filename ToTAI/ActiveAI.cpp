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
	double after_the_peak = value > increasing_bound;
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
	double local_score = 1;
	double optimum_max_speed = 200.0;
	std::array<double, CarAttributes::last> maximum_value;
	std::array<double, CarAttributes::last> value_weight;

	maximum_value[CarAttributes::max_speed] = optimum_max_speed;
	maximum_value[CarAttributes::max_accelerating] = car_attributes[CarAttributes::max_speed];
	maximum_value[CarAttributes::max_braking] = car_attributes[CarAttributes::max_speed] * 0.9;
	maximum_value[CarAttributes::hand_brake_value] = 30.0;
	maximum_value[CarAttributes::durability] = car_attributes[CarAttributes::max_speed] * 10;
	maximum_value[CarAttributes::visibility] = 7;
	maximum_value[CarAttributes::turn_mod] = 300;
	maximum_value[CarAttributes::drift_mod] = 300;

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
	double max_speed_modifier = CarParameterScore(car_attributes[CarAttributes::max_speed], maximum_value[CarAttributes::max_speed], false);
	for (int i = 0; i < CarAttributes::last; ++i)
	{
		double local_modifier = CarParameterScore(car_attributes[i], maximum_value[i], i == CarAttributes::hand_brake_value);
		if (i != CarAttributes::visibility && max_speed_modifier < local_modifier)
		{
			local_modifier = max_speed_modifier;
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
