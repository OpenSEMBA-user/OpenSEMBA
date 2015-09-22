// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
