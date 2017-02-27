#ifndef INFRASTRUCTURE_PERIPHERALS_INVALIDSPEAKERVALUESERROR_H_
#define INFRASTRUCTURE_PERIPHERALS_INVALIDSPEAKERVALUESERROR_H_

#include <stdexcept>
using std::runtime_error;

namespace infrastructure
{

class InvalidSpeakerValuesError: public runtime_error
{

public:

    InvalidSpeakerValuesError(int16_t frequency, int16_t duration):
    	runtime_error("Speaker is given invalid values."){}

};

} /* namespace infrastructure */

#endif /* INFRASTRUCTURE_PERIPHERALS_INVALIDSPEAKERVALUESERROR_H_ */
