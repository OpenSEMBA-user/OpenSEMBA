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
namespace Geometry {
namespace Coordinate {

template<typename C>
Group<C>::Group() {

}

template<typename C>
Group<C>::Group(const std::vector<Math::CVecR3>& pos) {
    addPos(pos, true);
}

template<typename C>
Group<C>::Group(const std::vector<Math::CVecI3>& pos) {
    addPos(pos, true);
}

template<typename C> template<typename C2>
Group<C>::Group(C2* elem)
:   SEMBA::Group::Identifiable<C,Id>(elem) {
    postprocess_(0);
}

template<typename C> template<typename C2>
Group<C>::Group(const std::vector<C2*>& elems)
:   SEMBA::Group::Identifiable<C,Id>(elems) {
    postprocess_(0);
}

template<typename C> template<typename C2>
Group<C>::Group(SEMBA::Group::Group<C2>& rhs)
:   SEMBA::Group::Identifiable<C,Id>(rhs) {
    postprocess_(0);
}

template<typename C> template<typename C2>
Group<C>::Group(const SEMBA::Group::Group<C2>& rhs)
:   SEMBA::Group::Identifiable<C,Id>(rhs) {
    postprocess_(0);
}

template<typename C>
Group<C>::Group(SEMBA::Group::Group<C>& rhs)
:   SEMBA::Group::Identifiable<C,Id>(rhs) {
    postprocess_(0);
}

template<typename C> template<typename C2>
Group<C>::Group(SEMBA::Group::Group<C2>&& rhs)
:   SEMBA::Group::Identifiable<C,Id>(std::move(rhs)) {
    postprocess_(0);
}

template<typename C>
Group<C>::Group(SEMBA::Group::Group<C>&& rhs)
:   SEMBA::Group::Identifiable<C,Id>(std::move(rhs)) {
    postprocess_(0);
}

template<typename C>
Group<C>::~Group() {

}

template<typename C>
Group<C>& Group<C>::operator=(SEMBA::Group::Group<C>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<C,Id>::operator=(rhs);
    postprocess_(0);
    return *this;
}

template<typename C>
Group<C>& Group<C>::operator=(SEMBA::Group::Group<C>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    SEMBA::Group::Identifiable<C,Id>::operator=(std::move(rhs));
    postprocess_(0);
    return *this;
}

template<typename C>
void Group<C>::clear() {
    SEMBA::Group::Identifiable<C,Id>::clear();
    indexUnstr_.clear();
    indexStr_.clear();
}

template<typename C>
const CoordR3* Group<C>::getPos(const Math::CVecR3& position) const {
    std::multiset<const CoordR3*, CoordComparator>::iterator it;
    CoordR3 aux(position);
    it = indexUnstr_.find(&aux);
    if (it != indexUnstr_.end()) {
        return *it;
    } else {
        return NULL;
    }
}

template<typename C>
const CoordI3* Group<C>::getPos(const Math::CVecI3& position) const {
    std::multiset<const CoordI3*, CoordComparator>::iterator it;
    CoordI3 aux(position);
    it = indexStr_.find(&aux);
    if (it != indexStr_.end()) {
        return *it;
    } else {
        return NULL;
    }
}

template<typename C>
SEMBA::Group::Group<C> Group<C>::add(SEMBA::Group::Group<C>& rhs) {
    Size lastSize = this->size();
    SEMBA::Group::Identifiable<C,Id>::add(rhs);
    postprocess_(lastSize);
    return rhs;
}

template<typename C>
SEMBA::Group::Group<C> Group<C>::add(SEMBA::Group::Group<C>&& rhs) {
    Size lastSize = this->size();
    SEMBA::Group::Identifiable<C,Id>::add(std::move(rhs));
    postprocess_(lastSize);
    return rhs;
}

template<typename C>
const C* Group<C>::addPos(const Math::CVecR3& newPosition,
                          const bool canOverlap) {
    std::vector<Math::CVecR3> aux;
    aux.push_back(newPosition);
    SEMBA::Group::Group<C> res = addPos(aux, canOverlap);
    if (res.empty()) {
        return getPos(newPosition);
    }
    return res(0);
}

template<typename C>
SEMBA::Group::Group<C> Group<C>::addPos(const std::vector<Math::CVecR3>& newPos,
                                        const bool canOverlap) {
    std::vector<C*> newCoords;
    newCoords.reserve(newPos.size());
    for(Size i = 0; i < newPos.size(); i++) {
        if (canOverlap || (getPos(newPos[i]) == NULL)) {
            CoordR3* newCoord = new CoordR3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    return this->addId(newCoords);
}

template<typename C>
const C* Group<C>::addPos(const Math::CVecI3& newPosition,
                          const bool canOverlap) {
    std::vector<Math::CVecI3> aux;
    aux.push_back(newPosition);
    SEMBA::Group::Group<C> res = addPos(aux, canOverlap);
    if (res.empty()) {
        return getPos(newPosition);
    }
    return res(0);
}

template<typename C>
SEMBA::Group::Group<C> Group<C>::addPos(const std::vector<Math::CVecI3>& newPos,
                                        const bool canOverlap) {
    std::vector<C*> newCoords;
    for(Size i = 0; i < newPos.size(); i++) {
        if (canOverlap || (getPos(newPos[i]) == NULL)) {
            CoordI3* newCoord = new CoordI3(newPos[i]);
            if (newCoord->template is<C>()) {
                newCoords.push_back(newCoord->template castTo<C>());
            } else {
                delete newCoord;
            }
        }
    }
    return this->addId(newCoords);
}

template<typename C>
void Group<C>::remove(const Size& pos) {
    if (this->get(pos)->template is<CoordR3>()) {
        indexUnstr_.erase(this->get(pos)->template castTo<CoordR3>());
    }
    if (this->get(pos)->template is<CoordI3>()) {
        indexStr_.erase(this->get(pos)->template castTo<CoordI3>());
    }
    SEMBA::Group::Identifiable<C,Id>::remove(pos);
}

template<typename C>
void Group<C>::remove(const std::vector<Size>& pos) {
    for (Size i = 0; i < pos.size(); i++) {
        if (this->get(pos[i])->template is<CoordR3>()) {
            indexUnstr_.erase(this->get(pos[i])->template castTo<CoordR3>());
        }
        if (this->get(pos[i])->template is<CoordI3>()) {
            indexStr_.erase(this->get(pos[i])->template castTo<CoordI3>());
        }
    }
    SEMBA::Group::Identifiable<C,Id>::remove(pos);
}

template<typename C>
void Group<C>::applyScalingFactor(const Math::Real factor) {
    for(Size i = 0; i < this->size(); i++) {
        if (this->get(i)->template is<CoordR3>()) {
            CoordR3* ptr = this->get(i)->template castTo<CoordR3>();
            *ptr *= factor;
        }
    }
}

template<typename C>
void Group<C>::printInfo() const {
    std::cout<< "--- Group info ---" << std::endl;
    std::cout<< "Total: " << this->size() << " coordinates." << std::endl;
    SEMBA::Group::Printable<C>::printInfo();
}

template<typename C>
void Group<C>::postprocess_(const Size fistStep) {
    for (Size i = fistStep; i < this->size(); i++) {
        if (this->get(i)->template is<CoordR3>()) {
            indexUnstr_.insert(this->get(i)->template castTo<CoordR3>());
        }
        if (this->get(i)->template is<CoordI3>()) {
            indexStr_.insert(this->get(i)->template castTo<CoordI3>());
        }
    }
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
