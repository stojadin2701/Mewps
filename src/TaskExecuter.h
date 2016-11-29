/*
 * TaskScheduler.h
 *
 *  Created on: Sep 24, 2016
 *      Author: schutzekatze
 */

#ifndef TASKEXECUTER_H_
#define TASKEXECUTER_H_

#include <queue>
using std::priority_queue;

#include <stack>
using std::stack;

#include <thread>
using std::thread;

#include <mutex>
using std::mutex;

#include <condition_variable>
using std::condition_variable;

#include <atomic>
using std::atomic;

#include "Task.h"

/*
 * TaskExecuter executes given tasks one at a time based on their priority.
 */
class TaskExecuter
{

public:

	enum Priority { CHILL, LOW, NORMAL, HIGH, EMERGENCY };

	/*
	 * Use these methods to start and stop active execution of tasks.
	 * Calling the stop method does not wait for the task currently being executed to stop.
	 * You can, however, acquire the current task and wait for it to be interrupted.
	 */
	static void start();
	static void stop();

	/*
	 * After starting the TaskExecuter, it is in the running state.
	 * While in the running state, the executer can either be active or idle.
	 * Idle means that it is running, but that there's no tasks to execute.
	 */
	static bool is_running() { return running; }
	static bool is_idle() { return idle; }

	/*
	 * Removes all queued tasks. (Does not stop the task currently being executed, if any.)
	 */
	static void clear();

	struct QueuedTask
	{
		Task* task;
		Priority priority;

		bool operator<(const QueuedTask& rhs) const { return priority < rhs.priority; }

	private:

		QueuedTask(Task* task, Priority priority): task(task), priority(priority) {}

		friend class TaskExecuter;

	};

	/*
	 * Enqueues the given task to be executed with given priority.
	 */
	static void enqueue_task(Task* task, Priority priority);

	static const QueuedTask& get_current_task();

private:

	static void perform_task();

	static void execute_tasks();
	static thread executer_thread;

	static priority_queue<QueuedTask> task_pool;

	static stack<QueuedTask> interrupted_tasks;

	static QueuedTask current_task;

	static mutex operation_mutex;

	static atomic<bool> running;
	static condition_variable running_condition;

	static atomic<bool> idle;
	static condition_variable idle_condition;
};

#endif /* TASKEXECUTER_H_ */
