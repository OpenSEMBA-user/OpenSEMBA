/*
 * ErrorFile.cpp
 *
 *  Created on: 4/5/2015
 *      Author: Daniel
 */

#include "ErrorFile.h"

ErrorFile::ErrorFile(const std::string& filename) {
    filename_ = filename;
}

ErrorFile::~ErrorFile() throw() {

}

std::string ErrorFile::getFilename() const {
    return filename_;
}

ErrorFileNotExists::ErrorFileNotExists(const std::string& filename)
:   Error(filename + std::string(" doesn't exists")),
    ErrorFile(filename) {

}

ErrorFileNotExists::~ErrorFileNotExists() throw() {

}

ErrorFileEmpty::ErrorFileEmpty(const std::string& filename)
:   Error(filename + std::string(" empty")),
    ErrorFile(filename) {

}

ErrorFileEmpty::~ErrorFileEmpty() throw() {

}
