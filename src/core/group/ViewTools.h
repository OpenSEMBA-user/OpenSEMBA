#pragma once

#include <algorithm>
#include <vector>
#include <functional>

namespace SEMBA {
namespace Group {
namespace View {

    std::vector<const Geometry::ElemR*> filterView(
        const std::vector<const Geometry::ElemR*>& view,
        const std::function<bool(const Geometry::ElemR*)>& map
    ) {
        std::vector<const Geometry::ElemR*> res;

        std::copy_if(
            view.begin(),
            view.end(),
            std::back_inserter(res),
            map
        );

        return res;
    }

    // TODO: To update Structured mesh and consider using Integer inside and communicate with Real outside
    std::vector<const Geometry::ElemR*> castToReal(
        const std::vector<const Geometry::Elem*>& view
    ) {

        for (const auto& elem : view) {
            if (!elem->is<Geometry::ElemR>()) {
                throw std::logic_error("View contains elements of type different than Geometry::ElemR");
            }
        }

        std::vector<const Geometry::ElemR*> res(view.size());
        std::transform(
            view.cbegin(), 
            view.cend(), 
            res.begin(),
            [](const Geometry::Elem* elem) { return elem->castTo<Geometry::ElemR>(); }
        );

        return res;
    }

} // Namespace View
} // Namespace Group
} // Namespace SEMBA