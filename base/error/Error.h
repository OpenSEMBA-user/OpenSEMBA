/*
 * Error.h
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_ERROR_ERROR_H_
#define COMMON_BASE_ERROR_ERROR_H_

#include <exception>
#include <ostream>
#include <string>
using namespace std;

class Error: public exception {
public:
    Error();
    Error(const string& msg);
    virtual ~Error() throw();

    void setMsg(const string& msg);

    const char* what() const throw();

private:
    string msg_;
};

ostream& operator<<(ostream& os, const Error& e);

#endif /* COMMON_BASE_ERROR_ERROR_H_ */
