/*
 * Task.h
 *
 *  Created on: Aug 29, 2016
 *      Author: schutzekatze
 */

#ifndef TASK_H_
#define TASK_H_

#include <atomic>
using std::atomic;

#include <mutex>
using std::mutex;

#include <condition_variable>
using std::condition_variable;
using std::unique_lock;

/*
 * A task is an active action intended to be performed by the robot.
 * Only one task can be executed at a time.
 */
class Task
{
public:

	/*
	 * States in which a task can be in, one at a time.
	 * CARDINALITY is not a state. It is merely used to obtain the number of the enumerations.
	 */
	enum State { INITIAL = 0, RUNNING, INTERRUPTED, ENDED, CARDINALITY };

	/*
	 * The interrupt method informs the task that it should pause immediately.
	 * One of the reasons why interrupt may be called is because another task
	 * with higher priority was issued.
	 * Every task should periodically check if it was interrupted, pause the
	 * current operations and optionally save the current state to resume later.
	 * The task will be resumed as soon as possible by calling the execute
	 * function again.
	 * The task should then read the previous state and continue executing.
	 */
	void interrupt() { interrupted = true; }

	bool is_interrupted() const { return interrupted; }

	/*
	 * The calling thread waits until the task reaches the given state.
	 * If the task reaches the ending state, all waiting threads are resumed.
	 */
	void wait_for_state(State state);

	State get_state();

	virtual ~Task() {}

protected:

	/*
	 * The execute method needs to be implemented by concrete task derived classes
	 * and should contain the logic of the task and direct instructions to the robot.
	 */
	virtual void execute() = 0;

	Task(): interrupted(false), state(INITIAL) {}

private:

	atomic<bool> interrupted;

	void set_state(State state);

	State state;
	mutex state_mutex;
	condition_variable state_conditions[State::CARDINALITY];

	friend class TaskExecuter;

};

#endif /* TASK_H_ */
