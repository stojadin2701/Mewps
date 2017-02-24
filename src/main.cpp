#include "infrastructure/peripherals/hardware/serial_comm.h"
#include "infrastructure/peripherals/Microphones.h"
using infrastructure::Microphones;

#include <iostream>
using std::cout;
using std::endl;

#include <exception>
using std::exception;

#include <thread>
#include <chrono>
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

#include <pthread.h>


#define NUM_THREADS 50


struct params {
	pthread_mutex_t mutex;
	pthread_cond_t done;
	int id;
};

typedef struct params params_t;

void* hello(void* arg){

	int id;
	/* Lock.  */
	pthread_mutex_lock(&(*(params_t*)(arg)).mutex);

	/* Work.  */
	id = (*(params_t*)(arg)).id;
	printf("Hello from %d\n", id);

	/* Unlock and signal completion.  */
	pthread_mutex_unlock(&(*(params_t*)(arg)).mutex);
	pthread_cond_signal (&(*(params_t*)(arg)).done);

	/* After signalling `main`, the thread could actually
	   go on to do more work in parallel.  */
}

int main()
{
	//sleep_for(milliseconds(2000));

	cout << "start" << endl;


	pthread_t threads[NUM_THREADS];

	params_t params;

	pthread_mutex_init (&params.mutex , NULL);
	pthread_cond_init (&params.done, NULL);

	/* Obtain a lock on the parameter.  */
	pthread_mutex_lock (&params.mutex);

	int i;
	for(i = 0; i < NUM_THREADS; i++) {

		/* Change the parameter (I own it).  */    
		params.id = i;

		/* Spawn a thread.  */
		pthread_create(&threads[i], NULL, hello, &params);

		/* Give up the lock, wait till thread is 'done',
		   then reacquire the lock.  */
		pthread_cond_wait (&params.done, &params.mutex);
	}


	for( i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	/*	try
		{
		for(int i=0; i<10000; i++){
		float f1, f2, f3;
		Microphones::get_intensities(&f1, &f2, &f3);
		cout << "Front: " << f1 << " Right: " << f2 << " Left: "<< f3 << endl;
		sleep_for(milliseconds(300));
		}


		}
		catch (exception &e)
		{
		cout << e.what() << endl;
		}
	 */



	/* Destroy all synchronization primitives.  */    
	pthread_mutex_destroy (&params.mutex);
	pthread_cond_destroy (&params.done);

	return 0;
}
