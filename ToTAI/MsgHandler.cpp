#include "MsgHandler.h"

void MsgHandler::HandlingFunction()
{
	std::string temp;
	do
	{
		std::cin >> temp;
		msgs.push(temp);
	} while (temp != "exit");
}

MsgHandler::MsgHandler()
{
	handling_thread = new std::thread(&MsgHandler::HandlingFunction, this);
}

MsgHandler::~MsgHandler()
{
	handling_thread->join();
	delete handling_thread;
}

std::string MsgHandler::DispatchMsg()
{
	std::string ret = "";
	if (MsgAvailable())
	{
		ret = msgs.front();
		msgs.pop();
	}
	return ret;
}

bool MsgHandler::MsgAvailable()
{
	return static_cast<bool>(msgs.size());
}
