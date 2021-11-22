#include "Group.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

template<typename C>
Group<C>::Group(const std::vector<Math::CVecR3>& pos) 
{
    addPos(pos, true);
}

template<typename C>
Group<C>::Group(const std::vector<Math::CVecI3>& pos) 
{
    addPos(pos, true);
}

template<typename C> 
Group<C>::Group(const Group<C>& rhs) 
{
    coords_ = rhs.coords_;
    updateIndices();
}

template<typename C>
Group<C>& Group<C>::operator=(const Group<C>& rhs) {
    coords_ = rhs.coords_;
    updateIndices();
    return *this;
}


template<typename C>
void Group<C>::add(const std::unique_ptr<C>& coord)
{
    coords_.add(coord);
    auto it = coords_.getId(coord.get()->getId());
    updateIndexEntry(it->get());
}

template<typename C>
void Group<C>::add(std::unique_ptr<C>&& coord)
{
    auto ptr = coord.get();
    coords_.add(std::move(coord));
    updateIndexEntry(ptr);
}

template<typename C>
const CoordR3* Group<C>::getPos(const Math::CVecR3& position) const {
    CoordR3 aux(position);
    auto it = indexUnstr_.find(&aux);
    if (it != indexUnstr_.end()) {
        return *it;
    } else {
        return nullptr;
    }
}

template<typename C>
const CoordI3* Group<C>::getPos(const Math::CVecI3& position) const {
    CoordI3 aux(position);
    auto it = indexStr_.find(&aux);
    if (it != indexStr_.end()) {
        return *it;
    } else {
        return nullptr;
    }
}

template<typename C> template<typename VEC>
void Group<C>::addPos(VEC newPosition) 
{
    CoordId newId;
    if (this->size() == 0) {
        newId = CoordId(1);
    }
    else {
        auto backIt = --this->end();
        newId = ++(backIt->get()->getId());
    }
    add(std::make_unique<C>(newId, newPosition));
}

template<typename C>
void Group<C>::applyScalingFactor(const Math::Real factor) 
{
    for (auto& c : *this) {
        if (c->is<CoordR3>()) {
            *c *= factor;
        }
    }
}

template<typename C>
void Group<C>::updateIndices() {
    for (auto const& c: *this) {
        updateIndexEntry(c.get());
    }
}

template<typename C>
void Group<C>::updateIndexEntry(const C* c) {
    if (c->is<CoordR3>()) {
        indexUnstr_.insert(c->castTo<CoordR3>());
    }
    if (c->is<CoordI3>()) {
        indexStr_.insert(c->castTo<CoordI3>());
    }
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
