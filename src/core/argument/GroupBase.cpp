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
    parent_      = parent;
    required_    = false;
    name_        = name;
    description_ = description;
    lastPosParsed_ = 0;
}

GroupBase::~GroupBase() {
    for (std::vector<PositionBase*>::const_iterator
         it = positions_.begin(); it != positions_.end(); ++it) {
        delete *it;
    }
    positions_.clear();
    positionsFull_.clear();
    for (std::vector<OptionBase*>::const_iterator
         it = options_.begin(); it != options_.end(); ++it) {
        delete *it;
    }
    options_.clear();
    optionsName_.clear();
}

std::size_t GroupBase::numPositions() const {
    return positions_.size();
}

const PositionBase& GroupBase::position(const std::size_t& i) const {
    return *positions_.at(i);
}

std::size_t GroupBase::numOptions() const {
    return options_.size();
}

const OptionBase& GroupBase::option(const std::size_t& i) const {
    return *options_.at(i);
}

void GroupBase::parsePreprocess(Object& out) const {
    out.setObject();
    for (std::size_t i = 0; i < child_.size(); i++) {
        if (!child_[i]->isMutuallyExclusive()) {
            Object aux;
            aux.setObject();
            child_[i]->parsePreprocess(aux);
            out.addMember(child_[i]->getName(), std::move(aux));
        }
    }
}

void GroupBase::parsePosition(Object& out,
                              std::vector<std::list<std::string>>& output,
                              std::vector<std::list<std::string>>& input) {
    if (positionsFull_.at(lastPosParsed_).first == NULL) {
        const std::string& name =
            positionsFull_.at(lastPosParsed_).second->getName();
        if (positionsFull_.at(lastPosParsed_).second->isMutuallyExclusive()) {
            positionsFull_.at(lastPosParsed_).second->parsePosition(out,
                                                                output,
                                                                input);
        } else {
            positionsFull_.at(lastPosParsed_).second->parsePosition(out[name],
                                                                output,
                                                                input);
        }
    } else {
        Object aux;
        positionsFull_.at(lastPosParsed_).first->parse(aux, output, input);
        out.addMember(positionsFull_.at(lastPosParsed_).first->getName(),
                      std::move(aux));
    }
    ++lastPosParsed_;
}

void GroupBase::parseOption(std::list<std::string>& name,
                            Object& out,
                            std::vector<std::list<std::string>>& output,
                            std::vector<std::list<std::string>>& input) {
    if (name.size() > 1) {
        std::string groupName = name.front();
        name.pop_front();
        GroupBase* child = child_[childName_.at(groupName)];
        if (child->isMutuallyExclusive()) {
            child->parseOption(name, out, output, input);
        } else {
            child->parseOption(name, out[groupName], output, input);
        }
    } else {
        std::string optName = name.front();
        Object aux;
        optionsName_. at(optName)->parse(aux, output, input);
        optionParsed_.at(optName) = true;
        out.addMember(optName, std::move(aux));
    }
}

void GroupBase::parseLastPosition(Object& out,
                                  std::vector<std::list<std::string>>& output,
                                  std::vector<std::list<std::string>>& input) {
    if (positionsFull_.back().first == NULL) {
        const std::string& name =
            positionsFull_.back().second->getName();
        if (positionsFull_.back().second->isMutuallyExclusive()) {
            positionsFull_.back().second->parsePosition(out,
                                                        output,
                                                        input);
        } else {
            positionsFull_.back().second->parsePosition(out[name],
                                                        output,
                                                        input);
        }
    } else {
        Object aux;
        positionsFull_.back().first->parse(aux, output, input);
        out.addMember(positionsFull_.back().first->getName(),
                      std::move(aux));
    }
    lastPosParsed_ = positionsFull_.size();
}

void GroupBase::parsePostprocess(Object& out) const {
    if (isMutuallyExclusive()) {
        std::vector<Argument*> argsAll;
        std::vector<Argument*> argsParsed;
        for (std::size_t i = 0; i < positionsFull_.size(); i++) {
            if (positionsFull_[i].first != NULL) {
                argsAll.push_back(positionsFull_[i].first);
                if (i < lastPosParsed_) {
                    argsParsed.push_back(positionsFull_[i].first);
                }
            }
        }
        for (std::map<std::string, OptionBase*>::const_iterator
             it = optionsName_.begin(); it != optionsName_.end(); ++it) {
            argsAll.push_back(it->second);
            if (optionParsed_.at(it->first)) {
                argsParsed.push_back(it->second);
            }
        }
        if (argsParsed.size() > 1) {
            throw Error::Excluded(argsParsed);
        } else if (argsParsed.empty() && isRequired()) {
            throw Error::GroupRequired(argsAll);
        }
    }
    if (!isMutuallyExclusive()) {
        for (std::size_t i = lastPosParsed_; i < positionsFull_.size(); i++) {
            if (positionsFull_[i].first != NULL) {
                std::string optName = positionsFull_[i].first->getName();
                Object aux;
                positionsFull_[i].first->noParsed(aux);
                out.addMember(optName, std::move(aux));
            }
        }
        for (std::map<std::string, OptionBase*>::const_iterator
             it = optionsName_.begin(); it != optionsName_.end(); ++it) {
            if (!optionParsed_.at(it->first)) {
                std::string optName = it->second->getName();
                Object aux;
                it->second->noParsed(aux);
                out.addMember(optName, std::move(aux));
            }
        }
        for (std::size_t i = 0; i < child_.size(); i++) {
            if (child_[i]->isMutuallyExclusive()) {
                child_[i]->parsePostprocess(out);
            } else {
                child_[i]->parsePostprocess(out[child_[i]->getName()]);
            }
        }
    }
}

