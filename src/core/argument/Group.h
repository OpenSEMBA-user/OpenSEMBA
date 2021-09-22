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

#include "MutuallyExclusiveGroup.h"

namespace SEMBA {
namespace Argument {

class Group : public GroupBase {
public:
    virtual ~Group();

    bool isMutuallyExclusive() const { return false; }

    Group& required();
    Group& description(const std::string& desc);

    std::size_t  numChilds() const;
    const GroupBase& child(const std::size_t& i) const;

    std::size_t      numChildPositions() const;
    const PositionBase& childPosition(const std::size_t&) const;
    const OptionBase&   childOption  (const std::string&) const;

    Group& addGroup(const std::string& = std::string(),
                    const std::string& = std::string());
    MEGroup& addMutuallyExclusiveGroup();

    void parsePreprocess(Object&);
    void parsePosition(Object&,
                       std::vector<std::list<std::string>>&,
                       std::vector<std::list<std::string>>&);
    void parseOption(const std::string&,
                     Object&,
                     std::vector<std::list<std::string>>&,
                     std::vector<std::list<std::string>>&);
    void parsePostprocess(Object&);

protected:
    Group(GroupBase* = nullptr,
          const std::string& = std::string(),
          const std::string& = std::string());

    void addPositionProcess(GroupBase*, PositionBase*);
    void addOptionProcess  (GroupBase*, OptionBase*);

private:
    std::size_t numMutExc_;

    std::vector<GroupBase*> positions_;
    std::size_t lastPosParsed_;

    std::map<std::string, GroupBase*> options_;

    std::vector<GroupBase*> child_;

    //Erased
    Group(const Group&);
    Group(Group&&);

    Group& operator=(const Group&);
    Group& operator=(Group&&);
};

} /* namespace Argument */
} /* namespace SEMBA */

