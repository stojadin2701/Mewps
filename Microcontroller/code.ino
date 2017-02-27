#include "../src/infrastructure/peripherals/hardware/serial_comm.h"
#include "../src/infrastructure/peripherals/comm_protocol.h"

#define DISTANCE_SAMPLING_NUM 5
#define MIC_SAMPLING_NUM 3

// Motor enable pins
const unsigned ENABLE_PINS[] = { 4, 8 };
// Left and right motor pins respectively
const unsigned MOTOR_PINS[] = { 5, 6, 9, 10 };
// Front, right, and left microphone input pins, respectively
const unsigned MIC_PINS[] = {A0, A1, A2};
// Sample window width in mS (50 mS = 20Hz)
const unsigned sample_window = 50;

//Trigger and Echo pins for the distance sensor
const unsigned trig_pin = 2;
const unsigned echo_pin = A5;

//Speaker output pin
const unsigned SPEAKER_PIN = 3;

long microsecondsToCentimeters(long microseconds){
	return microseconds/29/2;
}


void serial_comm_send(const uint16_t msg)
{
	uint16_t network_msg = ((msg << 8) & 0xff00) | ((msg >> 8) & 0x00ff);
	unsigned bytes;
	uint8_t checksum, response;

	unsigned i;
	for(i = 0; i < ATTEMPTS_BEFORE_ABORT; i++)
	{
		bytes = 0;
		while (bytes < sizeof(network_msg))
		{
			bytes += Serial.write((const uint8_t*)&network_msg + bytes, sizeof(network_msg) - bytes);
		}

		checksum = (network_msg & 0x00ff) + (network_msg >> 8 & 0x00ff);
		Serial.write(&checksum, sizeof(checksum));

		while (Serial.available() == 0);
		response = Serial.read();

		if (fabs((int)response - ACKNOWLEDGE) < fabs((int)response - NEGATIVE_ACKNOWLEDGE))
		{
			break;
		}
	}
}

uint16_t serial_comm_receive()
{
	uint16_t network_msg;
	uint8_t *byte, checksum;

	unsigned i, j;
	for(i = 0; i < ATTEMPTS_BEFORE_ABORT; i++)
	{
		byte = (uint8_t*)&network_msg;
		for (j = 0; j < sizeof(network_msg); j++)
		{
			while (Serial.available() == 0);

			*byte = Serial.read();
			byte++;
		}

		while (Serial.available() == 0);
		checksum = Serial.read();

		if ((uint8_t)((network_msg & 0x00ff) + (network_msg >> 8 & 0x00ff)) == checksum)
		{
			Serial.write((const uint8_t*)&ACKNOWLEDGE, sizeof(ACKNOWLEDGE));

			break;
		}
		else
		{
			Serial.write((const uint8_t*)&NEGATIVE_ACKNOWLEDGE, sizeof(NEGATIVE_ACKNOWLEDGE));
		}
	}

	return ((network_msg << 8) & 0xff00) | ((network_msg >> 8) & 0x00ff);
}

inline void read_mic(unsigned int mic, int16_t *value){
	unsigned long start_millis= millis();  // Start of sample window
	unsigned int peak_to_peak = 0;   // peak-to-peak level
	unsigned int sample = 0;
	unsigned int signal_max = 0;
	unsigned int signal_min = 1024;

	// collect data for 50 mS
	while (millis() - start_millis < sample_window)
	{
		sample = analogRead(mic);
		if (sample < 1024)  // toss out spurious readings
		{
			if (sample > signal_max)
			{
				signal_max = sample;  // save just the max levels
			}
			else if (sample < signal_min)
			{
				signal_min = sample;  // save just the min levels
			}
		}
	}
	peak_to_peak = signal_max - signal_min;  // max - min = peak-peak amplitude
	//double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
	//Serial.println(volts);
	*value = peak_to_peak;
}

inline void read_microphone_data(int16_t *intensity1, int16_t *intensity2, int16_t *intensity3)
{
	int16_t i1_sum=0, i2_sum=0, i3_sum=0;
	int16_t i1, i2, i3;
	for(int i=0; i<MIC_SAMPLING_NUM; i++){
		read_mic(A0, &i1);
		read_mic(A1, &i2);
		read_mic(A2, &i3);
		i1_sum+=i1;
		i2_sum+=i2;
		i3_sum+=i3;
	}
	*intensity1 = i1_sum/MIC_SAMPLING_NUM;
	*intensity2 = i2_sum/MIC_SAMPLING_NUM;
	*intensity3 = i3_sum/MIC_SAMPLING_NUM;
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
	long duration, cm;
	long sum=0;
	int16_t avg=0;

	for(int i=0; i<DISTANCE_SAMPLING_NUM; i++){
		pinMode(trig_pin, OUTPUT);
		digitalWrite(trig_pin, LOW);
		delayMicroseconds(2);
		digitalWrite(trig_pin, HIGH);
		delayMicroseconds(10);

		digitalWrite(trig_pin, LOW);

		pinMode(echo_pin, INPUT);
		duration=pulseIn(echo_pin, HIGH);
		cm=microsecondsToCentimeters(duration);
		sum+=cm;
		delay(35);
	}
	avg = sum/DISTANCE_SAMPLING_NUM;
	*distance = avg;
}

inline void read_accelerometer_data(int16_t *ax, int16_t *ay, int16_t *az)
{
	*ax = 0;
	*ay = 0;
	*az = 0;
}

inline void read_power_status(int16_t *status)
{
	*status = 0;
}

void setup()
{
	unsigned pin;

	for (pin = 0; pin < sizeof(ENABLE_PINS) / sizeof(unsigned); pin++)
	{
		pinMode(ENABLE_PINS[pin], OUTPUT);
		digitalWrite(ENABLE_PINS[pin], HIGH);
	}

	for (pin = 0; pin < sizeof(MOTOR_PINS) / sizeof(unsigned); pin++)
	{
		pinMode(MOTOR_PINS[pin], OUTPUT);
	}

	Serial.begin(BAUD_RATE);
}

inline void play_sound(int16_t frequency, int16_t duration, bool freeze_while_playing){
	tone(SPEAKER_PIN, frequency, duration);
	if(freeze_while_playing) delay(1+duration);
}

void loop()
{
	int16_t preamble = receive_preamble();
	switch (preamble)
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
			case PLAY_SOUND:
				{
					int16_t frequency;
					int16_t duration;

					receive_sound_data(&frequency, &duration);
					play_sound(frequency, duration, false);

					break;
				}
		case ACCELEROMETER_REQUEST:
			{
				int16_t ax;
				int16_t ay;
				int16_t az;

				read_accelerometer_data(&ax, &ay, &az);
				send_accelerometer_data(ax, ay, az);

				break;
			}
		case POWER_STATUS_REQUEST:
			{
				int16_t status;

				read_power_status(&status);
				send_power_status(status);

				break;
			}
	}
}
