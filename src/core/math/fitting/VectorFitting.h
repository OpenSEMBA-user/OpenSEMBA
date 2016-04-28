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

#include <vector>
#include <complex>

namespace SEMBA {
namespace Math {
namespace Fitting {

class VectorFitting {
public:
    // TODO: manage options in constructor.
    VectorFitting(  std::vector< std::vector<std::complex<double> > > data,
                    std::vector<double> samples,
                    std::vector<std::complex<double> > poles,
                    std::vector< std::vector<std::complex<double> > > weights);

    // This could be called from the constructor, but if an iterative algorithm
    // is preferred, it's a good idea to have it as a public method
    void fit();

    std::vector<std::complex<double> > getPoles();
    std::vector<std::complex<double> > getResidues();

    // TODO: declare getters of A,B,C,D,E matrices

private:
    // TODO: define class attributes to store: input data, output data,
    // ¿intermediate poles iterations?, options.
};

} /* namespace Fitting */
} /* namespace Math */
} /* namespace SEMBA */

#endif
