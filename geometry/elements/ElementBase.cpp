#include "ElementBase.h"

ElementBase::ElementBase(const LayerId layerId,
                         const uint matId)
:   layerId_(layerId),
    matId_  (matId  ) {

}

ElementBase::ElementBase(const ElementId id,
                         const LayerId layerId,
                         const uint matId)
:   ClassWithIdBase<ElementId>(id),
    layerId_(layerId),
    matId_  (matId  ) {

}

ElementBase::ElementBase(const ElementBase& rhs)
:   ClassWithIdBase<ElementId>(rhs),
    layerId_(rhs.layerId_),
    matId_  (rhs.matId_  ) {

}

ElementBase::ElementBase(const ElementId id, const ElementBase& rhs)
:   ClassWithIdBase<ElementId>(id),
    layerId_(rhs.layerId_),
    matId_  (rhs.matId_  ) {

}

ElementBase::~ElementBase() {

}
