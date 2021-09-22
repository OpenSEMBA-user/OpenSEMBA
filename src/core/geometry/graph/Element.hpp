

#include "Element.h"

#include <set>

namespace SEMBA {
namespace Geometry {
namespace Graph {

template<class ELEM, class BOUND>
Element<ELEM,BOUND>::Element(const ELEM* elem, const std::size_t& numBound) {
    vis_  = false;
    elem_ = elem;
    bounds_.resize(numBound);
}

template<class ELEM, class BOUND>
Element<ELEM,BOUND>::Element(const Element& rhs) {
    vis_       = rhs.vis_;
    elem_      = rhs.elem_;
    bounds_    = rhs.bounds_;
    neighbors_ = rhs.neighbors_;
    boundNeighbors_ = rhs.boundNeighbors_;
}

template<class ELEM, class BOUND>
Element<ELEM,BOUND>* Element<ELEM,BOUND>::clone() const {
    return new Element(*this);
}

template<class ELEM, class BOUND>
std::vector<const typename Element<ELEM,BOUND>::GraphBound*>
    Element<ELEM,BOUND>::getBounds() const {
    return std::vector<const GraphBound*>(bounds_.begin(), bounds_.end());
}

template<class ELEM, class BOUND>
void Element<ELEM,BOUND>::setBounds(
    typename Element<ELEM,BOUND>::GraphBound* bound) {
    bounds_.resize(1);
    bounds_[0] = bound;
}

template<class ELEM, class BOUND>
const typename Element<ELEM,BOUND>::GraphElem*
    Element<ELEM,BOUND>::getBoundNeighbor(std::size_t i, std::size_t j) const {
    return boundNeighbors_[i][j];
}

template<class ELEM, class BOUND>
typename Element<ELEM,BOUND>::GraphElem*
    Element<ELEM,BOUND>::getBoundNeighbor(std::size_t i, std::size_t j) {
    return boundNeighbors_[i][j];
}

template<class ELEM, class BOUND>
void Element<ELEM,BOUND>::constructNeighbors() {
    boundNeighbors_.clear();
    boundNeighbors_.resize(bounds_.size());
    for (std::size_t i = 0; i < bounds_.size(); i++) {
        for (std::size_t j = 0; j < bounds_[i]->numBounds(); j++) {
            if (bounds_[i]->getBound(j)->elem()->getId() != elem_->getId()) {
                boundNeighbors_[i].push_back(bounds_[i]->getBound(j));
            }
        }
    }
    std::set<GraphElem*> neighbors;
    for (std::size_t i = 0; i < boundNeighbors_.size(); i++) {
        for (std::size_t j = 0; j < boundNeighbors_[i].size(); j++) {
            neighbors.insert(boundNeighbors_[i][j]);
        }
    }
    neighbors_ = std::vector<GraphElem*>(neighbors.begin(), neighbors.end());
}

template<class ELEM, class BOUND>
void Element<ELEM,BOUND>::printInfo() const {
    std::cout << "--- Element Info ---" << std::endl;
    elem_->printInfo();
    std::cout << "Bounds:";
    for (std::size_t i = 0; i < bounds_.size(); ++i) {
        std::cout << " " << bounds_[i]->elem()->getId();
    }
    std::cout << std::endl;
    if (!neighbors_.empty()) {
        std::cout << "Neighbors:";
        for (std::size_t i = 0; i < neighbors_.size(); ++i) {
            std::cout << " " << neighbors_[i]->elem()->getId();
        }
        std::cout << std::endl;
        std::cout << "Bound Neighbors:" << std::endl;
        for (std::size_t i = 0; i < boundNeighbors_.size(); ++i) {
            std::cout << bounds_[i]->elem()->getId() << ":";
            for (std::size_t j = 0; j < boundNeighbors_[i].size(); ++j) {
                std::cout << " " << boundNeighbors_[i][j]->elem()->getId();
            }
            std::cout << std::endl;
        }
    }
}

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */
