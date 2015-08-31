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
   GroupElements<ElemR> getBoundary(
           const CartesianAxis dir,
           const CartesianBound bound,
           CoordR3Group& cG,
           const Grid3* grid,
           const Mesh* mesh,
           const OptionsMesher* opts) const;
   GroupElements<ElemR> getGridElems(
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
