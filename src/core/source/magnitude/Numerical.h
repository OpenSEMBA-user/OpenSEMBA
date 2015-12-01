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

#ifndef SEMBA_SOURCE_MAGNITUDE_NUMERICAL_H_
#define SEMBA_SOURCE_MAGNITUDE_NUMERICAL_H_



#include "Magnitude.h"
#include "filesystem/Project.h"

namespace SEMBA {
namespace Source {
namespace Magnitude {

class Numerical : public virtual Magnitude,
                  public virtual FileSystem::Project {
public:
    Numerical();
    Numerical(const FileSystem::Project& filename);
    Numerical(const FileSystem::Project& filename,
              const Magnitude& mag,
              const Math::Real timeStep,
              const Math::Real finalTime);
    virtual ~Numerical();

    SEMBA_CLASS_DEFINE_CLONE(Numerical);

    Numerical& operator=(const Numerical& rhs);

    bool operator==(const Magnitude&) const;
    Math::Real evaluate(const Math::Real time) const;

    void printInfo() const;

private:
    static const Size defaultNumberOfSteps = 1000;


};

} /* namespace Magnitude */
} /* namespace Source */
} /* namespace SEMBA */

#endif /* SEMBA_SOURCE_MAGNITUDE_NUMERICAL_H_ */
