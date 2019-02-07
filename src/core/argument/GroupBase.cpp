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

#include "GroupBase.h"

namespace SEMBA {
namespace Argument {

GroupBase::GroupBase(GroupBase* parent,
                     const std::string& name,
                     const std::string& description) {
    parent_ = parent;
    name_ = name;
    description_ = description;
    required_ = false;
    lastPosParsed_ = 0;
}

GroupBase::~GroupBase() {
    for (std::vector<PositionBase*>::const_iterator
         it = positions_.begin(); it != positions_.end(); ++it) {
        delete *it;
    }
    for (std::vector<OptionBase*>::const_iterator
         it = options_.begin(); it != options_.end(); ++it) {
        delete *it;
    }
}

GroupBase& GroupBase::required() {
    if (isMutuallyExclusive()) {
        required_ = true;
    }
    return *this;
}

GroupBase& GroupBase::description(const std::string& desc) {
    description_ = desc;
    return *this;
}

std::size_t GroupBase::numChilds() const {
    return 0;
}

std::size_t GroupBase::numPositions() const {
    return positions_.size();
}

std::size_t GroupBase::numOptions() const {
    return options_.size();
}

const GroupBase& GroupBase::child(const std::size_t& i) const {
    throw std::out_of_range("GroupBase::child");
}

const PositionBase& GroupBase::position(const std::size_t& i) const {
    return *positions_.at(i);
}

const OptionBase& GroupBase::option(const std::size_t& i) const {
    return *options_.at(i);
}

std::size_t GroupBase::numChildPositions() const {
    return numPositions();
}

const PositionBase& GroupBase::childPosition(const std::size_t& i) const {
    return position(i);
}

const OptionBase& GroupBase::childOption(const std::string& name) const {
    return *optionName_.at(name);
}

bool GroupBase::existsOption(const char& id) const {
    return (shortOpts_.count(id) != 0);
}

bool GroupBase::existsOption(const std::string& id,
                             const bool& abbrev) const {
    if (longOpts_.count(id) != 0) {
        return true;
    }
    if (!abbrev) {
        return false;
    }
    for (std::map<std::string, std::string>::const_iterator
            it = longOpts_.begin(); it != longOpts_.end(); ++it) {
        if (it->first.size() < id.size()) {
            continue;
        }
        std::string abbrev = it->first.substr(0, id.size());
        if (id == abbrev) {
            return true;
        }
    }
    return false;
}

const std::string& GroupBase::optionName(const char& id) const {
    return shortOpts_.at(id);
}

const std::string& GroupBase::optionName(const std::string& id) const {
    return longOpts_.at(id);
}

std::vector<std::string> GroupBase::possibleOptions(const std::string& id,
                                                    const bool& abbrev) const {
    std::vector<std::string> res;
    if (longOpts_.count(id) != 0) {
        res.push_back(longOpts_.at(id));
        return res;
    }
    if (!abbrev) {
        return res;
    }
    for (std::map<std::string, std::string>::const_iterator
         it = longOpts_.begin(); it != longOpts_.end(); ++it) {
        if (it->first.size() < id.size()) {
            continue;
        }
        std::string abbrevStr = it->first.substr(0, id.size());
        if (id == abbrevStr) {
            res.push_back(it->first);
        }
    }
    return res;
}

void GroupBase::parsePreprocess(Object& out) {
    out.setObject();
}

void GroupBase::parsePosition(Object& out,
                              std::vector<std::list<std::string>>& output,
                              std::vector<std::list<std::string>>& input) {
    Object aux;
    positions_.at(lastPosParsed_)->parse(aux, output, input);
    out.addMember(positions_.at(lastPosParsed_)->getName(), std::move(aux));
    ++lastPosParsed_;
}

void GroupBase::parseOption(const std::string& name,
                            Object& out,
                            std::vector<std::list<std::string>>& output,
                            std::vector<std::list<std::string>>& input) {
    std::string optName = optionName_.at(name)->getName();
    Object aux;
    optionName_.at(name)->parse(aux, output, input);
    optionParsed_.at(name) = true;
    out.addMember(optName, std::move(aux));
}

void GroupBase::parsePostprocess(Object& out) {
    if (isMutuallyExclusive()) {
        std::vector<Argument*> argsAll;
        std::vector<Argument*> argsParsed;
        for (std::size_t i = 0; i < positions_.size(); i++) {
            argsAll.push_back(positions_[i]);
            if (i < lastPosParsed_) {
                argsParsed.push_back(positions_[i]);
            }
        }
        for (std::map<std::string, OptionBase*>::const_iterator
             it = optionName_.begin(); it != optionName_.end(); ++it) {
            argsAll.push_back(it->second);
            if (optionParsed_.at(it->first)) {
                argsParsed.push_back(it->second);
            }
        }
        if (argsParsed.size() > 1) {
            throw Error::Group::Excluded(argsParsed);
        } else if (argsParsed.empty() && isRequired()) {
            throw Error::Group::Required(argsAll);
        }
    } else {
        for (std::size_t i = 0; i < positions_.size(); i++) {
            std::string optName = positions_[i]->getName();
            Object aux;
            positions_[i]->notParsed(aux);
            if (!aux.isNull()) {
                out.addMember(optName, std::move(aux));
            }
        }
        for (std::map<std::string, OptionBase*>::const_iterator
             it = optionName_.begin(); it != optionName_.end(); ++it) {
            if (!optionParsed_.at(it->first)) {
                std::string optName = it->second->getName();
                Object aux;
                it->second->notParsed(aux);
                if (!aux.isNull()) {
                    out.addMember(optName, std::move(aux));
                }
            }
        }
    }
}

PositionBase& GroupBase::addPosition(PositionBase* arg) {
    if (arg == nullptr) {
        throw Error::Group::NullArgument();
    }
    try {
        addPositionProcess(nullptr, arg);
    } catch (...) {
        delete arg;
        throw;
    }
    return *arg;
}

OptionBase& GroupBase::addOption(OptionBase* arg) {
    if (arg == nullptr) {
        throw Error::Group::NullArgument();
    }
    try {
        addOptionProcess(nullptr, arg);
    } catch (...) {
        delete arg;
        throw;
    }
    return *arg;
}

void GroupBase::addPositionProcess(GroupBase* child, PositionBase* pos) {
    try {
        if (child == nullptr) {
            insertName(pos->getName());
            positions_.push_back(pos);
        } else if (child->isMutuallyExclusive()) {
            insertName(pos->getName());
        }
        if (parent_ != nullptr) {
            parent_->addPositionProcess(this, pos);
        }
    } catch (...) {
        if (names_.count(pos->getName()) != 0) {
            names_.erase(pos->getName());
        }
        if ((child == nullptr) && !positions_.empty()) {
            if (positions_.back() == pos) {
                positions_.pop_back();
            }
        }
        throw;
    }
}

void GroupBase::addOptionProcess(GroupBase* child, OptionBase* opt) {
    std::string name;
    if (opt->hasShortIdentifier()) {
        name = std::string(1, opt->getShortIdentifier());
    } else {
        name = opt->getLongIdentifier();
    }
    try {
        if (opt->hasShortIdentifier()) {
            insertShortOpt(opt->getShortIdentifier(), name);
        }
        if (opt->hasLongIdentifier()) {
            insertLongOpt(opt->getLongIdentifier(), name);
        }
        if (child == nullptr) {
            insertName(opt->getName());
            if (optionName_.count(name) == 0) {
                options_.push_back(opt);
                optionName_[name] = opt;
                optionParsed_[name] = false;
            } else {
                throw Error::Group::Repeated(name);
            }
        } else if (child->isMutuallyExclusive()) {
            insertName(opt->getName());
        }
        if (parent_ != nullptr) {
            parent_->addOptionProcess(this, opt);
        }
    } catch (...) {
        if (names_.count(opt->getName()) != 0) {
            names_.erase(opt->getName());
        }
        if (opt->hasShortIdentifier() &&
            (shortOpts_.count(opt->getShortIdentifier()))) {
            shortOpts_.erase(opt->getShortIdentifier());
        }
        if (opt->hasLongIdentifier() &&
            (longOpts_.count(opt->getLongIdentifier()))) {
            longOpts_.erase(opt->getLongIdentifier());
        }
        if ((child == nullptr) && !options_.empty()) {
            if (options_.back() == opt) {
                options_.pop_back();
                if (optionName_.count(name) != 0) {
                    optionName_.erase(name);
                }
                if (optionParsed_.count(name) != 0) {
                    optionParsed_.erase(name);
                }
            }
        }
        throw;
    }
}

void GroupBase::insertName(const std::string& name) {
    if (names_.count(name) != 0) {
        throw Error::Group::Repeated(name);
    }
    names_.insert(name);
}

void GroupBase::insertShortOpt(const char& opt, const std::string& name) {
    if (shortOpts_.count(opt) != 0) {
        throw Error::Group::Repeated(std::string("-") + std::string(1,opt));
    }
    shortOpts_[opt] = name;
}

void GroupBase::insertLongOpt(const std::string& opt,
                              const std::string& name) {
    if (longOpts_.count(opt) != 0) {
        throw Error::Group::Repeated(std::string("--") + opt);
    }
    longOpts_[opt] = name;
}

} /* namespace Argument */
} /* namespace SEMBA */
