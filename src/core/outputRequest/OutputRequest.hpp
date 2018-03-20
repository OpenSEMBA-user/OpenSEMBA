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

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {

template<class T>
OutputRequest<T>::OutputRequest(const Domain& domain,
                                const Type type,
                                const std::string& name,
                                const Geometry::Element::Group<T>& elems)
:   Domain(domain),
    Base(type, name),
    Geometry::Element::Group<const T>(elems) {
    if (type == bulkCurrentElectric || type == bulkCurrentMagnetic) {
        if (this->getMatId(MatId(0)).size() != this->size()) {
            throw Error::Material();
        }
    }
}

template<class T>
OutputRequest<T>::OutputRequest(const OutputRequest<T>& rhs)
:   Domain(rhs),
    Base(rhs),
    Geometry::Element::Group<const T>(rhs) {

}

template<class T>
OutputRequest<T>::~OutputRequest() {

}

template <class T>
void OutputRequest<T>::set(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::operator=(elems);
}

template <class T>
void OutputRequest<T>::add(
        const Geometry::Element::Group<const Geometry::Elem>& elems) {
    Geometry::Element::Group<const T>::add(elems);
}

template<class T>
inline void OutputRequest<T>::printInfo() const {
    std::cout << " --- Output request instance --- " << std::endl;
    Base::printInfo();
    Geometry::Element::Group<const T>::printInfo();
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
