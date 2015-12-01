
#ifndef SEMBA_ARGUMENT_OPTIONBASE_H_
#define SEMBA_ARGUMENT_OPTIONBASE_H_

#include "Argument.h"

namespace SEMBA {
namespace Argument {

class OptionBase : public virtual Argument {
public:

    OptionBase();
    OptionBase(const char&);
    OptionBase(const std::string&);
    OptionBase(const char&, const std::string&);
    OptionBase(const OptionBase&);
    virtual ~OptionBase();

    std::string getArgName() const;

    bool        hasShortIdentifier() const { return hasShortId_; }
    const char& getShortIdentifier() const { return    shortId_; }

    bool               hasLongIdentifier() const { return hasLongId_; }
    const std::string& getLongIdentifier() const { return    longId_; }

    virtual OptionBase* clone() const = 0;

private:
    bool     hasShortId_;
    char        shortId_;

    bool     hasLongId_;
    std::string longId_;

    void check_();
};

namespace Error {

class NoIdentifier : public Error {
public:
    NoIdentifier(const Argument& arg)
    :   arg_(arg),
        str_(std::string("Argumen: ") + arg.getName() +
             " has no valid identifier") {}
    virtual ~NoIdentifier() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    const Argument& getArg() const { return arg_; }
private:
    const Argument& arg_;
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_OPTIONBASE_H_ */
