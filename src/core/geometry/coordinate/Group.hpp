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
    postprocess_(0);
}

template<typename C>
Group<C>& Group<C>::operator=(const Group<C>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    IdentifiableUnique<C>::operator=(rhs);
    postprocess_(0);
    return *this;
}

template<typename C>
Group<C>::Group(Group&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    IdentifiableUnique<C>::operator=(std::move(rhs));
    postprocess_(0);
    return *this;
}

template<typename C>
Group<C>& Group<C>::operator=(Group<C>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    IdentifiableUnique<C>::operator=(std::move(rhs));
    postprocess_(0);
    return *this;
}

template<typename C>
const CoordR3* Group<C>::getPos(const Math::CVecR3& position) const {
    std::multiset<const CoordR3*, CoordComparator>::iterator it;
    CoordR3 aux(position);
    it = indexUnstr_.find(&aux);
    if (it != indexUnstr_.end()) {
        return *it;
    } else {
        return nullptr;
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
    for (auto& item : this->items_) {
        if (this->get(i)->template is<CoordR3>()) {
            CoordR3* ptr = this->get(i)->template castTo<CoordR3>();
            *ptr *= factor;
        }
    }
}

template<typename C>
void Group<C>::postprocess_(const std::size_t fistStep) {
    for (std::size_t i = fistStep; i < this->size(); i++) {
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
