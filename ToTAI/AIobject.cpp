#include "AIobject.h"

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
		base /= static_cast<float>(car_modifiers[CarAttributes::drift_mod]) / 100.0f;
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
		base /= static_cast<float>(car_modifiers[CarAttributes::turn_mod]) / 100.0f;
		if (base > 100.0f)
		{
			base = 100.0f;
		}
		return (sqrt(base * -base + 200.0f * base) + 2.0f * base) / 3.0f;
	}
}
float AIobject::CalculateBurning(float value)
{
	float raw = value / static_cast<float>(car_modifiers[CarAttributes::max_speed]);
	float result = 0.0f;

	if (raw < 0.0f)
	{
		return 0.0f;
	}
	if (raw > 0.25f)
	{
		raw = 0.25f;
		value = static_cast<float>(car_modifiers[CarAttributes::max_speed]) * 0.25f;
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
		float result = (2.0f * chance*static_cast<float>(car_modifiers[CarAttributes::drift_mod]) + 10000.0f + 100.0f * static_cast<float>(atof(field.c_str()))) /
			(10000.0f / static_cast<float>(atof(field.c_str())) + 100.0f + static_cast<float>(car_modifiers[CarAttributes::drift_mod]));
		float secondary_result = (3.0f * chance*static_cast<float>(car_modifiers[CarAttributes::drift_mod]) + 20000.0f + 200.0f * static_cast<float>(atof(field.c_str()))) /
			(20000.0f / static_cast<float>(atof(field.c_str())) + 200.0f + static_cast<float>(car_modifiers[CarAttributes::drift_mod]));

		if (secondary_result > result)
		{
			result = secondary_result;
		}
		float base = ((result / static_cast<float>(atof(field.c_str())) - 1.0f) * 100.0f + result - static_cast<float>(atof(field.c_str()))) /
			(static_cast<float>(car_modifiers[CarAttributes::drift_mod]) / 100.0f);

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
		float base = (-(200.0f + 12.0f * chance) + sqrt(delta)) / -10.0f*((static_cast<float>(car_modifiers[CarAttributes::turn_mod])) / 100.0f);
		return static_cast<float>(atof(field.c_str())) + base / (100.0f / static_cast<float>(atof(field.c_str())) + 1.0f);
	}
}
float AIobject::TireEffectivness(const std::string &field)
{
	const int terrain = field[0] - 48;
	double x, y;
	double result = 0.0;
	for (int i = 0; i < static_cast<int>(tire_modifiers[terrain].size()); ++i)
	{
		if (tire_modifiers[terrain][i] == 'x')
		{
			x = atof(tire_modifiers[terrain].substr(0, i).c_str());
			y = atof(tire_modifiers[terrain].substr(i + 1, static_cast<int>(tire_modifiers[terrain].size()) - i - 1).c_str());
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
