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

bool stop = false;

void distanceThread(){
	int16_t distance;
	for(int i=0; i<300; i++){
		distance = DistanceSensor::get_distance();
		cout << "Distance: " << distance << endl;
		if(distance<=20){
			stop=true;
			Motors::set_powers(-0.5,-0.5);
			sleep_for(milliseconds(1000));
			Motors::set_powers(0,0);
			break;
		}

		sleep_for(milliseconds(35));
	}


}

int main()
{
	sleep_for(milliseconds(2000));

	cout << "start" << endl;

	thread disThread(distanceThread);

		try
		{
			for(int k=0; k<5; k++){
			//for(int i=0; i<30; i++){
				int16_t f1, f2, f3;
				Microphones::get_intensities(&f1, &f2, &f3);
				cout << "Front: " << f1 << " Right: " << f2 << " Left: "<< f3 << endl;
				if(f1>f2 && f1>f3){
					Motors::set_powers(0.5,0.5);
					sleep_for(milliseond(3000));
					Motors::set_powers(0,0);
				}
				else if(f2>f1 && f2>f3){
					Motors::set_powers(0.75, -0.8);
					sleep_for(milliseconds(1000));
					Motors::set_powers(0, 0);
				}
				else if(f3>f1 && f3>f2){
					Motors::set_powers(-0.8, 0.65);
					sleep_for(milliseconds(1000));
					Motors::set_powers(0,0);
				}
				//sleep_for(milliseconds(200));
				//}

			if(stop){
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
		}

		}
		catch (exception &e)
		{
		cout << e.what() << endl;
		}

		disThread.join();

	return 0;
}
