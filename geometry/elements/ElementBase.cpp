#include "ElementBase.h"

ElementBase::ElementBase(const LayerId layerId,
                         const MatId   matId)
:   layerId_(layerId),
    matId_  (matId  ) {

}

ElementBase::ElementBase(const ElementId id,
                         const LayerId layerId,
                         const MatId   matId)
:   ClassIdBase<ElementId>(id),
    layerId_(layerId),
    matId_  (matId  ) {

}

ElementBase::ElementBase(const ElementBase& rhs)
:   ClassIdBase<ElementId>(rhs),
    layerId_(rhs.layerId_),
    matId_  (rhs.matId_  ) {

}

ElementBase::~ElementBase() {

}
