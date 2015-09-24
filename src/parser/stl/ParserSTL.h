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
 * ParserSTL.h
 *
 *  Created on: Sep 23, 2013
 *      Author: luis
 */

#ifndef PARSERSTL_H_
#define PARSERSTL_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "parser/Parser.h"
#include "SmbData.h"

class ParserSTL : public Parser, public ProjectFile {
    friend class SmbData;
public:
    ParserSTL();
    ParserSTL(const string& fn);
    virtual ~ParserSTL();

    class ErrorGridNotExists : public Error {
    public:
        ErrorGridNotExists();
        virtual ~ErrorGridNotExists() throw();
    };

    SmbData* read(const OptionsMesher* = NULL);

    void printInfo() const;
};

#endif /* PARSERGID_H_ */
