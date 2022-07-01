#pragma once

#include <algorithm>
#include <vector>
#include <functional>

namespace SEMBA {
namespace Class {
namespace Group {
    class View {
    public:
        static inline Geometry::BoxR3 getBound(const Geometry::ElemView& view)
        {
            if (view.size() == 0) {
                return Geometry::BoxR3().setInfinity();
            }

            Geometry::BoxR3 bound;

            for (const auto& elem : view) {
                if (!elem->is<Geometry::ElemR>()) {
                    continue;
                }

                bound << elem->castTo<Geometry::ElemR>()->getBound();
            }

            for (auto const& elem : view) {
                if (!elem->is<Geometry::ElemI>()) {
                    continue;
                }

                Geometry::BoxI3 boxI = elem->castTo<Geometry::ElemI>()->getBound();
                Math::CVecI3 minP = boxI.getMin();
                Math::CVecI3 maxP = boxI.getMax();
                using Math::CVecR3;
                using namespace Math::Constants;
                bound << Geometry::BoxR3(CVecR3(minP(x), minP(y), minP(z)), CVecR3(maxP(x), maxP(y), maxP(z)));
            }

            return bound;
        }

        static inline std::vector<const Geometry::ElemR*> filterView(
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
        static inline std::vector<const Geometry::ElemR*> castToReal(
            const Geometry::ElemView& view
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
    };
} // Namespace Group
} // Namespace Class
} // Namespace SEMBA