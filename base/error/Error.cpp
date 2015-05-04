/*
 * Error.cpp
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#include "Error.h"

Error::Error() {

}

Error::Error(const string& msg) {
    msg_ = msg;
}

Error::~Error() {

}

void Error::setMsg(const string& msg) {
    msg_ = msg;
}

const char* Error::what() const {
    return msg_.c_str();
}

ostream& operator<<(ostream& os, const Error& e) {
    os << e.what();
    return os;
}
