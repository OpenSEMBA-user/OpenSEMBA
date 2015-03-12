#ifndef ELEMENTBASE_H_
#define ELEMENTBASE_H_

#include "ClassIdBase.h"
#include "../coordinates/Coordinate.h"
#include "../Box.h"

#include "IdBase.h"
#include "Layer.h"

CreateId(ElementId);
CreateId(MatId);

class ElementBase : public ClassIdBase<ElementId> {
public:
    ElementBase(const LayerId layerId = LayerId(0),
                const MatId   matId   = MatId(0));
    ElementBase(const ElementId id,
                const LayerId layerId = LayerId(0),
                const MatId   matId   = MatId(0));
    ElementBase(const ElementBase& rhs);
    virtual ~ElementBase();
    
    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }
    
    virtual UInt numberOfFaces      () const = 0;
    virtual UInt numberOfVertices   () const = 0;
    virtual UInt numberOfCoordinates() const = 0;
    
    virtual UInt numberOfSideVertices   (const UInt f = 0) const = 0;
    virtual UInt numberOfSideCoordinates(const UInt f = 0) const = 0;
    
    LayerId getLayerId() const { return layerId_; }
    MatId   getMatId  () const { return matId_;   }

    virtual BoxR3 getBound() const = 0;
    virtual const CoordR3* getV    (const UInt i) const = 0;
    virtual const CoordR3* getVertex    (const UInt i) const = 0;
    virtual bool isCoordinate(const CoordR3*) const = 0;

    virtual void setLayerId(const LayerId layerId) { layerId_= layerId; }
    virtual void setMatId  (const MatId   matId  ) { matId_  = matId;   }
    
    virtual void printInfo() const = 0;
    
private:
    LayerId layerId_;
    MatId   matId_;
};

#endif /* ELEMENTBASE_H_ */
