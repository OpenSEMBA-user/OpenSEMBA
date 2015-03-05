#ifndef ELEMENTBASE_H_
#define ELEMENTBASE_H_

#include "ClassWithIdBase.h"

#ifndef ELEMENT_ERROR
#define ELEMENT_ERROR 37311347
#endif

typedef unsigned int uint;

CreateId(ElementId);

class ElementBase : public ClassWithIdBase<ElementId> {
public:
    enum Type {
		undefined,
		node,
		line,
		surface,
		volume
	};
    
    ElementBase(const uint layerId = 0,
                const uint matId   = 0);
    ElementBase(const ElementId id,
                const uint layerId = 0,
                const uint matId   = 0);
    ElementBase(const ElementBase& rhs);
    ElementBase(const ElementId id, const ElementBase& rhs);
    virtual ~ElementBase();
    
    virtual bool isCurved   () const { return false; }
    virtual bool isQuadratic() const { return false; }
    
    virtual uint numberOfFaces      () const = 0;
    virtual uint numberOfVertices   () const = 0;
    virtual uint numberOfCoordinates() const = 0;
    
    virtual uint numberOfSideVertices   (const uint f = 0) const = 0;
    virtual uint numberOfSideCoordinates(const uint f = 0) const = 0;
    
    virtual Type getType() const { return undefined; }

    uint getLayerId() const { return layerId_; }
    uint getMatId  () const { return matId_;   }

    virtual void setLayerId(const uint layerId) { layerId_= layerId; }
    virtual void setMatId  (const uint matId  ) { matId_  = matId;   }
    
    virtual void printInfo() const = 0;
    
private:
    uint layerId_;
    uint matId_;
};

#endif /* ELEMENTBASE_H_ */
