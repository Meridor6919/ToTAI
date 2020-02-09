#pragma once
#include <queue>
#include <iostream>
#include <string>
#include <thread>
#include "Constants.h"

class MsgHandler {
	std::queue<std::string> msgs;
	void HandlingFunction();
	std::thread *handling_thread;

public:
	MsgHandler();
	~MsgHandler();
	std::string DispatchMsg();
	bool MsgAvailable();


};