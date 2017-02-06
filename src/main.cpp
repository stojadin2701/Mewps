/*
 * main.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "infrastructure/peripherals/hardware/serial_comm.h"
#include "infrastructure/peripherals/hardware/Microphones.h"
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

int main()
{
	sleep_for(milliseconds(2000));

	cout << "start" << endl;

	try
	{
		for(int i=0; i<10; i++){
				float f1, f2, f3;
				get_intensities(&f1, &f2, &f3);
				cout << "Front: " << f1 << "Right: " << f2 << "Left: "<< f3 << endl;
				sleep_for(milliseconds(3000));
		}


	/*int16_t msg, response;
		while (true)
		{
            msg = -7;

            serial_comm_send(msg);

            response = serial_comm_receive();

            cout << response << endl;
		}*/
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}

    return 0;
}
