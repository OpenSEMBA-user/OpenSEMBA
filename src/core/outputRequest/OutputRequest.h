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

#ifndef SEMBA_OUTPUTREQUEST_OUTPUTREQUEST_H_
#define SEMBA_OUTPUTREQUEST_OUTPUTREQUEST_H_

#include "Domain.h"
#include "geometry/element/Group.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"

namespace SEMBA {
namespace OutputRequest {

class Base : public virtual Domain,
             public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable,
             public virtual Class::Printable {
public:
    enum Type {
        electric,
        magnetic,
        electricFieldNormals,
        magneticFieldNormals,
		surfaceCurrentDensity,
        bulkCurrentElectric,
        bulkCurrentMagnetic,
        current,
		voltage,
        electricFarField
    };

    Base();
    Base(const Type outputType, const std::string& name);
    Base(const Base& rhs);
    virtual ~Base();

    virtual Geometry::Element::Group<const Geometry::Elem> elems() const = 0;

    virtual void set(
            const Geometry::Element::Group<const Geometry::Elem>&) = 0;
    virtual void add(
            const Geometry::Element::Group<const Geometry::Elem>&) = 0;

    const std::string& getName() const;
    Type getOutputType() const;
    Domain getDomain() const;

    virtual void printInfo() const = 0;

private:
    std::string name_;
    Type outputType_;

    std::string getTypeStr() const;
};

template<class T>
class OutputRequest : public virtual Base,
                      public virtual Geometry::Element::Group<const T> {
public:
    OutputRequest() {}
    OutputRequest(const Domain& domain,
                  const Type outputType,
                  const std::string& name,
                  const Geometry::Element::Group<T>& elems);
    OutputRequest(const OutputRequest& rhs);
    virtual ~OutputRequest();

    SEMBA_CLASS_DEFINE_CLONE(OutputRequest<T>);

    Geometry::Element::Group<const Geometry::Elem> elems() const {
        return *this;
    }

    void set(const Geometry::Element::Group<const Geometry::Elem>&);
    void add(const Geometry::Element::Group<const Geometry::Elem>&);

    virtual void printInfo() const;
};

namespace Error {

class Material {
public:
    Material() {}
    virtual ~Material() throw() {}

    const char* what() const throw() {
        return ("Bulk currents have to be defined "
                "over elements with no material assigned.");
    }
};

} /* namespace Error */
} /* namespace OutputRequest */
} /* namespace SEMBA */

#include "OutputRequest.hpp"

namespace SEMBA {

typedef OutputRequest::OutputRequest<Geometry::ElemR> OutRqElemR;
typedef OutputRequest::OutputRequest<Geometry::ElemI> OutRqElemI;

} /* namespace SEMBA */

#endif /* SEMBA_OUTPUTREQUEST_OUTPUTREQUEST_H_ */
