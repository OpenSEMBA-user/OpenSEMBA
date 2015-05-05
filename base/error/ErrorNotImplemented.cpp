/*
 * ErrorNotImplemented.cpp
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#include "ErrorNotImplemented.h"

ErrorNotImplemented::ErrorNotImplemented(const string& func)
:   Error(func + ": not implemented") {

}

ErrorNotImplemented::~ErrorNotImplemented() throw() {

}

string ErrorNotImplemented::getFunctionName() const {
    return func_;
}
