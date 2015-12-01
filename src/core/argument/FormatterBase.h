
#ifndef SEMBA_ARGUMENT_FORMATTERBASE_H_
#define SEMBA_ARGUMENT_FORMATTERBASE_H_

namespace SEMBA {
namespace Argument {

class FormatterBase {
public:
    FormatterBase() {}
    virtual ~FormatterBase() {}

    virtual FormatterBase* clone() const = 0;

    virtual void printUsage() = 0;
    virtual void printHelp () = 0;
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_FORMATTERBASE_H_ */
