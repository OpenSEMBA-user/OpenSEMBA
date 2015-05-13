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

    SmbData* read();

    void printInfo() const;
private:

};

#endif /* PARSERGID_H_ */
