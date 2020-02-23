#include "AIhandler.h"

double AIhandler::GetProbability(double number_of_tests, double wanted_number)
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

double AIhandler::GetParameterScore(double parameter_value, const double max, const double maximum_score)
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
AIhandler::AIhandler()
{
	behaviour = rand() % 3;
}

std::string AIhandler::GetName()
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

int AIhandler::GetCarScore(const int optimum_max_speed, const std::vector<int>& car_params)
{
	double final_score = 1;
	switch (behaviour)
	{
	case GameValues::BehaviourDrifter:
		{
			//max value - 5 000 000
			final_score *= GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 10.0);//acceleration
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 5.0);//max_speed
			final_score *= GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed]/10*9, 2.0);//max_braking
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed] - car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 10.0) + 0.01;//hand-brake-value
			final_score *= GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed]*10, 10.0);//durability
			final_score *= GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 100.0);//drift_mod
			final_score *= GetParameterScore(car_params[CarAttributes::visibility], 10, 5.0);//visibility
			break;
		}
		case GameValues::BehaviourAggressive:
		{
			//max value - 3 037 500
			final_score *= GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 15.0);//acceleration
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 15.0);//max_speed
			final_score *= GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed] / 10 * 9, 1.0);//max_braking
			final_score *= GetParameterScore(car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 1.0) + 0.01;//hand-brake-value
			final_score *= GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed] * 10, 15.0);//durability
			final_score *= GetParameterScore(car_params[CarAttributes::turn_mod], 1000, 30.0);//turn_mod
			final_score *= GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 30.0);//drift_mod
			final_score *= GetParameterScore(car_params[CarAttributes::visibility], 10, 1.0);//visibility
			break;
		}
		case GameValues::BehaviourBalanced:
		{
			//max value - 281 250 000
			final_score *= GetParameterScore(car_params[CarAttributes::max_accelerating], car_params[CarAttributes::max_speed], 15.0);//acceleration
			final_score *= GetParameterScore(car_params[CarAttributes::max_speed], optimum_max_speed, 10.0);//max_speed
			final_score *= GetParameterScore(car_params[CarAttributes::max_braking], car_params[CarAttributes::max_speed] / 10 * 9, 5.0);//max_braking
			final_score *= GetParameterScore(car_params[CarAttributes::hand_brake_value], car_params[CarAttributes::max_speed], 5.0);//hand-brake-value
			final_score *= GetParameterScore(car_params[CarAttributes::durability], car_params[CarAttributes::max_speed] * 10, 10.0);//durability
			final_score *= GetParameterScore(car_params[CarAttributes::turn_mod], 1000, 50.0);//turn_mod
			final_score *= GetParameterScore(car_params[CarAttributes::drift_mod], 1000, 30.0);//drift_mod
			final_score *= GetParameterScore(car_params[CarAttributes::visibility], 10, 5.0);//visibility
			break;
		}
	}
	return static_cast<int>(final_score);
}

int AIhandler::GetTireScore(const std::vector<int>& terrain, const std::vector<std::string>& tire_params)
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

void AIhandler::SetCarAttributes(const std::vector<int>& car_params)
{
}

void AIhandler::SetTireAttributes(const std::vector<std::string>& tire_params)
{
}
