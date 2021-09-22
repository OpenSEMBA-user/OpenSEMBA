

#include "Parser.h"

namespace SEMBA {
namespace Parsers {

Math::CVecR3 Parser::strToCartesianVector(const std::string& str) {
    std::stringstream iss(str);
    std::string sub;
    Math::CVecR3 res;
    for (std::size_t i = 0; i < 3; i++) {
        iss >> sub;
        res(i) = atof(sub.c_str());
    }
    return res;
}

bool Parser::strToBool(const std::string& value) {
    if (atoi(value.c_str()) == 1) {
        return true;
    } else {
        return false;
    }
}

void Parser::postReadOperations(Data& res) const {
    if (res.mesh != nullptr) {
        if (res.solver != nullptr) {
            try {
                Math::Real scalingFactor =
                        res.solver->getSettings()("geometryScalingFactor").getReal();
                res.mesh->applyScalingFactor(scalingFactor);
            }
            catch (...) {
                std::cerr << "Unable to find geometryScalingFactor "
                             "during postReadOperations" << std::endl;
                throw std::logic_error(
                        "Unable to find geometryScalingFactor "
                        "during postReadOperations");
            }
        }
    }
}

} /* namespace Parser */
} /* namespace SEMBA */
