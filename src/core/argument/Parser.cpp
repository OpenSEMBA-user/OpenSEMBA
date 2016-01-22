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

#include "Parser.h"

#include <cctype>
#include <iostream>

namespace SEMBA {
namespace Argument {

Parser::Parser(const int& argc, const char** argv)
:   Group<true>(NULL, "") {
    input_.clear();
    if (argc > 0) {
        prog_ = argv[0];
        std::size_t folderSep = prog_.find_last_of("/\\");
        if (folderSep != std::string::npos) {
            if (!prog_.substr(folderSep+1).empty()) {
                prog_ = prog_.substr(folderSep+1);
            }
        }
    }
    for (int i = 1; i < argc; i++) {
        input_.push_back(argv[i]);
    }
    initDefault_();
}

Parser::Parser(const std::vector<std::string>& argv)
:   Group<true>(NULL, "") {
    input_.clear();
    if (argv.size() > 0) {
        prog_ = argv[0];
        std::size_t folderSep = prog_.find_last_of("/\\");
        if (folderSep != std::string::npos) {
            if (!prog_.substr(folderSep + 1).empty()) {
                prog_ = prog_.substr(folderSep + 1);
            }
        }
    }
    for (int i = 1; i < argv.size(); i++) {
        input_.push_back(argv[i]);
    }
    initDefault_();
}

Parser::~Parser() {

}

Parser& Parser::prog(const std::string& prog) {
    prog_ = prog;
    return *this;
}

Parser& Parser::usage(const std::string& usage) {
    usage_ = usage;
    return *this;
}

Parser& Parser::description(const std::string& description) {
    setDescription_(description);
    return *this;
}

Parser& Parser::epilog(const std::string& epilog) {
    epilog_ = epilog;
    return *this;
}

Parser& Parser::prefixChars(const std::string& prefixChars) {
    prefixChars_ = prefixChars;
    return *this;
}

Parser& Parser::allowAbbrev(const bool allowAbbrev) {
    allowAbbrev_ = allowAbbrev;
    return *this;
}

Parser& Parser::formatter(const FormatterBase& form) {
    delete formatter_;
    formatter_ = form.clone();
    return *this;
}

Object Parser::parse() {
    Object res = parse_();
    if (!input_.empty()) {
        Error::Unknown err(input_);
        printUsage();
        std::cerr << prog_ << ": " << err.what() << std::endl;
        throw Error::Unknown(input_);
    }
    return res;
}

std::pair<Object, std::vector<std::string>> Parser::parseKnownArgs() {
    Object                   known = parse_();
    std::vector<std::string> unknown;
    unknown.push_back(prog_);
    unknown.insert(unknown.end(), input_.begin(), input_.end());
    return std::make_pair(known, unknown);
}

void Parser::printUsage() const {
    formatter_->printUsage();
}

void Parser::printHelp() const {
    formatter_->printHelp();
}

void Parser::initDefault_() {
    prefixChars_ = "-";
    allowAbbrev_ = true;
    formatter_ = new Formatter(this);
}

Object Parser::parse_() {
    if (input_.empty()) {
        printUsage();
        return Object();
    }
    std::vector<std::list<std::string>> unresolvedOptions;
    std::vector<std::vector<std::pair<std::string,
                                      std::list<std::string>>>> options;
    std::map<std::list<std::string>,
             std::vector<std::pair<std::size_t,
                                   std::size_t>>> optionsName;
    std::vector<std::string>                         begPositions;
    std::vector<std::vector<std::list<std::string>>> intPositions;
    std::vector<std::string>                         endPositions;

    //Begin processing input
    //Find --
    std::vector<std::string>::const_iterator sep = input_.begin();
    for (sep = input_.begin(); sep != input_.end(); ++sep) {
        if (getArgType_(*sep) == ArgType::Separator) {
            break;
        }
    }
    std::vector<std::string>::const_iterator it = input_.begin();
    //Find Positionalas before --
    for (; it != sep; ++it) {
        if (getArgType_(*it) != ArgType::Positional) {
            break;
        }
        begPositions.push_back(*it);
    }
    //Find Optionals before --
    for (; it != sep; ++it) {
        unresolvedOptions.push_back(std::list<std::string>());
        options.push_back(std::vector<std::pair<std::string,
                                                std::list<std::string>>>());
        intPositions.push_back(std::vector<std::list<std::string>>());
        std::pair<std::string, std::list<std::string>> arg;
        if (getArgType_(*it) == ArgType::ShortOpt) {
            std::size_t posPrev = 1, posNext;
            for (posNext = posPrev; posNext < it->size(); posNext++) {
                posPrev = posNext;
                if (shortOpts_.count((*it)[posPrev]) == 0) {
                    for (posNext = posPrev+1; posNext < it->size(); posNext++) {
                        if (shortOpts_.count((*it)[posNext]) != 0) {
                            break;
                        }
                    }
                    unresolvedOptions.back().push_back(
                        std::string(1,prefixChars_[0]) +
                        it->substr(posPrev, posNext - posPrev));
                    --posNext;
                } else {
                    arg.first = it->substr(posPrev, 1);
                    arg.second.clear();
                    ++posPrev;
                    for (posNext = posPrev; posNext < it->size(); posNext++) {
                        if (shortOpts_.count((*it)[posNext]) != 0) {
                            break;
                        }
                    }
                    if (posPrev != posNext) {
                        arg.second.push_back(it->substr(posPrev,
                                                        posNext-posPrev));
                    }
                    options.back().push_back(arg);
                    --posNext;
                }
            }
        } else if (getArgType_(*it) == ArgType::LongOpt) {
            std::size_t posEq = it->find("=");
            if (posEq != std::string::npos) {
                arg.first  = it->substr(2, posEq);
                arg.second.clear();
                if (!it->substr(posEq+1).empty()) {
                    arg.second.push_back(it->substr(posEq+1));
                }
            } else {
                arg.first  = it->substr(2);
                arg.second.clear();
            }
            bool found = false;
            if (longOpts_.count(arg.first) != 0) {
                found = true;
            } else if (allowAbbrev_) {
                std::vector<std::string> ambiguous;
                std::string abbrev1 = arg.first;
                ambiguous.clear();
                for (std::map<std::string,
                              std::list<std::string>>::const_iterator
                     it2  = longOpts_.begin();
                     it2 != longOpts_.end(); ++it2) {
                    if (it2->first.size() < abbrev1.size()) {
                        continue;
                    }
                    std::string abbrev2 = it2->first.substr(0, abbrev1.size());
                    if (abbrev1 == abbrev2) {
                        found = true;
                        ambiguous.push_back(it2->first);
                    }
                }
                if (ambiguous.size() > 1) {
                    Error::Ambiguous err(arg.first, ambiguous);
                    printUsage();
                    std::cerr << prog_ << ": " << err.what() << std::endl;
                    throw err;
                } else if (ambiguous.size() == 1) {
                    arg.first = ambiguous[0];
                }
            }
            if (found) {
                options.back().push_back(arg);
            } else {
                unresolvedOptions.back().push_back(
                    std::string(2,prefixChars_[0]) + arg.first);
                for (std::list<std::string>::const_iterator
                     it  = arg.second.begin();
                     it != arg.second.end(); ++it) {
                    unresolvedOptions.back().push_back(*it);
                }
            }
        }
        for (++it; it != sep; ++it) {
            if (getArgType_(*it) != ArgType::Positional) {
                break;
            }
            if (options.back().empty()) {
                unresolvedOptions.back().push_back(*it);
            } else {
                options.back().back().second.push_back(*it);
            }
        }
        --it;
        intPositions.back().resize(options.back().size());
    }
    //Find Positionalas after --
    if (sep != input_.end()) {
        it = sep + 1;
        for (; it != input_.end(); ++it) {
            endPositions.push_back(*it);
        }
    }
    //End processing input
    //Reading options position
    for (std::size_t i = 0; i < options.size(); i++) {
        for (std::size_t j = 0; j < options[i].size(); j++) {
            if (options[i][j].first.size() == 1) {
                optionsName[shortOpts_.at(options[i][j].first[0])].push_back(
                    std::pair<std::size_t, std::size_t>(i, j));
            } else {
                optionsName[longOpts_.at(options[i][j].first)].push_back(
                    std::pair<std::size_t, std::size_t>(i, j));
            }
        }
    }
    Object res;
    parsePreprocess(res);
    for (std::map<std::list<std::string>,
                  std::vector<std::pair<std::size_t,
                                        std::size_t>>>::const_iterator
         it = optionsName.begin(); it != optionsName.end(); ++it) {
        std::list<std::string> name = it->first;
        std::vector<std::list<std::string>> input, rem;
        for (std::vector<std::pair<std::size_t, std::size_t>>::const_iterator
             it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            input.push_back(options[it2->first][it2->second].second);
        }
        rem.resize(input.size());
        try {
            parseOption(name, res, rem, input);
        } catch (const StringParser::Error::Error& e) {
            name = it->first;
            printUsage();
            std::cerr << prog_ << ": Argument "
                      << getOption_(name)->getArgName() << ": " << e.what();
            printErrorList_(rem);
            std::cerr << std::endl;
            throw;
        } catch (const SEMBA::Argument::Error::Error& e) {
            printUsage();
            std::cerr << prog_ << ": " << e.what() << std::endl;
            throw;
        }
        std::size_t i = 0;
        for (std::vector<std::pair<std::size_t, std::size_t>>::const_iterator
             it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            intPositions[it2->first][it2->second] = rem[i++];
        }
    }
    std::vector<std::list<std::string>> positions(1);
    std::vector<std::list<std::string>> rem(1);
    for (std::size_t i = 0; i < begPositions.size(); i++) {
        positions.back().push_back(begPositions[i]);
    }
    for (std::size_t i = 0; i < intPositions.size(); i++) {
        for (std::size_t j = 0; j < intPositions[i].size(); j++) {
            positions.back().insert(positions.back().end(),
                                    intPositions[i][j].begin(),
                                    intPositions[i][j].end());
        }
    }
    for (std::size_t i = 0; i < endPositions.size(); i++) {
        positions.back().push_back(endPositions[i]);
    }
    if (!positionsFull_.empty()) {
        bool lastMulti = isLastPosMulti_();
        for (std::size_t i = 0; i < positionsFull_.size(); i++) {
            if (positions[0].empty()) {
                break;
            }
            if ((i+1 == positionsFull_.size()) && lastMulti) {
                break;
            }
            rem.clear();
            try {
                parsePosition(res, rem, positions);
            } catch(const StringParser::Error::Error& e) {
                printUsage();
                std::cerr << prog_ << ": Argument "
                          << getPosition_(i)->getArgName() << ": " << e.what();
                printErrorList_(rem);
                std::cerr << std::endl;
                throw;
            } catch (const SEMBA::Argument::Error::Error& e) {
                printUsage();
                std::cerr << prog_ << ": " << e.what() << std::endl;
                throw;
            }
            positions = rem;
        }
        if (lastMulti && !positions[0].empty()) {
            while (true) {
                std::vector<std::list<std::string>> auxpos, auxrem;
                auxpos.push_back(positions.back());
                auxrem.push_back(std::list<std::string>());
                try {
                    parseLastPosition(res, auxrem, auxpos);
                    positions.back() =  auxpos[0];
                    if (auxrem[0].empty()) {
                        break;
                    } else {
                        positions.push_back(auxrem[0]);
                    }
                } catch(...) {
                    break;
                }
            }
            rem.clear();
            rem.resize(positions.size());
            try {
                parseLastPosition(res, rem, positions);
            } catch(const StringParser::Error::Error& e) {
                printUsage();
                std::cerr << prog_ << ": Argument "
                          << getPosition_(
                                positionsFull_.size()-1)->getArgName()
                          << ": " << e.what();
                printErrorList_(rem);
                std::cerr << std::endl;
                throw;
            } catch (const SEMBA::Argument::Error::Error& e) {
                printUsage();
                std::cerr << prog_ << ": " << e.what() << std::endl;
                throw;
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                exit(0);
            }
        }
    }
    input_.clear();
    for (std::size_t i = 0; i < unresolvedOptions.size(); i++) {
        input_.insert(input_.end(),
                      unresolvedOptions[i].begin(),
                      unresolvedOptions[i].end());
    }
    for (std::size_t i = 0; i < rem.size(); i++) {
        if (!rem[i].empty()) {
            input_.push_back("--");
            break;
        }
    }
    for (std::size_t i = 0; i < rem.size(); i++) {
        input_.insert(input_.end(),
                      rem[i].begin(),
                      rem[i].end());
    }
    try {
        parsePostprocess(res);
    } catch (const SEMBA::Argument::Error::Error& e) {
        printUsage();
        std::cerr << prog_ << ": " << e.what() << std::endl;
        throw;
    }
    return res;
}

Parser::ArgType Parser::getArgType_(const std::string& arg) const {
    if (arg.size() < 2) {
        return ArgType::Positional;
    }
    if (arg.size() == 2) {
        if ((prefixChars_.find(arg[0]) != std::string::npos) &&
            (arg[0] == arg[1])) {
            return ArgType::Separator;
        }
    }
    if (arg.size() >= 2) {
        if ((prefixChars_.find(arg[0]) != std::string::npos) &&
            (std::isalpha(arg[1]))) {
            return ArgType::ShortOpt;
        }
    }
    if (arg.size() >= 4) {
        if ((prefixChars_.find(arg[0]) != std::string::npos) &&
            (arg[0] == arg[1]) &&
            std::isalpha(arg[2]) && std::isalpha(arg[3])) {
            return ArgType::LongOpt;
        }
    }
    return ArgType::Positional;
}

void Parser::printErrorList_(
        const std::vector<std::list<std::string>>& err) {
    for (std::size_t i = 0; i < err.size(); i++) {
        if (i == 0) {
            std::cerr << ":";
        } else {
            std::cerr << " ,";
        }
        for (std::list<std::string>::const_iterator
             it = err[i].begin(); it != err[i].end(); ++it) {
            std::cerr << " " << *it;
        }
    }
}

} /* namespace Argument */
} /* namespace SEMBA */
