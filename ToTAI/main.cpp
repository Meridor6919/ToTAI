#include <iostream>
#include "Constants.h"

int main()
{
	int number_of_ais;


	std::string temp;
	std::cout << ConnectionCodes::Start;
	std::cin >> temp;
	std::cout << ConnectionCodes::GetInit;
	std::cin >> temp;
	number_of_ais = temp[2] - 48;


	std::cin >> temp;
}