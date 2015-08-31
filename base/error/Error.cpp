/*
 * Error.cpp
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#include "Error.h"

Error::Error() {

}

Error::Error(const std::string& msg) {
    msg_ = msg;
}

Error::~Error() throw() {

}

void Error::setMsg(const std::string& msg) {
    msg_ = msg;
}

const char* Error::what() const throw() {
    return msg_.c_str();
}
