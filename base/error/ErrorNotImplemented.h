/*
 * ErrorNotImplemented.h
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_ERROR_ERRORNOTIMPLEMENTED_H_
#define COMMON_BASE_ERROR_ERRORNOTIMPLEMENTED_H_

#include <string>

#include "Error.h"

class ErrorNotImplemented: public Error {
public:
    ErrorNotImplemented(const std::string&);
    virtual ~ErrorNotImplemented() throw();

    std::string getFunctionName() const;

private:
    std::string func_;
};

#endif /* COMMON_BASE_ERROR_ERRORNOTIMPLEMENTED_H_ */
