/*
 * TaskExecuter.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: schutzekatze
 */

#include "TaskExecuter.h"

using std::unique_lock;
using std::defer_lock;

thread TaskExecuter::executer_thread(TaskExecuter::execute_tasks);

priority_queue<TaskExecuter::QueuedTask> TaskExecuter::task_pool;

stack<TaskExecuter::QueuedTask> TaskExecuter::interrupted_tasks;

TaskExecuter::QueuedTask TaskExecuter::current_task = { nullptr, CHILL };

mutex TaskExecuter::operation_mutex;

atomic<bool> TaskExecuter::running(false);
condition_variable TaskExecuter::running_condition;

atomic<bool> TaskExecuter::idle(false);
condition_variable TaskExecuter::idle_condition;

void TaskExecuter::start()
{
	unique_lock<mutex> lock(operation_mutex);

	if (!running)
	{
		running = true;
		idle = true;

		running_condition.notify_one();
	}
}

void TaskExecuter::stop()
{
	unique_lock<mutex> lock(operation_mutex);

	if (running)
	{
		running = false;

		if (idle)
		{
			idle_condition.notify_one();
			idle = false;
		}
		else
		{
			current_task.task->interrupt();
		}
	}
}

void TaskExecuter::clear()
{
	unique_lock<mutex> lock(operation_mutex);

	while (task_pool.size() > 0) task_pool.pop();
	while (interrupted_tasks.size() > 0) interrupted_tasks.pop();
}

void TaskExecuter::enqueue_task(Task* task, Priority priority)
{
	unique_lock<mutex> lock(operation_mutex);

	if (running && !idle && (priority > current_task.priority))
	{
		current_task.task->interrupt();

		interrupted_tasks.push(current_task);

		interrupted_tasks.push( { task, priority } );
	}
	else
	{
		task_pool.push( { task, priority } );
	}

	if (idle)
	{
		idle_condition.notify_one();
	}
}

const TaskExecuter::QueuedTask& TaskExecuter::get_current_task()
{
	unique_lock<mutex> lock(operation_mutex);

	return current_task;
}

void TaskExecuter::perform_task()
{
	current_task.task->set_state(Task::State::RUNNING);

	current_task.task->execute();

	idle = true;

	if (current_task.task->interrupted)
	{
		current_task.task->interrupted = false;

		current_task.task->set_state(Task::State::INTERRUPTED);
	}
	else
	{
		current_task.task->set_state(Task::State::ENDED);
	}
}

void TaskExecuter::execute_tasks()
{
	executer_thread.detach();

	unique_lock<mutex> lock(operation_mutex, defer_lock);

	for(;;)
	{
		lock.lock();

		if (!running)
		{
			running_condition.wait(lock);
		}

		if (!interrupted_tasks.empty() && !task_pool.empty())
		{
			if (interrupted_tasks.top().priority >= task_pool.top().priority)
			{
				current_task = interrupted_tasks.top();
				interrupted_tasks.pop();
			}
			else
			{
				current_task = task_pool.top();
				task_pool.pop();
			}

			idle = false;

			lock.unlock();

			perform_task();
		}
		else if (!interrupted_tasks.empty())
		{
			current_task = interrupted_tasks.top();
			interrupted_tasks.pop();

			idle = false;

			lock.unlock();

			perform_task();
		}

		else if(!task_pool.empty())
		{
			current_task = task_pool.top();
			task_pool.pop();

			idle = false;

			lock.unlock();

			perform_task();
		}
		else
		{
			idle_condition.wait(lock);

			lock.unlock();
		}
	}
}
