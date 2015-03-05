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

class OutputRequest : public Condition, public Domain {
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
    OutputRequest();
    OutputRequest(
            const Domain& domain,
            const Element::Type elementType,
            const OutputRequest::Type outputType,
            const string& name,
            const vector<uint>& elements);
    OutputRequest(
            const Domain& domain,
            const Element::Type elementType,
            const OutputRequest::Type outputType_,
            const string& name_,
            const BoxD3& box);
    OutputRequest& operator=(const OutputRequest& rhs);
    bool isSimilar(const OutputRequest& rhs) const;
    string outputTypeStr() const;
    string elementTypeStr() const;
    const string& getName() const;
    OutputRequest::Type getOutputType() const;
    const vector<uint>& getElem() const;
    const BoxD3& getBound() const;
    void setAdditionalElems(const vector<uint> elems);
    void printInfo() const;
private:
    string name_;
    OutputRequest::Type outputType_;
    bool usingBound_;
    BoxD3 bound_;
    vector<uint> elem_;
    void
    setThetaAndPhi(
            double iTh, double fTh, double sTh,
            double iPhi, double fPhi, double sPhi);
};

#	endif
