// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
#ifndef CELLTET_H_
#define CELLTET_H_

#include <vector>
#include <set>
#include <array>

using namespace std;

#include "Cell.h"
#include "math/Simplex.h"
#include "math/MathMatrix.h"
#include "../../../common/math/FunctionPolynomial.h"
#include "geometry/elements/Tet.h"

template <Int N, class E>
class CellTet : public Cell, public E {
public:
    static const SimplexTet<N> tet;
    static const UInt np = ((N+1)*(N+2)*(N+3)/6);
    static const UInt nfp = ((N+1)*(N+2)/2);
    typedef StaMatrix<Real,np,np> MatNpNp;
    typedef StaMatrix<Real,np,nfp> MatNpNfp;

    CellTet(const E& base);
    virtual ~CellTet();
    bool isLocalSide(
            const UInt side,
            const SurfR* surf) const;
    bool isLocalSide(const SurfR* surf) const;
    UInt getNfp() const;
    UInt getNodeVertex(const UInt i) const;
    CVecR3 getNode(const UInt i) const;
    void getCMatrices(MatNpNp C[3]) const;
    CVecR3 getSideNodePos(const UInt f, const UInt i) const;
    UInt getSideNode(UInt f, UInt i) const;
    void getCurvedLIFTnormal(
            MatNpNfp LIFTn[3],
            MatNpNfp LIFTcn[3],
            MatNpNfp LIFTrn[3],
            const UInt face) const;
    const Tet* getPtrToBase() const;
    MatNpNp getConductivityWithGeometricProfile(
            const PMVolumePML& mat,
            const UInt type,
            const Real maxSigma) const;
    MatNpNp getMassMatrix() const;
    void buildOperators();
    array<UInt,nfp> getNeighbourNodes(
            const UInt f,
            const Cell& neigh) const;
    void printInfo() const;
private:
    array<CVecR3,np> node_;
    void buildNodes();
    MatNpNp getCMatrix(
            const UInt x,
            const MatNpNp& invM,
            const MatR43 cJHat[SimplexTet<1>::ncp],
            const MatR44 cJ[SimplexTet<1>::ncp]) const;
    MatNpNp getMassMatrix(
            const Real cJDet[SimplexTet<1>::ncp]) const;
    MatNpNp getMassMatrixIntegratedWithScalar(
            const Real cScalar[SimplexTet<1>::ncp]) const;
    MatNpNfp getLIFTMatrix(
            const UInt s,
            const MatNpNfp& invM,
            const  Real csd[SimplexTri<1>::ncp]) const;
};

template <Int N, class E>
const SimplexTet<N> CellTet<N, E>::tet;

#include "CellTet.hpp"

#endif /* CELL_H_ */
