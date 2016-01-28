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

#ifndef SEMBA_ARGUMENT_GROUPBASE_H_
#define SEMBA_ARGUMENT_GROUPBASE_H_

#include <map>
#include <set>
#include <string>
#include <type_traits>

#include "Object.h"

#include "Switch.h"
#include "MultiSwitch.h"
#include "Option.h"
#include "MultiOption.h"
#include "Position.h"
#include "MultiPosition.h"

namespace SEMBA {
namespace Argument {

class GroupBase {
public:
    virtual ~GroupBase();

    virtual bool isRequired() const { return required_; }
    virtual bool isMutuallyExclusive() const = 0;

    const std::string& getName       () const { return name_;        }
    const std::string& getDescription() const { return description_; }

    virtual GroupBase& required();

    std::size_t   numChilds()                    const {
        return child_.size();
    }
    const GroupBase& child(const std::size_t& i) const {
        return *child_.at(i);
    }

    std::size_t      numPositions() const;
    const PositionBase& position(const std::size_t& i) const;

    std::size_t    numOptions() const;
    const OptionBase& option(const std::size_t& i) const;

    template <typename T>
    T* addArgument(const T& arg);

protected:
    void parsePreprocess(Object&) const;

    void parsePosition(Object&,
                       std::vector<std::list<std::string>>&,
                       std::vector<std::list<std::string>>&);
    void parseOption  (std::list<std::string>&,
                       Object&,
                       std::vector<std::list<std::string>>&,
                       std::vector<std::list<std::string>>&);
    void parseLastPosition(Object&,
                           std::vector<std::list<std::string>>&,
                           std::vector<std::list<std::string>>&);

    void parsePostprocess(Object&) const;

protected:
    GroupBase* parent_;
    bool required_;

    std::string name_;
    std::string description_;

    std::map<std::string, std::size_t> childName_;
    std::vector<GroupBase*>            child_;

    std::set<std::string> names_;

    std::vector<PositionBase*> positions_;
    std::vector<OptionBase*  > options_;

    std::size_t lastPosParsed_;
    std::vector<std::pair<PositionBase*, GroupBase*>> positionsFull_;

    std::map<std::string, bool>        optionParsed_;
    std::map<std::string, OptionBase*> optionsName_;
    std::map<char,        std::list<std::string>> shortOpts_;
    std::map<std::string, std::list<std::string>>  longOpts_;

    GroupBase(GroupBase* = NULL,
              const std::string& = std::string(),
              const std::string& = std::string());

    void setDescription_(const std::string& desc) { description_ = desc; }

    void registerPosition_(GroupBase*, std::list<std::string>&);
    void registerOption_  (GroupBase*, std::list<std::string>&,
                           const char&);
    void registerOption_  (GroupBase*, std::list<std::string>&,
                           const std::string&);
    void registerOption_  (GroupBase*, std::list<std::string>&,
                           const char&, const std::string&);

    void insertName_(const std::string&);
    void insertShortOpt_(const std::list<std::string>&, const char&);
    void insertLongOpt_ (const std::list<std::string>&, const std::string&);

    PositionBase* getPosition_(const std::size_t&) const;
    OptionBase*   getOption_  (std::list<std::string>&) const;

    bool isLastPosMulti_() const;

private:
    //Erased
    GroupBase(const GroupBase&);
    GroupBase(GroupBase&&);

    GroupBase& operator=(const GroupBase&);
    GroupBase& operator=(GroupBase&&);
};

template<typename T>
T* GroupBase::addArgument(const T& arg) {
    std::list<std::string> aux;
    aux.push_back(arg.getName());
    T* newArg = dynamic_cast<T*>(arg.clone());
    if (dynamic_cast<PositionBase*>(newArg) != NULL) {
        PositionBase* newPos = dynamic_cast<PositionBase*>(newArg);
        positions_.push_back(newPos);
        positionsFull_.push_back(
            std::pair<PositionBase*,GroupBase*>(newPos, NULL));
        registerPosition_(NULL, aux);
    } else if (dynamic_cast<OptionBase*>(newArg) != NULL) {
        OptionBase* newOpt = dynamic_cast<OptionBase*>(newArg);
        options_.push_back(newOpt);
        optionsName_ [newOpt->getName()] = newOpt;
        optionParsed_[newOpt->getName()] = false;
        if (newOpt->hasShortIdentifier() && newOpt->hasLongIdentifier()) {
            registerOption_(NULL, aux,
                            newOpt->getShortIdentifier(),
                            newOpt->getLongIdentifier());
        } else if (newOpt->hasShortIdentifier()) {
            registerOption_(NULL, aux,
                            newOpt->getShortIdentifier());
        } else if (newOpt->hasLongIdentifier()) {
            registerOption_(NULL, aux,
                            newOpt->getLongIdentifier());
        }
    } else {
        delete newArg;
    }
    return newArg;
}

namespace Error {

class Excluded : public Error {
public:
    Excluded(const std::vector<Argument*> args)
    :   args_(args) {
        std::stringstream aux;
        aux << "Parsing mutually excluded arguments ";
        for (std::size_t i = 0; i < args_.size(); i++) {
            if (i != 0) {
                aux << ", ";
            }
            aux << args_[i]->getArgName();
        }
        str_ = aux.str();
    }
    virtual ~Excluded() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    std::size_t getNumArgs() const { return args_.size(); }
    const Argument& getArg(const std::size_t i) const { return *args_[i]; }
private:
    const std::vector<Argument*> args_;
    std::string str_;
};

class GroupRequired : public Error {
public:
    GroupRequired(const std::vector<Argument*> args)
    :   args_(args) {
        std::stringstream aux;
        aux << "One of the arguments ";
        for (std::size_t i = 0; i < args_.size(); i++) {
            if (i != 0) {
                aux << ", ";
            }
            aux << args_[i]->getArgName();
        }
        aux << " is required";
        str_ = aux.str();
    }
    virtual ~GroupRequired() throw() {}

    const char* what() const throw() { return str_.c_str(); }

    std::size_t getNumArgs() const { return args_.size(); }
    const Argument& getArg(const std::size_t i) const { return *args_[i]; }
private:
    const std::vector<Argument*> args_;
    std::string str_;
};

class Repeated : public Error {
public:
    Repeated(const std::string& name)
    :   str_(std::string("Argument defined twice")) {}
    virtual ~Repeated() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_GROUPBASE_H_ */
