#include "AIInstance.h"

AIInstance::AIInstance()
{
	behaviour = rand() % 3;
}

std::string AIInstance::GetName()
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

int AIInstance::GetCarScore(const std::vector<std::string>& tour, const std::vector<int>& car_params)
{
	return 0;
}

int AIInstance::GetTireScore(const std::vector<std::string>& tour, const std::vector<std::string>& tire_params)
{
	auto Probability = [](double number_of_tests, double wanted_number) {

		double result = 1;
		int extra = number_of_tests - wanted_number;
		extra < 1 ? extra = 1 : 0;
		wanted_number < 1 ? wanted_number = 1 : 0;
		while (number_of_tests > 1)
		{
			result *= number_of_tests / wanted_number / extra / 2;
			--number_of_tests;
			--wanted_number < 1 ? wanted_number = 1: 0;
			--extra < 1 ? extra = 1: 0;
		}
		return result/2;
	};

	int terrain[GameValues::TerrainTypes] = { 0,0,0,0,0,0 };
	double points = 0;
	double x;
	double y;
	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		terrain[tour[i][0] - 48] += 1 + 9 * (static_cast<int>(tour[i].size()) > 1);
	}
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
			points += Probability(y, j) * terrain[i];
		}
	}
	return static_cast<int>(points*100.0f);
}

void AIInstance::SetCarAttributes(const std::vector<int>& car_params)
{
}

void AIInstance::SetTireAttributes(const std::vector<std::string>& tire_params)
{
}
