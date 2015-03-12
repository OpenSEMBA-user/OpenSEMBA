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
            const OutRq::Type outputType,
            const string& name,
            const vector<ElementId>& elements);

    virtual ClassBase* clone() const;

    OutRq& operator=(const OutRq& rhs);
    virtual bool isSimilar(const OutRq& rhs) const;
    string outputTypeStr() const;
    const string& getName() const;
    OutRq::Type getOutputType() const;
    const vector<ElementId>& getElem() const;
    void setAdditionalElems(const vector<ElementId> elems);
    void printInfo() const;
private:
    string name_;
    OutRq::Type outputType_;
    vector<ElementId> elem_;
};

#	endif
