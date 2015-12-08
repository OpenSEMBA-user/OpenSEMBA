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

#include "Tetrahedron.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

Tetrahedron::Tetrahedron() {

}

Tetrahedron::~Tetrahedron() {

}

Triangle3* Tetrahedron::getTri3Face(const std::size_t f) const {
    const CoordR3* coord[3];
    for (std::size_t i = 0; i < 3; i++) {
        coord[i] = getSideVertex(f,i);
    }
    return new Triangle3(Id(0), coord, getLayer(), getModel());
}

void Tetrahedron::getCubaturePositions(
        Math::CVecR3 res[Math::Simplex::Tetrahedron<1>::ncp]) const {
    static const std::size_t ncp = Math::Simplex::Tetrahedron<1>::ncp;
    for (std::size_t c = 0; c < ncp; c++) {
        for (std::size_t i = 0; i < 3; i++) {
            res[c](i) = 0.0;
        }
    }
    // Evaluates Lagrange's functions in positions specified by the
    // simplex coordinates of tet.
    Math::Real **lagrEv;
    lagrEv = new Math::Real*[ncp];
    for (std::size_t i = 0; i < ncp; i++) {
        lagrEv[i] = new Math::Real[numberOfCoordinates()];
    }
    for (std::size_t c = 0; c < ncp; c++) {
        for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
            lagrEv[c][i]= getTet().getLagr(i).eval(
                    getTet().cubatureCoordinate(c));
        }
    }
    // Computes nodes.
    for (std::size_t c = 0; c < ncp; c++) {
        for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
            res[c] += *(getV(i)) * lagrEv[c][i];
        }
    }
    for (std::size_t i = 0; i < ncp; i++) {
        delete[] lagrEv[i];
    }
    delete[] lagrEv;
}

void Tetrahedron::getCubatureJacobianDeterminant(
        Math::Real cJDet[Math::Simplex::Tetrahedron<2>::ncp],
        const Math::MatR44 cJ[Math::Simplex::Tetrahedron<2>::ncp]) const {
    for (std::size_t c = 0; c < Math::Simplex::Tetrahedron<2>::ncp; c++) {
        cJDet[c] = cJ[c].getDeterminant4x4();
    }
}

void Tetrahedron::getCubatureJacobianDeterminant(
        Math::Real cJDet[Math::Simplex::Tetrahedron<2>::ncp]) const {
    Math::MatR44 cJ[Math::Simplex::Tetrahedron<2>::ncp];
    getCubatureJacobian(cJ);
    for (std::size_t c = 0; c < Math::Simplex::Tetrahedron<2>::ncp; c++) {
        cJDet[c] = cJ[c].getDeterminant4x4();
    }
}

void Tetrahedron::getCubatureJacobian(
        Math::MatR44 res[Math::Simplex::Tetrahedron<2>::ncp]) const {
    for (std::size_t s = 0; s < numberOfFaces(); s++)
        for (std::size_t i = 0; i < numberOfCoordinates(); i++) {
            const Math::CVecR3 v = *(getV(i));
            for (std::size_t c = 0; c < Math::Simplex::Tetrahedron<2>::ncp;
                 c++) {
                res[c](0,s) += v(0) * getTet().getCda(i,s,c);
                res[c](1,s) += v(1) * getTet().getCda(i,s,c);
                res[c](2,s) += v(2) * getTet().getCda(i,s,c);
                res[c](3,s) = 1.0;
            }
        }
}

void Tetrahedron::getCubatureJacobianHat(
        Math::MatR43 cJHat[Math::Simplex::Tetrahedron<2>::ncp],
        const Math::MatR44 cJ[Math::Simplex::Tetrahedron<2>::ncp],
        const Math::Real cJDet[Math::Simplex::Tetrahedron<2>::ncp]) const {

    // PURPOSE:
    // See chapter 17.3.1 of Filippa's course on Advanced FEM.
    std::size_t ind[3];
    Math::MatR33 Jred, invJred;
    Math::MatR43 res;
    // Main loop, runs over all simplex coordinates.
    for (std::size_t j = 0; j < numberOfFaces(); j++) {
        // Chooses columns indices that will be substracted from the Jacobian.
        switch (j) {
        case 0:
            ind[0] = 1; ind[1] = 2; ind[2] = 3;
            break;
        case 1:
            ind[0] = 0; ind[1] = 2; ind[2] = 3;
            break;
        case 2:
            ind[0] = 0; ind[1] = 1; ind[2] = 3;
            break;
        case 3:
            ind[0] = 0; ind[1] = 1; ind[2] = 2;
            break;
        }
        // Builds reduced jacobian for all cubature points.
        for (std::size_t c = 0; c < Math::Simplex::Tetrahedron<1>::ncp; c++) {
            // Substracts column j to column ind[c], to build J reduced.
            for (std::size_t k = 0; k < 3; k++) {
                for (std::size_t i = 0; i < 3; i++) {
                    Jred(k,i) = cJ[c](k,ind[i]) - cJ[c](k,j);
                }
            }
            // Computes reduced jacobian inverse.
            invJred = Jred.invert();
            // Gets Jhat from the summation of reduced jacobians.
            for (std::size_t m = 0; m < 3; m++) {
                for (std::size_t n = 0; n < 3; n++) {
                    cJHat[c](j,m) -= invJred(n,m);
                }
            }
        }
    } // Ends j loop, running over faces.
    // Multiplies by Jacobian determinant.
    for (std::size_t c = 0; c < Math::Simplex::Tetrahedron<1>::ncp; c++) {
        cJHat[c] *= cJDet[c];
    }
}

} /* namespace Element */
} /* namespace Geometry */
} /* namespace SEMBA */
