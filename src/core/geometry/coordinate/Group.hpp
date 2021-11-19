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
Group<C>::Group(const Group<C>& rhs) : 
    IdentifiableUnique<C>(rhs)
{
    updateIndices();
}

template<typename C>
Group<C>& Group<C>::operator=(const Group<C>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    IdentifiableUnique<C>::operator=(rhs);
    updateIndices();
    return *this;
}

template<typename C>
Group<C>& Group<C>::operator=(Group<C>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    IdentifiableUnique<C>::operator=(std::move(rhs));
    updateIndices();
    return *this;
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

template<typename C>
std::vector<const C*> Group<C>::getAllInPos(const Math::CVecI3& pos) const {
	std::multiset<const CoordI3*, CoordComparator>::iterator it;
	CoordI3 aux(pos);
	SEMBA::Group::Group<const C> res;
	it = indexStr_.find(&aux);
	while (it != indexStr_.end()) {
		if ((*it)->pos() == pos) {
			res.add(*it);
			++it;
		} else {
			break;
		}
	}
	return res;
}

template<typename C> template<typename VEC>
void Group<C>::addPos(const VEC& newPosition, const bool canOverlap) 
{
    //if (canOverlap || (getPos(newPosition) == nullptr)) {
    //    if constexpr ((std::is_same<C, CoordI3> && std::is_same<VEC, Math::CVecI3>) ||
    //                  (std::is_same<C, CoordR3> && std::is_same<VEC, Math::CVecR3>)) {
    //        addWithNewId(std::make_unique<C>(newPosition));
    //    }
    //    else {
    //        __builtin_unreachable();
    //    }
    //} 
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
        if (c->is<CoordR3>()) {
            indexUnstr_.insert(c.get()->castTo<CoordR3>());
        }
        if (c->is<CoordI3>()) {
            indexStr_.insert(c.get()->castTo<CoordI3>());
        }
    }
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
