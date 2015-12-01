
#include "ValueMulti.h"

#include <stdexcept>

namespace SEMBA {
namespace Argument {

void ValueMulti::parse(Object& out,
                       std::vector<std::list<std::string>>& output,
                       std::vector<std::list<std::string>>& input) const {
    out.setArray();
    output.resize(input.size());
    for (std::size_t i = 0; i < input.size(); i++) {
        Object aux;
        parseSingle(aux, output[i], input[i]);
        if (!aux.isNull()) {
            out.addValue(std::move(aux));
        }
    }
}

} /* namespace Argument */
} /* namespace SEMBA */
