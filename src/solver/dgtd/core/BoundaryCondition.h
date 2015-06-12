#ifndef BOUNDARYCONDITION_H_
#define BOUNDARYCONDITION_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>			// Stream I/O.
#include <cmath>
#include <vector>
#include <utility>
#include "inputs/Condition.h"
#include "geometry/elements/Volume.h"

using namespace std;

class BoundaryCondition {
public:
   BoundaryCondition();
   BoundaryCondition(
         Face& boundary,
         const Condition* cond,
         const bool back = false);
   virtual ~BoundaryCondition();
   Face get() const;
   const VolR* getCell() const;
   uint getFace() const;
   bool
   hasSameBoundary(const BoundaryCondition& other) const {
      return (cell_ == other.cell_ && face_ == other.face_);
   }
   void  printInfo() const;
   const Condition* getCondition() const;
   bool isBack() const;

private:
   const Volume* cell_;
   uint face_;
   bool back_;
   const Condition* condition_;
};

#endif /* BOUNDARYCONDITION_H_ */
