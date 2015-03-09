#ifndef ELEMENTBASE_H_
#define ELEMENTBASE_H_

#include "ClassIdBase.h"

#ifndef ELEMENT_ERROR
#define ELEMENT_ERROR 37311347
#endif

#include "IdBase.h"
#include "Layer.h"

typedef unsigned int uint;

CreateId(ElementId);
CreateId(MatId);

class ElementBase : public ClassIdBase<ElementId> {
public:
    enum Type {
		undefined,
		node,
		line,
		surface,
		volume
	};
    
    ElementBase(const LayerId layerId = LayerId(0),
                const MatId   matId   = MatId(0));
    ElementBase(const ElementId id,
                const LayerId layerId = LayerId(0),
                const MatId   matId   = MatId(0));
    ElementBase(const ElementBase& rhs);
    virtual ~ElementBase();
    
    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }
    
    virtual uint numberOfFaces      () const = 0;
    virtual uint numberOfVertices   () const = 0;
    virtual uint numberOfCoordinates() const = 0;
    
    virtual uint numberOfSideVertices   (const uint f = 0) const = 0;
    virtual uint numberOfSideCoordinates(const uint f = 0) const = 0;
    
    virtual Type getType() const { return undefined; }

    LayerId getLayerId() const { return layerId_; }
    MatId   getMatId  () const { return matId_;   }

    virtual void setLayerId(const LayerId layerId) { layerId_= layerId; }
    virtual void setMatId  (const MatId   matId  ) { matId_  = matId;   }
    
    virtual void printInfo() const = 0;
    
private:
    LayerId layerId_;
    MatId   matId_;
};

#endif /* ELEMENTBASE_H_ */
