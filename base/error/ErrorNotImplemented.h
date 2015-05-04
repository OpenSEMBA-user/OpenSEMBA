/*
 * ErrorNotImplemented.h
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_ERROR_ERRORNOTIMPLEMENTED_H_
#define COMMON_BASE_ERROR_ERRORNOTIMPLEMENTED_H_

#include <string>

#include "base/error/Error.h"

class ErrorNotImplemented: public Error {
public:
    ErrorNotImplemented(const string&);
    virtual ~ErrorNotImplemented();

    string getFunctionName() const;

private:
    string func_;
};

#endif /* COMMON_BASE_ERROR_ERRORNOTIMPLEMENTED_H_ */
