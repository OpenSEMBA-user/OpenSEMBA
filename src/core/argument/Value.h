
#ifndef SEMBA_ARGUMENT_VALUE_H_
#define SEMBA_ARGUMENT_VALUE_H_

#include <utility>
#include <tuple>

#include "Argument.h"
#include "Object.h"
#include "StringParser.h"

namespace SEMBA {
namespace Argument {

template <typename V, typename T1, typename... T>
class Value : public V {
public:
    Value() {}
    virtual ~Value() {}

    void parseSingle(Object&,
                     std::list<std::string>&,
                     std::list<std::string>&) const;

    virtual void noParsed(Object&) const;
};

namespace Error {

class NotChoices : public Error {
    NotChoices(const Argument& arg,
               const std::vector<std::list<std::string>>& c)
    :   arg_(arg),
        choices_(c) {
        std::stringstream aux;
        aux << "Argument " << arg.getArgName()
            << " not matching possible choices"
            << " (choose from";
        for (std::size_t i = 0; i < c.size(); i++) {
            if (i != 0) {
                aux << " ,";
            }
            for (std::list<std::string>::const_iterator
                 it = c[i].begin(); it != c[i].end(); ++it) {
                aux << " " << *it;
            }
        }
        aux << ")";
    }
    virtual ~NotChoices() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const Argument& getArg() const { return arg_; }
    const std::vector<std::list<std::string>>& getChoices() const {
        return choices_;
    }
private:
    const Argument& arg_;
    std::vector<std::list<std::string>> choices_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#include "Value.hpp"

#endif /* SEMBA_ARGUMENT_VALUE_H_ */
