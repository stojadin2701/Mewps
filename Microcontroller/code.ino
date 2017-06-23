#include <math.h>
#include <limits.h>
#include "../src/infrastructure/peripherals/hardware/serial_comm.h"
#include "../src/infrastructure/peripherals/comm_protocol.h"

#define DISTANCE_SAMPLING_NUM 3

#define MIC_SAMPLING_NUM 3
//Sample window width in mS
#define SAMPLING_WINDOW 50

//MAG
#define MAG_ADDR  0x0E

//Motor enable pins
const unsigned ENABLE_PINS[] = { 4, 8 };
//Left and right motor pins respectively
const unsigned MOTOR_PINS[] = { 5, 6, 9, 10 };
//Front, right, and left microphone input pins, respectively
const unsigned MIC_PINS[] = {A0, A1, A2};

//Trigger and Echo pins for the distance sensor
const unsigned TRIG_PIN = 2;
const unsigned ECHO_PIN = A5;

const unsigned RED_PIN = 7;
const unsigned YELLOW_PIN = 11;
const unsigned GREEN_PIN = 12;

//Speaker output pin
const unsigned SPEAKER_PIN = 3;


const int MICROPHONE_COUNT = 3;
const float MICROPHONE_DISTANCE = 0.205;
const float SPEED_OF_SOUND = 340.29 / 1000.0 / 1000.0;

struct Microphone
{
  int id;
  unsigned long delay;
  float delay_distance;
};


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
	//Start of the sampling window
	unsigned long start_millis = millis();
	unsigned int peak_to_peak = 0;
	unsigned int sample = 0;
	unsigned int signal_max = 0;
	unsigned int signal_min = 1024;

	//Collect data during SAMPLING_WINDOW
	while (millis() - start_millis < SAMPLING_WINDOW)	{
		sample = analogRead(mic);
		//Throw away invalid readings
		if (sample < 1024){
			if (sample > signal_max){
				signal_max = sample;
			}
			else if (sample < signal_min){
				signal_min = sample;
			}
		}
	}

	peak_to_peak = signal_max - signal_min;
	*value = peak_to_peak;
}

