#include "Group.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

template<typename E>
std::vector<const E*> Group<E>::getCoordId(const CoordId& id) const 
{
	std::vector<const E*> res;
	for (auto const& item : *this) { 
		const E* elem = item;
		for (size_t j = 0; j < elem->numberOfCoordinates(); j++) {
			if (elem->getV(j)->getId() == id) {
				res.push_back(elem.get());
			}
		}
	}
	return res;
}

template<typename E>
std::vector<const E*> Group<E>::getMatId(const MatId& id) const
{
    std::vector<const E*> res;
    for (auto const& item : *this) {
        if (item->getMatId() == id) {
            res.push_back(item.get());
        }
    }
    return res;
}

template<typename E>
std::vector<const E*> Group<E>::getLayerId(const LayerId& id) const
{
    std::vector<const E*> res;
    for (auto const& item : *this) {
        if (item->getLayerId() == id) {
            res.push_back(item.get());
        }
    }
    return res;
}

template<typename E>
std::vector<const E*> Group<E>::getMatLayerId(const MatId& mId, const LayerId& lId) const
{
    std::vector<const E*> res;
    for (auto const& item : *this) {
        if (item->getMatId() == mId && item->getLayerId() == lId) {
            res.push_back(item.get());
        }
    }
    return res;
}

template<typename E>
BoxR3 Group<E>::getBound() const 
{
    if (this->size() == 0) {
        return BoxR3().setInfinity();
    }
    BoxR3 bound;
    for (auto const& elem: this->getOf<ElemR>()) {
        bound << elem->getBound();
    }
    for (auto const& elem: this->getOf<ElemI>()) {
        BoxI3 boxI = elem->getBound();
        Math::CVecI3 minP = boxI.getMin();
        Math::CVecI3 maxP = boxI.getMax();
        using Math::CVecR3;
        using namespace Math::Constants;
        bound << BoxR3(CVecR3(minP(x), minP(y), minP(z)), CVecR3(maxP(x), maxP(y), maxP(z)));
    }
    return bound;
}

template<typename E> template<class T>
void Group<E>::reassignPointers(const Coordinate::Group< Coordinate::Coordinate<T,3> >& vNew) 
{
    for (auto const& item: *this) {
        if (item->is<Element<T>>()) {
            Element<T>* elem = item->castTo<Element<T>>();
            for (std::size_t j = 0; j < elem->numberOfCoordinates(); j++) {
                CoordId vId = elem->getV(j)->getId();
                elem->setV(j, vNew.getId(vId));
            }
        }
    }
}

template<typename E>
void Group<E>::reassignPointers(const LayerGroup& lNew) 
{
    for (auto const& item: *this) {
        if (item->getLayer() != nullptr) {
            item->setLayer( lNew.getId( item->getLayerId() ) );
        }
    }
}

template<typename E>
void Group<E>::reassignPointers(const PMGroup& mNew) 
{
    for (auto const& item: *this) {
        if (item->getModel() != nullptr) {
            item->setModel( mNew.getId( item->getMatId() ) );
        }
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
