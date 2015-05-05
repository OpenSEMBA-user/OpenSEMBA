/*
 * ErrorFile.cpp
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#include <base/error/ErrorFile.h>

ErrorFile::ErrorFile(const string& filename) {
    filename_ = filename;
}

ErrorFile::~ErrorFile() throw() {

}

string ErrorFile::getFilename() const {
    return filename_;
}

ErrorFileNotExists::ErrorFileNotExists(const string& filename)
:   Error(filename + string(" doesn't exists")),
    ErrorFile(filename) {

}

ErrorFileNotExists::~ErrorFileNotExists() throw() {

}

ErrorFileEmpty::ErrorFileEmpty(const string& filename)
:   Error(filename + string(" empty")),
    ErrorFile(filename) {

}

ErrorFileEmpty::~ErrorFileEmpty() throw() {

}
