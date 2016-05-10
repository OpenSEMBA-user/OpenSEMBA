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

#include "VectorFitting.h"

#ifdef EIGEN_SUPPORT

namespace SEMBA {
namespace Math {
namespace Fitting {

VectorFitting::VectorFitting(
        const std::vector<Sample>& samples) {
}

void VectorFitting::fit() {
    // ########################################################################
    // ##################### STAGE 1: Pole identification #####################
    // ########################################################################

    // Define starting poles as a vector of complex conjugates -a + bi with
    // the imaginary part linearly distributed over the frequency range of
    // interest; i.e., for each pair of complex conjugates (see eqs 9 and 10):
    //      1. imagParts = linspace(range(samples), number_of_poles)
    //      2. realParts = imagParts / 100

    // Define matrix A following equation (A.3), where:
    //      s_k = sample[k].first()
    //      f(s_k) = sample[k].second()
    //      a_i = starting poles
    // If a_i, a_{i+1} is a complex conjugate (they always come in pairs;
    // otherwise, there is an error), the elements have to follow equation
    // (A.6)

    // Define column vector B following equation (A.4), where:
    //      f(s_k) = sample.second()

    // Solve AX = B (¿by singular value decomposition? Eigen!), whose elements
    // are:
    //      X[:N] = f residues
    //      X[N] = d
    //      X[N+1] = h
    //      X[N+2:] = sigma residues <- these values are the important ones

    // Define a diagonal matrix containing the starting poles, A, (see B.2)
    // as follows:
    //      If a_i is real => A[i,i] = a_i
    //      If a_i, a_{i+1} is a complex conjugate =>
    //          A[i,i] = A[i+1, i+1] = real(a_i)
    //          A[i,i+1] = imag(a_i)
    //          A[i+1,i] = -imag(a_i)
    // Define a column vector, B, as follows (see B.2):
    //      If a_i is real => B[i] = 1
    //      If a_i, a_{i+1} is a complex conjugate => B[i] = 2; B[i+1] = 0
    // Define a row vector, C, as follows:
    //      If a_i is real => C[i] = sigma_residues[i]
    //      If a_i, a_{i+1} is a complex conjugate =>
    //          C[i] = real(sigma_residues[i])
    //          C[i] = imag(sigma_residues[i])

    // Compute the eigen values of H = A - BC
}

std::vector<VectorFitting::Sample> VectorFitting::getFittedSamples(
        const std::vector<std::complex<Real> >& frequencies) const {

}

std::vector<std::complex<Real> > VectorFitting::getPoles() {
    // assert(computed_ == true);
    return poles_;
}

std::vector<std::complex<Real> > VectorFitting::getResidues() {
    // assert(computed_ == true);
    return residues_;
}

Real VectorFitting::getRMS() {

}

} /* namespace Fitting */
} /* namespace Math */
} /* namespace SEMBA */

#endif
