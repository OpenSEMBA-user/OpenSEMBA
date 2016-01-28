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

#include "Formatter.h"

#include <algorithm>
#include <iostream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include "Parser.h"

namespace SEMBA {
namespace Argument {

const std::string Formatter::PROG    = "${PROG}";
const std::string Formatter::DEFAULT = "${DEFAULT}";
const std::string Formatter::CHOICES = "${CHOICES}";

void Formatter::printUsage() {
    std::size_t tab = 0;
    std::string usage = parser_->getUsage();
    if (usage.empty()) {
        usage  = "usage: ${PROG} ";
        usage = replaceProg_(usage);
        tab = usage.size();
        usage  = "usage: ${PROG} ";
        usage += generateUsageArgs_(*parser_);
    }
    usage = replaceProg_(usage);
    printParagraph_(usage, 0, tab);
}

void Formatter::printHelp() {
    printUsage();
    std::string desc, epilog;
    desc   = parser_->getDescription();
    epilog = parser_->getEpilog();
    std::cout << std::endl;
    if (!desc.empty()) {
        desc = replaceProg_(desc);
        printParagraph_(desc);
        std::cout << std::endl;
    }
    printGroup_(*parser_, 0);
    if (!epilog.empty()) {
        std::cout << std::endl;
        epilog = replaceProg_(epilog);
        printParagraph_(epilog);
    }
}

std::string Formatter::generateUsageArgs_(const GroupBase& group) {
    std::string res;
    for (std::size_t i = 0; i < group.numPositions(); i++) {
        if (!res.empty()) {
            res += " ";
            if (group.isMutuallyExclusive()) {
                res += "| ";
            }
        }
        res += generatePosition_(group.position(i));
    }
    for (std::size_t i = 0; i < group.numOptions(); i++) {
        if (!res.empty()) {
            res += " ";
            if (group.isMutuallyExclusive()) {
                res += "| ";
            }
        }
        res += generateOption_(group.option(i), !group.isMutuallyExclusive());
    }
    for (std::size_t i = 0; i < group.numChilds(); i++) {
        std::string childRes = generateUsageArgs_(group.child(i));
        if (!childRes.empty()) {
            if (!res.empty()) {
                res += " ";
                if (group.isMutuallyExclusive()) {
                    res += "| ";
                }
            }
            res += childRes;
        }
    }
    if (group.isMutuallyExclusive()) {
        if (group.isRequired()) {
            res = std::string("(") + res + std::string(")");
        } else {
            res = std::string("[") + res + std::string("]");
        }
    }
    return res;
}

std::string Formatter::generatePosition_(const PositionBase& arg) {
    std::string res;
    std::string metavar = generateMetavar_(arg);
    res = metavar;
    if (dynamic_cast<const ValueMulti*>(&arg) != NULL) {
        res += std::string(" [") + metavar + std::string(" ...]");
    }
    return res;
}

std::string Formatter::generateOption_(const OptionBase& arg, bool req) {
    std::string res;
    std::string id;
    if (arg.hasShortIdentifier()) {
        id = std::string("-" ) + std::string(1,arg.getShortIdentifier());
    } else {
        id = std::string("--") + arg.getLongIdentifier();
    }
    res += id;
    if (dynamic_cast<const SwitchBase*>(&arg) != NULL) {
        if (dynamic_cast<const MultiSwitch*>(&arg) != NULL) {
            res += std::string(" [") + id + std::string(" ...]");
        }
    } else {
        const OptionValue& value = dynamic_cast<const OptionValue&>(arg);
        std::string metavar = generateMetavar_(value);
        res += std::string(" ") + metavar;
        if (dynamic_cast<const ValueMulti*>(&value) != NULL) {
            res += std::string(" [") + metavar + std::string(" ...]");
        }
    }
    if (!arg.getRequired() && req) {
        res = std::string("[") + res + std::string("]");
    }
    return res;
}

std::string Formatter::generateMetavar_(const ValueBase& value) {
    std::string res;
    const std::list<std::string>& metavar = value.getMetavar();
    if (metavar.empty()) {
        res = value.getName();
        std::transform(res.begin(), res.end(), res.begin(), toupper);
    } else {
        for (std::list<std::string>::const_iterator
             it = metavar.begin(); it != metavar.end(); ++it) {
            if (it != metavar.begin()) {
                res += " ";
            }
            res += *it;
        }
    }
    return res;
}

std::string Formatter::replaceProg_(const std::string& str) const {
    std::string res;
    std::string aux = str;
    std::string prog = parser_->getProg();

    while (aux.find(PROG) != std::string::npos) {
        res += aux.substr(0, aux.find(PROG)) + prog;
        aux  = aux.substr(aux.find(PROG) + PROG.size());
    }
    res += aux;
    return res;
}

std::string Formatter::replaceDefault_(const std::string& str,
                                       const std::list<std::string>& def) {
    std::string res;
    std::string aux = str;
    std::string defStr;
    for (std::list<std::string>::const_iterator
         it = def.begin(); it != def.end(); ++it) {
        if (it != def.begin()) {
            defStr += " ";
        }
        defStr += *it;
    }
    while (aux.find(DEFAULT) != std::string::npos) {
        res += aux.substr(0, aux.find(DEFAULT)) + defStr;
        aux  = aux.substr(aux.find(DEFAULT) + DEFAULT.size());
    }
    res += aux;
    return res;
}
std::string Formatter::replaceChoices_(
        const std::string& str,
        const std::vector<std::list<std::string>>& choices) {
    std::string res;
    std::string aux = str;
    std::string choicesStr;
    for (std::size_t i = 0; i < choices.size(); i++) {
        if(i != 0) {
            choicesStr += ", ";
        }
        for (std::list<std::string>::const_iterator
             it = choices[i].begin(); it != choices[i].end(); ++it) {
            if (it != choices[i].begin()) {
                choicesStr += " ";
            }
            choicesStr += *it;
        }
    }
    while (aux.find(CHOICES) != std::string::npos) {
        res += aux.substr(0, aux.find(CHOICES)) + choicesStr;
        aux  = aux.substr(aux.find(CHOICES) + CHOICES.size());
    }
    return res;
}

void Formatter::printParagraph_(const std::string& str,
                                const std::size_t& posFirst,
                                const std::size_t& min) {
    const std::size_t& max = getConsoleSize_();
    std::string word;
    std::stringstream aux(str);
    std::size_t pos = posFirst;
    bool first = true;
    while (aux >> word) {
        std::size_t newPos = pos + word.size();
        if (!first) {
            newPos++;
        }
        if (newPos > max && !first) {
            std::cout << std::endl << std::string(min, ' ') << word;
            pos = newPos;
        } else {
            if (!first) {
                std::cout << " ";
            }
            std::cout << word;
            pos = newPos;
        }
        first = false;
    }
    std::cout << std::endl;
}

std::size_t Formatter::getConsoleSize_() {
    std::size_t size = 80;
#ifdef _WIN32
    if (!_isatty(_fileno(stdout))) {
        return size;
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return size;
    }
    size = csbi.srWindow.Right-csbi.srWindow.Left+1;
#else
    if (!isatty(fileno(stdout))) {
        return size;
    }
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    size = w.ws_col;
#endif
    return size;
}

void Formatter::printGroup_(const GroupBase& group,
                            const std::size_t& depth) const {
    bool root = (depth == 0);
    const std::size_t& size   = getConsoleSize_();
    const std::size_t  tab    = size/8;
    std::size_t        begtab = (root ? 1 : 2);
    std::string arg, help;
    std::list<std::string> def;
    std::vector<std::list<std::string>> choices;
    if ((depth > 0) && !group.isMutuallyExclusive()) {
        printParagraph_(group.getName());
        std::string desc = group.getDescription();
        if (!desc.empty()) {
            std::cout << "  ";
            printParagraph_(desc, 2, 2);
            std::cout << std::endl;
        }
    } else if (group.isMutuallyExclusive()) {
        if (depth == 1) {
            begtab = 1;
            printParagraph_("mutually exclusive group:");
        } else {
            begtab = 4;
            std::cout << "  ";
            printParagraph_("mutually exclusive group:", 2, 2);
        }
    }
    for (std::size_t i = 0; i < group.numPositions(); i++) {
        if ((i == 0) && root) {
            printParagraph_("positional arguments:");
        }
        arg     = std::string(begtab,' ') +
                  generatePosition_(group.position(i));
        help    = group.position(i).getHelp();
        def     = group.position(i).getDefault();
        choices = group.position(i).getChoices();
        printArg_(arg, help, def, choices, tab);
    }
    if (root && (group.numPositions() != 0) &&
        ((group.numOptions() != 0) || (group.numChilds() != 0))) {
        std::cout << std::endl;
    }
    for (std::size_t i = 0; i < group.numOptions(); i++) {
        if ((i == 0) && root) {
            printParagraph_("optional arguments:");
        }
        arg  = std::string(begtab,' ') + generateOption_(group.option(i));
        help = group.option(i).getHelp();
        def.clear();
        choices.clear();
        if (dynamic_cast<const ValueBase*>(&group.option(i)) != NULL) {
            const ValueBase& value = dynamic_cast<const ValueBase&>(
                                         group.option(i));
            def     = value.getDefault();
            choices = value.getChoices();
        }
        printArg_(arg, help, def, choices, tab);
    }
    if (root && (group.numOptions() != 0) && (group.numChilds() != 0)) {
        std::cout << std::endl;
    }
    for (std::size_t i = 0; i < group.numChilds(); i++) {
        printGroup_(group.child(i), depth+1);
    }
}

void Formatter::printArg_(const std::string& arg,
                          const std::string& help,
                          const std::list<std::string>& def,
                          const std::vector<std::list<std::string>>& choices,
                          const std::size_t& tab) const {
    std::cout << arg;
    if (!help.empty()) {
        if (arg.size() >= tab) {
            std::cout << std::endl << std::string(tab,' ');
        } else {
            std::cout << std::string(tab-arg.size(),' ');
        }
        replaceProg_(help);
        replaceDefault_(help, def);
        replaceChoices_(help, choices);
        printParagraph_(help);
    } else {
        std::cout << std::endl;
    }
}

} /* namespace Argument */
} /* namespace SEMBA */
