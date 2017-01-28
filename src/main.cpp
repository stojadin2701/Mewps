/*
 * main.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

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

int main()
{
	sleep_for(milliseconds(2000));

	cout << "start" << endl;

	try
	{
		float a, b, c;
		while (true)
		{
            Microphones::get_intensities(&a, &b, &c);

            cout << a << " " << b << " " << c << endl;
		}
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}

    return 0;
}


