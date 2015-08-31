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

class Error: public std::exception {
public:
    Error();
    Error(const std::string& msg);
    virtual ~Error() throw();

    void setMsg(const std::string& msg);

    const char* what() const throw();

private:
    std::string msg_;
};

#endif /* COMMON_BASE_ERROR_ERROR_H_ */
