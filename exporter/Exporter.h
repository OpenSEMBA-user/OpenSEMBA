#ifndef COMMON_EXPORTER_EXPORTER_H_
#define COMMON_EXPORTER_EXPORTER_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

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
   virtual void
   printInfo() const;
protected:
   vector<const Result*> result_;
   void
   deleteExistentOutputFiles() const;
   UInt
   determineStepsSaved(
         const Real savingPeriod,
         const Real dt) const;
protected:
   string getOutputfilename() const;
private:
//   void writeAllFields(const FieldR3& field) const;
};

#endif /* COMMON_EXPORTER_EXPORTER_H_ */
