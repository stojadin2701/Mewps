#ifndef INFRASTRUCTURE_PERIPHERALS_SPEAKER_H_
#define INFRASTRUCTURE_PERIPHERALS_SPEAKER_H_

#include <cstdint>

namespace infrastructure
{

class Speaker
{

public:

    static void play_sound(int16_t frequency, int16_t duration);

private:

    Speaker() = delete;

};

}  /* namespace infrastructure */

#endif /* INFRASTRUCTURE_PERIPHERALS_SPEAKER_H_ */
