
#ifndef SEMBA_ARGUMENT_MULTISWITCH_H_
#define SEMBA_ARGUMENT_MULTISWITCH_H_

#include <stdexcept>
#include <string>

#include "SwitchBase.h"

namespace SEMBA {
namespace Argument {

class MultiSwitch : public SwitchBase {
public:
    MultiSwitch(const std::string&);
    MultiSwitch(const char&, const std::string&);
    MultiSwitch(const std::string&, const char&);
    MultiSwitch(const std::string&, const std::string&);
    MultiSwitch(const std::string&, const char&, const std::string&);
    MultiSwitch(const MultiSwitch&);
    virtual ~MultiSwitch();

    MultiSwitch* clone() const { return new MultiSwitch(*this); };

    MultiSwitch& required  (const bool& = true);
    MultiSwitch& help      (const std::string&);
    MultiSwitch& defaultVal(const std::size_t&);

    void parse(Object&,
               std::vector<std::list<std::string>>&,
               std::vector<std::list<std::string>>&) const;

    void noParsed(Object&) const;

private:
    bool hasDefault_;
    std::size_t default_;

    void initDefault_();
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_MULTISWITCH_H_ */
