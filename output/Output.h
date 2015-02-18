#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include "../math/CartesianVector.h"
#include "../math/Field.h"
#include "../../../common/gidpost/gidpost.h"
#include "../../../common/output/ResultGraph.h"
#include "ResultRCS.h"

using namespace std;

class Output : public ProjectFile {
   friend class DG;
public:
   Output();
   Output(
         const SmbData* smb,
         const DG* probes);
   virtual ~Output();
   virtual void
   process(
         const double time,
         const FieldD3& electric,
         const FieldD3& magnetic);
   virtual void
   writeResumeFile(
         const double time,
         const FieldD3& electric,
         const FieldD3& magnetic);
   virtual void
   printInfo() const;
protected:
   const SmbData* smb_;
   const DG* dg_;
   vector<const Result*> result_;
   void
   deleteExistentOutputFiles() const;
   uint
   determineStepsSaved(
         const double savingPeriod,
         const double dt) const;
protected:
   string getOutputfilename() const;
private:
   void writeAllFields(const FieldD3& field) const;
};

#endif /* OUTPUT_H_ */
