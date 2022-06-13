#include "Group.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

template<typename C>
Group<C>::Group(const std::vector<Math::CVecR3>& pos) 
{
    addPos(pos);
}

template<typename C>
Group<C>::Group(const std::vector<Math::CVecI3>& pos) 
{
    addPos(pos);
}

template<typename C> template<typename VEC>
typename IdentifiableUnique<C>::iterator 
Group<C>::addPos(VEC newPosition)
{
    CoordId newId;
    if (this->size() == 0) {
        newId = CoordId(1);
    }
    else {
        auto backIt = --this->end();
        newId = ++(backIt->get()->getId());
    }
    return add(std::make_unique<C>(newId, newPosition));
}

template<typename C>
void Group<C>::applyScalingFactor(const Math::Real factor) 
{
    for (auto& c : *this) {
        if (c->template is<CoordR3>()) {
            *c *= factor;
        }
    }
}

template<typename C> template<typename VEC>
std::map<VEC, std::vector<const C*>> Group<C>::getIndex() const {
    std::map<VEC, std::vector<const C*>> index;

    for (auto& c : *this) {
        auto cIt = index.find(c->pos());
        if (cIt == index.end()) {
            index.emplace(c->pos(), std::vector<const C*>({ c.get() }));

            continue;
        }

        auto& vector = cIt->second;
        vector.push_back(c.get());
    }

    return index;
}

} /* namespace Coordinate */
} /* namespace Geometry */
} /* namespace SEMBA */
