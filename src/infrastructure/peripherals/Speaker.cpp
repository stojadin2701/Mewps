/*
 * Motors.cpp
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#include "Speaker.h"

#include "CommProtocolThreadsafe.h"

#include "InvalidSpeakerValuesError.h"

namespace infrastructure
{

int16_t Speaker::power_left = 0;
int16_t Speaker::power_right = 0;

void Speaker::play_sound(int16_t frequency, int16_t duration)
{
    if(frequency<0 || duration < 0){
      throw InvalidSpeakerValuesError(frequency, duration);
    }

    CommProtocolThreadsafe::send_sound_data_ts(frequency, duration);
}

}  /* namespace infrastructure */
