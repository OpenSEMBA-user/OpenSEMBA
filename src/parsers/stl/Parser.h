

#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "Data.h"
#include "geometry/element/Triangle3.h"
#include "geometry/mesh/Geometric.h"
#include "physicalModel/Predefined.h"
#include "parsers/Parser.h"


namespace SEMBA {
namespace Parsers {
namespace STL {

class Parser : public SEMBA::Parsers::Parser {
public:
    Parser(const std::string& fn);
    Data read() const;
    Geometry::Mesh::Unstructured readAsUnstructuredMesh() const;
};

} /* namespace STL */
} /* namespace Parsers */
} /* namespace SEMBA */
