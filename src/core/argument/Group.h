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

#ifndef SEMBA_ARGUMENT_GROUP_H_
#define SEMBA_ARGUMENT_GROUP_H_

#include "GroupBase.h"

namespace SEMBA {
namespace Argument {

template <bool S = true>
class Group : public GroupBase {
    template <bool S2>
    friend class Group;
public:
    virtual ~Group();

    bool isRequired() const;
    bool isMutuallyExclusive() const { return !S; }

    Group& required();

    template <bool S2 = S, typename T = typename std::enable_if<S2>::type>
    Group<true >& addGroup(const std::string& = std::string(),
                           const std::string& = std::string());
    template <bool S2 = S, typename T = typename std::enable_if<S2>::type>
    Group<false>& addMutuallyExclusiveGroup();

protected:
    std::size_t numMutExc_;

    Group(Group<true>* = NULL,
          const std::string& = std::string(),
          const std::string& = std::string());

private:
    //Erased
    Group();
    Group(const Group&);
    Group(Group&&);

    Group& operator=(const Group&);
    Group& operator=(Group&&);
};

typedef Group<false> MutuallyExclusiveGroup;

} /* namespace Argument */
} /* namespace SEMBA */

#include "Group.hpp"

#endif /* SEMBA_ARGUMENT_GROUP_H_ */
