

#include "Value.h"

namespace SEMBA {
namespace Argument {

template<typename V, typename T1, typename... T>
void Value<V, T1, T...>::parseSingle(Object& out,
                                     std::list<std::string>& output,
                                     std::list<std::string>& input) const {
    StringParser::parse<T1, T...>(input, output);
    const std::vector<std::list<std::string>>& choices = this->getChoices();
    if (!choices.empty()) {
        bool found = true;
        for (std::vector<std::list<std::string>>::const_iterator
             it = choices.begin(); it != choices.end(); ++it) {
            found = true;
            if (it->size() != input.size()) {
                found = false;
                continue;
            }
            std::list<std::string>::const_iterator it1 = it->begin();
            std::list<std::string>::const_iterator it2 = input.begin();
            while ((it1 != it->end()) && (it2 != input.end())) {
                if ((*it1 != *it2) && (*it1 != "*")) {
                    found = false;
                    break;
                }
                ++it1;
                ++it2;
            }
            if (found) {
                break;
            }
        }
        if (!found) {
            throw Error::NotChoices(*this, choices);
        }
    }
    out.setGenericString(input);
}

template<typename V, typename T1, typename... T>
void Value<V, T1, T...>::notParsed(Object& out) const {
    const std::list<std::string>& defaultVar = this->getDefault();
    if (!defaultVar.empty()) {
        out.setGenericString(defaultVar);
    } else {
        Argument::notParsed(out);
    }
}

} /* namespace Argument */
} /* namespace SEMBA */
