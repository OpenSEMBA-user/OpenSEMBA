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

#include <physicalModel/surface/SIBCFile.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Surface {

SIBCFile::SIBCFile(const Id id,
                         const std::string& name,
                         const FileSystem::Project& file)
:   Identifiable<Id>(id),
    PhysicalModel(name),
    file_(file) {
    std::string extension = file_.getExtension();
    if (extension.compare(".mibc") != 0) {
        printInfo();
        throw std::logic_error("File extension must be .mibc in file: "
                + file_);
    }
}

SIBCFile::~SIBCFile() {

}

void SIBCFile::printInfo() const {
    std::cout << "--- Physical model information ---"   << std::endl;
    std::cout << "Surface Impedance Boundary Condition File" << std::endl;
    std::cout << file_ << std::endl;

}

const FileSystem::Project SIBCFile::getFile() const {
    return file_;
}

} /* namespace Surface */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
