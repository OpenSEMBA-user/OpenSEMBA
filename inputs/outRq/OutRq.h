#ifndef OUTPUTREQUEST_H_
#define OUTPUTREQUEST_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

#include "Domain.h"
#include "../../geometry/elements/ElementsGroup.h"

#include "ClassGroupBase.h"

class OutRqBase : public virtual Domain,
                  public virtual ClassGroupBase<Elem> {
public:
    typedef enum {
        undefined,
        electric,
        magnetic,
        electricFieldNormals,
        magneticFieldNormals,
        bulkCurrentElectric,
        bulkCurrentMagnetic,
        powerDensity,
        planeWaveDecomposition,
        current,
        voltage,
        power,
        sParameter,
        zParameter,
        yParameter,
        theveninVoltageGenerator,
        nortonCurrentGenerator,
        couplingCrossSection,
        poyntingVector,
        energy,
    } Type;

    OutRqBase();
    OutRqBase(const Type outputType, const string& name);
    OutRqBase(const OutRqBase& rhs);
    virtual ~OutRqBase();

    const string& getName() const;
    OutRqBase::Type getOutputType() const;
    Domain getDomain() const;

    virtual void printInfo() const = 0;

private:
    string name_;
    OutRqBase::Type outputType_;

    string getTypeStr() const;
};


template<class T>
class OutRq : public virtual OutRqBase,
              public virtual ElementsGroup<const T> {
public:
    OutRq() {}
    OutRq(const Domain& domain,
          const Type outputType,
          const string& name,
          const ElementsGroup<T>& elems);
    OutRq(const OutRq& rhs);
    virtual ~OutRq();

    DEFINE_CLONE(OutRq<T>);

    bool isSimilar(const ClassCompBase& rhs) const;

    Group<const Elem> elems() const { return *this; }

    void set(const Group<const Elem>&);
    void add(const Group<const Elem>&);

    void printInfo() const;
};

#include "OutRq.hpp"

typedef OutRq<Nod> OutRqNode;
typedef OutRq<Surf> OutRqSurface;
typedef OutRq<Vol> OutRqVolume;

#endif
