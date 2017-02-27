#include "infrastructure/peripherals/hardware/serial_comm.h"
#include "infrastructure/peripherals/Microphones.h"
using infrastructure::Microphones;

#include "infrastructure/peripherals/DistanceSensor.h"
using infrastructure::DistanceSensor;

#include "infrastructure/peripherals/Motors.h"
using infrastructure::Motors;

#include <iostream>
using std::cout;
using std::endl;

#include <exception>
using std::exception;

#include <thread>
using std::thread;
#include <chrono>
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

#include <mutex>
#include <condition_variable>
using std::mutex;
using std::unique_lock;
using std::condition_variable;

mutex stop, complete;
condition_variable cv;

bool kill = false;//kill recoveryThread
bool ready = true; //ready to listen

enum Direction { FRONT, RIGHT, LEFT, FRONT_SHORT, NONE };

//std::atomic<Direction> sound_direction {NONE};
Direction sound_direction = NONE;

void distanceThread(){
	int16_t distance;
	for(int i=0; i<200; i++){
		distance = DistanceSensor::get_distance();
		cout << "Distance: " << distance << endl;

		if(distance<=20){
			Motors::set_powers(0,0);
			unique_lock<mutex> lock_stop(stop);
			kill = true; //kill recoveryThread if exists
			lock_stop.unlock();

			unique_lock<mutex> lock_complete(complete);
			ready = false;
			lock_complete.unlock();

			thread recThread(recoveryThread);
		}

		sleep_for(milliseconds(35));
	}
}

void recoveryThread(){
	bool check;
	cout<<"RECOVERING"<<endl;
	Motors::set_powers(-0.5,-0.5);
	sleep_for(milliseconds(1000));
	Motors::set_powers(0,0);
	sleep_for(milliseconds(300));
	Motors::set_powers(0.75, -0.8);
	sleep_for(milliseconds(1000));
	Motors::set_powers(0, 0);
	sleep_for(milliseconds(500));
	Motors::set_powers(0.5, 0.5);
	sleep_for(milliseconds(1000));
	//kill
	unique_lock<mutex> lock_stop(stop);
	check = kill;
	kill = false;
	lock_stop.unlock();
	if(check)std::terminate();

	Motors::set_powers(0,0);
	sleep_for(milliseconds(500));
	Motors::set_powers(-0.8, 0.65);
	sleep_for(milliseconds(1000));
	Motors::set_powers(0,0);
	cout<<"RECOVERY COMPLETE"<<endl;
	//set atomic bool
	unique_lock<mutex> lock_complete(complete);
	ready = true;
	lock_complete.unlock();
	cv.notify_one();
}


Direction calculate_direction( int16_t i1, int16_t i2, int16_t i3){
	Direction dir = NONE;
	int16_t eps = 2;
	int16_t threshold = 1;

	if(i1 < threshold && i2 < threshold && i3 < threshold) dir=NONE;
	else if((i1-eps)>= i2 && (i1-eps)>=i3) dir = FRONT;
	else if((i2-eps)>= i1 && (i2-eps)>=i3) dir = RIGHT;
	else if((i3-eps)>= i1 && (i3-eps)>=i2) dir = LEFT;
	else if((i2+eps)<= i1 && (i2+eps)<=i3) dir = FRONT_SHORT;
	else if((i3+eps)<= i1 && (i3+eps)<=i2) dir = FRONT_SHORT;

	return dir;
}

int main()
{
	sleep_for(milliseconds(2000));

	cout << "start" << endl;

	thread disThread(distanceThread);

	try
	{
		for(int k=0; k<10; k++){
			//for(int i=0; i<30; i++){
			int16_t f1, f2, f3;
			sleep_for(milliseconds(2000));

			unique_lock<mutex> lock_complete(complete);
			cv.wait(lock_complete, []{return ready;});
			lock_complete.unlock();
			cout<<"LISTENING"<<endl;


			Microphones::get_intensities(&f1, &f2, &f3);
			cout << "Front: " << f1 << " Right: " << f2 << " Left: "<< f3 << endl;
			sound_direction = calculate_direction(f1, f2, f3);
			switch(sound_direction){
				case FRONT:
					cout<<"FRONT"<<endl;
					Motors::set_powers(0.5,0.5);
					sleep_for(milliseconds(1500));
					Motors::set_powers(0,0);
					break;
				case RIGHT:
					cout<<"RIGHT"<<endl;

					Motors::set_powers(0.75, -0.8);
					sleep_for(milliseconds(1000));
					Motors::set_powers(0, 0);
					break;
				case LEFT:
					cout<<"LEFT"<<endl;
					Motors::set_powers(-0.8, 0.65);
					sleep_for(milliseconds(1000));
					Motors::set_powers(0,0);
					break;
				case FRONT_SHORT:
					cout<<"FRONT_SHORT"<<endl;
					Motors::set_powers(0.5, 0.5);
					sleep_for(milliseconds(800));
					Motors::set_powers(0, 0);
					break;
				case NONE:
					cout << "cry :\'(" << endl;
					sleep_for(milliseconds(500));
					break;
			}


		}

		}
		catch (exception &e)
		{
			cout << e.what() << endl;
		}

		disThread.join();

		return 0;
	}
