#include "../src/serial_comm.h"

void serial_comm_initialize()
{
	Serial.begin(BAUD_RATE);
}

void serial_comm_finalize()
{
	Serial.end();
}

void serial_comm_send(const int16_t msg)
{
	int16_t network_msg = ((msg << 8) & 0xff00) | ((msg >> 8) & 0x00ff);
	Serial.write((const char*)&network_msg, sizeof(network_msg));
}

int16_t serial_comm_receive()
{
	int16_t msg;
	char* byte = ((char*)&msg) + 1;
	for(char i = 0; i < 2; i++)
	{
		while (Serial.available() == 0);
		
		*byte = Serial.read();
		byte--;
	}
	return msg;
}

int16_t preamble;
int16_t motor;
int16_t power;
int16_t intensity1;
int16_t intensity2;
int16_t intensity3;
int16_t distance;

void setup() {
	serial_comm_initialize();
}

void loop()
{
	preamble = receive_preamble();
	switch(preamble)
	{
		case MICROPHONE_REQUEST:
		{
			/*
				Get microphone data
			*/
			
			send_microphone_data(intensity1, intensity2, intensity3);
			
			break;		
		}		
		case MOTOR_COMMAND:
		{
			receive_motor_command(&motor, &power);
			
			/*
				Set corresponding motor power
			*/
			
			break;
		}
		case DISTANCE_REQUEST:
		{
			/*
				Read distance sensor
			*/
			
			send_distance(distance);
			
			break;
		}
	}
}
