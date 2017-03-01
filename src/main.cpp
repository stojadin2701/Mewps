#include "infrastructure/peripherals/hardware/serial_comm.h"
#include "infrastructure/peripherals/Microphones.h"
using infrastructure::Microphones;

#include "infrastructure/peripherals/DistanceSensor.h"
using infrastructure::DistanceSensor;

#include "infrastructure/peripherals/Motors.h"
using infrastructure::Motors;

#include "infrastructure/peripherals/Speaker.h"
using infrastructure::Speaker;

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
#include <atomic>
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::atomic;

atomic<bool> program_terminated(false);

mutex stop, complete, forward;
condition_variable cv;

int recovery_counter = 0;

bool kill = false; //kill recoveryThread
bool going_forward = false;
bool ready = true; //ready to listen

enum Direction { FRONT, RIGHT, LEFT, FRONT_SHORT, NONE };

//std::atomic<Direction> sound_direction {NONE};
Direction sound_direction = NONE;

void go_reverse(int16_t duration){
	Motors::set_powers(-0.5,-0.5);
	sleep_for(milliseconds(duration));
	Motors::set_powers(0, 0);
}

void turn_right(int16_t duration){
	Motors::set_powers(0.75, -0.8);
	sleep_for(milliseconds(duration));
	Motors::set_powers(0, 0);
}

void turn_left(int16_t duration){
	Motors::set_powers(-0.8, 0.65);
	sleep_for(milliseconds(duration));
	Motors::set_powers(0,0);
}

void go_forward(int16_t duration){
	unique_lock<mutex> lock_forward(forward);
	going_forward = true;
	lock_forward.unlock();
	Motors::set_powers(0.5, 0.5);
	sleep_for(milliseconds(duration));
	Motors::set_powers(0, 0);
	lock_forward.lock();
	going_forward = false;
	lock_forward.unlock();
}

void recoveryThread(){
	bool check;
	cout<<"RECOVERING"<<endl;
	go_reverse(1000);
	sleep_for(milliseconds(300));
	turn_right(1000);
	sleep_for(milliseconds(500));
	go_forward(1000);
	sleep_for(milliseconds(500));
	//kill
	unique_lock<mutex> lock_stop(stop);
	check = kill;
	if(!check)
		recovery_counter = 0;
	kill = false;
	lock_stop.unlock();
	if(!check){
		Motors::set_powers(0,0);
		sleep_for(milliseconds(500));
		turn_left(1000);
		cout<<"RECOVERY COMPLETE"<<endl;
		//set atomic bool
		unique_lock<mutex> lock_complete(complete);
		ready = true;
		lock_complete.unlock();
		cv.notify_one();
	}
	else {
		cout << "RECOVERY KILLED" << endl;
	}
}



void distanceThread(){
	int16_t distance;
	for(int i=0; i<200; i++){
		distance = DistanceSensor::get_distance();
		cout << "Distance: " << distance << endl;
		unique_lock<mutex> lock_forward(forward);
		if(distance<=20 && going_forward == true){
			cout << "OBSTACLE DETECTED" << endl;
			going_forward = false;
			lock_forward.unlock();
			Motors::set_powers(0,0);
			unique_lock<mutex> lock_stop(stop);
			if(recovery_counter++ > 0)
					kill = true; //kill recoveryThread if exists
			if(recovery_counter > 4 ){
				  Motors::set_powers(0,0);
					cout<<"RUNNING IN CIRCLES!"<<endl;
					lock_stop.unlock();
					program_terminated.store(true);
					break;
			}
			cout << "NUMBER OF RECOVERY THREADS: "<<recovery_counter<< endl;
			lock_stop.unlock();

			unique_lock<mutex> lock_complete(complete);
			ready = false;
			lock_complete.unlock();

			thread recThread(recoveryThread);

			Speaker::play_sound(4000, 50);
			sleep_for(milliseconds(70));
			Speaker::play_sound(4000, 50);
			sleep_for(milliseconds(70));
			Speaker::play_sound(4000, 50);
			sleep_for(milliseconds(70));
			Speaker::play_sound(4000, 50);


			recThread.detach();

		}
		else{
			lock_forward.unlock();
		}
		sleep_for(milliseconds(35));
	}
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
		for(int k=0; k<10 && !program_terminated.load(); k++){
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
					/*
					Speaker::play_sound(2000, 100);
					sleep_for(milliseconds(140));
					Speaker::play_sound(2000, 100);
					sleep_for(milliseconds(140));
					Speaker::play_sound(2000, 100);
					*/
					go_forward(1500);
					break;
				case RIGHT:
					cout<<"RIGHT"<<endl;
					/*
					Speaker::play_sound(1000, 250);
					sleep_for(milliseconds(280));
					Speaker::play_sound(1000, 250);
					*/
					turn_right(1000);
					break;
				case LEFT:
					cout<<"LEFT"<<endl;
					/*
					Speaker::play_sound(500, 500);
					*/
					turn_left(1000);
					break;
				case FRONT_SHORT:
					cout<<"FRONT_SHORT"<<endl;
					go_forward(800);
					break;
				case NONE:
					cout << "cry :\'(" << endl;
					Speaker::play_sound(300, 1500);
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
		Motors::set_powers(0,0);
		return 0;
	}
