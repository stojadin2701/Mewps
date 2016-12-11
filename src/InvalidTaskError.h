/*
 * InvalidTaskError.h
 *
 *  Created on: Dec 11, 2016
 *      Author: schutzekatze
 */

#ifndef INVALIDTASKERROR_H_
#define INVALIDTASKERROR_H_

#include <exception>
using std::runtime_error;

class InvalidTaskError: public runtime_error
{

public:

	InvalidTaskError(): runtime_error("Invalid task acquired.") {}

};

#endif /* INVALIDTASKERROR_H_ */
