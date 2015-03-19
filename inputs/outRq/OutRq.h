#ifndef OUTPUTREQUEST_H_
#define OUTPUTREQUEST_H_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "Condition.h"
#include "Domain.h"
#include "ElementsGroup.h"

using namespace std;

class OutRq : public Condition, public Domain, public ClassBase {
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
    OutRq(const Domain& domain, const Type outputType, const string& name);
    virtual ~OutRq() = 0;

    OutRq& operator=(const OutRq& rhs);
    virtual bool isSimilar(const OutRq& rhs) const;
    virtual void setAdditionalElements(const ElementsGroup<>& elems);
    string outputTypeStr() const;
    const string& getName() const;
    OutRq::Type getOutputType() const;
    void printInfo() const;
private:
    string name_;
    OutRq::Type outputType_;
};

#	endif
