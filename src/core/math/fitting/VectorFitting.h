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

#ifndef SEMBA_MATH_FITTING_VECTOR_H_
#define SEMBA_MATH_FITTING_VECTOR_H_

#ifdef EIGEN_SUPPORT

#include <vector>
#include <complex>

#include "math/Types.h"
#include "math/matrix/Dynamic.h"

namespace SEMBA {
namespace Math {
namespace Fitting {

class VectorFitting {
public:
    typedef std::complex<Real> complex;
    typedef std::pair<complex, std::vector<complex>> Sample;

    // TODO: manage options in constructor.
    // TODO: Add parameter to control order of approximation (now hard-coded in
    // the implementation)
    /**
     * @param samples   Data to be fitted.
     * @param N         Order of approximation. It shall be an even number.
     */
    VectorFitting(const std::vector<Sample>& samples, size_t order = 20);

    // This could be called from the constructor, but if an iterative algorithm
    // is preferred, it's a good idea to have it as a public method
    void fit();

    std::vector<Sample> getFittedSamples(
            const std::vector<std::complex<Real>>& frequencies) const;
    std::vector<std::complex<Real> > getPoles();
    std::vector<std::complex<Real> > getResidues();
    Real getRMS();

private:
    std::vector<Sample> samples_;

    std::vector<std::complex<Real>> poles_, residues_;

    Matrix::Dynamic<std::complex<Real>>  A_, C_;
    std::vector<Real>  B_, D_, E_;

    size_t order_;
};

} /* namespace Fitting */
} /* namespace Math */
} /* namespace SEMBA */

#endif // EIGEN_SUPPORT

#endif // SEMBA_MATH_FITTING_VECTOR_H_
