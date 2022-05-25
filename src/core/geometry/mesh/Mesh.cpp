
#include "Mesh.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {
	ElemView Mesh::reassign(const ElemView& inGroup) {
        ElemView res;

        res.reserve(inGroup.size());
        for (const auto& elem: inGroup) {
            res.push_back(this->elems_.getId(
                elem->getId()
            ));
        }

        return res;
    }
}
}
}