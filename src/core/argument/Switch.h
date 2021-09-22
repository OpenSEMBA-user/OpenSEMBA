

#pragma once

#include <stdexcept>
#include <string>

#include "ValueSingle.h"
#include "SwitchBase.h"

namespace SEMBA {
namespace Argument {

class Switch : public virtual SwitchBase {
public:
    Switch(const std::string&);
    Switch(const char&, const std::string&);
    Switch(const std::string&, const char&);
    Switch(const std::string&, const std::string&);
    Switch(const std::string&, const char&, const std::string&);
    Switch(const Switch&);
    virtual ~Switch();

    Switch* required  (const bool& = true);
    Switch* help      (const std::string&);
    Switch* defaultVal(const bool&);

    void parse(Object&,
               std::vector<std::list<std::string>>&,
               std::vector<std::list<std::string>>&) const;

    void notParsed(Object&) const;

private:
    bool hasDefault_;
    bool default_;

    void initDefault_();
};

} /* namespace Argument */
} /* namespace SEMBA */

