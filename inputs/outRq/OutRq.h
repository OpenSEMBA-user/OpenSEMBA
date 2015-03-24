#ifndef OUTPUTREQUEST_H_
#define OUTPUTREQUEST_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "Domain.h"
#include "../../geometry/elements/ElementsGroup.h"

using namespace std;

template<class T = void>
class OutRq;

template<>
class OutRq<void> : public virtual Domain,
                    public virtual ClassBase {
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

    OutRq();
    OutRq(const Type outputType, const string& name);
    OutRq(const OutRq& rhs);
    virtual ~OutRq();


    const string& getName() const;
    OutRq::Type getOutputType() const;

    void printInfo() const;

private:
    string name_;
    OutRq::Type outputType_;

    string getTypeStr() const;
};


template<class T>
class OutRq : public virtual OutRq<void>,
              public virtual ElementsGroup<T> {
public:
    OutRq() {}
    OutRq(const Domain& domain,
          const Type outputType,
          const string& name,
          const ElementsGroup<T>& elems);
    OutRq(const OutRq& rhs);
    virtual ~OutRq();

    ClassBase* clone() const;

    template<class T2>
    OutRq<T>& operator=(const Group<T2>& rhs);

    template<class T2>
    bool isSimilar(
            const OutRq<T2>* rhs,
            const bool rev = false) const;

    template<class T2>
    void add(OutRq<T2>* outRq);

    ElementsGroup<T>&       elems()       { return *this; }
    const ElementsGroup<T>& elems() const { return *this; }

    void printInfo() const { OutRq<void>::printInfo(); }
};

#include "OutRq.hpp"

typedef OutRq<Nod> OutRqNode;
typedef OutRq<Surf> OutRqSurface;
typedef OutRq<Vol> OutRqVolume;

#endif
