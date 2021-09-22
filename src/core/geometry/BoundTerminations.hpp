

#include "BoundTerminations.h"

namespace SEMBA {
namespace Geometry {

template <std::size_t D>
BoundTerminations<D>::BoundTerminations() {

}

template <std::size_t D>
BoundTerminations<D>::BoundTerminations(
    const std::array<std::array<const Bound*, 2>, D>& bounds) {
    setBounds(bounds);
}

template <std::size_t D>
BoundTerminations<D>::BoundTerminations(const BoundTerminations& rhs) {
    bounds_ = rhs.bounds_;
}

template <std::size_t D>
BoundTerminations<D>::~BoundTerminations() {

}

template <std::size_t D>
BoundTerminations<D>& BoundTerminations<D>::operator=(
        const BoundTerminations& rhs) {
    if (this == &rhs) {
        return *this;
    }
    bounds_ = rhs.bounds_;
    return *this;
}

template <std::size_t D>
const Bound* BoundTerminations<D>::operator()(std::size_t d,
                                              std::size_t p) const {
    if (bounds_[d][p]) {
        return bounds_[d][p].get();
    }
    return nullptr;
}

template <std::size_t D>
void BoundTerminations<D>::setBound(std::size_t d, std::size_t p,
                                    const Bound* bound) {
    if (bound != nullptr) {
        bounds_[d][p] =
            std::dynamic_pointer_cast<const Bound>(bound->getSharedPtr());
    } else {
        bounds_[d][p].reset();
    }
}

template <std::size_t D>
void BoundTerminations<D>::setBounds(
        const std::array<std::array<const Bound*, 2>, D>& bounds) {
    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t p = 0; p < 2; p++) {
            if (bounds[d][p] != nullptr) {
                bounds_[d][p] = bounds->getSharedPtr();
            }
        }
    }
}

} /* namespace Geometry */
} /* namespace SEMBA */
