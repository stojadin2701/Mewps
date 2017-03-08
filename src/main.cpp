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

#define DISTANCE_THRESHOLD 20
#define LISTENING_NUM 10

atomic<bool> program_terminated(false);
//indicates whether both motors are going forward
atomic<bool> going_forward(false);
//ready to listen
atomic<bool> ready(true);
atomic<bool> listening_ended(false);

mutex stop, complete;
condition_variable cv;

//number of currently active recovery threads
atomic<int> recovery_counter(0);

bool kill = false;

enum Direction { FRONT, RIGHT, LEFT, FRONT_SHORT, NONE };

Direction sound_direction = NONE;

Direction calculate_direction(int16_t i1, int16_t i2, int16_t i3) {
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

void go_reverse(int16_t duration){
	cout<<"GOING BACKWARDS"<<endl;
	Motors::set_powers(-0.5,-0.5);
	sleep_for(milliseconds(duration));
	Motors::set_powers(0, 0);
}

void turn_right(int16_t duration){
	cout<<"TURNING RIGHT"<<endl;
	Motors::set_powers(0.75, -0.8);
	sleep_for(milliseconds(duration));
	Motors::set_powers(0, 0);
}

void turn_left(int16_t duration){
	cout<<"TURNING LEFT"<<endl;
	Motors::set_powers(-0.8, 0.65);
	sleep_for(milliseconds(duration));
	Motors::set_powers(0,0);
}

void go_forward(int16_t duration){
	going_forward.store(true);
	cout<<"GOING FORWARD"<<endl;
	Motors::set_powers(0.5, 0.5);
	sleep_for(milliseconds(duration));
	if(going_forward.load()){
		cout<<"SUCCESSFULLY FINISHED FORWARD MOVEMENT"<<endl;
		Motors::set_powers(0, 0);
	}
	going_forward.store(false);
}

void play_death_song(){
	Speaker::play_sound(1500, 1000);
	sleep_for(milliseconds(1200));
	Speaker::play_sound(1000, 1000);
	sleep_for(milliseconds(1200));
	Speaker::play_sound(300, 2000);
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
	unique_lock<mutex> lock_stop(stop);
	check = kill;
	if(!check)
		recovery_counter.store(0);
	kill = false;
	lock_stop.unlock();
	if(!check){
		Motors::set_powers(0,0);
		sleep_for(milliseconds(500));
		turn_left(1000);
		cout<<"RECOVERY COMPLETE"<<endl;
		ready.store(true);
		cv.notify_one();
	}
	else {
		cout << "RECOVERY KILLED" << endl;
	}
}

void distanceThread(){
	int16_t distance;
	while(!listening_ended.load() || (recovery_counter.load() > 0)){
		distance = DistanceSensor::get_distance();
		cout << "Distance: " << distance << endl;
		if(distance <= DISTANCE_THRESHOLD && going_forward.load()){
			cout << "OBSTACLE DETECTED" << endl;
			going_forward.store(false);
			Motors::set_powers(0,0);
			unique_lock<mutex> lock_stop(stop);
			//killing the previous recovery thread if an obstacle is detected while recovering
			int rec_num = recovery_counter.load();
			if(rec_num++ > 0){
					kill = true; //kill recoveryThread if exists
			}
			recovery_counter.store(rec_num);
			if(recovery_counter.load() > 4){
				  Motors::set_powers(0,0);
					cout<<"RUNNING IN CIRCLES!"<<endl;
					lock_stop.unlock();
					program_terminated.store(true);
					ready.store(true);
					cv.notify_one();
					break;
			}
			cout << "NUMBER OF RECOVERY THREADS: "<<recovery_counter.load()<< endl;
			lock_stop.unlock();
			ready.store(false);

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
		sleep_for(milliseconds(35));
	}
}

int main()
{
	sleep_for(milliseconds(2000));
	cout << "STARTING..." << endl;
	thread disThread(distanceThread);
	try
	{
		for(int k=0; k < LISTENING_NUM && !program_terminated.load(); k++){
			int16_t f1, f2, f3;
			sleep_for(milliseconds(2000));
			unique_lock<mutex> lock_complete(complete);
			cv.wait(lock_complete, []{return ready.load();});
			lock_complete.unlock();
			if(program_terminated.load()){
				cout<<"GOODBYE CRUEL WORLD!"<<endl;
				break;
			}
			cout<<"LISTENING"<<endl;
			Microphones::get_intensities(&f1, &f2, &f3);
			cout << "Front: " << f1 << " Right: " << f2 << " Left: "<< f3 << endl;
			sound_direction = calculate_direction(f1, f2, f3);
			switch(sound_direction){
				case FRONT:
					cout<<"HEARING FRONT"<<endl;
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
					cout<<"HEARING RIGHT"<<endl;
					/*
					Speaker::play_sound(1000, 250);
					sleep_for(milliseconds(280));
					Speaker::play_sound(1000, 250);
					*/
					turn_right(1000);
					break;
				case LEFT:
					cout<<"HEARING LEFT"<<endl;
					/*
					Speaker::play_sound(500, 500);
					*/
					turn_left(1000);
					break;
				case FRONT_SHORT:
					cout<<"HEARING FRONT_SHORT"<<endl;
					go_forward(800);
					break;
				case NONE:
					cout << "DIRECTION NOT DETERMINED :\'(" << endl;
					Speaker::play_sound(300, 1500);
					sleep_for(milliseconds(500));
					break;
			}
		}
		listening_ended.store(true);
		disThread.join();
		Motors::set_powers(0,0);
		play_death_song();
		cout<<"SHUTTING DOWN..."<<endl;
		}
		catch (exception &e)
		{
			cout << e.what() << endl;
		}
		return 0;
	}