void GroupBase::registerPosition_(GroupBase* child,
                                  std::list<std::string>& name) {
    if (child == NULL) {
        insertName_(name.front());
    } else if (child->isMutuallyExclusive()) {
        insertName_(name.back());
    }
    if (child != NULL) {
        positionsFull_.push_back(
            std::pair<PositionBase*, GroupBase*>(NULL, child));
    }
    name.push_front(getName());
    if (parent_ != NULL) {
        parent_->registerPosition_(this, name);
    }
}

void GroupBase::registerOption_(GroupBase* child,
                                std::list<std::string>& name,
                                const char& shortId) {
    try {
        if (child == NULL) {
            insertName_(name.front());
        } else if (child->isMutuallyExclusive()) {
            insertName_(name.back());
        }
        insertShortOpt_(name, shortId);
    } catch (...) {
        if (name.size() == 1) {
            names_.erase(name.front());
        }
        shortOpts_.erase(shortId);
        throw;
    }
    name.push_front(getName());
    if (parent_ != NULL) {
        parent_->registerOption_(this, name, shortId);
    }
}

void GroupBase::registerOption_(GroupBase* child,
                                std::list<std::string>& name,
                                const std::string& longId) {
    try {
        if (child == NULL) {
            insertName_(name.front());
        } else if (child->isMutuallyExclusive()) {
            insertName_(name.back());
        }
        insertLongOpt_(name, longId);
    } catch (...) {
        if (name.size() == 1) {
            names_.erase(name.front());
        }
        longOpts_.erase(longId);
        throw;
    }
    longOpts_[longId] = name;
    name.push_front(getName());
    if (parent_ != NULL) {
        parent_->registerOption_(this, name, longId);
    }
}

void GroupBase::registerOption_(GroupBase* child,
                                std::list<std::string>& name,
                                const char& shortId,
                                const std::string& longId) {
    try {
        if (child == NULL) {
            insertName_(name.front());
        } else if (child->isMutuallyExclusive()) {
            insertName_(name.back());
        }
        insertShortOpt_(name, shortId);
        insertLongOpt_ (name, longId);
    } catch (...) {
        if (name.size() == 1) {
            names_.erase(name.front());
        }
        shortOpts_.erase(shortId);
        longOpts_. erase(longId);
        throw;
    }
    name.push_front(getName());
    if (parent_ != NULL) {
        parent_->registerOption_(this, name, shortId, longId);
    }
}

void GroupBase::insertName_(const std::string& name) {
    if (names_.count(name) != 0) {
        throw Error::Repeated(name);
    }
    names_.insert(name);
}

void GroupBase::insertShortOpt_(const std::list<std::string>& name,
                                const char& shortId) {
    if (shortOpts_.count(shortId) != 0) {
        throw Error::Repeated(std::string("-")+std::string(1,shortId));
    }
    shortOpts_[shortId] = name;
}

void GroupBase::insertLongOpt_(const std::list<std::string>& name,
                               const std::string& longId) {
    if (longOpts_.count(longId) != 0) {
        throw Error::Repeated(std::string("--")+std::string(longId));
    }
    longOpts_[longId] = name;
}

PositionBase* GroupBase::getPosition_(const std::size_t& i) const {
    if (positionsFull_.at(i).first != NULL) {
        return positionsFull_.at(i).first;
    }
    std::size_t child_i = 0;
    for (std::size_t j = 0; j < i; j++) {
        if (positionsFull_.at(i).second == positionsFull_.at(j).second) {
            child_i++;
        }
    }
    return positionsFull_.at(i).second->getPosition_(child_i);
}

OptionBase* GroupBase::getOption_(std::list<std::string>& name) const {
    if (name.size() > 1) {
        std::string groupName = name.front();
        name.pop_front();
        return child_[childName_.at(groupName)]->getOption_(name);
    }
    std::string optName = name.front();
    return optionsName_.at(optName);
}

bool GroupBase::isLastPosMulti_() const {
    if (positionsFull_.empty()) {
        return false;
    }
    if (positionsFull_.back().first == NULL) {
        if (positionsFull_.back().second == NULL) {
            return false;
        }
        return positionsFull_.back().second->isLastPosMulti_();
    } else {
        return dynamic_cast<ValueMulti*>(positionsFull_.back().first) != NULL;
    }
    return false;
}

} /* namespace Argument */
} /* namespace SEMBA */
