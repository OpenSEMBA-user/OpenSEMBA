
#ifndef SEMBA_ARGUMENT_VALUEBASE_H_
#define SEMBA_ARGUMENT_VALUEBASE_H_

#include "Argument.h"

namespace SEMBA {
namespace Argument {

class ValueBase : public virtual Argument {
public:
    ValueBase() {}
    virtual ~ValueBase() {}

    using Argument::getMetavar;
    using Argument::getDefault;
    using Argument::getChoices;

    ValueBase& required  (const bool& = true);
    ValueBase& help      (const std::string&);
    ValueBase& metavar   (const std::list<std::string>&);
    ValueBase& defaultVal(const std::list<std::string>&);
    ValueBase& choices   (const std::vector<std::list<std::string>>&);
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_VALUEBASE_H_ */
