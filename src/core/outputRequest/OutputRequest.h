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
#ifndef OUTPUTREQUEST_H_
#define OUTPUTREQUEST_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

#include "Domain.h"
#include "geometry/elements/GroupElements.h"

#include "base/class/ClassGroupBase.h"

class OutRqBase : public virtual Domain,
                  public virtual ClassGroupBase<GroupElements<const Elem>> {
public:
    typedef enum {
        electric,
        magnetic,
        electricFieldNormals,
        magneticFieldNormals,
        bulkCurrentElectric,
        bulkCurrentMagnetic,
        powerDensity,
        planeWaveDecomposition,
        current,
        voltage,
        power,
        sParameter,
        zParameter,
        yParameter,
        theveninVoltageGenerator,
        nortonCurrentGenerator,
        couplingCrossSection,
        poyntingVector,
        energy,
    } Type;

    OutRqBase();
    OutRqBase(const Type outputType, const string& name);
    OutRqBase(const OutRqBase& rhs);
    virtual ~OutRqBase();

    virtual bool hasSameProperties(const OutRqBase& rhs) const;
    virtual bool isSimilar(const OutRqBase& rhs) const = 0;

    const string& getName() const;
    OutRqBase::Type getOutputType() const;
    Domain getDomain() const;

    virtual void printInfo() const = 0;

private:
    string name_;
    OutRqBase::Type outputType_;

    string getTypeStr() const;
};


template<class T>
class OutRq : public virtual OutRqBase,
              public virtual GroupElements<const T> {
public:
    OutRq() {}
    OutRq(const Domain& domain,
          const Type outputType,
          const string& name,
          const GroupElements<T>& elems);
    OutRq(const OutRq& rhs);
    virtual ~OutRq();

    DEFINE_CLONE(OutRq<T>);

    bool isSimilar(const OutRqBase& rhs) const;

    GroupElements<const Elem> elems() const { return *this; }

    void set(const GroupElements<const Elem>&);
    void add(const GroupElements<const Elem>&);

    void printInfo() const;
};

#include "OutRq.hpp"

typedef OutRq<Nod > OutRqNode;
typedef OutRq<Lin > OutRqLine;
typedef OutRq<Surf> OutRqSurface;
typedef OutRq<Vol > OutRqVolume;

#endif
