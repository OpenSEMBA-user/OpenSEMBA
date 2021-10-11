

#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "Data.h"
#include "geometry/element/Triangle3.h"
#include "geometry/mesh/Geometric.h"
#include "physicalModel/predefined/PEC.h"
#include "parsers/Parser.h"


namespace SEMBA {
namespace Parsers {
namespace STL {

class Parser : public SEMBA::Parsers::Parser {
public:
    Parser(const std::string& fn) : SEMBA::Parsers::Parser(fn) {};
    Data read() const;
};

} /* namespace STL */
} /* namespace Parsers */
} /* namespace SEMBA */
