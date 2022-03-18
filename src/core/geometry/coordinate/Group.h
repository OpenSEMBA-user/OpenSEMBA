#pragma once

#include <set>
#include <type_traits>

#ifdef _MSC_VER
#pragma warning(disable:4250)
#endif

#include "Coordinate.h"
#include "group/IdentifiableUnique.h"

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
class Group final : public IdentifiableUnique<C> {
public:
    Group() = default;
    
    Group(const std::vector<Math::CVecR3>&);
    Group(const std::vector<Math::CVecI3>&);
    
    Group(const Group<C>&);

    Group(Group<C>&&) noexcept = default;
    
    Group<C>& operator=(const Group<C>& rhs);
    Group<C>& operator=(Group<C>&& rhs) = default;

    virtual ~Group() = default;

    const CoordR3* getPos(const Math::CVecR3& pos) const;
    const CoordI3* getPos(const Math::CVecI3& pos) const;

    template<typename VEC>
    void addPos(VEC);

    typename IdentifiableUnique<C>::iterator add(const std::unique_ptr<C>&);
    typename IdentifiableUnique<C>::iterator add(std::unique_ptr<C>&&) final;
    typename IdentifiableUnique<C>::iterator addAndAssignId(std::unique_ptr<C>&& item) final;

    void applyScalingFactor(const Math::Real factor);
   
private:
    std::multiset<const CoordR3*, CoordComparator> indexUnstr_;
    std::multiset<const CoordI3*, CoordComparator> indexStr_;
    
    void updateIndices();
    void updateIndexEntry(const C*);
};

} /* namespace Coordinate */

typedef Coordinate::Group<CoordR3> CoordR3Group;
typedef Coordinate::Group<CoordI3> CoordI3Group;
typedef Coordinate::Group<> CoordGroup;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

