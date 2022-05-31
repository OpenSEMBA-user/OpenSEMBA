
#include "Box.h"
#include "element/Element.h"

namespace SEMBA {
namespace Geometry {

    template< class It >
	BoxR3 getBound(It first, It last) {
        
        if (first == last) {
            return BoxR3().setInfinity();
        }

        BoxR3 bound;
        
        while (first != last) {
            if ((*first)->is<ElemR>()) {
                bound << (*first)->castTo<ElemR>()->getBound();
            }

            if ((*first)->is<ElemI>()) {
                BoxI3 boxI = (*first)->castTo<ElemI>()->getBound();
                Math::CVecI3 minP = boxI.getMin();
                Math::CVecI3 maxP = boxI.getMax();
                using Math::CVecR3;
                using namespace Math::Constants;
                bound << BoxR3(CVecR3(minP(x), minP(y), minP(z)), CVecR3(maxP(x), maxP(y), maxP(z)));
            }
            
            first++;
        }

        return bound;
	}

} /* namespace Geometry */
} /* namespace SEMBA */
