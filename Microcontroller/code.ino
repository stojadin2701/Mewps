#include "../src/serial_comm.h"
#include "../src/comm_protocol.h"

// Motor enable pins
const unsigned ENABLE_PINS[] = { 4, 8 };
// Left and right motor pins respectively
const unsigned MOTOR_PINS[] = { 5, 6, 9, 10 };

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
	Serial.write((const uint8_t*)&network_msg, sizeof(network_msg));
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

inline void set_motors_power(int16_t power_left, int16_t power_right)
{
	if (power_left < -255 || power_left > 255) return;
	if (power_right < -255 || power_right > 255) return;
	
	int16_t powers[] = { power_left, 0, power_right, 0 };
	
	if (power_left < 0)
	{
		powers[0] = 0;
		powers[1] = -power_left;
	}
	
	if (power_right < 0)
	{
		powers[2] = 0;
		powers[3] = -power_right;
	}
	
	unsigned i;
	for(i = 0; i < sizeof(MOTOR_PINS) / sizeof(unsigned); i++)
	{
		analogWrite(MOTOR_PINS[i], powers[i]);
	}
}

inline void read_distance(int16_t *distance)
{
	*distance = 0;
}

void setup()
{
	unsigned pin;

	for(pin = 0; pin < sizeof(ENABLE_PINS) / sizeof(unsigned); pin++)
	{
		pinMode(ENABLE_PINS[pin], OUTPUT);
		digitalWrite(ENABLE_PINS[pin], HIGH);
	}

	for(pin = 0; pin < sizeof(MOTOR_PINS) / sizeof(unsigned); pin++)
	{
		pinMode(MOTOR_PINS[pin], OUTPUT);
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
		case MOTORS_COMMAND:
		{
			int16_t power_left;
			int16_t power_right;
		
			receive_motors_command(&power_left, &power_right);
			set_motors_power(power_left, power_right);
			
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
