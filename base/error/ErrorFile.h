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
    ErrorFile(const string& filename);
    virtual ~ErrorFile();

    string getFilename() const;
private:
    string filename_;
};

class ErrorFileNotExists : public ErrorFile {
public:
    ErrorFileNotExists(const string& filename);
    ~ErrorFileNotExists();
};

class ErrorFileEmpty : public ErrorFile {
public:
    ErrorFileEmpty(const string& filename);
    ~ErrorFileEmpty();
};

#endif /* COMMON_BASE_ERROR_ERRORFILE_H_ */
