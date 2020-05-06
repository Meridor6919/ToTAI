#include "PipeConnection.h"

std::string PipeConnection::ProcessMsg(Report::StreamType input, std::string msg)
{
	if (input)
	{
		std::cin >> msg;
	}
	else
	{
		std::cout << msg << std::endl;
	}
	output_stream.open("ToTAI.log", std::ios::app);
	output_stream << (input ? "Input: " : "Output: ") + msg << '\n';
	output_stream.close();
	return msg;
}
void PipeConnection::CloseOutputStream()
{
	output_stream.close();
}
PipeConnection::PipeConnection()
{
	output_stream.open("ToTAI.log");
	output_stream.close();
}
void PipeConnection::Start()
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::Start));
	ProcessMsg(Report::Input);
}
void PipeConnection::Exit()
{
	ProcessMsg(Report::Input);
	CloseOutputStream();
}
std::pair<int, int> PipeConnection::GetInit()
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::GetInit));
	const int number_of_ais = ProcessMsg(Report::Input)[0] - 48;
	const int number_of_participants = ProcessMsg(Report::Input)[0] - 48;
	return { number_of_ais, number_of_participants };
}
bool PipeConnection::NewTurn(int id)
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::NewTurn) + static_cast<char>(id + 48));
	bool result = ProcessMsg(Report::Input)[0] - 48;
	if (!result)
	{
		return true;
	}
	else
	{
		return false;
	}
}
std::vector<std::string> PipeConnection::GetCarNames()
{
	std::vector<std::string> ret;
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::GetCarNames));
	const int number_of_cars = atoi(ProcessMsg(Report::Input).c_str());
	for (int i = 0; i < number_of_cars; ++i)
	{
		ret.emplace_back(ProcessMsg(Report::Input));
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetTireNames()
{
	std::vector<std::string> ret;
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::GetTireNames));
	const int number_of_tires = atoi(ProcessMsg(Report::Input).c_str());
	for (int i = 0; i < number_of_tires; ++i)
	{
		ret.emplace_back(ProcessMsg(Report::Input));
	}
	return ret;
}
std::vector<int> PipeConnection::GetCarParams(std::string car_path)
{
	std::vector<int> ret;
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::GetCarParams) + car_path);
	for (int i = 0; i < CarAttributes::last; ++i)
	{
		ret.emplace_back(atoi(ProcessMsg(Report::Input).c_str()));
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetTireParams(std::string tire_path)
{
	std::vector<std::string> ret;
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::GetTireParams) + tire_path);
	for (int i = 0; i < GameValues::TerrainTypes; ++i)
	{
		ret.emplace_back(ProcessMsg(Report::Input));
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetAllAtributes(int number_of_participants)
{
	std::vector<std::string> ret;
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::GetAllAttributes));
	for (int i = 0; i < number_of_participants*3; ++i)
	{
		ret.emplace_back(ProcessMsg(Report::Input));
	}
	return ret;
}
std::vector<std::string> PipeConnection::GetTour()
{
	std::vector<std::string> ret;
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::GetTour));
	const int size = atoi(ProcessMsg(Report::Input).c_str());
	for (int i = 0; i < size; ++i)
	{
		ret.emplace_back(ProcessMsg(Report::Input));
	}
	return ret;
}
void PipeConnection::SetAction(int id, int action_type, int value)
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::SetAction) + static_cast<char>(id + 48) + static_cast<char>(action_type + 48) + std::to_string(value));
}
void PipeConnection::SetAttack(int id, int target)
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::SetAttack) + static_cast<char>(id + 48) + std::to_string(target));
}
void PipeConnection::SetName(int id, std::string name)
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::SetName) + static_cast<char>(id + 48) + name);
}
void PipeConnection::SetTires(int id, std::string tire_path)
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::SetTires) + static_cast<char>(id + 48) + tire_path);
}
void PipeConnection::SetCar(int id, std::string car_path)
{
	ProcessMsg(Report::Output, std::to_string(ConnectionCodes::SetCar) + static_cast<char>(id + 48) + car_path);
}
