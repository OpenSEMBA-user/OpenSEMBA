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
    virtual GroupBase& description(const std::string& desc);

    virtual std::size_t numChilds() const;
    virtual std::size_t numPositions() const;
    virtual std::size_t numOptions() const;

    virtual const GroupBase&    child   (const std::size_t& i) const;
    virtual const PositionBase& position(const std::size_t& i) const;
    virtual const OptionBase&   option  (const std::size_t& i) const;

    bool existsOption(const char&) const;
    bool existsOption(const std::string&, const bool& abbrev = false) const;

    const std::string& optionName(const char&) const;
    const std::string& optionName(const std::string&) const;
    std::vector<std::string> getPossibleOptions(const std::string&,
                                                const bool& = false) const;

    virtual PositionBase& addPosition(const PositionBase&);
    virtual OptionBase&   addOption  (const OptionBase&);

    virtual void parsePreprocess(Object&);
    virtual void parsePosition(Object&,
                               std::vector<std::list<std::string>>&,
                               std::vector<std::list<std::string>>&);
    virtual void parseOption(const std::string&,
                             Object&,
                             std::vector<std::list<std::string>>&,
                             std::vector<std::list<std::string>>&);
    virtual void parsePostprocess(Object&);

    virtual std::size_t         numAllPositions() const;
    virtual const PositionBase& getAllPosition(const std::size_t&) const;
    virtual const OptionBase&   getAllOption(const std::string&) const;

protected:
    GroupBase(GroupBase* = NULL,
              const std::string& = std::string(),
              const std::string& = std::string());

    virtual void addPositionProcess(GroupBase*, PositionBase*);
    virtual void addOptionProcess  (GroupBase*, OptionBase*);

    virtual void insertName(const std::string&);
    virtual void insertShortOpt(const char&, const std::string&);
    virtual void insertLongOpt(const std::string&, const std::string&);

private:
    GroupBase* parent_;

    std::string name_;
    std::string description_;
    bool required_;

    std::set<std::string> names_;

    std::vector<PositionBase*> positions_;
    std::vector<PositionBase*>::iterator lastPosParsed_;

    std::vector<OptionBase*> options_;
    std::map<std::string, bool> optionParsed_;
    std::map<std::string, OptionBase*> optionName_;
    std::map<char, std::string> shortOpts_;
    std::map<std::string, std::string> longOpts_;

    //Erased
    GroupBase(const GroupBase&);
    GroupBase(GroupBase&&);

    GroupBase& operator=(const GroupBase&);
    GroupBase& operator=(GroupBase&&);
};

namespace Error {

namespace Group {

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

class Required : public Error {
public:
    Required(const std::vector<Argument*> args)
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
    virtual ~Required() throw() {}

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
    :   str_(std::string("Argument \"") + name +
             std::string("\" defined twice")) {}
    virtual ~Repeated() throw() {}

    const char* what() const throw() { return str_.c_str(); }
private:
    std::string str_;
};

} /* namespace Group */
} /* namespace Error */
} /* namespace Argument */
} /* namespace SEMBA */

#endif /* SEMBA_ARGUMENT_GROUPBASE_H_ */
