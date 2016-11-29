/*
 * main.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: schutzekatze
 */

#include "TestTask.h"
#include "TestTask2.h"

#include <thread>
#include <chrono>

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "TaskExecuter.h"

int main()
{
	Task* test = new TestTask();
	Task* test2 = new TestTask2();

	TaskExecuter::start();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	TaskExecuter::enqueue_task(test, TaskExecuter::Priority::NORMAL);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	TaskExecuter::enqueue_task(test2, TaskExecuter::Priority::HIGH);

	test->wait_for_state(Task::State::ENDED);

	cout << "meow: " << endl;
	int i;
	cin >> i;

	delete test;
	delete test2;

	return 0;
}


