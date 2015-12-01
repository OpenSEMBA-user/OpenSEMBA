
#ifndef SEMBA_ARGUMENT_VALUESINGLE_H_
#define SEMBA_ARGUMENT_VALUESINGLE_H_

#include "ValueBase.h"

namespace SEMBA {
namespace Argument {

class ValueSingle : public virtual ValueBase {
public:
    ValueSingle() {}
    virtual ~ValueSingle() {}

    virtual void parse(Object&,
                       std::vector<std::list<std::string>>&,
                       std::vector<std::list<std::string>>&) const;

    virtual void parseSingle(Object&,
                             std::list<std::string>&,
                             std::list<std::string>&) const = 0;
};

namespace Error {

class Multiple : public Error {
public:
    Multiple(const Argument& arg)
    :   arg_(arg),
        str_(std::string("Argument ") + arg.getArgName() +
             " declared multiple times") {}
    virtual ~Multiple() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const Argument& getArg() const { return arg_; }
private:
    const Argument& arg_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_VALUESINGLE_H_ */
