#ifndef OUTPUTREQUEST_H_
#define OUTPUTREQUEST_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "../geometry/elements/Element.h"
#include "Condition.h"
#include "Domain.h"

using namespace std;

class OutRq : public Condition, public Domain, public ClassBase {
public:
    typedef enum {
        undefined,
        electricField,
        magneticField,
        electricFieldNormals,
        magneticFieldNormals,
        powerDensity,
        planeWaveDecomposition,
        current,
        voltage,
        bulkCurrentElectric,
        bulkCurrentMagnetic,
        power,
        sParameter,
        zParameter,
        yParameter,
        theveninVoltageGenerator,
        nortonCurrentGenerator,
        couplingCrossSection,
        poyntingVector,
        energy,
        farField
    } Type;
    OutRq(
            const Domain& domain,
            const Element::Type elementType,
            const OutRq::Type outputType,
            const string& name,
            const vector<uint>& elements);
    OutRq(
            const Domain& domain,
            const Element::Type elementType,
            const OutRq::Type outputType_,
            const string& name_,
            const BoxD3& box);
    OutRq& operator=(const OutRq& rhs);
    virtual bool isSimilar(const OutRq& rhs) const;
    string outputTypeStr() const;
    string elementTypeStr() const;
    const string& getName() const;
    OutRq::Type getOutputType() const;
    const vector<uint>& getElem() const;
    const BoxD3& getBound() const;
    void setAdditionalElems(const vector<uint> elems);
    void printInfo() const;

    virtual ClassBase* clone() const;

private:
    string name_;
    OutRq::Type outputType_;
    bool usingBound_;
    BoxD3 bound_;
    vector<uint> elem_;
};

#	endif
