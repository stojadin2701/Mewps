/*
 * TestTask2.h
 *
 *  Created on: Oct 3, 2016
 *      Author: schutzekatze
 */

#ifndef TESTTASK2_H_
#define TESTTASK2_H_

#include <thread>
#include <chrono>

#include <iostream>
using std::cout;
using std::endl;

#include "Task.h"

class TestTask2: public Task
{

public:

	void execute() override
	{
		cout << "a" << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		cout << "b" << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

};

#endif /* TESTTASK2_H_ */

