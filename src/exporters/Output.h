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

#pragma once
//
//#include "base/class/ClassBase.h"
//#include "outRq/OutRq.h"
//
//class OutputBase : public ClassBase {
//public:
//    OutputBase(const OutRqBase* outRq);
//    virtual ~OutputBase();
//protected:
//    const OutRqBase* outRq_;
//};
//
//template<class T>
//class Output : public OutputBase {
//public:
//        Output( const OutRqBase* outRq,
//            multimap<ElementId, vector<CVecR3>>& electric,
//            multimap<ElementId, vector<CVecR3>>& magnetic);
//    virtual ~Output();
//
//
//private:
//    multimap<ElementId, vector<T>> data_;
//    //!< Stores several values for a single element which is needed when data
//    //! is multivaluated (e.g. Discontinuous Galerkin). vector<T> must
//    //!  contain data at the Gauss points of the element. e.g:
//    //!      Cell Type    |   vector size
//    //!      ------------ |   --------------
//    //!      Node         |        1
//    //!      Lin (0)      |        1
//    //!      Lin (1)      |        2
//    //!      Tet (1)      |        4
//    //!      Tet (2)      |       10
//};
//