inline void read_microphone_data(int16_t *intensity1, int16_t *intensity2, int16_t *intensity3){
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

float determine_angle(unsigned long* delays)
{
  int main_id;
  int i;
  Microphone microphones[MICROPHONE_COUNT];

  for(i = 0; i < MICROPHONE_COUNT; i++)
  {
    if (delays[i] == 0)
    {
      main_id = i;
      break;
    }
  }

  for(i = 0; i < MICROPHONE_COUNT; i++)
  {
    microphones[i].id = (main_id + i) % MICROPHONE_COUNT;
    microphones[i].delay = delays[microphones[i].id];
    microphones[i].delay_distance = microphones[i].delay * SPEED_OF_SOUND;
  }

  float alpha = acos(microphones[1].delay_distance / MICROPHONE_DISTANCE);

  float sign = (microphones[1].delay > microphones[2].delay) ? -1 : 1;
  float angle = M_PI / 3.0 + sign * alpha;
  float global_angle = -(M_PI / 2.0 - angle) + 2 * M_PI / 3.0 * main_id;
  while (global_angle <= -M_PI) global_angle += M_PI * 2;
  while (global_angle > M_PI) global_angle -= M_PI * 2;

  return global_angle / M_PI * 180.0;
}

inline void read_microphone_turn_angle(int16_t *turn_angle) {
  *turn_angle = INT_MAX;

	byte m0, m1, m2;
  unsigned long t0, t1, t2;
  unsigned long t_micros;

  unsigned long initial_time;
  float angles[3];
	int loop = 0;
	while(loop < 3) {
		digitalWrite(RED_PIN, HIGH);
		digitalWrite(YELLOW_PIN, HIGH);
		digitalWrite(GREEN_PIN, HIGH);
	  for(int i = 0; i < 3; i++)
	  {
	      m0 = 0; m1 = 0; m2 = 0;
	      while(!(m0 & m1 & m2))
	      {
	        t_micros = micros();

	        if (!m0 && (m0 = digitalRead(A0))) { t0 = t_micros; }
	        if (!m1 && (m1 = digitalRead(A1))) { t1 = t_micros; }
	        if (!m2 && (m2 = digitalRead(A2))) { t2 = t_micros; }
	      }

	      initial_time = t0;
	      if (t1 < initial_time) initial_time = t1;
	      if (t2 < initial_time) initial_time = t2;

	      t0 -= initial_time;
	      t1 -= initial_time;
	      t2 -= initial_time;

	      unsigned long delays[3] = {t0, t1, t2};
	      float turn_angle = determine_angle(delays);

	      if (isnan(turn_angle))
	      {
	        i--;
	      }
	      else
	      {
	        angles[i] = turn_angle;

					switch (i)
					{
						case 0: digitalWrite(RED_PIN, LOW); break;
						case 1: digitalWrite(YELLOW_PIN, LOW); break;
						case 2: digitalWrite(GREEN_PIN, LOW); break;
					}
	      }

	      delay(200);
	  }

	  float diffs[3];

	  diffs[0] = fabs(angles[0] - angles[1]);
	  diffs[1] = fabs(angles[1] - angles[2]);
	  diffs[2] = fabs(angles[2] - angles[0]);

	  float sum = 0;
	  int count = 0;
	  for(int i = 0; i < 3; i++) {
	    if (diffs[i] <= 45.0) {
	      sum += angles[i] + angles[(i + 1) % 3];
	      count += 2;
	    }
	  }

	  if (count >= 2) {
	     sum /= count;
			 int sign = (sum>=0?1:-1);
			 sum = fabs(sum);
	     loop = 3;
			 *turn_angle = (sign)*(((int)ceil(sum/22.5))/2)*45;
			 digitalWrite(RED_PIN, HIGH);
	 		 digitalWrite(YELLOW_PIN, HIGH);
	 		 digitalWrite(GREEN_PIN, HIGH);
			 delay(200);
			 digitalWrite(RED_PIN, LOW);
	 		 digitalWrite(YELLOW_PIN, LOW);
	 		 digitalWrite(GREEN_PIN, LOW);
			 delay(200);
			 digitalWrite(RED_PIN, HIGH);
	 		 digitalWrite(YELLOW_PIN, HIGH);
	 		 digitalWrite(GREEN_PIN, HIGH);
			 delay(200);
	  }
	  else {
			loop ++;
	  }
		digitalWrite(RED_PIN, LOW);
		digitalWrite(YELLOW_PIN, LOW);
		digitalWrite(GREEN_PIN, LOW);
  }
}

inline void set_motors_power(int16_t power_left, int16_t power_right){
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

inline void read_distance(int16_t *distance){
	long duration, cm;
	long sum = 0;
	int16_t avg = 0;

	for(int i=0; i<DISTANCE_SAMPLING_NUM; i++){
		pinMode(TRIG_PIN, OUTPUT);
		digitalWrite(TRIG_PIN, LOW);
		delayMicroseconds(2);
		digitalWrite(TRIG_PIN, HIGH);
		delayMicroseconds(10);

		digitalWrite(TRIG_PIN, LOW);

		pinMode(ECHO_PIN, INPUT);
		duration=pulseIn(ECHO_PIN, HIGH);
		cm=microsecondsToCentimeters(duration);
		sum+=cm;
		delay(35);
	}
	avg = sum/DISTANCE_SAMPLING_NUM;
	*distance = avg;
}

inline void play_sound(int16_t frequency, int16_t duration, bool freeze_while_playing){
	tone(SPEAKER_PIN, frequency, duration);
	if(freeze_while_playing) delay(1+duration);
}


inline void read_accelerometer_data(int16_t *ax, int16_t *ay, int16_t *az){
	*ax = 0;
	*ay = 0;
	*az = 0;
}

inline void read_power_status(int16_t *status){
	*status = 0;
}

inline void read_magnetometer_data(int16_t *mx, int16_t *my, int16_t *mz){
	*mx = read16Data(0x01,0x02);
	*my = read16Data(0x03,0x04);
	*mz = read16Data(0x05,0x06);  
}

int16_t read16Data(byte MSB, byte LSB)
{
  int16_t xl, xh;  
  
  Wire.beginTransmission(MAG_ADDR); 
  Wire.write(MSB);              
  Wire.endTransmission();       
 
  delayMicroseconds(2); 
  
  Wire.requestFrom(MAG_ADDR, 1); 
  while(Wire.available()){ 
    xh = Wire.read(); 
  }
  
  delayMicroseconds(2); 
  
  Wire.beginTransmission(MAG_ADDR); 
  Wire.write(LSB);              
  Wire.endTransmission();       
 
  delayMicroseconds(2); 
  
  Wire.requestFrom(MAG_ADDR, 1); 
  while(Wire.available()){ 
    xl = Wire.read();
  }
  
  int out = (xl|(xh << 8));
  if (out & 0b1000000000000000){
    return float ((~out & 0b0111111111111111)+ 1)*(-1) ;
  }
  return float (out);
}

void setup(){
	unsigned pin;

	for (pin = 0; pin < sizeof(ENABLE_PINS) / sizeof(unsigned); pin++){
		pinMode(ENABLE_PINS[pin], OUTPUT);
		digitalWrite(ENABLE_PINS[pin], HIGH);
	}

	for (pin = 0; pin < sizeof(MOTOR_PINS) / sizeof(unsigned); pin++){
		pinMode(MOTOR_PINS[pin], OUTPUT);
	}

	pinMode(RED_PIN, OUTPUT);
	pinMode(YELLOW_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);

	digitalWrite(RED_PIN, LOW);
	digitalWrite(YELLOW_PIN, LOW);
	digitalWrite(GREEN_PIN, LOW);

	Wire.beginTransmission(MAG_ADDR);
    Wire.write(0x11);              
    Wire.write(0x80);              
    Wire.endTransmission();       
  
    delay(15);
  
    Wire.beginTransmission(MAG_ADDR);
    Wire.write(0x10);              
    Wire.write(1);                 
    Wire.endTransmission();

	Serial.begin(BAUD_RATE);
}

void loop()
{
	int16_t preamble = receive_preamble();
	switch (preamble)
	{
		case MICROPHONE_REQUEST:
			{
				// int16_t intensity1;
				// int16_t intensity2;
				// int16_t intensity3;
				//
				// read_microphone_data(&intensity1, &intensity2, &intensity3);
				// send_microphone_data(intensity1, intensity2, intensity3);

				int16_t turn_angle;
				read_microphone_turn_angle(&turn_angle);
				send_microphone_turn_angle(turn_angle);

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
		case MAGNETOMETER_REQUEST:
			{
				int16_t mx;
				int16_t my;
				int16_t mz;

				read_magnetometer_data(&mx, &my, &mz);
				send_magnetometer_data(mx, my, mz);

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
