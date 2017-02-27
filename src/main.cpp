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
using std::mutex;
using std::unique_lock;

mutex recover;

bool stop = false;

void distanceThread(){
	int16_t distance;
	for(int i=0; i<200; i++){
		distance = DistanceSensor::get_distance();
		cout << "Distance: " << distance << endl;
		if(distance<=20){
			unique_lock<mutex> lock(recover);
			stop = true;
			lock.unlock();
			Motors::set_powers(-0.5,-0.5);
			sleep_for(milliseconds(1000));
			Motors::set_powers(0,0);
			//	break;
		}

		sleep_for(milliseconds(35));
	}
}

void recoveryThread(){
	cout<<"RECOVERING"<<endl;
	sleep_for(milliseconds(300));
	Motors::set_powers(0.75, -0.8);
	sleep_for(milliseconds(1000));
	Motors::set_powers(0, 0);
	sleep_for(milliseconds(500));
	Motors::set_powers(0.5, 0.5);
	sleep_for(milliseconds(1000));
	Motors::set_powers(0,0);
	sleep_for(milliseconds(500));
	Motors::set_powers(-0.8, 0.65);
	sleep_for(milliseconds(1000));
	Motors::set_powers(0,0);
}

//enum Direction { FRONT, RIGHT, LEFT, FRONT_RIGHT, FRONT_LEFT,  NONE };
enum Direction { FRONT, RIGHT, LEFT, FRONT_SHORT, NONE };

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
			Microphones::get_intensities(&f1, &f2, &f3);
			cout << "Front: " << f1 << " Right: " << f2 << " Left: "<< f3 << endl;
			Direction dir = calculate_direction(f1, f2, f3);
			switch(dir){
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

			unique_lock<mutex> lock(recover);
			if(stop){
				stop = false;
				lock.unlock();
				cout<<"RECOVERING"<<endl;
				sleep_for(milliseconds(300));
				Motors::set_powers(0.75, -0.8);
				sleep_for(milliseconds(1000));
				Motors::set_powers(0, 0);
				sleep_for(milliseconds(500));
				Motors::set_powers(0.5, 0.5);
				sleep_for(milliseconds(1000));
				Motors::set_powers(0,0);
				sleep_for(milliseconds(500));
				Motors::set_powers(-0.8, 0.65);
				sleep_for(milliseconds(1000));
				Motors::set_powers(0,0);
				lock.lock();
				stop = false;
				lock.unlock();

			}else{
				lock.unlock();
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
