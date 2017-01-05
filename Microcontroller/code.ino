#include "../src/serial_comm.h"
#include "../src/comm_protocol.h"

const unsigned MOTOR_PINS[] = { 5, 6, 7, 8 };

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

inline void set_motors_power(int16_t power1, int16_t power2)
{
	if (power1 < -255 || power1 > 255) return;
	if (power2 < -255 || power2 > 255) return;
	
	int16_t powers[] = { power1, 0, power2, 0 };
	
	if (power1 < 0)
	{
		powers[0] = 0;
		powers[1] = -power1;
	}
	
	if (power2 < 0)
	{
		powers[2] = 0;
		powers[3] = -power2;
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
		case MOTORS_COMMAND:
		{
			int16_t power1;
			int16_t power2;
		
			receive_motors_command(&power1, &power2);
			set_motors_power(power1, power2);
			
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
