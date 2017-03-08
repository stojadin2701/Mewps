#include "Speaker.h"

#include "CommProtocolThreadsafe.h"

#include "InvalidSpeakerValuesError.h"

namespace infrastructure
{

void Speaker::play_sound(int16_t frequency, int16_t duration)
{
    if(frequency < 0 || duration < 0){
      throw InvalidSpeakerValuesError(frequency, duration);
    }

    CommProtocolThreadsafe::send_sound_data_ts(frequency, duration);
}

}  /* namespace infrastructure */
