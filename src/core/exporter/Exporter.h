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
#ifndef COMMON_EXPORTER_EXPORTER_H_
#define COMMON_EXPORTER_EXPORTER_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

#include "SmbData.h"
#include "math/CartesianVector.h"
#include "math/Field.h"

#include "ResultGraph.h"

class Exporter : public ProjectFile {
   friend class DG;
public:
   Exporter();
   Exporter(const string& name);
   virtual ~Exporter();
   virtual void process(
         const Real time,
         const FieldR3& electric,
         const FieldR3& magnetic);
//   virtual void writeResumeFile(
//         const Real time,
//         const FieldR3& electric,
//         const FieldR3& magnetic);
   virtual void printInfo() const;
protected:
   vector<const Result*> result_;
   void deleteExistentOutputFiles() const;
   UInt determineStepsSaved(
         const Real savingPeriod,
         const Real dt) const;
protected:
   string getOutputfilename() const;
   Group<ElemR> getBoundary(
           const CartesianAxis dir,
           const CartesianBound bound,
           CoordR3Group& cG,
           const Grid3* grid,
           const Mesh* mesh,
           const OptionsMesher* opts) const;
   Group<ElemR> getGridElems(
           CoordR3Group& cG,
           const Grid3* grid) const;
   static string getBoundaryName(
           const OptionsMesher* opts,
           const UInt i,
           const UInt j);
private:
//   void writeAllFields(const FieldR3& field) const;
};



#endif /* COMMON_EXPORTER_EXPORTER_H_ */
