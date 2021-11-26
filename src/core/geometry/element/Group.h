#pragma once

#include "Element.h"
#include "Node.h"
#include "Line.h"
#include "Surface.h"
#include "Volume.h"

#include "group/IdentifiableUnique.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<typename E = Elem>
class Group final : public IdentifiableUnique<E> {
public:
    std::vector<const E*> getCoordId(const CoordId&) const;
	std::vector<const E*> getMatId(const MatId&) const;
    std::vector<const E*> getLayerId(const LayerId&) const ;
    std::vector<const E*> getMatLayerId(const MatId&, const LayerId&) const;
    
    BoxR3 getBound() const;
    
    template<class T>
    void reassignPointers(const Coordinate::Group<Coordinate::Coordinate<T,3>>& vNew);

    void reassignPointers(const LayerGroup& lNew);
    void reassignPointers(const PMGroup& mNew);
};

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Group.hpp"

namespace SEMBA {
namespace Geometry {

typedef Element::Group<ElemR> ElemRGroup;
typedef Element::Group<ElemI> ElemIGroup;
typedef Element::Group<SurfR> SurfRGroup;
typedef Element::Group<VolR> VolRGroup;

} /* namespace Geometry */
} /* namespace SEMBA */


