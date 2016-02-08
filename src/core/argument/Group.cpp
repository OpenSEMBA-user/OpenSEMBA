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

#include "Group.h"

namespace SEMBA {
namespace Argument {

Group::Group(GroupBase* group,
             const std::string& name,
             const std::string& desc)
:   GroupBase(group, name, desc) {
    numMutExc_ = 0;
    lastPosParsed_ = 0;
}

Group::~Group() {

}

Group& Group::required() {
    GroupBase::required();
    return *this;
}

Group& Group::description(const std::string& desc) {
    GroupBase::description(desc);
    return *this;
}

std::size_t Group::numChilds() const {
    return child_.size();
}

const GroupBase& Group::child(const std::size_t& i) const {
    return *child_[i];
}

std::size_t Group::numChildPositions() const {
    return positions_.size();
}

const PositionBase& Group::childPosition(const std::size_t& i) const {
    std::size_t numSameGroup = 0;
    std::vector<GroupBase*>::const_iterator aux = positions_.begin() + i;
    for (std::vector<GroupBase*>::const_iterator
         it = positions_.begin(); it < aux; ++it) {
        if (*it == *aux) {
            numSameGroup++;
        }
    }
    if (*aux == NULL) {
        return position(numSameGroup);
    }
    return (*aux)->childPosition(numSameGroup);
}

const OptionBase& Group::childOption(const std::string& name) const {
    if (options_.at(name) == NULL) {
        return GroupBase::childOption(name);
    }
    return options_.at(name)->childOption(name);
}

Group& Group::addGroup(const std::string& name,
                       const std::string& desc) {
    insertName(name);
    Group* newGroup = new Group(this, name, desc);

    child_.push_back(newGroup);

    return *newGroup;
}

MEGroup& Group::addMutuallyExclusiveGroup() {
    std::stringstream aux;
    aux << "__aux" << ++numMutExc_ << "__";
    std::string name = aux.str();
    insertName(aux.str());
    MEGroup* newGroup = new MEGroup(this, name);

    child_.push_back(newGroup);

    return *newGroup;
}

void Group::parsePreprocess(Object& out) {
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

void Group::parsePosition(Object& out,
                          std::vector<std::list<std::string>>& output,
                          std::vector<std::list<std::string>>& input) {
    if (positions_.at(lastPosParsed_) != NULL) {
        const std::string& name = positions_.at(lastPosParsed_)->getName();
        if (positions_.at(lastPosParsed_)->isMutuallyExclusive()) {
            positions_.at(lastPosParsed_)->parsePosition(out, output, input);
        } else {
            positions_.at(lastPosParsed_)->parsePosition(out(name),
                                                         output, input);
        }
    } else {
        GroupBase::parsePosition(out, output, input);
    }
    ++lastPosParsed_;
}

void Group::parseOption(const std::string& name,
                        Object& out,
                        std::vector<std::list<std::string>>& output,
                        std::vector<std::list<std::string>>& input) {
    if (options_.at(name) != NULL) {
        GroupBase* child = options_.at(name);
        std::string childName = child->getName();
        if (child->isMutuallyExclusive()) {
            child->parseOption(name, out, output, input);
        } else {
            child->parseOption(name, out(childName), output, input);
        }
    } else {
        GroupBase::parseOption(name, out, output, input);
    }
}

void Group::parsePostprocess(Object& out) {
    GroupBase::parsePostprocess(out);
    for (std::size_t i = 0; i < child_.size(); i++) {
        if (child_[i]->isMutuallyExclusive()) {
            child_[i]->parsePostprocess(out);
        } else {
            child_[i]->parsePostprocess(out(child_[i]->getName()));
        }
    }
}

void Group::addPositionProcess(GroupBase* child, PositionBase* pos) {
    GroupBase::addPositionProcess(child, pos);
    positions_.push_back(child);
}

void Group::addOptionProcess(GroupBase* child, OptionBase* opt) {
    std::string name;
    if (opt->hasShortIdentifier()) {
        name = std::string(1, opt->getShortIdentifier());
    } else {
        name = opt->getLongIdentifier();
    }
    GroupBase::addOptionProcess(child, opt);
    options_[name] = child;
}

} /* namespace Argument */
} /* namespace SEMBA */
