
#include "ValueSingle.h"

#include <stdexcept>

namespace SEMBA {
namespace Argument {

void ValueSingle::parse(Object& out,
                        std::vector<std::list<std::string>>& output,
                        std::vector<std::list<std::string>>& input) const {
    if (input.size() != 1) {
        throw Error::Multiple(*this);
    }
    output.resize(1);
    parseSingle(out, output[0], input[0]);
}

} /* namespace Argument */
} /* namespace SEMBA */
