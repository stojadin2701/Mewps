/*
 * InvalidTaskError.h
 *
 *  Copyright 2017 Vladimir Nikolić
 */

#ifndef SYSTEM_INVALIDTASKERROR_H_
#define SYSTEM_INVALIDTASKERROR_H_

#include <exception>
using std::runtime_error;

class InvalidTaskError: public runtime_error
{

public:

    InvalidTaskError(): runtime_error("Invalid task acquired.") {}

};

#endif /* SYSTEM_INVALIDTASKERROR_H_ */
