#pragma once

#include <type_traits>
#include <map>

#ifdef _MSC_VER
#pragma warning(disable:4250)
#endif

#include "Coordinate.h"
#include "group/GroupIdentifiableUnique.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

struct CoordComparator {
    template<class T, std::size_t D>
    bool operator() (const Coordinate<T,D>* lhs,
                     const Coordinate<T,D>* rhs) const {
        return (lhs->pos() < rhs->pos());
    }
};

template<typename C = Coord>
class Group final : public GroupIdentifiableUnique<C> {
public:
    Group() = default;
    
    Group(const std::vector<Math::CVecR3>&);
    Group(const std::vector<Math::CVecI3>&);
    
    template<typename VEC>
    typename GroupIdentifiableUnique<C>::iterator  addPos(VEC);

    void applyScalingFactor(const Math::Real factor);

    template<typename VEC>
    std::map<VEC, std::vector<const C*>> getIndex() const;
};

} /* namespace Coordinate */

typedef Coordinate::Group<CoordR3> CoordR3Group;
typedef Coordinate::Group<CoordI3> CoordI3Group;
typedef Coordinate::Group<> CoordGroup;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

