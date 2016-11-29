/*
 * Task.cpp
 *
 *  Created on: Nov 28, 2016
 *      Author: schutzekatze
 */

#include "Task.h"

void Task::wait_for_state(State state)
{
	unique_lock<mutex> lock(state_mutex);

	if (this->state != state)
	{
		state_conditions[state].wait(lock);
	}
}

Task::State Task::get_state()
{
	unique_lock<mutex> lock(state_mutex);

	return state;
}

void Task::set_state(State state)
{
	unique_lock<mutex> lock(state_mutex);

	this->state = state;

	if (state == State::ENDED)
	{
		for(condition_variable& condition : state_conditions)
		{
			condition.notify_all();
		}
	}
	else
	{
		state_conditions[state].notify_all();
	}
}

