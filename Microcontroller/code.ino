#include "../src/serial_comm.h"

const int MOTOR_PINS[] = { 5, 6, 7, 8 };

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

inline void read_microphone_data(int16_t *intensity1, int16_t *intensity2, int16_t *intensity3)
{
	*intensity1 = 0;
	*intensity2 = 0;
	*intensity3 = 0;
}

inine void set_motor_power(int16_t motor, int16_t power)
{
	if (power < -255 || power > 255) return;
	if (motor < 0 || motor > 1) return;

	int pin1 = MOTOR_PINS[motor * 2];
	int pin2 = MOTOR_PINS[motor * 2 + 1];
	
	if (power < 0)
	{
		int temp = pin1;
		pin1 = pin2;
		pin2 = temp;
		
		power *= -1;
	}
	
	analogWrite(pin1, power);
	analogWrite(pin2, 0);
}

inline void read_distance(int16_t *distance)
{
	*distance = 0;
}

void setup()
{
	int pin;
	for(pin = 0; pin < sizeof(MOTOR_PINS) / sizeof(int); pin++)
	{
		pinMode(pin, OUTPUT);
	}

	serial_comm_initialize();
}

void loop()
{
	int16_t preamble = receive_preamble();
	switch(preamble)
	{
		case MICROPHONE_REQUEST:
		{
			int16_t intensity1;
			int16_t intensity2;
			int16_t intensity3;
		
			read_microphone_data(&intensity1, &intensity2, &intensity3);
			send_microphone_data(intensity1, intensity2, intensity3);
			
			break;		
		}		
		case MOTOR_COMMAND:
		{
			int16_t motor;
			int16_t power;
		
			receive_motor_command(&motor, &power);
			set_motor_power(motor, power);
			
			break;
		}
		case DISTANCE_REQUEST:
		{
			int16_t distance;
		
			read_distance(&distance);
			send_distance(distance);
			
			break;
		}
	}
}
