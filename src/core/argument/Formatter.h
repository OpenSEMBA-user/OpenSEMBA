// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <string>

#include "GroupBase.h"

namespace SEMBA {
namespace Argument {

class Parser;

class Formatter {
public:
    static const std::string PROG;
    static const std::string DEFAULT;
    static const std::string CHOICES;

    Formatter(const Parser* parser) : parser_(parser) {}
    Formatter(const Formatter& rhs) : parser_(rhs.parser_) {}
    virtual ~Formatter() {}

    virtual void printUsage();
    virtual void printHelp();

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

