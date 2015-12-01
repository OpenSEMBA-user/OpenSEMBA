
#ifndef SEMBA_ARGUMENT_FORMATTER_H_
#define SEMBA_ARGUMENT_FORMATTER_H_

#include <string>

#include "GroupBase.h"

#include "FormatterBase.h"

namespace SEMBA {
namespace Argument {

class Parser;

class Formatter : public FormatterBase {
public:
    static const std::string PROG;
    static const std::string DEFAULT;
    static const std::string CHOICES;

    Formatter(const Parser* parser) : parser_(parser) {}
    Formatter(const Formatter& rhs) : parser_(rhs.parser_) {}
    virtual ~Formatter() {}

    Formatter* clone() const { return new Formatter(*this); }

    void printUsage();
    void printHelp ();

private:
    const Parser* parser_;

    static std::string generateUsageArgs_(const GroupBase&);
    static std::string generatePosition_ (const PositionBase&);
    static std::string generateOption_   (const OptionBase&, bool = false);
    static std::string generateMetavar_  (const ValueBase&);

    std::string        replaceProg_   (const std::string&) const;
    static std::string replaceDefault_(const std::string&,
                                       const std::list<std::string>&);
    static std::string replaceChoices_(
            const std::string&,
            const std::vector<std::list<std::string>>&);

    static void printParagraph_(const std::string&,
                                const std::size_t& = 0,
                                const std::size_t& = 0);
    void printGroup_(const GroupBase&, const std::size_t&) const;
    void printArg_(const std::string& name,
                   const std::string& help,
                   const std::list<std::string>& def,
                   const std::vector<std::list<std::string>>& choices,
                   const std::size_t& tab) const;

    static std::size_t getConsoleSize_();
};

} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_FORMATTER_H_ */
