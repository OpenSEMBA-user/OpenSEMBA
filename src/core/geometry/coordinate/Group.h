

#pragma once

#include <set>

#ifdef _MSC_VER
#pragma warning(disable:4250)
#endif

#include "Coordinate.h"
#include "group/Cloneable.h"
#include "group/Identifiable.h"

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
class Group : public SEMBA::Group::Cloneable<C>,
              public SEMBA::Group::Identifiable<C, Id> {
public:
    Group();
    Group(const std::vector<Math::CVecR3>&);
    Group(const std::vector<Math::CVecI3>&);
    template<typename C2>
    Group(C2*);
    template<typename C2>
    Group(const std::vector<C2*>&);
    template<typename C2>
    Group(SEMBA::Group::Group<C2>&);
    template<typename C2>
    Group(const SEMBA::Group::Group<C2>&);
    Group(SEMBA::Group::Group<C>&);
    template<typename C2>
    Group(SEMBA::Group::Group<C2>&&);
    Group(SEMBA::Group::Group<C>&&);
    virtual ~Group();

    SEMBA_GROUP_DEFINE_CLONE(Group, C);

    Group& operator=(SEMBA::Group::Group<C>&);
    Group& operator=(SEMBA::Group::Group<C>&&);

    void clear();

    const CoordR3* getPos(const Math::CVecR3& pos) const;
    const CoordI3* getPos(const Math::CVecI3& pos) const;

	SEMBA::Group::Group<const C> getAllInPos(const Math::CVecI3& pos) const;


    using SEMBA::Group::Identifiable<C,Id>::add;
    SEMBA::Group::Group<C> add(SEMBA::Group::Group<C>&);
    SEMBA::Group::Group<C> add(SEMBA::Group::Group<C>&&);
    
    const C*               addPos(const Math::CVecR3&,
                                  const bool = false);
    SEMBA::Group::Group<C> addPos(const std::vector<Math::CVecR3>&,
                                  const bool = false);
    const C*               addPos(const Math::CVecI3&,
                                  const bool = false);
    SEMBA::Group::Group<C> addPos(const std::vector<Math::CVecI3>&,
                                  const bool = false);

    void remove(const std::size_t&);
    void remove(const std::vector<std::size_t>&);

    void applyScalingFactor(const Math::Real factor);
    
    void printInfo() const;

private:
    std::multiset<const CoordR3*, CoordComparator> indexUnstr_;
    std::multiset<const CoordI3*, CoordComparator> indexStr_;

    void postprocess_(const std::size_t i);
};

} /* namespace Coordinate */

typedef Coordinate::Group<CoordR3> CoordR3Group;
typedef Coordinate::Group<CoordI3> CoordI3Group;
typedef Coordinate::Group<const CoordI3> ConstCoordI3Group;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

