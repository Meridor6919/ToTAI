#include "ToTAIFunctions.h"

int main()
{
	ToTAIFunctions::Start();
	auto a = ToTAIFunctions::GetInit();
	auto b = ToTAIFunctions::GetAllAtributes(a.second);
	auto c = ToTAIFunctions::GetCarNames();
	auto d = ToTAIFunctions::GetCarParams(c[0]);
	auto e = ToTAIFunctions::GetTireNames();
	auto f = ToTAIFunctions::GetTireParams(e[0]);
	auto g = ToTAIFunctions::GetTour();
	auto h = ToTAIFunctions::NewTurn(0);
	std::cout << "DONE" << std::endl;
	ToTAIFunctions::Exit();
}