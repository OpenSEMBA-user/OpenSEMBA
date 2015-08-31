// File: parser.cpp
// =============== Includes headers ===========================================
#include "Parser.h"

Parser::Parser() {
    scalingFactor_ = 1.0;
}

Parser::~Parser() {

}

CVecR3 Parser::strToCartesianVector(const string& str) {
    istringstream iss(str);
    string sub;
    CartesianVector<Real,3> res;
    for (UInt i = 0; i < 3; i++) {
        iss >> sub;
        res(i) = atof(sub.c_str());
    }
    return res;
}

bool Parser::strToBool(const string& value) {
    if (atoi(value.c_str()) == 1) {
        return true;
    } else {
        return false;
    }
}

void Parser::postReadOperations(SmbData* res) {
    if (res->mesh != nullptr) {
        res->mesh->applyScalingFactor(scalingFactor_);
    }
    if (res->grid != nullptr) {
        res->grid->enlarge(boundaryPadding_, boundaryMeshSize_);
        res->grid->applyScalingFactor(scalingFactor_);
    }
}
