/*
 * SolverSource.h
 *
 *  Created on: Sep 2, 2013
 *      Author: luis
 */

#ifndef SOLVERSOURCE_H_
#define SOLVERSOURCE_H_

#include <utility>
#include <vector>

using namespace std;

#include "sources/EMSource.h"
#include "geometry/maps/Map.h"
#include "../../core/CellGroup.h"
#include "../../core/Comm.h"

class DGSource {
public:
    typedef enum {
        totalField,
        scatteredField,
        totalFieldNotBacked
    } BackingType;
    DGSource();
    virtual ~DGSource();
    void addJumps(
            const UInt e1,
            const UInt e2);
    virtual void computeExcitation(
            const Real intTime,
            const Real minDT) = 0;
    virtual void printInfo() const = 0;
protected:
    const static UInt N = ORDER_N;
    const static UInt np = (N+1) * (N+2) * (N+3) / 6;
    const static UInt np2 = np * 2;
    const static UInt nfp = (N+1) * (N+2) / 2;
    const static UInt npnfp = np * nfp;
    const static UInt npnp = np * np;
    const static UInt faces = 4;
    const static UInt nfpfaces = nfp * faces;
    // Excitation fields.
    FieldR3 ETInc, ESInc, EIncNB;
    FieldR3 HTInc, HSInc, HIncNB;
    // Excitation total field jumps pointers.
    UInt nETF;
    UInt *ETFe;
    Real **dExT, **dEyT, **dEzT;
    Real **dHxT, **dHyT, **dHzT;
    // Excitation scattered field jumps pointers.
    UInt nESF;
    UInt *ESFe;
    Real **dExS, **dEyS, **dEzS;
    Real **dHxS, **dHyS, **dHzS;
    // Excitation total field not backed jumps.
    UInt nETFNB;
    UInt *ETFNBe;
    Real **dExTNB, **dEyTNB, **dEzTNB;
    Real **dHxTNB, **dHyTNB, **dHzTNB;
    void initSource(
            const MapGroup& map,
            const CellGroup& cells,
            FieldR3& dE, FieldR3& dH,
            const Int vmapM[faces][nfp]);
    vector<pair<UInt,UInt> > getElemFaces(
            const MapGroup& map,
            const CellGroup& cells,
            const BackingType type) const;
    CVecR3* initPositions(
            const vector<pair<UInt, UInt> >& elemFace,
            const CellGroup& cells) const;
};

#endif /* SOLVERSOURCE_H_ */
