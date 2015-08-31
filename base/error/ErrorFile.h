/*
 * ErrorFile.h
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#ifndef COMMON_BASE_ERROR_ERRORFILE_H_
#define COMMON_BASE_ERROR_ERRORFILE_H_

#include <string>

#include "Error.h"

class ErrorFile : public virtual Error {
public:
    ErrorFile(const std::string& filename);
    virtual ~ErrorFile() throw();

    std::string getFilename() const;
private:
    std::string filename_;
};

class ErrorFileNotExists : public ErrorFile {
public:
    ErrorFileNotExists(const std::string& filename);
    ~ErrorFileNotExists() throw();
};

class ErrorFileEmpty : public ErrorFile {
public:
    ErrorFileEmpty(const std::string& filename);
    ~ErrorFileEmpty() throw();
};

#endif /* COMMON_BASE_ERROR_ERRORFILE_H_ */
