#include "Group.h"

namespace SEMBA {
namespace Geometry {
namespace Layer {

template<typename L>
const L* Group<L>::getName(const std::string name) const {
    for (auto const& layer : this->items_) {
        if (layer->get()->getName() == name) {
            return layer->get();
        }
    }
    return nullptr;
}

} /* namespace Layer */
} /* namespace Geometry */
} /* namespace SEMBA */
