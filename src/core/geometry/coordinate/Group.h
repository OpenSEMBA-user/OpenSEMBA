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
class Group {
public:
    typedef IdentifiableUnique<C> Container;
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    Group() = default;
    
    Group(const std::vector<Math::CVecR3>&);
    Group(const std::vector<Math::CVecI3>&);
    
    Group(const Group<C>&);

    Group(Group<C>&&) noexcept = default;
    
    Group<C>& operator=(const Group<C>& rhs);
    Group<C>& operator=(Group<C>&& rhs) = default;

    virtual ~Group() = default;

    iterator begin() { return coords_.begin(); }
    iterator end() { return coords_.end(); }

    const_iterator begin() const { return coords_.begin(); }
    const_iterator end() const { return coords_.end(); }

    iterator getId(const Id& id) { return coords_.getId(id); }
    const_iterator getId(const Id& id) const { return coords_.getId(id); }

    bool existId(const Id& id) const { return coords_.existId(id); }
    std::size_t size() const { return coords_.size(); }

    const CoordR3* getPos(const Math::CVecR3& pos) const;
    const CoordI3* getPos(const Math::CVecI3& pos) const;

    template<typename VEC>
    void addPos(VEC);

    void add(const std::unique_ptr<C>&);
    void add(std::unique_ptr<C>&&);

    void applyScalingFactor(const Math::Real factor);
   
private:
    std::multiset<const CoordR3*, CoordComparator> indexUnstr_;
    std::multiset<const CoordI3*, CoordComparator> indexStr_;
    IdentifiableUnique<C> coords_;

    void updateIndices();
    void updateIndexEntry(const C*);
};

} /* namespace Coordinate */

typedef Coordinate::Group<CoordR3> CoordR3Group;
typedef Coordinate::Group<CoordI3> CoordI3Group;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

